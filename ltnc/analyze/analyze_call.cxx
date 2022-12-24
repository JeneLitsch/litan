#include "analyze.hxx"
#include "stdxx/functional.hxx"
#include <string_view>
#include "conversion.hxx"
#include "qualify_overload.hxx"

namespace ltn::c {
	CompilerError undefined_function(
		const std::string_view & name,
		const ast::Node & node) {
		std::stringstream ss;
		ss << "Function " << name << " is not defined";
		return CompilerError { ss.str(), node.location };
	}



	namespace {
		sst::expr_ptr do_call(
			const ast::Call & call,
			const ast::Functional & fx,
			std::vector<sst::expr_ptr> function_args,
			Context & context,
			Scope & scope,
			const std::optional<Label> id_override = std::nullopt) {

			guard_private(fx, scope.get_namespace(), call.location);
			
			if(scope.is_const() && !fx.is_const) {
				throw CompilerError {
					"Cannot call non-const function from a const functions",
					call.location};
			}

			for(std::size_t i = 0; i < call.parameters.size(); ++i) {
				auto & parameter = fx.parameters[i];
				const auto expr_type = function_args[i]->type;
				const auto param_type = instantiate_type(parameter.type, scope);
				function_args[i] = conversion_on_pass(
					std::move(function_args[i]),
					param_type,
					{call.location,i}
				);
			}

			const auto return_type = instantiate_type(fx.return_type, scope);
			const auto fx_label = make_function_label(
				fx.namespaze,
				fx.name,
				fx.parameters.size()
			);
			const auto label = id_override.value_or(fx_label);
			
			return std::make_unique<sst::Call>(
				label,
				std::move(function_args),
				return_type
			);
		}



		sst::expr_ptr do_invoke(
			const ast::Call & call,
			std::vector<sst::expr_ptr> function_args,
			Context & context,
			Scope & scope) {

			auto expr = analyze_expression(*call.function_ptr, context, scope);

			const auto type = type::deduce_invokation(expr->type);
			return std::make_unique<sst::Invoke>(
				std::move(expr),
				std::move(function_args),
				type
			);
		}



		sst::expr_ptr do_call_template(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> function_args,
			Context & context,
			Scope & scope) {
			
			const auto tmpl = get_template(
				var.name,
				var.namespaze,
				call.parameters.size(),
				call.template_args.size(),
				var.location,
				context,
				scope
			);
			const auto template_args = stx::fx::mapped(instantiate_type)(
				call.template_args,
				scope
			);
			context.fx_queue.stage_template(*tmpl, template_args);
			const auto label = make_template_label(tmpl, template_args);
			MinorScope inner_scope(&scope);
			add_template_args(
				inner_scope,
				tmpl->template_parameters,
				call.template_args);

			return do_call(
				call,
				*tmpl->fx,
				std::move(function_args),
				context,
				inner_scope,
				label
			);
		}



		sst::expr_ptr try_template(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> & function_args,
			Context & context,
			Scope & scope) {
			
			if(call.template_args.empty()) return nullptr;
			return do_call_template(
				call,
				var,
				std::move(function_args),
				context,
				scope
			);
		}



		sst::expr_ptr try_local(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> & function_args,
			Context & context,
			Scope & scope) {
			
			if(!var.namespaze.empty()) return nullptr;
			const auto * local = scope.resolve(var.name, var.location);
			if(!local) return nullptr;
			return do_invoke(
				call,
				std::move(function_args),
				context,
				scope
			);
		}



		sst::expr_ptr try_function(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> & function_args,
			Context & context,
			Scope & scope) {
			
			const auto * fx = context.fx_table.resolve(
				var.name,
				scope.get_namespace(),
				var.namespaze,
				call.parameters.size());
			
			if(!fx) return nullptr;

			context.fx_queue.stage_function(*fx);
			return do_call(
				call,
				*fx,
				std::move(function_args),
				context,
				scope
			);
		}



		sst::expr_ptr try_overload(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> & function_args,
			Context & context,
			Scope & scope) {

			const auto * overload = context.overload_table.resolve(
				var.name,
				scope.get_namespace(),
				var.namespaze,
				call.parameters.size());

			if(!overload) return nullptr;

			auto * fx = qualify_overload(
				*overload,
				context.fx_table,
				function_args,
				scope
			);

			if(!fx) return nullptr;

			context.fx_queue.stage_function(*fx);
			return do_call(
				call,
				*fx,
				std::move(function_args),
				context,
				scope
			);
		}



		sst::expr_ptr try_definition(
			const ast::Call & call,
			const ast::Var & var,
			std::vector<sst::expr_ptr> & function_args,
			Context & context,
			Scope & scope) {
			
			const auto * def = context.definition_table.resolve(
				var.name,
				scope.get_namespace(),
				var.namespaze);
			
			if(!def) return nullptr;

			return do_invoke(
				call,
				std::move(function_args),
				context,
				scope
			);
		}



		auto try_all(const auto & fxs, auto && ... args) 
			-> decltype(fxs[0](args...)) {
			for(const auto & r : fxs) {
				if(auto result = r(args...)) return result;
			}
			return nullptr;
		}
	}



	// compiles function call fx(...)
	sst::expr_ptr analyze_expr(
		const ast::Call & call,
		Context & context,
		Scope & scope) {
		
		std::vector<sst::expr_ptr> arguments;
		for(const auto & param : call.parameters) {
			arguments.push_back(analyze_expression(*param, context, scope));
		}
		
		if(const auto * var = as<ast::Var>(*call.function_ptr)) {

			static constexpr auto steps = std::to_array({
				try_template,
				try_local,
				try_function,
				try_overload,
				try_definition
			});

			auto expr = try_all(steps, call, *var, arguments, context, scope);
			if(expr) return expr;
				
			throw undefined_function(var->name, call);
		}

		return do_invoke(call, std::move(arguments), context, scope);
	}
}