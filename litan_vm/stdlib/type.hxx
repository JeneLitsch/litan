#pragma once
#include "litan_vm/Value.hxx"
#include "litan_vm/native/native.hxx"


namespace ltn::vm::stdlib {
	struct id {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct clone {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct is {
		constexpr static inline std::uint64_t arity = 2;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct cast {
		constexpr static inline std::uint64_t arity = 2;
		static Value func(ltn_Context * context, const Value * args);
	};



	struct null_new {
		constexpr static inline std::uint64_t arity = 0;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct null_is {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};



	struct stop_new {
		constexpr static inline std::uint64_t arity = 0;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct stop_is {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct stop_cast {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};



	struct type_is {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};

	struct type_cast {
		constexpr static inline std::uint64_t arity = 1;
		static Value func(ltn_Context * context, const Value * args);
	};
}