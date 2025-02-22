#include "litan_vm/utils/cast.hxx"
#include "type_check.hxx"
#include "litan_vm/Exception.hxx"
#include "convert.hxx"
#include "stdxx/float64_t.hxx"

namespace ltn::vm::cast {
	std::string to_string(Value value) {
		return convert::to_string(value);
	}


	Array to_array(Value value) {
		return Array{{value}};
	}

	std::int64_t to_int(Value value) {
		if(is_float(value)) {
			return static_cast<std::int64_t>(value.f);
		}
		
		return convert::to_int(value);
	}



	stx::float64_t to_float(Value value) {
		if (is_clock(value)) {
			const auto & clock = *value::as<Clock>(value);
			return clock.getSeconds();
		}

		return convert::to_float(value);
	}


	bool to_bool(Value value, VMCore & core) {
		return convert::to_bool(value, core);
	}
}