#pragma once
#include <vector>
#include "Node.hxx"
#include "Switch.hxx"
#include "ltnc/type/Type.hxx"
#include "ltn/casts.hxx"
#include "Binding.hxx"

namespace ltn::c::ast {
	struct Expression;
	struct Assignable;
	class Statement : public Node {
	public:
		Statement(const SourceLocation & location) : Node(location) {}
		virtual ~Statement() = default;
	};


	
	struct DoNothing : public Statement {
		DoNothing(const SourceLocation & location) : Statement(location) {}
		virtual ~DoNothing() = default;
	};



	struct Throw final : public Statement {
		Throw(
			Expression expression,
			const SourceLocation & location) 
			: Statement(location)
			, expression(std::move(expression)) {}
		virtual ~Throw() = default;
		Expression expression;
	};



	struct Block final : public Statement {
		Block(
			std::vector<std::unique_ptr<Statement>> statements,
			const SourceLocation & location) 
			: Statement(location)
			, statements(std::move(statements)) {}
		virtual ~Block() = default;
		std::vector<std::unique_ptr<Statement>> statements;
	};



	struct NewVar final : public Statement {
		NewVar(
			std::unique_ptr<Binding> binding,
			Expression expression,
			const SourceLocation & location,
			const std::optional<type::IncompleteType> & type = type::IncompleteType{type::Any{}})
			: Statement(location)
			, binding(std::move(binding))
			, expression(std::move(expression))
			, type{type} {}
		virtual ~NewVar() = default;
		std::unique_ptr<Binding> binding;		
		Expression expression;
		std::optional<type::IncompleteType> type;
	};



	struct IfElse final : public Statement {
		IfElse(
			Expression condition,
			std::unique_ptr<Statement> if_branch,
			std::unique_ptr<Statement> else_branch,
			const SourceLocation & location)
			: Statement(location)
			, condition(std::move(condition))
			, if_branch(std::move(if_branch))
			, else_branch(std::move(else_branch)) {}
		virtual ~IfElse() = default;
		Expression condition;
		std::unique_ptr<Statement> if_branch;
		std::unique_ptr<Statement> else_branch;
	};



	struct While final : public Statement {
		While(
			Expression condition,
			std::unique_ptr<Statement> body,
			const SourceLocation & location)
			: Statement(location)
			, condition(std::move(condition))
			, body(std::move(body)) {}

		virtual ~While() = default;
		Expression condition;
		std::unique_ptr<Statement> body;
	};



	struct InfiniteLoop final : public Statement {
		InfiniteLoop(
			std::unique_ptr<Statement> body,
			const SourceLocation & location)
			: Statement(location)
			, body(std::move(body)) {}

		virtual ~InfiniteLoop() = default;
		std::unique_ptr<Statement> body;
	};



	struct For final : public Statement {
		For(
			std::string index_name,
			Expression from,
			Expression to,
			std::optional<Expression> step,
			std::unique_ptr<Statement> body,
			const SourceLocation & location)
			: Statement(location)
			, index_name(std::move(index_name))
			, from(std::move(from))
			, to(std::move(to))
			, step(std::move(step))
			, body(std::move(body)) {}

		virtual ~For() = default;
		std::string index_name;
		Expression from;
		Expression to;
		std::optional<Expression> step;
		std::unique_ptr<Statement> body;
	};



	struct ForEach final : public Statement {
		ForEach(
			std::string index_name,
			Expression expr,
			std::unique_ptr<Statement> body,
			const SourceLocation & location)
			: Statement(location)
			, index_name(std::move(index_name))
			, expr(std::move(expr))
			, body(std::move(body)) {}

		virtual ~ForEach() = default;
		std::string index_name;
		Expression expr;
		std::unique_ptr<Statement> body;
	};



	struct StatementExpression final : public Statement {
		StatementExpression(
			Expression expression,
			const SourceLocation & location)
			: Statement(location)
			, expression(std::move(expression)) {}
		virtual ~StatementExpression() = default;
		Expression expression;
	};



	struct Return final : public Statement {
		Return(
			Expression expression,
			const SourceLocation & location)
			: Statement(location)
			, expression(std::move(expression)) {}
		virtual ~Return() = default;
		Expression expression;
	};


	
	struct InitMember final : public Statement {
		InitMember(
			std::string member,
			std::string param,
			type::IncompleteType type,
			const SourceLocation & location)
			: Statement(location)
			, member(std::move(member))
			, param(std::move(param))
			, type{type} {}
		virtual ~InitMember() = default;
		std::string member;
		std::string param;
		type::IncompleteType type;
	};



	struct Assign final : public Statement {
	public:
		Assign(
			Expression l,
			Expression r,
			const SourceLocation & location)
			: Statement(location)
			, l(std::move(l))
			, r(std::move(r)) {}
		virtual ~Assign() = default;
		Expression l;
		Expression r;
	};

	using StmtSwitch = Switch<Statement>;



	auto visit_statement(const ast::Statement & stmt, auto && fx) {
		if(auto s = as<ast::Block>(stmt)) return fx(*s);
		if(auto s = as<ast::IfElse>(stmt)) return fx(*s);
		if(auto s = as<ast::While>(stmt)) return fx(*s);
		if(auto s = as<ast::InfiniteLoop>(stmt)) return fx(*s);
		if(auto s = as<ast::For>(stmt)) return fx(*s);
		if(auto s = as<ast::ForEach>(stmt)) return fx(*s);
		if(auto s = as<ast::NewVar>(stmt)) return fx(*s);
		if(auto s = as<ast::Return>(stmt)) return fx(*s);
		if(auto s = as<ast::Throw>(stmt)) return fx(*s);
		if(auto s = as<ast::StmtSwitch>(stmt)) return fx(*s);
		if(auto s = as<ast::StatementExpression>(stmt)) return fx(*s);
		if(auto s = as<ast::DoNothing>(stmt)) return fx(*s);
		if(auto s = as<ast::Assign>(stmt)) return fx(*s);
		throw std::runtime_error{"Unknown AST statement"};
	}
}