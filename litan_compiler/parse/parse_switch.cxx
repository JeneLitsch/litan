#include "parse.hxx"
#include "litan_compiler/CompilerError.hxx"
#include "litan_compiler/ast/expr/Null.hxx"
#include "litan_compiler/ast/expr/Choose.hxx"
#include "litan_compiler/ast/stmt/Switch.hxx"
#include "litan_compiler/ast/stmt/NoOp.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;
	}

	template<typename NodeT, TT START_TOKEN, auto body_fx>
	std::unique_ptr<NodeT> parse_any_switch(Tokens & tokens) {
		if(match(START_TOKEN, tokens)) {
			if(!match(TT::PAREN_L, tokens)) throw CompilerError {
				"Expected (", location(tokens)
			};

			auto sw1tch = std::make_unique<NodeT>(location(tokens));
			sw1tch->condition = parse_expression(tokens);

			if(!match(TT::PAREN_R, tokens)) throw CompilerError {
				"Expected )", location(tokens)
			};

			if(!match(TT::BRACE_L, tokens)) throw CompilerError {
				"Expected {", location(tokens)
			};

			while (match(TT::CASE, tokens)) {
				auto case_expr = parse_expression(tokens);
				
				if(!match(TT::DRARROW, tokens)) throw CompilerError {
					"Expected :", location(tokens)
				};

				auto case_body = body_fx(tokens);

				sw1tch->cases.push_back(std::pair{
					std::move(case_expr),
					std::move(case_body)});
			}
			
			if(match(TT::ELSE, tokens)) {
				if(!match(TT::DRARROW, tokens)) throw CompilerError {
					"Expected :", location(tokens)
				};
				sw1tch->d3fault = body_fx(tokens);
			}

			if(!match(TT::BRACE_R, tokens)) throw CompilerError {
				"Expected }", location(tokens)
			};


			return sw1tch;
		}
		return nullptr;
	} 



	ast::stmt_ptr parse_stmt_switch(Tokens & tokens) {
		auto sw1tch = parse_any_switch<ast::stmt::Switch, TT::SWITCH, parse_statement>(tokens);
		if(sw1tch && !sw1tch->d3fault) {
			sw1tch->d3fault = ast::stmt::no_op(location(tokens));
		}
		return sw1tch;
	} 



	ast::expr_ptr parse_expr_switch(Tokens & tokens) {
		auto sw1tch = parse_any_switch<ast::expr::Choose, TT::SWITCH, parse_expression>(tokens);
		if(sw1tch && !sw1tch->d3fault) {
			sw1tch->d3fault = std::make_unique<ast::expr::Null>(location(tokens));
		}
		return sw1tch;
	}
}