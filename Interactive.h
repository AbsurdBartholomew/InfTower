// Interactive.h: interface for the CInteractive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_)
#define AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node3D.h"

enum InteractionIcon
{
	ICON_TALK_TO,
	ICON_GENERIC_INTERACTION,

	NUM_INTERACTION_ICONS
};

class CInteractive : public CNode3D  
{
public:
	CInteractive();
	virtual ~CInteractive();

	void Draw();
	
	virtual bool Check(void* data) = 0;
	virtual void Action() = 0;

	bool GetIsInRange(Vector3 target);

	void DrawInteractionIcon();

	float m_range;
	InteractionIcon m_interactionIcon;

	unsigned char *m_bitmapData;
	GLuint m_iconTexture;

	bool m_showInteractionIcon;
	bool m_active;
};

#endif // !defined(AFX_INTERACTIVE_H__55AB947D_FCCB_478E_8766_9BAF15EA1488__INCLUDED_)
