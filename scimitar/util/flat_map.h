#pragma once

#include <vector>
#include <utility>
#include <iosfwd>

namespace scimitar::util {
	// Linear mapping, separate key and value storage; should be very fast for small-ish key/value stores
	// 
	// [NOTE] key must implement < and == operators; value should be at least moveable
	// [NOTE] for larger amounts of keys, a hash map is probably better

	template <
		typename tKey, 
		typename tValue
	>
	class FlatMap {
	public:
		// Full value semantics (default construct + destruct + copy + move)

		tValue*       operator[](const tKey& key);       // returns nullptr if not found
		const tValue* operator[](const tKey& key) const; // returns nullptr if not found

		std::pair<tKey, tValue> at(size_t index) const; // will throw if index is out of bounds

		void assign(const tKey& key, const tValue& value);
		void assign(const tKey& key, tValue&& value);

		[[nodiscard]] bool insert(const tKey& key, const tValue& value); // returns false if it would overwrite an entry
		[[nodiscard]] bool insert(const tKey& key, tValue&& value);      // returns false if it would overwrite an entry

		bool contains(const tKey& key) const;
		void erase(const tKey& key);
		void clear() noexcept;

		size_t size() const noexcept;
		bool is_empty() const noexcept;

		const std::vector<tKey>&   get_keys() const noexcept;
		const std::vector<tValue>& get_values() const noexcept;

		void foreach(std::function<void(const tKey&, const tValue&)> fn) const;

		friend std::ostream& operator << (std::ostream& os, const FlatMap& fm);

	private:
		std::vector<tKey>   m_Keys;
		std::vector<tValue> m_Values;
	};
}

#include "flat_map.inl"