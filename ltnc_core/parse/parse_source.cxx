#include "parse.hxx"
#include "ltnc_core/CompilerError.hxx"
#include <iostream>
namespace ltn::c::parse {
	namespace {
		using TT = ltn::c::lex::Token::Type;


		CompilerError anonymous_namespace(const lex::Lexer & lexer) {
			return CompilerError {
				"Expected namespace name. Anonymous not supported",
				lexer.location()};
		}



		CompilerError unknown_declaration(const lex::Lexer & lexer) {
			return CompilerError(
				"Unknown declaration."
				"Exprected function, namespace or asm-function.",
				lexer.location());
		}



		CompilerError unclosed_namespace(const lex::Lexer & lexer) {
			return CompilerError {
				"Unclosed namespace. Expected }",
				lexer.location()};
		}



		CompilerError missing_brace_l(const lex::Lexer & lexer) {
			return CompilerError {
				"Expected {",
				lexer.location()};
		}



		CompilerError extra_brace_r(const lex::Lexer & lexer) {
			return CompilerError {
				"Extra }",
				lexer.location()};
		}



		// parses: namespace foo { ...
		std::optional<std::string> open_namespace(lex::Lexer & lexer) {
			if(lexer.match(TT::NAMESPACE)) {
				if(auto name = lexer.match(TT::INDENTIFIER)) {
					if(lexer.match(TT::BRACE_L)) {
						return name->str;
					}
					else throw missing_brace_l(lexer);
				}
				else throw anonymous_namespace(lexer);
			}
			return {};
		}



		// }
		bool close_namespace(
			lex::Lexer & lexer,
			ast::Namespace & namespaze) {
			if(lexer.match(TT::BRACE_R)) {
				if(namespaze.empty()) {
					throw extra_brace_r(lexer);
				}
				return true;
			}
			return false;
		}
	}



	ast::srce_ptr source(lex::Lexer & lexer) {
		auto source = std::make_unique<ast::Source>();
		auto & functions = source->functions;
		auto & globals = source->globals;
		auto & presets = source->presets;
		ast::Namespace namespaze;
		while(!lexer.match(TT::___EOF___)) {
			// try {
			if(auto ns = open_namespace(lexer)) {
				namespaze.push_back(*ns);
			}
			else if(close_namespace(lexer, namespaze)) {
				namespaze.pop_back();
			}
			else if(auto fx = parse::functional(lexer, namespaze)) {
				functions.push_back(std::move(fx));
			}
			else if(auto global = parse::definition(lexer, namespaze)) {
				globals.push_back(std::move(global));
			}
			else if(auto preset = parse::preset(lexer, namespaze)) {
				presets.push_back(std::move(preset));
			}
			else if(lexer.match(TT::ENUM)) {
				auto g = parse::enumeration(lexer, namespaze);
				for(auto & global : g) {
					globals.push_back(std::move(global));
				}
			}			
			else throw unknown_declaration(lexer);
		}
		if(!namespaze.empty()) {
			throw unclosed_namespace(lexer);
		}
		return source; 
	}
}