#include "algorithm.hxx"
#include "ltnvm/utils/convert.hxx"
#include "ltnvm/Exception.hxx"
#include "ltnvm/utils/index.hxx"
#include "ltnvm/inst/instructions.hxx"
#include "ltnvm/utils/stringify.hxx"

namespace ltn::vm::build_in {
	Value pop(VMCore & core) {
		const auto ref = core.stack.pop();

		if(is_array(ref)) {
			Array * container = value::as<Array>(ref);
			if(std::empty(*container)) throw except::out_of_range();
			const auto elem = container->back();
			container->pop_back();
			return elem;
		}

		if(is_string(ref)) {
			String * container = value::as<String>(ref);
			if(std::empty(*container)) throw except::out_of_range();
			const auto elem = container->back();
			container->pop_back();
			return value::character(elem);
		}

		if(is_queue(ref)) {
			Segmented * container = value::as<Segmented>(ref);
			if(std::empty(*container)) throw except::out_of_range();
			const auto elem = container->front();
			container->pop_front();
			return elem;
		}
	
		throw except::invalid_argument();
	}
}