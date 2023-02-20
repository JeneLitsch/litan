#include "analyze.hxx"
#include "stdxx/functional.hxx"

namespace ltn::c {
	namespace {
		auto analyze_capture_stores(const auto & captures, Context & context, auto & outer_scope) {
			auto fx = [&] (const auto & capture) {
				auto expr = analyze_expr(*capture, context, outer_scope);
				return stx::static_unique_cast<sst::Var>(std::move(expr));
			};
			return stx::fx::map(fx, captures);
		};



		type::FxPtr deduce_type(const auto & lambda, Scope & inner_scope) {
			auto instantiate_all = stx::fx::mapped([&] (const auto & param) {
				return instantiate_type(param.type, inner_scope);
			});
			return type::FxPtr {
				.return_type = instantiate_type(lambda.fx->return_type, inner_scope),
				.parameter_types = instantiate_all(lambda.fx->parameters), 
			};
		}



		MajorScope create_inner_scope(
			Scope & outer_scope,
			const ast::Lambda & lambda) {
			MajorScope inner_scope {
				outer_scope.get_namespace(),
				lambda.fx->is_const
			};

			inner_scope.inherit_types_from(outer_scope);

			for(const auto & capture : lambda.captures) {
				inner_scope.insert(capture->name, location(*lambda.fx));
			}

			return inner_scope;
		}
	}



	sst::expr_ptr analyze_expr(
		const ast::Lambda & lambda,
		Context & context,
		Scope & outer_scope) {
		
		auto inner_scope = create_inner_scope(outer_scope, lambda);
		
		const auto label = make_lambda_label(lambda);

		auto sst_fx = analyze_function(*lambda.fx, context, inner_scope, label);
		auto store_captures = analyze_capture_stores(lambda.captures, context, outer_scope);
		auto type = deduce_type(lambda, inner_scope);

		return std::make_unique<sst::Lambda>(
			std::move(sst_fx),
			std::move(store_captures),
			type
		);
	}
}