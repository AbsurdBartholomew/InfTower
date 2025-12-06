// MidiPlayer.h: interface for the CMidiPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIPLAYER_H__235F1098_8AE1_4E94_AC61_E4941116F228__INCLUDED_)
#define AFX_MIDIPLAYER_H__235F1098_8AE1_4E94_AC61_E4941116F228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CMidiPlayer  
{
public:
	CMidiPlayer();
	virtual ~CMidiPlayer();
	
	static DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName);
};

#endif // !defined(AFX_MIDIPLAYER_H__235F1098_8AE1_4E94_AC61_E4941116F228__INCLUDED_)
