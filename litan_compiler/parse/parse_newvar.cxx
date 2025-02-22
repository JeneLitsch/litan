#include "parse.hxx"
#include "litan_compiler/ast/stmt/NewVar.hxx"
#include "litan_compiler/ast/bind/NewVar.hxx"
#include "litan_compiler/ast/bind/Group.hxx"

namespace ltn::c {
	ast::bind_ptr parse_binding(const Token & start, Tokens & tokens);
	namespace {
		using TT = Token::Type;



		ast::bind_ptr parse_single_binding(const Token & start, Tokens & tokens) { 
			return ast::bind::new_local(start.location, parse_variable_name(tokens));
		}
		


		ast::bind_ptr parse_group_binding(const Token & start, Tokens & tokens) {
			auto binding = ast::bind::group(start.location);
			const auto add_var = [&] () {
				auto sub = parse_binding(start, tokens);
				binding->sub_bindings.push_back(std::move(sub));
			};

			add_var();
			while(match(TT::COMMA, tokens)) {
				add_var();
			}
			if(!match(TT::PAREN_R, tokens)) throw CompilerError {
				"Expected )", start.location
			};
			return binding;
		}


	}



	ast::bind_ptr parse_binding(const Token & start, Tokens & tokens) {
		if(match(TT::PAREN_L, tokens)) {
			return parse_group_binding(start, tokens);
		}
		else {
			return parse_single_binding(start, tokens);
		}
	}
	


	ast::stmt_ptr parse_newvar(Tokens & tokens) {
		if(auto start = match(TT::VAR, tokens)) {
			auto binding = parse_binding(*start, tokens);
			auto && r = parse_assign_r(tokens);
			semicolon(tokens);
			return ast::stmt::new_local(std::move(binding), std::move(r), start->location);
		}
		return nullptr;
	}
}