#include "parse.hxx"
#include "litan_compiler/CompilerError.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;
	}



	std::string parse_indentifier(Tokens & tokens, const std::string & errMsg){
		if(auto token = match(TT::INDENTIFIER, tokens)) {
			return token->str;
		}
		throw ltn::c::CompilerError{errMsg, location(tokens)};
	}



	std::string parse_preset_name(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid preset name");
	}



	std::string parse_enum_name(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid preset name");
	}



	std::string parse_definition(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid definition name");
	}



	std::string parse_variable_name(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid variable name");
	}



	std::string parse_parameter_name(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid parameter name");
	}



	std::string parse_function_name(Tokens & tokens) {
		return parse_indentifier(tokens, "invalid function name");
	}
}