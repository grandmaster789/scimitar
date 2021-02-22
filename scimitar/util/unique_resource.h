#pragma once

#include <utility>
#include <optional>

namespace scimitar::util {
	template <
		typename tResource, 
		typename tDeleter
	>
	class UniqueResource {
	public:
		explicit UniqueResource(
			tResource&& resource,
			tDeleter&&  deleter
		) noexcept;
		~UniqueResource() noexcept;

		UniqueResource             (const UniqueResource&) = delete;
		UniqueResource& operator = (const UniqueResource&) = delete;
		UniqueResource             (UniqueResource&& ur) noexcept(std::is_nothrow_move_constructible_v<tResource>);
		UniqueResource& operator = (UniqueResource&& ur) noexcept(std::is_nothrow_move_assignable_v<tResource>);
		
		void reset() noexcept;
		
		      tResource release() noexcept; // will default-construct if the resource was nullopt

		const tResource& operator * () const noexcept;
		      tResource* operator ->() const noexcept;

		const tResource& get()        const noexcept;
		const tDeleter&  getDeleter() const noexcept;

	private:
		std::optional<tResource> m_Resource;
		tDeleter                 m_Deleter;
	};
}

#include "unique_resource.inl"