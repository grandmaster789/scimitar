#include "../unittest.h"

namespace test {
	using namespace scimitar::util;

	TEST(util, biguint) {
		{
			ubig128 a = 1;
			ubig128 b(1);

			ASSERT_EQ(a, b);
		}

		{
			Biguint<uint64_t, 1> ubig64_1 = 0xAAAA'BBBB'CCCC'DDDDull;
			Biguint<uint32_t, 2> ubig32_2 = 0xAAAA'BBBB'CCCC'DDDDull;

			ASSERT_EQ(ubig64_1, ubig32_2);
		}
	}
}