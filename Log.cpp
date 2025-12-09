// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////

#include "Log.h"
#include <windows.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FILE *Log::m_fp;

Log::Log()
{

}

Log::~Log()
{

}

bool Log::SetLogOutPath(const char *path)
{
	m_fp = fopen(path, "w");
	if(m_fp == NULL)
	{
		MessageBox(NULL, "Couldn't create log", "Whoops", MB_OK);
		return false;
	}

	return true;
}

void Log::Print(const char *fmt, ...)
{
	char str[512];
    va_list argp;

    va_start(argp, fmt);

    vsprintf(str, fmt, argp);
    printf("%s\n", str);
	if(m_fp != NULL)
	{
		// TODO: timestamps would be nice...
		fprintf(m_fp, str);
	}

    va_end(argp);
}

void Log::CloseLogFile()
{
	if(m_fp != NULL)
	{
		fclose(m_fp);
	}
}