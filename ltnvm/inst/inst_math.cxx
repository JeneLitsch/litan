#include "instructions.hxx"
#include "ltnvm/type_check.hxx"
#include <cmath>
#include "ltnvm/MathFunctions.hxx"
#include "ltnvm/convert.hxx"
#include "ltnvm/Operations.hxx"

namespace ltn::vm::inst {
	#define FETCH\
		const auto r = core.reg.pop();\
		const auto l = core.reg.pop();


	bool less(const Value & l, const Value & r) {
		return convert::to_float(l) <= convert::to_float(r);
	}





	template<class Fx>
	Value function(const Value & value) {
		constexpr Fx fx{};
		if(is_float(value)) {
			return Value{fx(value.f)};
		}
		if(is_int(value)) {
			return Value{fx(value.i)};
		}
		if(is_bool(value)) {
			return Value{fx(value.b)};
		}
		if(is_char(value)) {
			return Value{fx(value.c)};
		}
		throw except::invalid_argument();
	}






	void abs(VmCore & core) {
		core.reg.push(function<Absolute>(core.reg.pop()));
	}


	void hypot(VmCore & core) {
		FETCH
		core.reg.push(std::hypot(
			convert::to_float(l),
			convert::to_float(r)));
	}


	void sqrt(VmCore & core) {
		const auto value = core.reg.pop();
		core.reg.push(std::sqrt(convert::to_float(value)));
	}


	void log(VmCore & core) {
		FETCH
		const auto result 
			= std::log(convert::to_float(l))
			/ std::log(convert::to_float(r));
		core.reg.push(result);
	}


	void ln(VmCore & core) {
		const auto value = core.reg.pop();
		core.reg.push(std::log(convert::to_float(value)));
	}



	

	void sin(VmCore & core) {
		core.reg.push(function<Sinus>(core.reg.pop()));
	}


	void cos(VmCore & core) {
		core.reg.push(function<Cosinus>(core.reg.pop()));
	}


	void tan(VmCore & core) {
		core.reg.push(function<Tangents>(core.reg.pop()));
	}
	

	#undef FETCH
}