// AVIPlayer.h: interface for the CAVIPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIPLAYER_H__0BEC37D4_DC58_4EE8_9AC7_6D750A5C2FB7__INCLUDED_)
#define AFX_AVIPLAYER_H__0BEC37D4_DC58_4EE8_9AC7_6D750A5C2FB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAVIPlayer  
{
public:
	CAVIPlayer();
	virtual ~CAVIPlayer();
	
	void Load(const char *filePath);
	void GetFrame(int frame);
	void Close();

private:
	void Flip(void* buffer);
};

#endif // !defined(AFX_AVIPLAYER_H__0BEC37D4_DC58_4EE8_9AC7_6D750A5C2FB7__INCLUDED_)
