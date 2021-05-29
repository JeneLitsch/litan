#include "LtncLexerNodeCreator.hxx"
#include <map>

ltnc::LexerNode ltnc::LexerNodeCreator::chr(char chr, std::vector<LexerNode> nodes) {
	return LexerNode([chr, nodes](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			for(const LexerNode & node : nodes) {
				if(node.eval(lexPkg)) {
					return true;
				}
			}
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::chr(char chr, TokenType type) {
	return LexerNode([chr, type](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::chr(char chr, TokenType type, std::vector<LexerNode> nodes) {
	return LexerNode([chr, nodes, type](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			for(const LexerNode & node : nodes) {
				if(node.eval(lexPkg)) {
					return true;
				}
			}
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::connector(std::vector<LexerNode> nodes) {
	return LexerNode([nodes](LexerPackage & lexPkg) {
		for(const LexerNode & node : nodes) {
			if(node.eval(lexPkg)) {
				return true;
			}
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::ignoreLine() {
	return LexerNode([](LexerPackage & lexPkg) {
		while (!lexPkg.isAtEnd()){
			if(lexPkg.match('\n')) {
				lexPkg.newLine();
				break;
			} 
			lexPkg.next();
		}
		return true;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::newLine() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.match('\n'))  {
			lexPkg.newLine();
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::ignore(char chr) {
	return LexerNode([chr](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::numliteral() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.matchDigit()) {
			bool isFloat = false;
			while(!lexPkg.isAtEnd() && lexPkg.matchDigit());
			if(lexPkg.match('.')) {
				isFloat = true;
				while(!lexPkg.isAtEnd() && lexPkg.matchDigit());
			}
			std::string lexeme = lexPkg.makeLexeme();
			TokenType type = isFloat?TokenType::NUMBER_FLT:TokenType::NUMBER_INT;
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::identifier() {
	std::map<std::string, TokenType> keywords;
	keywords.insert({"int", TokenType::INT});
	keywords.insert({"flt", TokenType::FLT});
	keywords.insert({"voi", TokenType::VOI});
	keywords.insert({"con", TokenType::CON});
	keywords.insert({"fnx", TokenType::FNX});
	keywords.insert({"print", TokenType::PRINT});
	keywords.insert({"if", TokenType::IF});
	keywords.insert({"else", TokenType::ELSE});
	keywords.insert({"for", TokenType::FOR});
	keywords.insert({"while", TokenType::WHILE});
	keywords.insert({"return", TokenType::RETURN});
	keywords.insert({"repeat", TokenType::REPEAT});
	keywords.insert({"asm", TokenType::ASM});

	return LexerNode([keywords](LexerPackage & lexPkg) {
		if(lexPkg.matchAlpha()) {
			while(!lexPkg.isAtEnd() && lexPkg.matchAlpha());
			std::string lexeme = lexPkg.makeLexeme();
			if(keywords.contains(lexeme)) {
				lexPkg.newToken(keywords.at(lexeme));
			}
			else {
				lexPkg.newToken(TokenType::IDENTIFIER);
			}
			return true;
		}
		return false;
	});
}

ltnc::LexerNode ltnc::LexerNodeCreator::string() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.match('"')){
			while(!lexPkg.match('"')) {
				lexPkg.next();
				if(lexPkg.isAtEnd()) {
					lexPkg.error("Unterminated string");
				}
			}
			std::string lexeme = lexPkg.makeLexeme(); 
			lexPkg.newToken(
				TokenType::STRING_LITERAL,
				lexeme.substr(1, lexeme.size()-2));
			return true;
		}
		return false;
	});
}