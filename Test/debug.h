/*
 * debug.h
 *
 *  Created on: 10 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include <assert.h>
#include "runcfg.h"

#ifdef NODEBUG

#define dout(format, ...) ((void)0)

#else

#define dout(format, ...) _debug_out(__func__, __FILE__, __LINE__, format, ## __VA_ARGS__)

#endif // NODEBUG

extern char * timeToStr(unsigned long aTime);
extern void _debug_out(const char *__func, const char *__file, int __lineno, const char *__fmt, ...);
extern void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp);

#endif /* DEBUG_H_ */
