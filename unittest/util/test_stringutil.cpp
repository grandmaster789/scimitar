#include "../unittest.h"

namespace test {
	using namespace scimitar::util;

	TEST(util, str_to_upperlower) {
		ASSERT_EQ(to_lower("AbCDeF\nABCdef"), "abcdef\nabcdef");
		ASSERT_EQ(to_upper("AbCDeF\nABCdef"), "ABCDEF\nABCDEF");
	}

	TEST(util, normalize_feeds) {
		ASSERT_EQ(normalize_linefeeds("ping\npong\nfoo\nbar"),     "ping\npong\nfoo\nbar");
		ASSERT_EQ(normalize_linefeeds("ping\r\npong\nfoo\r\nbar"), "ping\npong\nfoo\nbar");
		ASSERT_EQ(normalize_linefeeds("ping\npong\r\nfoo\nbar"),   "ping\npong\nfoo\nbar");
	}

	TEST(util, str_split) {
		{
			std::string x = "abacadabra";

			auto spliced = split(x, "a"); // ~> ["b", "c", "d", "br"]

			ASSERT_EQ(spliced.size(), 4); 
		}

		{
			auto spliced = split("foo/bar/baz", "/");

			std::vector<std::string> expected = {
				"foo",
				"bar",
				"baz"
			};

			ASSERT_EQ(spliced, expected);
		}
	}

	TEST(util, stringify) {
		auto s = stringify(1, 2, 3);
		ASSERT_EQ(s, "123");
	}

	TEST(util, columnize) {
		auto col = columnize("123451234512345", 5);
		ASSERT_EQ(col, "12345\n12345\n12345\n");
	}

	TEST(util, concat) {
		std::vector<std::string>      str_parts  = { "123", "456", "789" };
		std::vector<const char*>      char_parts = { "abc", "def", "ghi" };
		std::vector<std::string_view> sv_parts   = { "aaa", "bbb", "ccc" };

		auto con_str  = concat(str_parts);
		auto con_char = concat(char_parts);
		auto con_sv   = concat(sv_parts);

		ASSERT_EQ(con_str,  "123456789");
		ASSERT_EQ(con_char, "abcdefghi");
		ASSERT_EQ(con_sv,   "aaabbbccc");
	}
}