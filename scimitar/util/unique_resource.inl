#pragma once

#include "unique_resource.h"

namespace scimitar::util {
	template <typename T, typename D>
	constexpr UniqueResource<T, D>::UniqueResource(
		T&& handle,
		D&& deleter
	):
		m_Handle (std::forward<T>(handle)),
		m_Deleter(std::forward<D>(deleter))
	{
	}

	template <typename T, typename D>
	UniqueResource<T, D>::~UniqueResource() {
		if (m_Handle)
			m_Deleter(*m_Handle);
	}

	template <typename T, typename D>
	constexpr UniqueResource<T, D>::UniqueResource(UniqueResource&& ur) noexcept(
		std::is_nothrow_constructible_v<T>&&
		std::is_nothrow_constructible_v<D>
	):
		m_Handle (std::move(ur.m_Handle)),
		m_Deleter(std::move(ur.m_Deleter))
	{
	}

	template <typename T, typename D>
	constexpr UniqueResource<T, D>::UniqueResource& operator = (UniqueResource&& ur) noexcept(
		std::is_nothrow_move_assignable_v<T>&&
		std::is_nothrow_move_assignable_v<D>
	) {
		if (m_Handle)
			m_Deleter(*m_Handle);

		m_Handle  = std::move(ur.m_Handle);
		m_Deleter = std::move(ur.m_Deleter);

		return *this;
	}

	template <typename T, typename D>
	constexpr T UniqueResource<T, D>::get() const noexcept {
		return *m_Handle;
	}

	template <typename T, typename D>
	constexpr T UniqueResource<T, D>::operator -> () const noexcept {
		return *m_Handle;
	}

	template <typename T, typename D>
	constexpr void UniqueResource<T, D>::release() {
		m_Handle.release();
	}

	template <typename T, typename D>
	constexpr D& UniqueResource<T, D>::getDeleter() {
		return m_Deleter;
	}

	template <typename T, typename D>
	constexpr const D& UniqueResource<T, D>::getDeleter() const {
		return m_Deleter;
	}
}
