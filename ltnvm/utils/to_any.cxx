#include "ltnvm/utils/to_any.hxx"
#include "ltnvm/utils/type_check.hxx"

namespace ltn::vm {
	Any to_any(const Value & value, Heap &) {
		return to_any(value);
	}
	
	Any to_any(const Value & value) {
		if(is_null(value)) {
			return Any{};
		}

		if(is_bool(value)) {
			return Any{value.b};
		}
		
		if(is_char(value)) {
			return Any{value.c};
		}

		if(is_int(value)) {
			return Any{value.i};
		}

		if(is_float(value)) {
			return Any{value.f};
		}

		if(is_string(value)) {
			return Any{value::as<String>(value)->get_underlying()};
		}

		if(is_contiguous(value)) {
			auto & array = *value::as<Contiguous>(value);
			std::vector<Any> vector;
			for(const auto & elem : array) {
				vector.push_back(to_any(elem));
			}
			return Any{vector};
		}

		return Any{};
	}
}