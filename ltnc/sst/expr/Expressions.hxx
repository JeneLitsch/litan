#pragma once
#include <vector>
#include <bitset>
#include <variant>
#include "ltn/casts.hxx"
#include "ltnc/Namespace.hxx"
#include "ltnc/Label.hxx"
#include "ltnc/Operations.hxx"
#include "ltnc/sst/expr/Expression.hxx"
#include "ltnc/sst/stmt/Statement.hxx"


namespace ltn::c::sst {
	struct Expression;
	struct Var;
	struct Function;
	class Statement;



	struct Literal : public Expression {
		Literal() : Expression{} {}
		virtual ~Literal() = default;
	};



	struct Integer final : public Literal {
		Integer(std::bitset<64> value)
			: Integer(static_cast<std::int64_t>(value.to_ullong())) {}
		
		Integer(std::int64_t value)
			: Literal()
			, value(value) {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::int64_t value;
	};



	struct Float final : public Literal {
		Float(stx::float64_t value)
			: Literal()
			, value(value) {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		stx::float64_t value;
	};



	struct Bool final : public Literal {
		Bool(bool value)
			: Literal()
			, value(value) {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		bool value;
	};



	struct Null final : public Literal {
		Null() : Literal{} {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}
	};



	struct Char final : public Literal {
		Char(std::uint8_t value)
			: Literal()
			, value(value) {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint8_t value;
	};



	struct String final : public Literal {
		String(const std::string & value)
			: Literal()
			, value(value) {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string value;
	};



	struct Array final: public Literal {
		Array() : Literal() {}

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->elements) {
				count = std::max(elem->alloc(), count);
			}
			return count;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<std::unique_ptr<Expression>> elements;
	};


	
	struct Tuple final: public Literal {
		Tuple() : Literal() {}
		virtual ~Tuple() = default;

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->elements) {
				count = std::max(elem->alloc(), count);
			}
			return count;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<std::unique_ptr<Expression>> elements;
	};



	struct Index final : public Expression {
		Index(
			std::unique_ptr<Expression> expr,
			std::unique_ptr<Expression> index)
			: Expression{}
			, expr(std::move(expr))
			, index(std::move(index)) {}

		virtual std::uint64_t alloc() const override {
			return std::max({
				this->expr->alloc(),
				this->index->alloc(),
			});
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> expr;
		std::unique_ptr<Expression> index;
	};



	struct Var final : public Expression {
	public:
		Var(
			std::size_t address)
			: Expression{}
			, address{address} {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::size_t address;
	};



	struct GlobalVar final : public Expression {
	public:
		GlobalVar(
			std::size_t address)
			: Expression{}
			, address { address } {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::size_t address;
	};



	struct Member final : public Expression {
		Member(
			std::unique_ptr<Expression> expr,
			std::size_t address)
			: Expression{}
			, expr(std::move(expr))
			, address { address } {};

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> expr;
		std::size_t address;
	};



	struct Iife final : public Expression {
		Iife(
			std::string return_label,
			std::unique_ptr<Statement> stmt) 
			: Expression{}
			, return_label{return_label}
			, stmt(std::move(stmt)) {}

		virtual std::uint64_t alloc() const override {
			return this->stmt->direct_alloc() + this->stmt->nested_alloc();
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string return_label;
		std::unique_ptr<Statement> stmt;
	};



	struct FxPointer final : public Expression {
		FxPointer(
			const Label & label,
			std::size_t arity,
			bool is_variadic)
			: Expression{}
			, label{label}
			, arity{arity}
			, is_variadic{is_variadic} {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		Label label;
		std::size_t arity;
		std::vector<std::unique_ptr<Var>> captures;
		bool is_variadic;
	};



	struct Call final : public Expression {
		Call(
			const Label & label,
			std::vector<std::unique_ptr<Expression>> arguments)
			: Expression{}
			, label{label}
			, arguments(std::move(arguments)) {}

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->arguments) {
				count = std::max(elem->alloc(), count);
			}
			return count;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint64_t arity() const {
			return std::size(arguments);
		}

		Label label;
		std::vector<std::unique_ptr<Expression>> arguments;
	};



	struct Invoke final : public Expression {
		Invoke(
			std::unique_ptr<Expression> function_ptr,
			std::vector<std::unique_ptr<Expression>> arguments)
			: Expression{}
			, function_ptr(std::move(function_ptr))
			, arguments(std::move(arguments)) {}
		virtual ~Invoke() = default;

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->arguments) {
				count = std::max(elem->alloc(), count);
			}
			return std::max(count, function_ptr->alloc());
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint64_t arity() const {
			return std::size(arguments);
		}

		std::unique_ptr<Expression> function_ptr;
		std::vector<std::unique_ptr<Expression>> arguments;
	};



	struct InvokeMember final : public Expression {
		InvokeMember(
			std::unique_ptr<Expression> object,
			std::uint64_t member_id,
			std::vector<std::unique_ptr<Expression>> arguments)
			: Expression{}
			, object(std::move(object))
			, member_id{member_id}
			, arguments(std::move(arguments)) {}

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->arguments) {
				count = std::max(elem->alloc(), count);
			}
			return std::max(count, object->alloc());
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint64_t arity() const {
			return std::size(arguments) + 1;
		}

		std::unique_ptr<Expression> object;
		std::uint64_t member_id;
		std::vector<std::unique_ptr<Expression>> arguments;
	};



	struct InitStruct final : public Expression {
		InitStruct() : Expression{} {}

		struct Member {
			std::uint64_t address;
			std::unique_ptr<Expression> expr;
		};

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & elem : this->members) {
				count = std::max(elem.expr->alloc(), count);
			}
			return count;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<Member> members;
	};



	struct Choose final : Expression {
		Choose() : Expression{} {}

		virtual std::uint64_t alloc() const override {
			std::uint64_t count = 0;
			for(const auto & [c4se, expr] : this->cases) {
				count = std::max({c4se->alloc(), expr->alloc(), count});
			}
			return count;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> condition;

		std::vector<std::pair<
			std::unique_ptr<Expression>,
			std::unique_ptr<Expression>
		>> cases;

		std::unique_ptr<Expression> d3fault;
	};
	


	struct Type final : Expression {
		Type(std::vector<std::uint8_t> type_code)
			: Expression{}
			, type_code{std::move(type_code)} {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<std::uint8_t> type_code;
	};



	struct BuildIn final : Expression {
		BuildIn(std::string key)
			: Expression{}
			, key{std::move(key)} {}

		virtual std::uint64_t alloc() const override {
			return 0;
		}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string key;
	};
}