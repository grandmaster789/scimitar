#pragma once

#include "unique_resource.h"

namespace scimitar::util {
	template <typename R, typename D>
	UniqueResource<R, D>::UniqueResource(
		R&& resource, 
		D&& deleter
	)
		noexcept
	:
		m_Resource(std::forward<R>(resource)),
		m_Deleter (std::forward<D>(deleter))
	{
	}

	template <typename R, typename D>
	UniqueResource<R, D>::~UniqueResource() noexcept {
		reset();
	}

	template <typename R, typename D>
	UniqueResource<R, D>::UniqueResource(UniqueResource&& ur) 
		noexcept(std::is_nothrow_move_constructible_v<R>)
	:
		m_Resource(std::move(*ur.m_Resource)),
		m_Deleter (std::forward<D>(ur.m_Deleter))
	{
		ur.m_Resource = std::nullopt;
	}

	template <typename R, typename D>
	UniqueResource<R, D>& UniqueResource<R, D>::operator = (UniqueResource&& ur)
		noexcept(std::is_nothrow_move_assignable_v<R>)
	{
		reset();
	}

	template <typename R, typename D>
	void UniqueResource<R, D>::reset() noexcept {
		if (m_Resource)
			m_Deleter(*m_Resource);
	}

	template <typename R, typename D>
	R UniqueResource<R, D>::release() noexcept {
		if (m_Resource) {
			R result = std::move(*m_Resource);
			
			m_Resource = std::nullopt;

			return result;
		}

		return {};
	}

	template <typename R, typename D>
	const R& UniqueResource<R, D>::operator * () const noexcept {
		return *m_Resource;
	}

	template <typename R, typename D>
	R* UniqueResource<R, D>::operator ->() const noexcept {
		return &*m_Resource;
	}

	template <typename R, typename D>
	const R& UniqueResource<R, D>::get() const noexcept {
		return *m_Resource;
	}

	template <typename R, typename D>
	const D& UniqueResource<R, D>::getDeleter() const noexcept {
		return m_Deleter;
	}
}
