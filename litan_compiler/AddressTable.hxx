#pragma once
#include <unordered_map>
#include <cstdint>
#include <string>

namespace ltn::c {
	using AddressTable = std::unordered_map<std::string, std::uint64_t>;
}