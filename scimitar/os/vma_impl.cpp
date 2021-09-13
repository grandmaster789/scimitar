#include "../preprocessor.h"

// not my code; assume that it's all good and suppress warnings
#if SCIMITAR_COMPILER == SCIMITAR_COMPILER_MSVC
	#pragma warning(disable: 4100) // unreferenced format parameter
	#pragma warning(disable: 4127) // conditional expression is constant
	#pragma warning(disable: 4189) // local variable is initialized but not referenced
	#pragma warning(disable: 4324) // structure was padded due to declspec
#endif

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>