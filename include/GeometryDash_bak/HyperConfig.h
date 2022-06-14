/* HyperConfig.h
 * Contains configuration macros
 */
#ifdef _MSC_VER
#define HYPERDASH_WINDOWS
#endif

#ifndef HYPERDASH_WINDOWS
#define HYPERDASH_ANDROID
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef HYPERDASH_BUILD
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #define DLL_PUBLIC __attribute__ ((visibility ("default")))
  #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define EXPORT_DECORATOR __attribute__ ((dllexport))
  #else
    #define EXPORT_DECORATOR __declspec(dllexport)
  #endif
#else
  #define EXPORT_DECORATOR __attribute__ ((visibility ("default")))
#endif
