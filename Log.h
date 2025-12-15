// Log.h: interface for the Log class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__D2323E90_A777_4925_BA65_C9F2C9822C0D__INCLUDED_)
#define AFX_LOG_H__D2323E90_A777_4925_BA65_C9F2C9822C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

class Log  
{
public:
	Log();
	virtual ~Log();

	static bool SetLogOutPath(const char *path);
	static void Print(const char *fmt, ...);
	static void CloseLogFile();

private:
	static void PrintLine();

	static FILE *m_fp;
};

#endif // !defined(AFX_LOG_H__D2323E90_A777_4925_BA65_C9F2C9822C0D__INCLUDED_)
