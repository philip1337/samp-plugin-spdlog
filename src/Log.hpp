#ifndef SAMPLOG_LOG_HPP
#define SAMPLOG_LOG_HPP

SAMPLOG_BEGIN_NS

class Log {
public:
	Log(AMX *amx): m_natives(amx)
	{
		m_amx = amx;
	};

	int init();
	void closeHandles();
	
	// Instance handling
	static void registerInstance(AMX *amx, Log *log);
	static Log * getInstance(AMX *amx);
private:
	AMX *m_amx;
	Natives m_natives;
};

SAMPLOG_END_NS
#endif