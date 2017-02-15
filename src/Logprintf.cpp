#include "Config.hpp"

#include <cstdarg>
#include <cstdio>

#include "Logprintf.hpp"

#define LOGPRINTF_BUFFER_SIZE 1024

#ifdef _MSC_VER
  #define vsnprintf vsprintf_s
#endif

logprintf_t logprintf;

SAMPLOG_BEGIN_NS

void vlogprintf(const char *format, va_list va) {
	char buffer[LOGPRINTF_BUFFER_SIZE];
	vsnprintf(buffer, sizeof(buffer), format, va);

	buffer[sizeof(buffer) - 1] = '\0';
	logprintf("%s", buffer);
}

SAMPLOG_END_NS