#if !defined(OSAL_EXPORT_H)
#define OSAL_EXPORT_H

#if defined(OS_WINDOWS)
#define EXPORT	__declspec(dllexport)
#define CALL		__cdecl
#else  /* Not WINDOWS */
#define EXPORT 	__attribute__((visibility("default")))
#define CALL
#endif

#endif /* #define OSAL_EXPORT_H */
