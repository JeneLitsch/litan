#include "parse.hxx"
#include "litan_compiler/CompilerError.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;
	}



	void brace_l(Tokens & tokens) {
		if(!match(TT::BRACE_L, tokens)) {
			throw CompilerError{"Expected {", location(tokens)};
		}
	}



	void brace_r(Tokens & tokens) {
		if(!match(TT::BRACE_R, tokens)) {
			throw CompilerError{"Expected }", location(tokens)};
		}
	}
}