#ifndef DEFINES_H
#define DEFINES_H


#ifdef PROJECT_EXPORT
// Exports
#ifdef _MSC_VER
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define PROJECT_API __declspec(dllimport)
#else
#define PROJECT_API
#endif
#endif

#endif // DEFINES_H