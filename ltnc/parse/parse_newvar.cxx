#include "parse.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;



		ast::stmt_ptr parse_simple_newvar(const Token & start, Tokens & tokens) {
			auto name = parse_variable_name(tokens);
			auto type = parse_var_type_auto(tokens);
			auto && r = parse_assign_r(tokens);
			semicolon(tokens);
			return stx::make_unique<ast::NewVar>(
				name,
				std::move(r),
				location(tokens),
				type
			);
		}




		namespace {
			ast::bind_ptr parse_binding(const Token & start, Tokens & tokens);



			ast::bind_ptr parse_single_binding(const Token & start, Tokens & tokens) { 
				return std::make_unique<ast::VarBinding>(
					start.location,
					parse_variable_name(tokens)
				);
			}
			


			ast::bind_ptr parse_group_binding(const Token & start, Tokens & tokens) {
				auto binding = std::make_unique<ast::GroupBinding>(start.location);
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


			ast::bind_ptr parse_binding(const Token & start, Tokens & tokens) {
				if(match(TT::PAREN_L, tokens)) {
					return parse_group_binding(start, tokens);
				}
				else {
					return parse_single_binding(start, tokens);
				}
			}
		}



		ast::stmt_ptr parse_unpack_newvar(const Token & start, Tokens & tokens) {
			
			auto binding = parse_group_binding(start, tokens);
			auto && r = parse_assign_r(tokens);
			if(!r) throw CompilerError {
				"Structure bindings must be assigned", start.location
			};
			semicolon(tokens);
			return std::make_unique<ast::StructuredBinding>(
				std::move(binding),
				std::move(r),
				start.location
			); 
		}
	}



	ast::stmt_ptr parse_newvar(Tokens & tokens) {
		if(auto start = match(TT::VAR, tokens)) {
			if(match(TT::PAREN_L, tokens)) {
				return parse_unpack_newvar(*start, tokens);
			}
			else {
				return parse_simple_newvar(*start, tokens);
			} 
		}
		return nullptr;
	}
}