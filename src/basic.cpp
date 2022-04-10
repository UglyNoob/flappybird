#include "basic.h"

#include <cstdarg>
#include <cstdio>

#ifdef __WIN32

#else

void log_normal(const char *fmt, ...) {
	va_list vlist;
	va_start(vlist, fmt);
	vfprintf(stderr, fmt, vlist);
	fputc('\n', stderr);
	va_end(vlist);
}

void log_warning(const char *fmt, ...) {
	va_list vlist;
	va_start(vlist, fmt);
	fprintf(stderr, "\033[93m\033[1m");
	vfprintf(stderr, fmt, vlist);
	fprintf(stderr, "\033[0m\n");
	va_end(vlist);
}

void log_error(const char *fmt, ...) {
	va_list vlist;
	va_start(vlist, fmt);
	fprintf(stderr, "\033[91m\033[1m");
	vfprintf(stderr, fmt, vlist);
	fprintf(stderr, "\033[0m\n");
	va_end(vlist);
}

#endif
