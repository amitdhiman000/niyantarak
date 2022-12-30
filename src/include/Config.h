#ifndef __CONFIG_AMU__
#define __CONFIG_AMU__

#define NAMESPACE_POSIX posix
#define NAMESPACE_BASE base
#define NAMESPACE_CORE core
#define NAMESPACE_NET net
#define NAMESPACE_UI ui

#ifdef __APPLE__
#define APPLE 1
#endif

#ifdef __linux__
#define LINUX 1
#endif

#ifdef __unix__
#define UNIX 1
#endif

#ifdef _WIN32
#define WINDOWS 1
#endif


#endif /* __CONFIG_AMU__ */
