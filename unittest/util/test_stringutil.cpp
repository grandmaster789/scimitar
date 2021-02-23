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
			auto spliced = split("foo/bar/baz", "/b");

			std::vector<std::string> expected = {
				"foo",
				"ar",
				"az"
			};

			ASSERT_EQ(spliced, expected);
		}
	}
}