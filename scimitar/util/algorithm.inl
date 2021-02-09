#include "algorithm.h"
#include <algorithm>

namespace scimitar::util {
	template <
		template <typename, typename> class C, 
		typename T, 
		typename A,
		typename B
	>
	C<T*, B> create_non_owning_copy(const C<std::unique_ptr<T>, A>& container) {
		C<T*, B> result;

		// TODO possibly we could try and figure out if the container has a .reserve() method instead
		if constexpr (std::is_same_v<std::vector<T*>, decltype(result)>)
		{
			result.reserve(container.size());
		}

		for (auto& entry : container)
			result.push_back(entry.get());

		return result;
	}

	template <typename T>
	std::vector<T> raw_to_vector(T* raw_array, size_t num_elements) {
		// vector has 10 constructors, one of which does what we need here...
		// https://en.cppreference.com/w/cpp/container/vector/vector (#5)
		return std::vector<T>(
			raw_array, 
			raw_array + num_elements
		);
	}

	template <typename C, typename E>
	bool contains(const C& c, const E& x) {
		return std::find(
			std::begin(c), 
			std::end(c), 
			x
		) != std::end(c);
	}

	template <typename C, typename P>
	bool contains_if(
		const C& c,
		      P  pred
	) {
		return std::find_if(
			std::begin(c),
			std::end(c),
			pred
		) != std::end(c);
	}

	template <typename C>
	bool contains_all_of(
		const C& a,
		const C& b
	) {
		for (const auto& value : b)
			if (!contains(a, value))
				return false;

		return true;
	}

	template <typename C, typename E>
	typename C::const_iterator find(
		const C& c,
		const E& value
	) {
		return std::find(
			std::begin(c),
			std::end(c),
			value
		);
	}

	template <typename C, typename Pred>
	typename C::const_iterator find_if(
		const C& c,
		Pred     pred
	) {
		return std::find_if(
			std::begin(c),
			std::end(c),
			pred
		);
	}

	template <typename C>
	void sort(C& c) {
		std::sort(
			std::begin(c), 
			std::end(c)
		);
	}

	template <typename C>
	void uniquify(C& c) {
		sort(c);

		auto it = std::unique(
			std::begin(c),
			std::end(c)
		);

		c.erase(
			it, 
			std::end(c)
		);
	}
}