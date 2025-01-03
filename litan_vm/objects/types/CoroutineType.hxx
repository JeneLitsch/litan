#pragma once

#include "TypeObject.hxx"

namespace ltn::vm {
	class CoroutineType : public StaticObject {
	public:
		virtual Value get_member(VMCore & core, std::uint64_t id) const override;
		Value get_nonstatic_member(VMCore & core, std::uint64_t id) const;
	};
}