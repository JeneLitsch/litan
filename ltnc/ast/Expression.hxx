#pragma once
#include <vector>
#include <utility>
#include <any>
#include <bitset>
#include "ltn/casts.hxx"
#include "Switch.hxx"
#include "Node.hxx"
#include "ltnc/Namespace.hxx"
#include "ltnc/Operations.hxx"
#include "ltn/Visitor.hxx"


namespace ltn::c::ast {
	class Statement;
	struct Function;



	struct Binary;
	struct Unary;
	struct Integer;
	struct Float;
	struct Bool;
	struct Char;
	struct Null;
	struct String;
	struct Array;
	struct Tuple;
	struct Call;
	struct Var;
	struct Index;
	struct Lambda;
	struct FxPointer;
	struct Member;
	struct GlobalVar;
	struct Iife;
	struct Ternary;
	struct ExprSwitch;
	struct TypedUnary;
	struct Reflect;
	struct ForwardDynamicCall;
	struct InitStruct;



	struct ExprVisitor : Visitor<
		ast::Binary,
		ast::Unary,
		ast::Integer,
		ast::Float,
		ast::Bool,
		ast::Char,
		ast::Null,
		ast::String,
		ast::Array,
		ast::Tuple,
		ast::Call,
		ast::Var,
		ast::Index,
		ast::Lambda,
		ast::FxPointer,
		ast::Member,
		ast::GlobalVar,
		ast::Iife,
		ast::Ternary,
		ast::ExprSwitch,
		ast::TypedUnary,
		ast::Reflect,
		ast::ForwardDynamicCall,
		ast::InitStruct>
	{};



	struct Expression : public Node {
		Expression(const SourceLocation & location) : Node(location) {}
		virtual ~Expression() = default;

		virtual void accept(const ExprVisitor & visitor) const = 0;
	};



	struct ForwardDynamicCall : public Expression {
		ForwardDynamicCall(
			std::uint64_t addr,
			std::uint64_t arity,
			const SourceLocation & location)
			: Expression(location)
			, addr{addr}
			, arity{arity} {}
		virtual ~ForwardDynamicCall() = default;
		std::uint64_t addr;
		std::uint64_t arity;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};
	


	struct Ternary : public Expression {
		Ternary(
			const SourceLocation & location,
			std::unique_ptr<Expression> condition,
			std::unique_ptr<Expression> if_branch,
			std::unique_ptr<Expression> else_branch) 
			: Expression(location)
			, condition(std::move(condition))
			, if_branch(std::move(if_branch))
			, else_branch(std::move(else_branch)) {}
		virtual ~Ternary() = default;
	
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Expression> if_branch;
		std::unique_ptr<Expression> else_branch;
		
		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};




