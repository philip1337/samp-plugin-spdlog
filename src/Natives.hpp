#ifndef SAMPLOG_NATIVES_HPP
#define SAMPLOG_NATIVES_HPP
#include "plugin.h"

SAMPLOG_BEGIN_NS

class Natives {
public:
	Natives(AMX *amx) {
		m_amx = amx;
	};
	AMX_NATIVE_INFO* get();

private:
	AMX *m_amx;
};

SAMPLOG_END_NS
#endif