/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_PREREQ_H
#define INPUT_PREREQ_H



//-------------- Determine Compiler ---------------------------------
#if defined( _MSC_VER )
#	define COMPILER_MSVC
#elif defined( __GNUC__ )
#	if defined( __WIN32__ ) || defined( _WIN32 )
#		define COMPILER_MINGW
#	else
#		define COMPILER_GCC
#	endif
#elif defined( __BORLANDC__ )
#	define COMPILER_BORLAND
#else
#	error No Recognized Compiler!
#endif

// --------------- Determine Operating System Platform ---------------
#if defined( __WIN32__ ) || defined( _WIN32 )
#	if defined ( _XBOX )
#		define PLATFORM_XBOX
#	else
#		define PLATFORM_WIN32
#	endif
#elif defined( __APPLE_CC__ ) // Apple OS X
    // Device                                       Simulator
#   if __IPHONE_OS_VERSION_MIN_REQUIRED >= 20201 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 20000
//#   if __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#       define PLATFORM_IPHONE
#   else
#       define PLATFORM_APPLE
#   endif
#   define _INPUTExport __attribute__((visibility("default")))
#elif defined( __linux__ )
#	define PLATFORM_LINUX
#else
# error Platform not supported!
#endif

//Is Processor 32 or 64 bits...
#if defined(__x86_64__)
#	define PLATFORM_64
#else
#	define PLATFORM_32
#endif

#endif // !INPUT_PREREQ_H
