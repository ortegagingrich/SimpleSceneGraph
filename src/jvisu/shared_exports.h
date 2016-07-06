/*
 * Just some Windows hackery.  Thanks to Gernot Klingler for his very helpful article.
 */

#ifndef SHARED_EXPORTS_H
#define SHARED_EXPORTS_H

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT
#endif



#endif