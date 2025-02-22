#include "parse.hxx"
#include "stdxx/iife.hxx"
#include "litan_compiler/CompilerError.hxx"
#include <iostream>
#include "litan_compiler/ast/decl/Function.hxx"
#include "litan_compiler/ast/expr/BuildIn.hxx"
#include "litan_compiler/ast/expr/Lambda.hxx"
#include "litan_compiler/ast/expr/ForwardDynamicCall.hxx"
#include "litan_compiler/ast/expr/Var.hxx"
#include "litan_compiler/ast/stmt/Return.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;


		// Returns a array of all parameters
		ast::decl::Parameters parse_parameters_variadic(Tokens & tokens) {
			
			if(match(TT::PAREN_R, tokens)) {
				return {};
			}

			ast::decl::Parameters parameters{};
			while(true) {
				auto name = parse_parameter_name(tokens);
				if(match(TT::ELLIPSIS, tokens)) {
					parameters.variadic = ast::decl::Parameter {
						.name = name
					};
					if(!match(TT::PAREN_R, tokens)) throw CompilerError {
						"Variadic parameter must the last one",
						location(tokens)
					};
					return parameters;
				}
				else {
					parameters.simple.push_back(ast::decl::Parameter{
						.name = std::move(name),
					});
					if(match(TT::PAREN_R, tokens)) return parameters;
					if(!match(TT::COMMA, tokens)) throw CompilerError {
						"expected comma between parameters",
						location(tokens)
					};
				}
			}
		}



		// Returns a array of all parameters
		ast::decl::Parameters parse_parameters(Tokens & tokens) {
			
			if(match(TT::PAREN_R, tokens)) {
				return {};
			}

			ast::decl::Parameters parameters{};
			while(true) {
				auto name = parse_parameter_name(tokens);
				parameters.simple.push_back(ast::decl::Parameter{
					.name = std::move(name),
				});
				if(match(TT::PAREN_R, tokens)) return parameters;
				if(!match(TT::COMMA, tokens)) throw CompilerError {
					"expected comma between parameters",
					location(tokens)
				};
			}
		}



		ast::decl::Parameters parse_lambda_parameters(Tokens & tokens) {
			if(match(TT::PAREN_L, tokens)) return parse_parameters_variadic(tokens);
			return {};
		}

		
		ast::decl::Parameters parse_except_parameters(Tokens & tokens) {
			if(match(TT::PAREN_L, tokens)) return parse_parameters(tokens);
			throw ltn::c::CompilerError{"missing (", location(tokens)};
		}


		ast::decl::Parameters parse_function_parameters(Tokens & tokens) {
			if(match(TT::PAREN_L, tokens)) return parse_parameters_variadic(tokens);
			throw ltn::c::CompilerError{"missing (", location(tokens)};
		}



		std::vector<std::unique_ptr<ast::expr::Var>> parse_captures(Tokens & tokens) {
			if(!match(TT::BRACKET_L, tokens)) return {};

			std::vector<std::unique_ptr<ast::expr::Var>> captures{};
			if(!match(TT::BRACKET_R, tokens)) {
				while(true) {
					const auto name = parse_variable_name(tokens);
					const auto & loc = location(tokens);
					auto var = ast::expr::variable(loc, name, Namespace{});
					captures.push_back(std::move(var));
					if(match(TT::BRACKET_R, tokens)) break;
					if(!match(TT::COMMA, tokens)) {
						throw CompilerError {
							"expected comma between captures",
							location(tokens)
						};
					}
				}
			} 
			return captures;
		}



		std::string parse_build_in_key(Tokens & tokens, [[maybe_unused]] std::uint64_t arity) {
			if(auto str = match(TT::INDENTIFIER, tokens)) {
				return str->str;
			}
			else {
				throw CompilerError {
					"Expected build_in key after @",
					location(tokens)
				};
			}
		}



		ast::stmt_ptr parse_dynamic_body(Tokens & tokens, std::uint64_t arity, bool is_variadic) {
			if(!match(TT::PAREN_L, tokens)) throw CompilerError {
				"Expected ("
			};
			auto i = match(TT::INTEGER_HEX, tokens);
			if(!i)  throw CompilerError {
				"Unsupported dynamic link code " + i->str, i->location
			};
			if(!match(TT::PAREN_R, tokens)) throw CompilerError {
				"Expected )"
			};
			std::stringstream iss{i->str};
			std::uint64_t address;
			iss >> std::hex >> address;
			auto expr = std::make_unique<ast::expr::ForwardDynamicCall>(
				address, arity + is_variadic, i->location
			);
			return ast::stmt::retvrn(std::move(expr), i->location);
		}




		ast::stmt_ptr parse_build_in_body(Tokens & tokens, std::uint64_t arity, const Token & begin) {
			if(!match(TT::PAREN_L, tokens)) throw CompilerError {
				"Expected ("
			};
			
			auto key = parse_build_in_key(tokens, arity);

			if(!match(TT::PAREN_R, tokens)) throw CompilerError {
				"Expected )"
			};

			return ast::stmt::retvrn(ast::expr::build_in(begin.location, std::move(key)), begin.location);
		}


		
		ast::stmt_ptr parse_body(Tokens & tokens, std::uint64_t arity, bool is_variadic) {
			if(auto begin = match(TT::DRARROW, tokens)) {
				if(match(TT::DYNAMIC, tokens)) {
					return parse_dynamic_body(tokens, arity, is_variadic);
				}
				if(match(TT::BUILD_IN, tokens)) {
					return parse_build_in_body(tokens, arity, *begin);
				}
				auto expr = parse_expression(tokens);
				const auto & loc = location(tokens);
				return ast::stmt::retvrn(std::move(expr), loc);
			}
			else {
				return parse_block(tokens);
			}
		}



		std::unique_ptr<ast::decl::Except> parse_except(Tokens & tokens) {
			if(match(TT::EXCEPT, tokens)) {
				auto params = parse_except_parameters(tokens);
				if(params.simple.size() != 1) {
					throw CompilerError {
						"Except only takes one error parameter",
						location(tokens)
					};
				}
				auto body = parse_body(tokens, 1, false);
				return std::make_unique<ast::decl::Except>(
					params.simple[0].name,
					std::move(body),
					location(tokens)
				);
			}
			else return nullptr;
		}



		Qualifiers parse_qualifiers(Tokens & tokens) {
			Qualifiers q;
			while(auto t = match(TT::INDENTIFIER, tokens)) {
				if(t->str == "const") {
					q.is_const = true;
				}
				else if(t->str == "private") {
					q.is_private = true;
				}
				else if(t->str == "extern") {
					q.is_extern = true;
				}
				else if(t->str == "coroutine") {
					q.is_coroutine = true;
				}
				else {
					throw CompilerError {
						"Unknown function qualifier: " + t->str,
						t->location
					};
				}
			}
			return q;
		}
	}



	// parses and returns a functional node
	ast::func_ptr parse_function(Tokens & tokens, const Namespace & namespaze) {
		auto name = parse_function_name(tokens);
		auto parameters = parse_function_parameters(tokens);

		Qualifiers qualifiers = parse_qualifiers(tokens); 
		auto body = parse_body(tokens, std::size(parameters.simple), parameters.variadic.has_value());
		auto fx = std::make_unique<ast::decl::Function>(
			std::move(name),
			std::move(namespaze),
			std::move(parameters),
			std::move(body),
			location(tokens)
		);
		fx->qualifiers = qualifiers;
		if(!fx->qualifiers.is_const) fx->except = parse_except(tokens);
		return fx;
	}



	ast::expr_ptr parse_lambda(Tokens & tokens) {
		if(auto start = match(TT::LAMBDA, tokens)) {
			auto captures = parse_captures(tokens);
			auto parameters = parse_lambda_parameters(tokens);
			Qualifiers qualifiers = parse_qualifiers(tokens);
			auto body = parse_body(tokens, std::size(parameters.simple), parameters.variadic.has_value()); 
			auto fx = std::make_unique<ast::decl::Function>(
				"lambda" + std::to_string(*stx::unique{}), 
				Namespace{},
				std::move(parameters),
				std::move(body),
				location(tokens));
			fx->except = parse_except(tokens);
			fx->qualifiers = qualifiers;
			if(qualifiers.is_private) throw CompilerError { "Lambda cannot be private", location(tokens)};
			if(qualifiers.is_extern)  throw CompilerError {"Lambda cannot be extern", location(tokens)};
			return ast::expr::lambda(start->location, std::move(fx), std::move(captures)); 
		}
		return nullptr;
	}
}