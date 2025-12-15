// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////

#include "Log.h"
#include <windows.h>
#include <stdarg.h>
#include <time.h>

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
	Log::Print("Start of Log\n");
	PrintLine();

	return true;
}

void Log::Print(const char *fmt, ...)
{
	char str[512];
	char *fuck; // temporary asctime string needed to remove the fucking newline
	int fuckIndex;
	time_t rawTime;
	tm *timeInfo;
    va_list argp;

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	fuck = asctime(timeInfo);
	fuckIndex = strlen(fuck) - 1;

	memmove(&fuck[fuckIndex], &fuck[fuckIndex + 1], fuckIndex - 1);

    va_start(argp, fmt);

    vsprintf(str, fmt, argp);
    printf("%s\n", str);
	if(m_fp != NULL)
	{
		fprintf(m_fp, "%s: %s", fuck, str);
		fflush(m_fp);
	}

    va_end(argp);
}

void Log::PrintLine()
{
	printf("===============================================================\n");
	if(m_fp != NULL)
	{
		fprintf(m_fp, "===============================================================\n");
		fflush(m_fp);
	}
}

void Log::CloseLogFile()
{
	if(m_fp != NULL)
	{
		PrintLine();
		Print("End of Log\n");

		fclose(m_fp);
	}
}