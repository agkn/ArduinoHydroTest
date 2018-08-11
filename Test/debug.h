/*
 * debug.h
 *
 *  Created on: 10 авг. 2018 г.
 *      Author: Andrey
 */

#ifndef DEBUG_H_
#define DEBUG_H_

extern void _debug_out(const char *__func, const char *__file, int __lineno, const char *__fmt, ...);
#define dout(format, ...) _debug_out(__func__, __FILE__, __LINE__, format, ## __VA_ARGS__)

extern char * timeToStr(unsigned long aTime);

#endif /* DEBUG_H_ */
