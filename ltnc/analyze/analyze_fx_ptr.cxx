#include "analyze.hxx"
#include "stdxx/functional.hxx"
#include "stdxx/iife.hxx"
#include <string_view>

namespace ltn::c {
	namespace {
		std::vector<type::Type> instantiate_parameters(
			const ast::Parameters & parameters,
			Scope & scope) {
			
			return stx::fx::map([&] (const auto & parameter) {
				return analyze_parameter_type(parameter.type, scope);
			}, parameters);
		}



		auto resolve_fx_ptr_normal(
			const ast::FxPointer & fx_ptr,
			Scope & scope,
			Context & context) {

			const auto fx = context.fx_table.resolve(
				fx_ptr.name,
				scope.get_namespace(),
				fx_ptr.namespaze,
				fx_ptr.placeholders
			);

			if(!fx) throw undefined_function(fx_ptr);

			context.fx_queue.stage_function(*fx, {});

			const auto label = make_function_label(*fx);

			return std::tuple{fx, label};
		}



		bool refers_to_template(const ast::FxPointer & fx_ptr) {
			return !fx_ptr.template_arguments.empty();
		}



		auto resolve_fx_ptr(
			const ast::FxPointer & fx_ptr,
			Scope & scope,
			Context & context) {
			return resolve_fx_ptr_normal(fx_ptr, scope, context);
		}
	}



	sst::expr_ptr analyze_expr(
		const ast::FxPointer & fx_ptr,
		Context & context,
		Scope & scope) {

		MinorScope inner_scope{&scope};
		auto [funtional, label] = resolve_fx_ptr(fx_ptr, inner_scope, context);

		const auto return_type 
			= instantiate_type(funtional->return_type, inner_scope);
		
		const auto parameter_types
			= instantiate_parameters(funtional->parameters, inner_scope);

		const auto type = type::FxPtr {
			.return_type = return_type,
			.parameter_types = parameter_types,
		};

		return std::make_unique<sst::FxPointer>(
			label,
			funtional->parameters.size(),
			type
		);
	}
}