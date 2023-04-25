#pragma once

#include <tuple>
#include <vector>
#include <sstream>
#include "ltn/unique.hxx"
#include "ltnc/Namespace.hxx"
#include "ltnc/type/Type.hxx"
#include "ltnc/ast/decl/Declaration.hxx"

namespace ltn::c::ast {
	class Statement;
	struct Parameter {
		struct Infered {
			std::string name;
		};
		std::string name;
		using DeclType = std::variant<type::IncompleteType, Infered>;
		DeclType type = type::IncompleteType{type::Any{}};
	};
	using Parameters = std::vector<Parameter>;



	inline std::string mangle(
		const std::string & name,
		const Namespace & namespaze,
		const Parameters & parameters) {
		std::ostringstream oss;
		oss << namespaze.to_string();
		oss << name << "(";
		oss << std::size(parameters) << ")";
		return oss.str();
	}



	struct Except final : public Node {
		Except(
			const std::string & errorname,
			std::unique_ptr<Statement> && body,
			const SourceLocation & location)
			: Node(location)
			, body(std::move(body))
			, errorname(errorname) {}
		virtual ~Except() = default;
		std::unique_ptr<Statement> body;
		std::string errorname;
	};



	struct Functional : public Declaration {
		Functional(
			const std::string & name,
			Namespace namespaze,
			Parameters parameters,
			const type::IncompleteType & return_type,
			const SourceLocation & location)
			: Declaration(location, name, namespaze)
			, parameters(parameters)
			, return_type{return_type} {}
		virtual ~Functional() = default;

		Parameters parameters;
		bool is_const = false;
		bool is_private = false;
		bool is_extern = false;

		type::IncompleteType return_type;

		const std::string & get_resolve_name() const {
			return this->name;
		}

		const Namespace & get_resolve_namespace() const {
			return this->namespaze;
		}
	};



	struct Function final : public Functional {
		Function(
			const std::string & name,
			Namespace namespaze,
			Parameters parameters,
			std::unique_ptr<Statement> && body,
			const type::IncompleteType & return_type,
			const SourceLocation & location)
			: Functional{name, namespaze, parameters, return_type, location}
			, body(std::move(body)) {}

		Function(
			const std::string & name,
			Namespace namespaze,
			Parameters parameters,
			std::unique_ptr<Statement> && body,
			const SourceLocation & location)
			: Functional{name, namespaze, parameters, type::IncompleteType{type::Any{}}, location}
			, body(std::move(body)) {}
		virtual ~Function() = default;
		std::unique_ptr<Statement> body;
		std::unique_ptr<Except> except;
	};



	struct BuildIn final : public Functional {
		BuildIn(
			const std::string & name,
			Namespace namespaze,
			Parameters parameters,
			const std::string & key,
			const type::IncompleteType & return_type,
			const SourceLocation & location)
			: Functional{name, namespaze, parameters, return_type, location}
			, key(key) {}
		virtual ~BuildIn() = default;
		std::string key;		
	};
}