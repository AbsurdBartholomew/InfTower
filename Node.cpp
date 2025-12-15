// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "Node.h"
#include <windows.h>
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::vector<CNode*> allNodes;

CNode::CNode()
{
	m_name = GetResourceName();
}

CNode::~CNode()
{

}

void CNode::QueueFree()
{
	
}

void CNode::AddChild(CNode *node)
{
	node->m_parent = this;
	node->m_level = m_level + 1;
	node->Init();

	m_children.push_back(node);
	allNodes.push_back(node);
}

std::vector<CNode*> CNode::GetDescendants()
{
	std::vector<CNode*> nodeList;
	CNode *lastNode = this;
	CNode *nextNode = this;
	int recursion = 0;

	while(lastNode != NULL)
	{
		
		for(int i = 0; i < lastNode->m_children.size(); i++)
		{
			Log::Print("Found node of type %s with recursion %d\n", nextNode->m_children[i]->m_name, recursion);
			nodeList.push_back(nextNode->m_children[i]);
			nextNode = nextNode->m_children[i];
			recursion++;
		}
		lastNode = nextNode;
		recursion = 0;
	}

	Log::Print("Aaaand done here\n");

	return nodeList;
}

void CNode::Register(unsigned short version)
{
	HashToConstructorType registry;
	registry.m_id = m_id;
	registry.m_constructor = (FnNew)New;

	constructorArray[nRegisteredTypes] = registry;
	nRegisteredTypes++;
}

void CNode::Load(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "rb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not open DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fread(&m_id, sizeof(m_id), 1, fPtr);
	fread(&m_version, sizeof(m_version), 1, fPtr);

	fclose(fPtr);
}

void CNode::Serialize(const char *filePath)
{
	FILE *fPtr = fopen(filePath, "wb");

	if(fPtr == NULL)
	{
		MessageBox(NULL, "Could not create DL file!", "Display List", MB_OK);
		exit(EXIT_FAILURE);
	}

	fwrite(&m_id, sizeof(m_id), 1, fPtr);
	fwrite(&m_version, sizeof(m_version), 1, fPtr);

	fclose(fPtr);
}