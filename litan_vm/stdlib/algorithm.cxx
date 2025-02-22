#include "algorithm.hxx"
#include "litan_vm/utils/compare.hxx"
#include "litan_vm/utils/function.hxx"
#include "litan_vm/Exception.hxx"
#include <algorithm>
#include <numeric>

namespace ltn::vm::stdlib {
	static auto to_cpp_range(const Value ref) {
		if(is_array(ref)) {
			Array * array = value::as<Array>(ref);
			const auto begin = std::begin(*array);
			const auto end = std::end(*array);
			return std::make_pair(begin, end);
		}

		throw except::invalid_argument();
	}



	auto smaller(VMCore & core) {
		return [&core] (const Value l, const Value r) {
			return compare(l, r, core) < 0;
		};
	}



	auto bigger(VMCore & core) {
		return [&core] (const Value l, const Value r) {
			return compare(l, r, core) > 0;
		};
	}



	auto predicate(VMCore & core, const Value key) {
		return [&core, key] (const Value elem) {
			return compare(elem, key, core) == 0;
		};
	}




	// Algorithms
	Value sort_desc::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		const auto comp = bigger(core);
		std::sort(begin, end, comp);
		return value::null;
	}



	Value sort_ascn::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		const auto comp = smaller(core);
		std::sort(begin, end, comp);
		return value::null;
	}



	Value is_sorted_ascn::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		const auto comp = smaller(core);
		return value::boolean(std::is_sorted(begin, end, comp));
	}



	Value is_sorted_desc::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		const auto comp = bigger(core);
		return value::boolean(std::is_sorted(begin, end, comp));
	}



	Value find::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto key = args[1];
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		const auto pred = predicate(core, key);
		const auto found = std::find_if(begin, end, pred);
		if(found == end) {
			return value::null;
		}
		else {
			const auto dist = std::distance(begin, found); 
			return value::integer(dist);
		}
	}


	
	Value fill::func(ltn_Context * context, const Value * args) {
		const auto value = args[1];
		const auto [begin, end] = to_cpp_range(args[0]);
		std::fill(begin, end, value);
		return value::null;
	}



	Value reverse::func(ltn_Context * context, const Value * args) {
		const auto ref = args[0];
		const auto [begin, end] = to_cpp_range(ref);
		std::reverse(begin, end);
		return value::null;
	}



	namespace {
		Value reduce_l_impl(VMCore & core, const Value & function, Value value, Iterator & iter) {
			while(true) {
				auto elem = iter.next();
				if(is_iterator_stop(elem)) break;
				core.stack.push(value);
				const std::array<Value, 2> inner_args = { value, elem };
				value = invoke_function_immediatly(core, function, inner_args.data(), inner_args.size());
				core.stack.pop();
			}

			return value;
		}
	}



	Value reduce_l_2::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		const auto function = args[1];
		const auto container = args[0];

		auto iter_ref = iterator::wrap(container, core);
		Iterator * iter = value::as<Iterator>(iter_ref);

		auto first = iter->next();
		if(is_iterator_stop(first)) return value::null;

		core.stack.push(function);
		core.stack.push(iter_ref);

		auto result = reduce_l_impl(core, function, first, *iter);

		core.stack.pop();
		core.stack.pop();

		return result;
	}



	Value reduce_l_3::func(ltn_Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		auto init = args[2];
		const auto function = args[1];
		const auto container = args[0];
		auto iter_ref = iterator::wrap(container, core);
		Iterator * iter = value::as<Iterator>(iter_ref);

		core.stack.push(init);
		core.stack.push(iter_ref);
		core.stack.push(function);

		auto result = reduce_l_impl(core, function, init, *iter);

		core.stack.pop();
		core.stack.pop();
		core.stack.pop();

		return result;
	}
}