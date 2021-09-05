#pragma once

// some more information about architecture and platform macros can be found here: https://abseil.io/docs/cpp/platforms/macros

// lay down some constants
#define SCIMITAR_PLATFORM_WINDOWS 1
#define SCIMITAR_PLATFORM_LINUX 2

// try to autodetect the platform
#ifdef _WIN32
	#define SCIMITAR_PLATFORM SCIMITAR_PLATFORM_WINDOWS
#else // this is very crude
	#define SCIMITAR_PLATFORM SCIMITAR_PLATFORM_LINUX
#endif

// perform platform-specific fixes
#if SCIMITAR_PLATFORM == SCIMITAR_PLATFORM_WINDOWS

	// WIN32_LEAN_AND_MEAN may already be defined via the CMake script
	// (it disables a lot of headers from being pulled in when including windows.h)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#define NOGDICAPMASKS       // CC_ * , LC_*, PC_*, CP_*, TC_*, RC_
	//#define NOVIRTUALKEYCODES // VK_ *
	//#define NOWINMESSAGES     // WM_ * , EM_*, LB_*, CB_*
	//#define NOWINSTYLES       // WS_ * , CS_*, ES_*, LBS_*, SBS_*, CBS_*
	//#define NOSYSMETRICS      // SM_ *
	#define NOMENUS             // - MF_ *
	//#define NOICONS           // - IDI_ *
	#define NOKEYSTATES         // - MK_ *
	#define NOSYSCOMMANDS       // - SC_ *
	#define NORASTEROPS         // - Binary and Tertiary raster ops
	//#define NOSHOWWINDOW      // - SW_ *
	#define OEMRESOURCE         // - OEM Resource values
	#define NOATOM              // - Atom Manager routines
	#define NOCLIPBOARD         // - Clipboard routines
	#define NOCOLOR             // - Screen colors
	#define NOCTLMGR            // - Control and Dialog routines
	#define NODRAWTEXT          // - DrawText() and DT_*
	//#define NOGDI             // - All GDI defines and routines
	#define NOKERNEL            // - All KERNEL defines and routines
	//#define NOUSER            // - All USER defines and routines
	#define NONLS               // - All NLS defines and routines
	//#define NOMB              // - MB_ * and MessageBox()
	#define NOMEMMGR            // - GMEM_ * , LMEM_*, GHND, LHND, associated routines
	#define NOMETAFILE          // - typedef METAFILEPICT
	#define NOMINMAX            // - Macros min(a, b) and max(a, b)
	//#define NOMSG             // - typedef MSG and associated routines
	#define NOOPENFILE          // - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
	#define NOSCROLL            // - SB_ * and scrolling routines
	#define NOSERVICE           // - All Service Controller routines, SERVICE_ equates, etc.
	#define NOSOUND             // - Sound driver routines
	#define NOTEXTMETRIC        // - typedef TEXTMETRIC and associated routines
	#define NOWH                // - SetWindowsHook and WH_*
	//#define NOWINOFFSETS      // - GWL_ * , GCL_*, associated routines
	#define NOCOMM              // - COMM driver routines
	#define NOKANJI             // - Kanji support stuff.
	#define NOHELP              // - Help engine interface.
	#define NOPROFILER          // - Profiler interface.
	#define NODEFERWINDOWPOS    // - DeferWindowPos routines
	#define NOMCX               // - Modem Configuration Extensions

	#include <SDKDDKVer.h>
	#include <Windows.h>
	#include <intrin.h>
	#include <debugapi.h>

	#define VK_USE_PLATFORM_WIN32_KHR
#endif

// figure out what processor this is built for (really only tested on x64)
#define SCIMITAR_PROCESSOR_X64 1
#define SCIMITAR_PROCESSOR_ARM 2

#if defined(__x86_64__) || defined(__amd64__) || defined(_M_AMD64)
	#define SCIMITAR_PROCESSOR SCIMITAR_PROCESSOR_X64
#elif defined(__arm__) || defined(_M_ARM)
	#define SCIMITAR_PROCESSOR SCIMITAR_PROCESSOR_ARM
#else
	#error "Could not detect processor"
#endif

// Build types
#define SCIMITAR_BUILD_DEBUG   1
#define SCIMITAR_BUILD_RELEASE 2

// with MSVC we can figure out wheter this is a debug build
#ifdef _DEBUG
	#define SCIMITAR_BUILD SCIMITAR_BUILD_DEBUG
#else
	// assume release build in other cases
	#define SCIMITAR_BUILD SCIMITAR_BUILD_RELEASE
#endif

// NO_MINMAX should mean that these are not defined, but just in case
#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

// compiler related
#define SCIMITAR_COMPILER_MSVC  1
#define SCIMITAR_COMPILER_CLANG 2
#define SCIMITAR_COMPILER_GCC   3

#if defined(_MSC_BUILD)
	#define SCIMITAR_COMPILER SCIMITAR_COMPILER_MSVC

	#define unreachable() __assume(0)
	#define force_inline __forceinline
	#define no_inline __declspec(noinline)
#elif defined(__clang__)
	#define SCIMITAR_COMPILER SCIMITAR_COMPILER_CLANG

	#define unreachable()        __builtin_unreachable()
	#define force_inline  inline __attribute__((always_inline))
	#define no_inline            __attribute__((noinline))
#elif defined(__GNUC__)
	#define SCIMITAR_COMPILER SCIMITAR_COMPILER_GCC

	// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
	#define unreachable()        __builtin_unreachable()
	#define force_inline  inline __attribute__((always_inline))
	#define no_inline            __attribute__((noinline))
#else
	#error "unsupported compiler"
#endif

// make preprocessor stuff available 
namespace scimitar {
	enum class ePlatform {
		windows = SCIMITAR_PLATFORM_WINDOWS,
		linux   = SCIMITAR_PLATFORM_LINUX,

		current = SCIMITAR_PLATFORM
	};

	enum class eProcessor {
		x64 = SCIMITAR_PROCESSOR_X64,
		arm = SCIMITAR_PROCESSOR_ARM,

		current = SCIMITAR_PROCESSOR
	};

	enum class eCompiler {
		msvc  = SCIMITAR_COMPILER_MSVC,
		clang = SCIMITAR_COMPILER_CLANG,
		gcc   = SCIMITAR_COMPILER_GCC,

		current = SCIMITAR_COMPILER
	};

	enum class eBuild {
		debug   = SCIMITAR_BUILD_DEBUG,
		release = SCIMITAR_BUILD_RELEASE,

		current = SCIMITAR_BUILD
	};

	constexpr bool has_SSE = (eProcessor::current == eProcessor::x64);
	
	// try to figure out in more detail which instructions are actually available
	// most of the architecture flags are handled at the project root CMakeLists.txt
#if SCIMITAR_PROCESSOR == SCIMITAR_PROCESSOR_X64

#endif

	// these should be part of the standard, but right now aren't implemented by clang or gcc
#if SCIMITAR_PROCESSOR == SCIMITAR_PROCESSOR_X64
	constexpr size_t hardware_constructive_inference_size = 64;
	constexpr size_t hardware_destructive_inference_size  = 128;

#elif SCIMITAR_PROCESSOR == SCIMITAR_PROCESSOR_ARM
	constexpr size_t hardware_constructive_inference_size = 64;
	constexpr size_t hardware_destructive_inference_size  = 64;

#else
	#error "not implemented"
#endif
}