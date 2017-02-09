#include "stdafx.h"
#include "LogFuncs.h"

void writeInLog(const std::string& message)
{
	time_t t;
	tm *tk;

	time(&t);
	tk = localtime(&t);

	std::fstream log("log.txt", std::ios_base::app);

	log << "data " << 1900 + tk->tm_year << ":" << 1 + tk->tm_mon << ":" << tk->tm_mday << " time " << tk->tm_hour << ":" << tk->tm_min << ":" << tk->tm_sec << " " << message.c_str() << std::endl;

	log.close();
}