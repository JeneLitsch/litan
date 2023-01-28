#include "analyze.hxx"
#include "ltnc/type/traits.hxx"

namespace ltn::c {
	sst::expr_ptr analyze_expr(
		const ast::ForwardDynamicCall & forward,
		Context & context,
		Scope & scope) {
		
		std::vector<sst::expr_ptr> arguments;
		for(std::uint64_t i = 0; i < forward.arity; ++i) {
			arguments.push_back(std::make_unique<sst::Var>(i, type::Any{}));
		}

		return std::make_unique<sst::Invoke>(
			std::make_unique<sst::Integer>(forward.addr, type::Int{}),
			std::move(arguments),
			type::Any{}
		);
	}

	// compiles any expression
	sst::expr_ptr analyze_expression(
		const ast::Expression & expr,
		Context & context,
		Scope & scope) {

		auto result = ast::visit_expression(expr, [&](const auto & e) {
			return analyze_expr(e, context, scope);
		});

		if(is_error(result->type)) throw CompilerError {
			"Invalid operands for expression",
			expr.location
		};

		return result;
	}
}