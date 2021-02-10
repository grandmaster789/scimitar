#pragma once

#include <cstdint>
#include <array>

namespace scimitar::util {
	template <typename> class Function;
	template <
		typename    tReturn,
		typename... tArgs
	>
	struct Function<tReturn(tArgs...)> final {
		using FnPtr = tReturn(*)(void*, tArgs&&...); // type-erased function pointer

		void* const m_Instance;
		const FnPtr m_FunctionPtr;

		template <auto FunctionPtr>
		static Function bind();

		template <auto MemberPtr, typename Object>
		static Function bind(Object* obj);

		template <typename Lambda>
		static Function bind(Lambda* ptr);

		template <typename...Xs>
		tReturn operator()(Xs&&... args) const;
	};
}

#include "function.inl"