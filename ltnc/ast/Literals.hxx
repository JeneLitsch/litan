#pragma once
#include "Expression.hxx"
#include <bitset>

namespace ltn::c::ast {
	struct Functional;
	struct Var;
	
	struct Literal : public Primary {
		Literal(const lex::DebugInfo & debugInfo) : Primary(debugInfo) {}
		virtual ~Literal() = default;
	};

	struct Integer : public Literal {
	public:
		Integer(std::bitset<64> value, const lex::DebugInfo & debugInfo)
			:	Integer(static_cast<std::int64_t>(value.to_ullong()), debugInfo) {}

		Integer(std::int64_t value, const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo), value(value) {}
		virtual ~Integer() = default;
		const std::int64_t value;
	};

	struct Float : public Literal {
	public:
		Float(double value, const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo), value(value) {}
		virtual ~Float() = default;
		const double value;
	};

	struct Bool : public Literal {
	public:
		Bool(bool value, const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo), value(value) {}
		virtual ~Bool() = default;
		const bool value;
	};

	struct String : public Literal {
	public:
		String(const std::string & value, const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo), value(value) {}
		virtual ~String() = default;
		const std::string value;
	};

	struct Array : public Literal {
	public:
		Array(
			std::vector<std::unique_ptr<Expression>> && initElements,
			const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo),
			initElements(std::move(initElements)) {}
		virtual ~Array() = default;
		std::vector<std::unique_ptr<Expression>> initElements;
	};

	struct Lambda : public Literal {
		Lambda(
			std::unique_ptr<Functional> fx,
			std::vector<std::unique_ptr<Var>> captures,
			const lex::DebugInfo & debugInfo)
			:	Literal(debugInfo),
				fx(std::move(fx)),
				captures(std::move(captures)) {}
		virtual ~Lambda() = default;
		const std::unique_ptr<Functional> fx;
		const std::vector<std::unique_ptr<Var>> captures;
	};
}