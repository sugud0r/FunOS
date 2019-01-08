#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length)
{
	const unsigned char* bytes = (const unsigned char*) data;
	register size_t i;

	for (i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;

	return true;
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) goto overflow;

			if (!print(format, amount))
				return -1;

			format += amount;
			written += amount;

			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) goto overflow;
			
			if (!print(&c, sizeof(c)))
				return -1;

			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			
			if (maxrem < len) goto overflow;

			if (!print(str, len))
				return -1;

			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);

			if (maxrem < len) goto overflow;

			if (!print(format, len))
				return -1;

			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;

overflow:
	errno = EOVERFLOW;
	va_end(parameters);
	return-1;
}
