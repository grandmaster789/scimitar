#pragma once

#include <array>
#include <array>
#include <vector>
#include <memory>
#include <tuple>
#include <iterator>

namespace scimitar::util {
	// 
	// vector<unique_ptr<T>>   ~~~>   vector<T*>
	// 
	// (not compatible with *all* STL containers, just all sequence containers that is: 
	//  array, vector, deque, list and forward_list. Slightly optimized for vector)
	// 
	template <
		template <typename, typename> class tContainer,
		typename                            tElement,
		typename                            tAllocatorA,
		typename                            tAllocatorB = std::allocator<tElement*>
	>
	tContainer<tElement*, tAllocatorB> create_non_owning_copy(
		const tContainer<std::unique_ptr<tElement>, tAllocatorA>& x
	);

	template <typename T>
	std::vector<T> raw_to_vector(
		T*     raw_array, 
		size_t num_elements
	);

	template <typename tContainer, typename tElement>
	bool contains(
		const tContainer& c, 
		const tElement&   x
	);

	template <typename tContainer, typename tPredicate>
	bool contains_if(
		const tContainer& c, 
		      tPredicate  pred
	);

	template <typename tContainer>
	bool contains_all_of(
		const tContainer& a,
		const tContainer& b
	);

	template <typename tContainer, typename tElement>
	typename tContainer::const_iterator find(
		const tContainer& c,
		const tElement&   value
	);

	template <typename tContainer, typename tPredicate>
	typename tContainer::const_iterator find_if(
		const tContainer& c,
		      tPredicate  pred
	);

	template <typename tIteratorA, typename tIteratorB>
	constexpr tIteratorA find_any_of(
		tIteratorA haystack_first,
		tIteratorA haystack_last,
		tIteratorB needles_first,
		tIteratorB needles_last
	) noexcept; // returns haystack_last if none was found

	template <typename tContainer>
	void sort(tContainer& c);

	template <typename tContainer>
	void uniquify(tContainer& c); // eliminates duplicate values from a container (also sorts it)

	template <typename T, typename U>
	bool assign_if_changed(T& old_value, U&& new_value) noexcept; // returns true if the value was changed

	template <
		typename tContainerA,
		typename tContainerB,
		typename tFunctor
	>
	tContainerA transform(
		const tContainerB& source,
		tFunctor fn
	);

	template <typename C, typename V>
	constexpr void fill(C& container, const V& value);

	template <
		typename T,
		size_t   N,
		typename tFunctor // fn(size_t) -> T
	>
	constexpr std::array<T, N> generate_array(tFunctor fn);
}

#include "algorithm.inl"