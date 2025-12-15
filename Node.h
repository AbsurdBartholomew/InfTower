// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__EA067A95_6183_42B2_BE80_50E7EEF3DB28__INCLUDED_)
#define AFX_NODE_H__EA067A95_6183_42B2_BE80_50E7EEF3DB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Rsrc.h"
#include "resource.h"

class CNode  : public CRsrc
{
public:
	CNode();
	virtual ~CNode();
	
	virtual void Init() = 0;
	virtual void Update(int dT) = 0;
	virtual void Draw() = 0;

#ifdef _EDITOR
	virtual int GetPropertiesWindowID() { return IDD_P_CANNOTEDIT; }
#endif

	void QueueFree();

	virtual void InternalUpdate(int dT) 
	{ 
		for(int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->InternalUpdate(dT);
		}

		Update(dT);
	}

	void AddChild(CNode *node);
	std::vector<CNode*> GetDescendants();

	const char *m_name;

	unsigned int m_version;

	CNode *m_parent;
	std::vector<CNode*> m_children;

	static CNode *New() { return NULL; }
	void Register(unsigned short version);

	void Serialize(const char *filePath);
	void Load(const char *filePath);

	const char *GetResourceName() { return "Node"; }
	unsigned int GetResourceVersion() { return 0; }
	const char *GetResourceExtension() { return ".nod"; }

	int m_level;
};

extern std::vector<CNode*> allNodes;

#endif // !defined(AFX_NODE_H__EA067A95_6183_42B2_BE80_50E7EEF3DB28__INCLUDED_)
