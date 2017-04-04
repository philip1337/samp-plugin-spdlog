#include "Config.hpp"
#include "Logger.hpp"

#include <boost/system/config.hpp>
#include <boost/filesystem.hpp>

SAMPLOG_BEGIN_NS

const std::string Logger::getString(AMX *amx, cell param)
{
	char *tmp;
	amx_StrParam(amx, param, tmp);
	return (tmp != NULL) ? tmp : std::string();
}

bool Logger::formatString(const std::string& loggerName, AMX *amx, cell *params, fmt::MemoryWriter &writer) {
	// Data
	std::string data;

	// Message
	auto message = getString(amx, params[2]);

	// If no message defined
	auto lenght = message.length();
	if (message.length() == 0) {
		logprintf("[SPDLog] [%s] Error invalid message lenght %i.", loggerName.c_str(), lenght);
		return false;
	}

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Parse message using the type specifiers %*
	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = message.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (message.length() - 1))
			return false;

		writer << message.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (message.at(spec_pos + 1))
		{
		case '%': // '%' escape
			writer << '%';
			break;
		case 's': // string
			writer << getString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	writer << message.substr(spec_offset);
	return true;
}

bool Logger::checkPath(const std::string& path)
{
	// Initialize boost path
	boost::filesystem::path p(path);

	// If file already exists do nothing
	if (boost::filesystem::is_regular_file(p))
		return true;

	// Parent path
	auto parent = p.parent_path();

	// If directory already exists do nothing
	if (boost::filesystem::is_directory(parent))
		return true;

	// If path is a file
	if (boost::filesystem::is_regular_file(parent)) {
		logprintf("[SPDLog] Failed to create directory (%s) path is a file. resolved from (%s).", parent.c_str(), path.c_str());
		return false;
	}

	// Create directory tree
	auto ret = boost::filesystem::create_directories(parent);
	if (!ret)
		logprintf("[SPDLog] Failed to create directory (%s).", parent.c_str());

	return ret;
}

SAMPLOG_END_NS