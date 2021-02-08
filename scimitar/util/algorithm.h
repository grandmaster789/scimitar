#pragma once

#include <vector>
#include <memory>

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

	template <typename tContainer>
	void sort(tContainer& c);

	template <typename tContainer>
	void uniquify(tContainer& c); // eliminates duplicate values from a container (also sorts it)
}

#include "algorithm.inl"