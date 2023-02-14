#include "parse.hxx"
#include <sstream>
namespace ltn::c {
	using TT = Token::Type;

	namespace {
		ast::Expression parse_cast(Tokens & tokens, ast::Expression expr) {
			const auto make = [&] (ast::TypedUnary::Op op) {
				auto type = parse_type(tokens);
				const auto loc = location(expr);
				return parse_cast(tokens, ast::TypedUnary {
					.location = loc,
					.op = op, 
					.type = type,
					.expr = std::move(expr),
				});
			};
			if(match(TT::COLON, tokens)) {
				if(match(TT::STAR, tokens)) {
					return make(ast::TypedUnary::Op::STATIC_COPY);
				}
				else {
					return make(ast::TypedUnary::Op::STATIC_CAST);
				}
			}
			if(match(TT::TILDE, tokens)) {
				if(match(TT::STAR, tokens)) {
					return make(ast::TypedUnary::Op::DYNAMIC_COPY);
				}
				else {
					return make(ast::TypedUnary::Op::DYNAMIC_CAST);
				}
			}
			if(match(TT::XMARK, tokens)) {
				if(match(TT::STAR, tokens)) {
					return make(ast::TypedUnary::Op::FORCE_COPY);
				}
				else {
					return make(ast::TypedUnary::Op::FORCE_CAST);
				}
			}
			return expr;
		}
	}


	// generic expression
	ast::Expression parse_expression(Tokens & tokens) {
		auto expr = parse_expression_no_cast(tokens);
		return parse_cast(tokens, std::move(expr));

	}



	ast::Expression parse_expression_no_cast(Tokens & tokens) {
		return parse_conditional(tokens);
	}
}

