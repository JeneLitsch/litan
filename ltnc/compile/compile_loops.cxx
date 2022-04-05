#include "compile.hxx"

namespace ltn::c::compile {
	StmtCode while_loop(const ast::While & stmt, CompilerInfo & info, Scope & scope) {
		// outer scope of loop 
		Scope loop_scope{&scope}; 
		
		// compile parts
		const auto condition = expression(*stmt.condition, info, scope);
		const auto body = statement(*stmt.body, info, loop_scope);
		const auto id = make_jump_id("WHILE");
		const auto begin = id + "_BEGIN";
		const auto end = id + "_END";

		// generate asm code
		std::stringstream ss;
		ss 	<< inst::jumpmark(begin)
			<< condition.code
			<< inst::ifelse(end)
			<< body.code
			<< inst::jump(begin)
			<< inst::jumpmark(end);

		return {ss.str(), body.var_count};
	}



	StmtCode infinite_loop(const ast::InfiniteLoop & stmt, CompilerInfo & info, Scope & scope) {
		// outer scope of loop 
		Scope loop_scope{&scope}; 
		
		// compile parts
		const auto body = statement(*stmt.body, info, loop_scope);
		const auto jump = make_jump_id("INFINETE_LOOP");

		// generate asm code
		std::stringstream ss;
		ss 	<< inst::jumpmark(jump)
			<< body.code
			<< inst::jump(jump);

		return {ss.str(), body.var_count};
	}



	StmtCode for_loop(const ast::For & stmt, CompilerInfo & info, Scope & scope) {
		// outer scope of loop 
		Scope loop_scope{&scope};

		// compile parts
		const auto var = new_const(*stmt.var, info, loop_scope);
		const auto from = expression(*stmt.from, info, loop_scope);
		const auto to = expression(*stmt.to, info, loop_scope);
		
		const auto loop_id = make_jump_id("FOR");
		const auto begin = loop_id + "_BEGIN";
		const auto end = loop_id + "_END";

		// get address of index var
		const auto i_var    = loop_scope.resolve(stmt.var->name, stmt.location).address;
		const auto from_var = loop_scope.insert("__FROM_" + loop_id + "__", stmt.location).address;
		const auto to_var   = loop_scope.insert("__TO_" + loop_id + "__", stmt.location).address;
		const auto body = statement(*stmt.body, info, loop_scope);
				
		std::stringstream ss;
		
		// Init
		ss << to.code;
		ss << from.code;
		ss << inst::duplicate;
		ss << inst::write_x(i_var);
		ss << inst::write_x(from_var);
		ss << inst::write_x(to_var);

		// Condition
		ss << inst::jumpmark(begin);
		ss << inst::read_x(to_var);
		ss << inst::read_x(from_var);
		ss << inst::read_x(i_var);
		ss << inst::between;
		ss << inst::ifelse(end);

		// body
		ss << body.code;

		// Increments
		ss << inst::read_x(i_var);
		if(stmt.step) {
			ss << expression(*stmt.step, info, loop_scope).code;
			ss << inst::add;
		}
		else {
			ss << inst::inc;
		}
		ss << inst::write_x(i_var);

		// End of loop
		ss << inst::jump(begin);
		ss << inst::jumpmark(end);

		return StmtCode{ss.str(), body.var_count + 3};
	}
}