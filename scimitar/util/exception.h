#pragma once

#include <exception>

// top-level namespace to reduce clutter
namespace scimitar {
	class Exception:
		public std::exception
	{
	public:
		Exception() = default;
	};

	class AsyncException:
		public Exception
	{
	public:
		AsyncException() = default;
	};
}