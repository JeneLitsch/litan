#include "analyze.hxx"
#include "stdxx/functional.hxx"
#include "ltnc/sst/expr/Literal.hxx"

namespace ltn::c {
	std::vector<sst::defn_ptr> analyze_enumeration(const ast::Enumeration & enumeration, std::uint64_t & id) {

		auto namespaze = enumeration.namespaze;
		namespaze.push_back(enumeration.name);

		return stx::fx::mapped([&] (const auto & label) {
			return std::make_unique<sst::Definition>(
				label.name, namespaze,
				std::make_unique<sst::Integer>(label.value),
				id++
			);
		}) (enumeration.labels);
	}
}