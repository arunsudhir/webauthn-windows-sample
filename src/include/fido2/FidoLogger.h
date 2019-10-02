#pragma once
#include <stdint.h>

class FidoLogger
{
public:
	void log_init();
	void log_xxd(const uint8_t* buf, size_t count);
	void log_debug(const char* fmt, ...);
};

