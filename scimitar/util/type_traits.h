#pragma once

#include <type_traits>

namespace scimitar::util {
	template <typename T>
	struct TypeIdentity {
		using type = T;
	};

	template <typename T>
	using type_identity_t = typename TypeIdentity<T>::type;

	// remove reference, const and volatile qualifiers
	template <typename T>
	using noqualifier_t = std::remove_cv_t<std::remove_reference_t<T>>;

	template <typename T>
	using true_t = std::integral_constant<bool, std::is_same_v<T, T>>;

	template <typename T>
	using false_t = std::integral_constant<bool, !std::is_same_v<T, T>>;
}