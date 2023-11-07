#include "analyze.hxx"
#include "ltnc/sst/expr/Invoke.hxx"
#include "ltnc/sst/expr/Literal.hxx"
#include "ltnc/sst/expr/Var.hxx"
#include "ltnc/sst/expr/BuildIn.hxx"

namespace ltn::c {
	sst::expr_ptr analyze_expr(const ast::ForwardDynamicCall & forward, Scope &) {
		
		std::vector<sst::expr_ptr> arguments;
		for(std::uint64_t i = 0; i < forward.arity; ++i) {
			arguments.push_back(std::make_unique<sst::Var>(i));
		}

		return std::make_unique<sst::Invoke>(
			std::make_unique<sst::Integer>(forward.address),
			std::move(arguments)
		);
	}




	sst::expr_ptr analyze_expr(const ast::BuildIn & build_in, Scope &) {
		return std::make_unique<sst::BuildIn>(build_in.key);
	}



	// compiles any expr
	sst::expr_ptr analyze_expression(const ast::Expression & expr, Scope & scope) {
		return ast::visit_expression(expr, [&](const auto & e) {
			return analyze_expr(e, scope);
		});
	}
}