	struct Unary : public Expression {
		using Op = UnaryOp;
		Unary(
			Op op,
			std::unique_ptr<Expression> expression,
			const SourceLocation & location)
			: Expression(location)
			, op(op)
			, expression(std::move(expression)) {}
		virtual ~Unary() = default;
		Op op;
		std::unique_ptr<Expression> expression;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Binary : public Expression {
		using Op = BinaryOp;
		Binary(
			Op op,
			std::unique_ptr<Expression> l,
			std::unique_ptr<Expression> r,
			const SourceLocation & location)
			: Expression(location)
			, op(op)
			, l(std::move(l))
			, r(std::move(r)) {}
		virtual ~Binary() = default;
		Op op;
		std::unique_ptr<Expression> l;
		std::unique_ptr<Expression> r;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Reflect : public Expression {
		struct NamespaceQuery {
			Namespace namespaze;
		};
		struct FunctionQuery {
			Namespace namespaze;
			std::string name;
			std::size_t arity;
		};
		struct FileQuery {};
		struct LineQuery {};
		struct LocationQuery {
			FileQuery file;
			LineQuery line;
		};
		struct ExprQuery {
			std::unique_ptr<Expression> expr;
		};
		struct TypeQuery {
			type::IncompleteType type;
		};
		using Query = std::variant<
			NamespaceQuery,
			FunctionQuery,
			FileQuery,
			LineQuery,
			LocationQuery,
			ExprQuery,
			TypeQuery
		>;
		Reflect(
			Query query,
			const SourceLocation & location)
			: Expression(location)
			, query{std::move(query)} {}
		virtual ~Reflect() = default;
		Query query;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct TypedUnary final : public Expression {
	public:
		using Op = TypedUnaryOp;
		TypedUnary(
			Op op,
			const type::IncompleteType & type,
			std::unique_ptr<Expression> expr,
			const SourceLocation & location)
			: Expression{location}
			, op{op}
			, type{type}
			, expr{std::move(expr)} {}
		virtual ~TypedUnary() = default;
		Op op;
		type::IncompleteType type;
		std::unique_ptr<Expression> expr;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};


	struct Functional;
	struct Var;
	class Statement;

	struct Integer final : public Expression {
		Integer(std::bitset<64> value, const SourceLocation & location)
			: Integer(static_cast<std::int64_t>(value.to_ullong()), location) {}

		Integer(std::int64_t value, const SourceLocation & location)
			: Expression(location), value(value) {}
		virtual ~Integer() = default;
		std::int64_t value;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Float final : public Expression {
		Float(stx::float64_t value, const SourceLocation & location)
			: Expression(location), value(value) {}
		virtual ~Float() = default;
		stx::float64_t value;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Bool final : public Expression {
		Bool(bool value, const SourceLocation & location)
			: Expression(location), value(value) {}
		virtual ~Bool() = default;
		bool value;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Null final : public Expression {
		Null(const SourceLocation & location)
			: Expression(location) {}
		virtual ~Null() = default;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Char final : public Expression {
		Char(std::uint8_t value, const SourceLocation & location)
			: Expression(location), value(value) {}
		virtual ~Char() = default;
		std::uint8_t value;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct String final : public Expression {
		String(const std::string & value, const SourceLocation & location)
			: Expression(location), value(value) {}
		virtual ~String() = default;
		std::string value;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Array final: public Expression {
		Array(
			const SourceLocation & location,
			std::vector<std::unique_ptr<Expression>> elements)
			: Expression(location)
			, elements{std::move(elements)} {}

		std::vector<std::unique_ptr<Expression>> elements;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Tuple final: public Expression {
		Tuple(
			const SourceLocation & location,
			std::vector<std::unique_ptr<Expression>> elements)
			: Expression(location)
			, elements{std::move(elements)} {}
		virtual ~Tuple() = default;
		std::vector<std::unique_ptr<Expression>> elements;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Lambda final : public Expression {
		Lambda(
			std::unique_ptr<Function> fx,
			std::vector<std::unique_ptr<Var>> captures,
			const SourceLocation & location)
			: Expression(location)
			, fx(std::move(fx))
			, captures(std::move(captures)) {}
		virtual ~Lambda() = default;
		std::unique_ptr<Function> fx;
		std::vector<std::unique_ptr<Var>> captures;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};


	
	struct Index final : public Expression {
		Index(
			std::unique_ptr<Expression> expression,
			std::unique_ptr<Expression> index,
			const SourceLocation & location)
			: Expression(location)
			, expression(std::move(expression))
			, index(std::move(index)) {}
		virtual ~Index() = default;
		std::unique_ptr<Expression> expression;
		std::unique_ptr<Expression> index;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Var final : public Expression {
		Var(
			const std::string & name,
			const Namespace & namespaze,
			const SourceLocation & location)
			: Expression(location)
			, name{name}
			, namespaze{namespaze} {}

		virtual ~Var() = default;
		std::string name;
		Namespace namespaze;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct GlobalVar final : public Expression {
		GlobalVar(
			const SourceLocation & location,
			const Namespace & namespaze,
			const std::string & name)
			: Expression(location)
			, name { name }
			, namespaze { namespaze } {}
		virtual ~GlobalVar() = default;
		std::string name;
		Namespace namespaze;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Member final : public Expression {
		Member(
			std::unique_ptr<Expression> expr,
			const std::string & name,
			const SourceLocation & location)
			: Expression(location)
			, expr(std::move(expr))
			, name(std::move(name)){};
		virtual ~Member() = default;
		std::unique_ptr<Expression> expr;
		std::string name;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Iife final : public Expression {
		Iife(
			const SourceLocation & location,
			std::unique_ptr<Statement> stmt,
			type::IncompleteType return_type) 
			: Expression(location)
			, stmt(std::move(stmt))
			, return_type{return_type} {}
		virtual ~Iife() = default;
		
		std::unique_ptr<Statement> stmt;
		type::IncompleteType return_type;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct FxPointer final : public Expression {
		FxPointer(
			const std::string & name,
			const Namespace & namespaze,
			const std::size_t placeholders,
			const SourceLocation & location)
			: Expression(location)
			, name(name)
			, namespaze(namespaze)
			, placeholders(std::move(placeholders)) {}
		virtual ~FxPointer() = default;
		std::string name;
		Namespace namespaze;
		std::size_t placeholders;
		std::vector<type::IncompleteType> template_arguements;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct Call final : public Expression {
		Call(
			std::unique_ptr<Expression> function_ptr,
			std::vector<std::unique_ptr<Expression>> parameters,
			const SourceLocation & location)
			: Expression(location)
			, function_ptr(std::move(function_ptr))
			, parameters(std::move(parameters)) {}
		virtual ~Call() = default;
		std::unique_ptr<Expression> function_ptr;
		std::vector<std::unique_ptr<Expression>> parameters;
		std::vector<type::IncompleteType> template_args;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct InitStruct final : public Expression {
		InitStruct(const SourceLocation & location)
			: Expression{location} {}
		virtual ~InitStruct() = default;
		struct Member {
			std::string name;
			std::unique_ptr<Expression> expr;
		};
		std::vector<Member> members;

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	struct ExprSwitch : Switch<Expression> {
		ExprSwitch(const SourceLocation & location)
			: Switch<Expression>{location} {}

		virtual void accept(const ExprVisitor & visitor) const override { visitor.visit(*this); }
	};



	template<typename Callable>
	class ExprVisitorImpl : public ExprVisitor {
		using Ret = decltype(std::declval<const Callable &>()(std::declval<const ast::Binary &>())); 
	public:
		ExprVisitorImpl(Callable fx) : fx{fx} {}

		virtual void visit(const ast::Binary & x)             const override { ret = fx(x); };
		virtual void visit(const ast::Unary & x)              const override { ret = fx(x); };
		virtual void visit(const ast::Integer & x)            const override { ret = fx(x); };
		virtual void visit(const ast::Float & x)              const override { ret = fx(x); };
		virtual void visit(const ast::Bool & x)               const override { ret = fx(x); };
		virtual void visit(const ast::Char & x)               const override { ret = fx(x); };
		virtual void visit(const ast::Null & x)               const override { ret = fx(x); };
		virtual void visit(const ast::String & x)             const override { ret = fx(x); };
		virtual void visit(const ast::Array & x)              const override { ret = fx(x); };
		virtual void visit(const ast::Tuple & x)              const override { ret = fx(x); };
		virtual void visit(const ast::Call & x)               const override { ret = fx(x); };
		virtual void visit(const ast::Var & x)                const override { ret = fx(x); };
		virtual void visit(const ast::Index & x)              const override { ret = fx(x); };
		virtual void visit(const ast::Lambda & x)             const override { ret = fx(x); };
		virtual void visit(const ast::FxPointer & x)          const override { ret = fx(x); };
		virtual void visit(const ast::Member & x)             const override { ret = fx(x); };
		virtual void visit(const ast::GlobalVar & x)          const override { ret = fx(x); };
		virtual void visit(const ast::Iife & x)               const override { ret = fx(x); };
		virtual void visit(const ast::Ternary & x)            const override { ret = fx(x); };
		virtual void visit(const ast::ExprSwitch & x)         const override { ret = fx(x); };
		virtual void visit(const ast::TypedUnary & x)         const override { ret = fx(x); };
		virtual void visit(const ast::Reflect & x)            const override { ret = fx(x); };
		virtual void visit(const ast::ForwardDynamicCall & x) const override { ret = fx(x); };
		virtual void visit(const ast::InitStruct & x)         const override { ret = fx(x); };

		Ret operator()(const ast::Expression & expr) const {
			expr.accept(*this);
			return std::move(ret);
		}
	private:
		Callable fx;
		mutable Ret ret;
	};

	



	auto visit_expression(const ast::Expression & expr, auto && fx) {
		const auto visitor = ExprVisitorImpl{fx};
		return visitor(expr);
	}
}