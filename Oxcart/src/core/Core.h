#ifndef CORE_HEADER
#define CORE_HEADER

	#ifdef OX_BUILD_DLL
		#define OX_API __declspec(dllexport)
	#else
		#define OX_API __declspec(dllimport)
	#endif

#endif