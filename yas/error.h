#ifndef _ERROR_H_
#define _ERROR_H_

void fatal(const char *fmt, ...);
void error(const char *fmt, ...);
void warning(const char *fmt, ...);

#define MAXERR 100

#endif
