#pragma once
#include <vector>
#include "stdxx/oop.hxx"
#include "ltnc/CompilerError.hxx"
#include "namespace_resolution.hxx"

namespace ltn::c {
	// Holds and resolves functions at compile time
	template<typename Symbol, typename ... Args>
	class SymbolTable : stx::non_copyable, stx::non_moveable {
	public:
		virtual const Symbol * resolve(
			const std::string_view name,
			const Namespace & from,
			const Namespace & to,
			Args ... args) const = 0;

		virtual const Symbol * resolve(
			const std::string_view name,
			const Namespace & full,
			Args ... args) const = 0;

		virtual void insert(
			const Symbol & sym,
			Args ... args) = 0;

		virtual const std::vector<const Symbol *> & get_symbols() const = 0;
	protected:
		SymbolTable() = default;
	};



	// Declaration and usage are allowed.
	template<typename Symbol, typename Err, typename ... Args>
	class ValidSymbolTable : public SymbolTable<Symbol, Args...> {
	public:

		ValidSymbolTable() = default;
		virtual const Symbol * resolve(
			const std::string_view name,
			const Namespace & from,
			const Namespace & to,
			Args ... args) const override {
		
			return ltn::c::resolve(this->symbols, from, to, name, args...);
		}



		virtual const Symbol * resolve(
			const std::string_view name,
			const Namespace & full,
			Args ... args) const override {

			return ltn::c::resolve(this->symbols, {}, full, name, args...);
		}



		virtual void insert(
			const Symbol & symbol,
			Args ... args) override {
			// Prevent redefinition
			if(this->resolve_exact(symbol.get_resolve_name(), symbol.get_resolve_namespace(), args...)) {
				throw Err::redef(symbol);
			}
			this->symbols.push_back(&symbol);
		}


		virtual const std::vector<const Symbol *> & get_symbols() const override {
			return symbols;
		}

	private:
		const Symbol * resolve_exact(
			const std::string_view name,
			const Namespace & full,
			Args ... args) const {

			return ltn::c::resolve_exact(this->symbols, {}, full, name, args...);
		}


		std::vector<const Symbol *> symbols;
	};



	// Declaration and usage are prohibited.
	template<typename Symbol, typename Err, typename ... Args>
	class InvalidSymbolTable : public SymbolTable<Symbol, Args...> {
	public:

		InvalidSymbolTable(const std::string & inside) 
			: inside { inside } {}



		virtual const Symbol * resolve(
			const std::string_view,
			const Namespace &,
			const Namespace &,
			Args ...) const override {
			throw Err::use(this->inside);
		}



		virtual const Symbol * resolve(
			const std::string_view,
			const Namespace &,
			Args ...) const override {
			throw Err::use(this->inside);
		}



		virtual void insert(
			const Symbol &,
			Args ...) override {
			throw Err::decl(this->inside);
		}

		virtual const std::vector<const Symbol *> & get_symbols() const override {
			static const std::vector<const Symbol *> vec;
			return vec;
		}

	private:
		std::string inside;
	};
}