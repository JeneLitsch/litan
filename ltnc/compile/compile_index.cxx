#include "compile.hxx"
#include <string_view>
namespace ltn::c {
	// compiles index read operation
	InstructionBuffer compile_expr(
		const sst::Index & index,
		CompilerInfo & info,
		Scope & scope) {

		const auto arr = compile_expression(*index.expression, info, scope);
		const auto idx = compile_expression(*index.index, info, scope);
		
		InstructionBuffer buf;
		
		buf	
			<< arr.code
			<< idx.code
			<< inst::at();

		const auto type = type::deduce_index(arr.deduced_type, idx.deduced_type);

		return InstructionBuffer{ 
			.code = buf,
			.deduced_type = type,
		};
	}
}