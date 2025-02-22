#include "parse.hxx"
#include "litan_compiler/CompilerError.hxx"
#include <sstream>
#include "parse_utils.hxx"
#include "litan_compiler/ast/expr/Call.hxx"
#include "litan_compiler/ast/expr/Member.hxx"
#include "litan_compiler/ast/expr/Index.hxx"
#include "litan_compiler/ast/expr/Unary.hxx"
#include "litan_compiler/ast/expr/InvokeMember.hxx"
namespace ltn::c {
	namespace {
		using TT = Token::Type;
		using OP = UnaryOp;







		ast::expr_ptr parse_index(Tokens & tokens) {
			auto index = parse_expression(tokens);
			if(match(TT::BRACKET_R, tokens)) {
				return index;
			}
			throw CompilerError{"Missing ]", location(tokens)};
		}



		auto parse_arguments(Tokens & tokens) {
			std::vector<ast::expr_ptr> parameters;
			if(match(TT::PAREN_R, tokens)) {
				 return parameters;
			}
			while(true) {
				parameters.push_back(parse_expression(tokens));
				if(match(TT::PAREN_R, tokens)) {
					return parameters;
				}
				if(!match(TT::COMMA, tokens)) throw CompilerError {
					"Expected ,", location(tokens)
				};
			}
		}



		ast::expr_ptr parse_postfix(
			Tokens & tokens,
			ast::expr_ptr l) {

			if(auto start = match(TT::BRACKET_L, tokens)) {
				auto index = parse_index(tokens);
				auto subscript = ast::expr::index(start->location, std::move(l), std::move(index));
				return parse_postfix(tokens, std::move(subscript));
			}

			if(auto start = match(TT::DOT, tokens)) {
				std::string name = parse_member(tokens);
				ast::expr_ptr access = ast::expr::member(start->location, std::move(l), std::move(name));
				return parse_postfix(tokens, std::move(access));
			}


			if(auto start = match(TT::RARROW, tokens)) {
				auto name = parse_member(tokens);

				if(!match(TT::PAREN_L, tokens)) throw CompilerError {
					"Expected ( after member call"
				};

				auto args = parse_arguments(tokens);
				auto access = ast::expr::invoke_member(
					start->location,
					std::move(l),
					std::move(name),
					std::move(args)
				);
				return parse_postfix(tokens, std::move(access));
			}

			if(auto start = match(TT::PAREN_L, tokens)) {
				auto args = parse_arguments(tokens);
				auto call = ast::expr::call(start->location, std::move(l), std::move(args));
				return parse_postfix(tokens, std::move(call));
			}

			return l;
		}
	}



	ast::expr_ptr parse_prefix(Tokens & tokens) {
		// left unary
		const std::array table {
			std::pair{TT::MINUS, OP::NEG},
			std::pair{TT::XMARK, OP::NOT},
			std::pair{TT::QMARK, OP::NUL},
			std::pair{TT::TILDE, OP::BITNOT},
			std::pair{TT::STAR,  OP::DEREF},
		};
		
		if(auto op = match_op(tokens, table)) {
			auto && r = parse_prefix(tokens);
			return ast::expr::unary(location(tokens), *op, std::move(r));
		}
		return parse_postfix(tokens, parse_primary(tokens));
	}



	ast::expr_ptr parse_unary(Tokens & tokens) {
		return parse_prefix(tokens);
	}
}
