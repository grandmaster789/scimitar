#pragma once

#include <utility>
#include <optional> // may make my own at some point

namespace scimitar::util {
	template <
		typename tHandle,
		typename tDeleter
	>
	class UniqueResource {
	public:
		constexpr UniqueResource() = default; // initializes the handle to std::none_t
		constexpr UniqueResource(
			tHandle&&  handle,
			tDeleter&& deleter = {}
		);
		~UniqueResource();

		// no-copy, custom-move (transfer deletion responsibility)
		constexpr UniqueResource             (const UniqueResource&) = delete;
		constexpr UniqueResource& operator = (const UniqueResource&) = delete;

		// NOTE exception specification is a bit tricky here
		constexpr UniqueResource(UniqueResource&& ur) noexcept(
			std::is_nothrow_constructible_v<tHandle>  &&
			std::is_nothrow_constructible_v<tDeleter>
		);

		constexpr UniqueResource& operator = (UniqueResource&& ur) noexcept(
			std::is_nothrow_move_assignable_v<tHandle>  &&
			std::is_nothrow_move_assignable_v<tDeleter>
		);

		explicit constexpr operator bool() const noexcept; // yields false if handle is std::none_t

		constexpr       tHandle  get()          const noexcept;
		constexpr       tHandle* operator -> () const noexcept;
		constexpr const tHandle& operator * ()  const noexcept;
		
		constexpr void release();
		constexpr void reset();

	private:
		std::optional<tHandle> m_Handle;  // optional to allow default construction
		tDeleter               m_Deleter;
	};
}

#include "unique_resource.inl"