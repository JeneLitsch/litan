#pragma once
#include <vector>
#include "Object.hxx"

namespace ltn::vm {
	struct Core;
	struct FunctionPointer : Object {

		virtual void stringify(VMCore & core, std::ostream & oss, bool nested) override;
		virtual Value get_member(VMCore & core, std::uint64_t id) const override;

		FunctionPointer(std::uint64_t parameter_count, bool is_variadic = false)
			: Object{}
			, params{ parameter_count }
			, is_variadic {is_variadic} {} 
		std::uint64_t params;
		std::uint64_t arity() const {
			return this->params;
		}
		bool is_variadic;

		std::vector<Value> captured;

		virtual ~FunctionPointer() = default;
	};
}