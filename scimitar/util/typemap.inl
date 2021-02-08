#pragma once

#include "typemap.h"
#include "algorithm.h"

namespace scimitar::util {
	template <typename T>
	void Typemap::insert(T* ptr) {
		std::type_index key(typeid(T));

		if (!contains(m_Keys, key)) {
			m_Keys  .push_back(key);
			m_Values.push_back(ptr);
		}
		else
			throw std::runtime_error("Cannot store multiple pointers of the same type");
	}

	template <typename T>
	void Typemap::remove(T* ptr) {
		std::type_index key(typeid(T));

		auto it = find(m_Keys, key);

		if (it != m_Keys.end()) {
			size_t position = std::distance(std::begin(m_Keys), it);

			m_Keys.erase(it);
			m_Values.erase(std::begin(m_Values) + position);
		}
		else
			throw std::runtime_error("Type could not be found in this container");
	}

	template <typename T>
	T* Typemap::get() const {
		std::type_index key(typeid(T));
		auto it = find(m_Keys, key);

		if (it == std::end(m_Keys))
			return nullptr;
		else {
			size_t position = std::distance(std::begin(m_Keys), it);

			return static_cast<T*>(std::begin(m_Values) + position);
		}
	}
}