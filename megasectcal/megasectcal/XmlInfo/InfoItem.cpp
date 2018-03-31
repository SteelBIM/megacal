#include "InfoItem.h"
#include <afxtempl.h>

using namespace slim;
////////////////////////////////CInfoBase//////////////////////////////////////////
IMPLEMENT_DYNCREATE(CInfoBase,CObject)

CInfoBase::CInfoBase()
{
	m_parent = NULL;
}
CInfoBase::~CInfoBase()
{

}
bool CInfoBase::readString(slim::XmlNode* pNode,const CString& strTag,CString& Result)
{
	return readSubItemString(pNode, strTag, Result);
}
bool CInfoBase::writeString(slim::XmlNode* pNode,const CString& strTag,const CString& ToWrite)
{
	return writeSubItemString(pNode, strTag, ToWrite);
}

bool CInfoBase::readSubItemString(XmlNode* pNode,const CString& strTag,CString& Result)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlNode *pStrNode = pNode->findChild(strTag);

	if (NULL!=pStrNode)
	{
		Result = pStrNode->getString();
		return true;
	}

	return false;
}
bool CInfoBase::writeSubItemString(XmlNode* pNode,const CString& strTag,const CString& ToWrite)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlNode *pStrNode = pNode->addChild(strTag);

	if (NULL!=pStrNode)
	{
		pStrNode->setString(ToWrite);
		return true;
	}

	return false;
}

bool CInfoBase::readSubItemInt(slim::XmlNode* pNode,const CString& strTag,int& Result)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlNode *pStrNode = pNode->findChild(strTag);

	if (NULL!=pStrNode)
	{
		Result = pStrNode->getInt();
		return true;
	}

	return false;

}
bool CInfoBase::writeSubItemInt(slim::XmlNode* pNode,const CString& strTag,const int& ToWrite)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlNode *pStrNode = pNode->addChild(strTag);

	if (NULL!=pStrNode)
	{
		pStrNode->setInt(ToWrite);
		return true;
	}

	return false;
}


bool CInfoBase::readAttrString(slim::XmlNode* pNode,const CString& strTag,CString& Result)
{
	if (NULL==pNode)
	{
		return false;
	}
	XmlAttribute *pStr = pNode->findAttribute(strTag);
	if (NULL!=pStr)
	{
		Result = pStr->getString();
		return true;
	}

	return false;
}
bool CInfoBase::writeAttrString(slim::XmlNode* pNode,const CString& strTag,const CString& ToWrite)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlAttribute *pStr = pNode->addAttribute(strTag);

	if (NULL!=pStr)
	{
		pStr->setString(ToWrite);
		return true;
	}

	return false;
}

bool CInfoBase::readAttrInt(slim::XmlNode* pNode,const CString& strTag,int& Result)
{
	if (NULL==pNode)
	{
		return false;
	}
	XmlAttribute *pStr = pNode->findAttribute(strTag);
	if (NULL!=pStr)
	{
		Result = pStr->getInt();
		return true;
	}

	return false;
}
bool CInfoBase::writeAttrInt(slim::XmlNode* pNode,const CString& strTag, const int & ToWrite)
{
	if (NULL==pNode)
	{
		return false;
	}

	XmlAttribute *pStr = pNode->addAttribute(strTag);

	if (NULL!=pStr)
	{
		pStr->setInt(ToWrite);
		return true;
	}

	return false;
}

bool CInfoBase::readFrom(XmlNode* pNode)
{
	return false;
}
bool CInfoBase::writeTo(XmlNode* pNode)
{
	return false;
}
 bool CInfoBase::readElements(XmlNode* pNodeElements,const CString& elementTag,
 		CArray<CInfoBase*,CInfoBase*>& elements,CRuntimeClass* pRC)
{
 
	if(NULL==pNodeElements)
	{
		return false;
	}
	if(TRUE == elementTag.IsEmpty())
	{
		return false;
	}
	if (pRC==NULL)
	{
		return false;
	}


	NodeIterator itor;
	XmlNode *pItorNode = NULL;
	for (pItorNode = pNodeElements->findFirstChild(elementTag,itor);
		pItorNode!= NULL;
		pItorNode = pNodeElements->findNextChild(elementTag,itor))
	{
		CInfoBase *pInfoBase = (CInfoBase *)pRC->CreateObject();
		if (NULL!=pInfoBase)
		{
			if (true == pInfoBase->readFrom(pItorNode))
			{
				pInfoBase->SetParent(this);
				elements.Add(pInfoBase);
			}
			else
			{
				ASSERT(FALSE);
				continue;
			}

		}
		else
		{
			ASSERT(FALSE);
			continue;
		}
	}

 	return true;
 
}
bool CInfoBase::writeElements(XmlNode* pNodeElements,const CString& elementTag,
	 CArray<CInfoBase*,CInfoBase*>& elements)
{
	if(NULL==pNodeElements)
	{
		return false;
	}
	if (TRUE == elementTag.IsEmpty())
	{
		return false;
	}

	int  i;
	CInfoBase * pInfoBase =NULL;
	XmlNode * pNode = NULL;
	for (i=0;i<elements.GetSize();i++)
	{
		pInfoBase = elements.GetAt(i);
		if (NULL!=pInfoBase)
		{
			pNode = pNodeElements->addChild(elementTag);
			if (NULL!=pNode)
			{
				if(false == pInfoBase->writeTo(pNode))
				{
					ASSERT(FALSE);
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}

	return true;

}
CInfoBase* CInfoBase::GetParent()
{
	ASSERT(m_parent);
	return m_parent;
}
void CInfoBase::SetParent(CInfoBase* pParent)
{
	ASSERT(pParent);
	m_parent = pParent;
}
