#include "../unittest.h"

namespace test {
	using namespace scimitar::util;

	TEST(util, byteswap) {
		uint16_t u16 = 0xAABB;
		uint32_t u32 = 0xAABB'1122;
		uint64_t u64 = 0xAABB'1122'CCDD'3344;

		int16_t i16 = static_cast<int16_t>(0xAABB);
		int32_t i32 = static_cast<int32_t>(0xAABB'1122);
		int64_t i64 = static_cast<int64_t>(0xAABB'1122'CCDD'3344);

		uint16_t ex_u16 = 0xBBAA;
		uint32_t ex_u32 = 0x2211'BBAA;
		uint64_t ex_u64 = 0x4433'DDCC'2211'BBAA;

		int16_t ex_i16 = static_cast<int16_t>(0xBBAA);
		int32_t ex_i32 = static_cast<int32_t>(0x2211'BBAA);
		int64_t ex_i64 = static_cast<int64_t>(0x4433'DDCC'2211'BBAA);

		ASSERT_EQ(byte_swap(u16), ex_u16);
		ASSERT_EQ(byte_swap(u32), ex_u32);
		ASSERT_EQ(byte_swap(u64), ex_u64);

		ASSERT_EQ(byte_swap(i16), ex_i16);
		ASSERT_EQ(byte_swap(i32), ex_i32);
		ASSERT_EQ(byte_swap(i64), ex_i64);
	}
}