#include "common.h"
#include <stdlib.h>
#include <stdio.h>

void Info(const char *fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	vprintf(fmt, argp);
	putchar('\n');
	fflush(stdout);
	va_end(argp);
}

void Error(const char *fmt, ...)
{
	va_list argp;

	printf("Error: ");
	va_start(argp, fmt);
	vprintf(fmt, argp);
	putchar('\n');
	va_end(argp);

	exit(EXIT_FAILURE);
}
