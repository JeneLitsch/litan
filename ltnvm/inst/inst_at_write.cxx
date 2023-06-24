#include "instructions.hxx"
#include "ltnvm/utils/index.hxx"
#include "ltnvm/utils/convert.hxx"

namespace ltn::vm::inst {
	void at_write(VmCore & core) {
		const auto key = core.stack.pop();
		const auto ref = core.stack.pop();
		const auto elem = core.stack.pop();
		
		if(is_array(ref) || is_tuple(ref)) {
			auto & arr = core.heap.read<Array>(ref.u);
			const auto index = to_index(key, std::size(arr.data));
			arr.data[static_cast<std::size_t>(index)] = elem;
			return;
		}

		if(is_string(ref)) {
			auto & str = core.heap.read<String>(ref.u);
			const auto index = to_index(key, std::size(str.data));
			str.data[static_cast<std::size_t>(index)] = convert::to_char(elem);
			return;
		}

		if(is_map(ref)) {
			auto & map = core.heap.read<Map>(ref.u);
			map[key] = elem;
			return;
		}

		throw except::invalid_argument();
	}
}