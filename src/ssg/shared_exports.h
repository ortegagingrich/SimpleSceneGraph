
#ifndef SHARED_EXPORTS_H
#define SHARED_EXPORTS_H




#ifndef internal
	#define internal public
#endif


/*
 * Just some Windows hackery.
 * 
 * Thanks to Gernot Klingler for his very helpful article.
 */


#ifdef _WIN32
	#ifndef INCLUDE
		#define SHARED_EXPORT __declspec(dllexport)
	#else
		#define SHARED_EXPORT
	#endif
#else
	#define SHARED_EXPORT
#endif


#endif
