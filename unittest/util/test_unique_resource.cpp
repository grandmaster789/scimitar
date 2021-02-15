#include "../unittest.h"

namespace {
	struct Foo {
		int i = 1;
		
		~Foo() {
			i = 2;
		}
	};

	Foo alloc_foo() {
		return { 3 };
	}

	void dealloc_foo(Foo& f) {
		f.~Foo();
	}
}

namespace test {
	using scimitar::util::UniqueResource;

	TEST(util, unique_resource) {
		UniqueResource foo(alloc_foo(), dealloc_foo);
	}
}