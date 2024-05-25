#include "GlobalScope.hxx"

namespace ltn::c {
	GlobalScope::GlobalScope(stx::reference<Context> context)
		: Scope {}
		, context { context } {}



	std::uint64_t GlobalScope::size() const {
		return 0;
	}



	bool GlobalScope::is_const() const {
		return true;
	}



	bool GlobalScope::is_coroutine() const {
		return false;
	}
	
	

	const Namespace & GlobalScope::get_namespace() const {
		return {};
	}
	
	
	
	std::optional<std::string> GlobalScope::get_return() const {
		return std::nullopt;
	}
	
	

	Context & GlobalScope::get_context() const {
		return context;
	}
	
	

	LocalVariableInfo GlobalScope::declare_local_variable(const std::string & name, const SourceLocation & location) {
		throw std::runtime_error { "Cannot declare local varaible in global scope" };
	}



	stx::optref<const LocalVariableInfo> GlobalScope::resolve_local_variable(const std::string & name, const SourceLocation & location) const {
		return stx::nullref;
	}



	stx::optref<const ast::decl::Function> GlobalScope::resolve_function(const std::string & name, const Namespace & ns, std::size_t arity, VariadicMode var_mode) const {
		return stx::nullref;
	}



	stx::optref<const sst::decl::Global> GlobalScope::resolve_global_variable(const std::string & name, const Namespace & ns) const {
		return stx::nullref;
	}



	stx::optref<const sst::decl::Definition> GlobalScope::resolve_definiton(const std::string & name, const Namespace & ns) const {
		return stx::nullref;
	}



	namespace {
		std::uint64_t resolve_member_id_impl(auto & table, const std::string & str) {
			if(table.contains(str)) {
				return table.at(str).id;
			}
			const auto id = table.size() + 256;
			table.insert({str, MemberInfo {
				.id = id,
			}});
			return id;
		}



		std::uint64_t resolve_member_id_impl(auto &, MemberCode code) {
			return static_cast<std::uint64_t>(code);
		}
	}



	std::uint64_t GlobalScope::resolve_member_id(const std::variant<std::string, MemberCode> & name) const {
		return std::visit([&] (auto & m) {
			return resolve_member_id_impl(this->member_info, m);
		}, name);
	}



	sst::expr_ptr GlobalScope::resolve_custom_literal(const std::string & type, const std::string & value) const {
		return this->get_context().custom_resolver.resolve(type, value);
	}
}