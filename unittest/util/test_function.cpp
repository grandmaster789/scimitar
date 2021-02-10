#include "../unittest.h"

namespace {
	void increment_arg(int& x) {
		x++;
	}

	struct Foo {
		int i = 0;

		void increment_i() {
			++i;
		}
	};
}

namespace test {
	using scimitar::util::Function;

	TEST(util, function_global) {
		auto fn = Function<void(int&)>::bind<&increment_arg>();

		int x = 5;

		fn(x);

		ASSERT_EQ(x, 6);

		// alternatively (via lambda-compatible binding)
		auto fn2 = Function<void(int&)>::bind(&increment_arg);

		fn2(x);

		ASSERT_EQ(x, 7);
	}

	TEST(util, function_member) {
		Foo foo;

		auto fn = Function<void()>::bind<&Foo::increment_i>(&foo);

		fn();
		fn();
		fn();

		ASSERT_EQ(foo.i, 3);
	}

	TEST(util, function_lambda) {
		int value = 5;

		// capture something with the lambda so it should generate a functor object
		auto x = [=] {
			return value;
		};

 		auto fn = Function<int()>::bind(&x);

		ASSERT_EQ(fn(), 5);
  	}
}