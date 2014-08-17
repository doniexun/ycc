#include <stdio.h>
#include <stdarg.h>

#include "error.h"

static errcnt = 0;
static wrncnt = 0;

void fatal(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void warning(const char *format, ...)
{
	va_list ap;
	wrncnt++;
	fprintf(stderr, "%s, line %d: WARNING: ", filepath, pos.y);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void error(const char *format, ...)
{
	va_list ap;
	if(errcnt > MAXERR)
		fatal("%s", "too many errors, abort.");
	errcnt++;
	fprintf(stderr, "%s, line %d: ERROR: ", filepath, pos.y);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}