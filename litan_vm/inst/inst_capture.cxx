#include "instructions.hxx"
#include "litan_vm/utils/type_check.hxx"
#include <sstream>
namespace ltn::vm::inst {
	void capture(VMCore & core) {
		const auto var = core.stack.pop();
		const auto fxptr = core.stack.peek();
		if(is_fxptr(fxptr)) {
			auto & lambda = *value::as<FunctionPointer>(fxptr);
			lambda.captured.push_back(var);
		}
		else throw except::invalid_argument();
	}
}