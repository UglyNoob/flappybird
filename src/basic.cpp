#include "basic.h"

#include <cstdarg>
#include <cstdio>

void log_normal(const char *fmt, ...) {
	va_list vlist;
	va_start(vlist, fmt);
	vfprintf(stderr, fmt, vlist);
	fputc('\n', stderr);
	va_end(vlist);
}

#ifdef __WIN32

#include <windows.h>

void log_warning(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	WORD general = info.wAttributes;
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN);
	vfprintf(stderr, fmt, args);
	SetConsoleTextAttribute(handle, general);
	fprintf(stderr, "\n");
	va_end(args);
}

void log_error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	WORD general = info.wAttributes;
	SetConsoleTextAttribute(handle, FOREGROUND_RED);
	vfprintf(stderr, fmt, args);
	SetConsoleTextAttribute(handle, general);
	fprintf(stderr, "\n");
	va_end(args);
}


#else

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
