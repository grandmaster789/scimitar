#pragma once

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

	#define NO_MINMAX

	#include <SDKDDKVer.h>
	#include <Windows.h>
	#include <intrin.h>
	// TODO: stack tracing via <dbghelp.h>, or alternatively via boost::stacktrace
#endif

// with MSVC we can figure out wheter this is a debug build
#ifdef _DEBUG
	#define SCIMITAR_DEBUG
#endif

// NO_MINMAX should mean that these are not defined, but just in case
#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif
