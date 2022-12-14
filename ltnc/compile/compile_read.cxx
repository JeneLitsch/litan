#include "compile.hxx"
#include <string_view>
namespace ltn::c {
	namespace {
		InstructionBuffer compile_read_local_variable(const Variable & var) {
			InstructionBuffer buf;
			buf << inst::read_x(var.address);
			return InstructionBuffer{ 
				.code = buf,
				.deduced_type = var.type
			};
		}



		bool is_inner_namespace(
			const Namespace & call_ns,
			const Namespace & fx_ns) {
			if(fx_ns.size() > call_ns.size()) return false;
			for(std::size_t i = 0; i < fx_ns.size(); i++) {
				if(call_ns[i] != fx_ns[i]) {
					return false;
				}
			}
			return true;
		}
	}



	// compiles an variable read accessc
	InstructionBuffer compile_expr(
		const sst::Var & expr,
		CompilerInfo & info,
		Scope & scope) {

		const auto & name = expr.name;
		const auto & namespaze = scope.get_namespace();
		
		const auto * var = scope.resolve(expr.name, expr.location);
		if(var && expr.namespaze.empty()) {
			return compile_read_local_variable(*var);
		}
		
		const auto * def = info.definition_table.resolve(
			name,
			namespaze,
			expr.namespaze);
		
		if(def) {
			InstructionBuffer buf;
			buf << inst::global_read(def->id);
			return InstructionBuffer{ buf };
		}
		
		throw CompilerError {
			"Undefined variable " + expr.namespaze.to_string() + name,
			expr.location
		};
	}

	
	InstructionBuffer compile_expr(
		const sst::Member & access,
		CompilerInfo & info,
		Scope & scope) {

		InstructionBuffer buf;
		buf << compile_expression(*access.expr, info, scope).code;
		const auto id = info.member_table.get_id(access.name);
		buf << inst::member_read(id);
		return InstructionBuffer{ buf };
	}
}