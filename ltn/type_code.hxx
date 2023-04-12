#pragma once
#include <cstdint>

namespace ltn::type_code {
	inline constexpr std::uint8_t ANY      = 'x';
	inline constexpr std::uint8_t NVLL     = 'N';
	inline constexpr std::uint8_t OPTIONAL = '?';
	inline constexpr std::uint8_t BOOL     = 'b';
	inline constexpr std::uint8_t CHAR     = 'c';
	inline constexpr std::uint8_t INT      = 'i';
	inline constexpr std::uint8_t FLOAT    = 'f';
	inline constexpr std::uint8_t ARRAY    = 'a';
	inline constexpr std::uint8_t STRING   = 's';
	inline constexpr std::uint8_t ISTREAM  = '>';
	inline constexpr std::uint8_t OSTREAM  = '<';
}