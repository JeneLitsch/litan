#include "compile.hxx"

namespace ltn::c {
	InstructionBuffer compile_expr(const sst::InitStruct & init) {
		InstructionBuffer buf;
		buf << inst::newstruct();
		for(const auto & [member, expr, is_operator] : init.members) {
			buf << inst::duplicate();
			buf << compile_expression(*expr);
			buf << inst::swap();
			if(is_operator) {
				buf << inst::operator_write(static_cast<MemberCode>(member));
			}
			else {
				buf << inst::member_write(member);
			}
		}
		return buf;
	}
}