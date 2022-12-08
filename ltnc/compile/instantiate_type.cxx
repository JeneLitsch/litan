#include "instantiate_type.hxx"
#include "stdxx/functional.hxx"
#include "ltnc/CompilerError.hxx"

namespace ltn::c {
	type::Type instantiate_type(
		const type::Type & incomplete,
		const Scope & scope);


	type::Any instantiate(
		const type::Any & incomplete,
		const Scope & scope) {
		return type::Any{};
	}



	type::Type instantiate(
		const type::Other & incomplete,
		const Scope & scope) {
		auto * type = scope.resolve_type(incomplete.type_name);
		if(type) return *type;
		throw CompilerError{
			"Undeclared type " + incomplete.type_name 
		};
	}



	type::Optional instantiate(
		const type::Optional & incomplete,
		const Scope & scope) {
		return type::Optional{
			.contains = instantiate_type(**incomplete.contains, scope)
		};
	}



	type::Error instantiate(
		const type::Error & incomplete,
		const Scope & scope) {
		return type::Error{};
	}



	type::Null instantiate(
		const type::Null & incomplete,
		const Scope & scope) {
		return type::Null{};
	}



	type::Bool instantiate(
		const type::Bool & incomplete,
		const Scope & scope) {
		return type::Bool{};
	}



	type::Char instantiate(
		const type::Char & incomplete,
		const Scope & scope) {
		return type::Char{};
	}



	type::Int instantiate(const type::Int & incomplete,
		const Scope & scope) {
		return type::Int{};
	}



	type::Float instantiate(const type::Float & incomplete,
		const Scope & scope) {
		return type::Float{};
	}



	type::String instantiate(const type::String & incomplete,
		const Scope & scope) {
		return type::String{};
	}



	type::Array instantiate(const type::Array & incomplete,
		const Scope & scope) {
		return type::Array{
			.contains = instantiate_type(**incomplete.contains, scope)
		};
	}



	type::FxPtr instantiate(
		const type::FxPtr & incomplete,
		const Scope & scope) {
		std::vector<type::Type> parameters;
		for(const auto & parameter : incomplete.parameter_types) {
			parameters.push_back(instantiate_type(parameter, scope));
		}
		return type::FxPtr{
			.return_type = instantiate_type(**incomplete.return_type, scope),
			.parameter_types = std::move(parameters),
		};
	}



	type::Queue instantiate(
		const type::Queue & incomplete,
		const Scope & scope) {
		return type::Queue{
			.contains = instantiate_type(**incomplete.contains, scope)
		};
	}



	type::Stack instantiate(
		const type::Stack & incomplete,
		const Scope & scope) {
		return type::Stack{
			.contains = instantiate_type(**incomplete.contains, scope)
		};
	}



	type::Map instantiate(
		const type::Map & incomplete,
		const Scope & scope) {
		return type::Map{
			.key = instantiate_type(**incomplete.key, scope),
			.val = instantiate_type(**incomplete.val, scope),
		};
	}



	type::Type instantiate_type(
		const type::Type & type,
		const Scope & scope) {
		return type.visit([&] (auto & t) {
			return type::Type{instantiate(t, scope)};
		});
	}



	type::Type instantiate_type(
		const type::IncompleteType & incomplete,
		const Scope & scope) {
		return instantiate_type(incomplete.type, scope);
	}
}