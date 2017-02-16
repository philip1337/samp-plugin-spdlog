#ifndef SAMPLOG_LOGPRINTF_HPP
#define SAMPLOG_LOGPRINTF_HPP

#include <cstdarg>
#include <spdlog/sinks/sink.h>

typedef void (*logprintf_t)(const char *format, ...);
extern logprintf_t logprintf;

SAMPLOG_BEGIN_NS

void vlogprintf(const char *format, std::va_list va);

SAMPLOG_END_NS
#endif