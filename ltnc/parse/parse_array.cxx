#include "parse.hxx"
#include "parse_utils.hxx"

namespace ltn::c {
	using TT = Token::Type;

	namespace {
		ast::expr_ptr parse_empty_array(Tokens & tokens) {
			return std::make_unique<ast::Array>(
				location(tokens),
				std::vector<ast::expr_ptr>{}
			);
		}



		auto parse_member(Tokens & tokens) {
			if(!match(TT::DOT, tokens)) {
				throw CompilerError{"Expected . (dot)", location(tokens)};
			}
			auto name = parse_variable_name(tokens);
			if(!match(TT::ASSIGN, tokens)) {
				throw CompilerError{"Expected =", location(tokens)};
			}
			auto expr = parse_expression(tokens);

			return ast::InitStruct::Member{
				.name = std::move(name),
				.expr = std::move(expr),
			};
		}


		ast::expr_ptr parse_struct_init(Tokens & tokens) {
			auto init_struct = std::make_unique<ast::InitStruct>(location(tokens));
			init_struct->members = list_of<ast::InitStruct::Member>(TT::BRACKET_R, "]", tokens, parse_member);
			return ast::expr_ptr(std::move(init_struct));
		}



		ast::expr_ptr parse_filled_array(Tokens & tokens) {
			auto elements = list_of<ast::expr_ptr>(TT::BRACKET_R, "]", tokens, parse_expression);
			return std::make_unique<ast::Array>(
				location(tokens),
				std::move(elements)
			);
		}
	}



	ast::expr_ptr parse_array(Tokens & tokens) {
		if(match(TT::BRACKET_L, tokens)) {
			if(match(TT::BRACKET_R, tokens)) {
				return parse_empty_array(tokens);
			}
			else if(check(TT::DOT, tokens)) {
				return parse_struct_init(tokens);
			}
			else {
				return parse_filled_array(tokens);
			}
		}
		return ast::expr_ptr();
	}
}