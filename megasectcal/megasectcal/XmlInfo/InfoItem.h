#include "afx.h"
#include "SlimXml.h"

#ifndef INFO_DLLEXPORT
#define INFO_DLLEXPORT 
#endif //INFO_DLLEXPORT

#ifndef INFOITEM_H
#define INFOITEM_H


class  INFO_DLLEXPORT CInfoBase:public CObject
{
	DECLARE_DYNCREATE(CInfoBase)
public:
	CInfoBase();
	virtual ~CInfoBase();
public:
	virtual bool readFrom(slim::XmlNode* pNode);
	virtual bool writeTo(slim::XmlNode* pNode);
public:
	static bool readString(slim::XmlNode* pNode,const CString& strTag,CString& Result);
	static bool writeString(slim::XmlNode* pNode,const CString& strTag,const CString& ToWrite);

	//ÐÂÔö
	static bool readSubItemString(slim::XmlNode* pNode,const CString& strTag,CString& Result);
	static bool writeSubItemString(slim::XmlNode* pNode,const CString& strTag,const CString& ToWrite);

	static bool readSubItemInt(slim::XmlNode* pNode,const CString& strTag,int& Result);
	static bool writeSubItemInt(slim::XmlNode* pNode,const CString& strTag,const int& ToWrite);



	static bool readAttrString(slim::XmlNode* pNode,const CString& strTag,CString& Result);
	static bool writeAttrString(slim::XmlNode* pNode,const CString& strTag,const CString& ToWrite);

	static bool readAttrInt(slim::XmlNode* pNode,const CString& strTag,int& Result);
	static bool writeAttrInt(slim::XmlNode* pNode,const CString& strTag, const int & ToWrite);




	bool readElements(slim::XmlNode* pNode,const CString& elementTag,CArray<CInfoBase*,CInfoBase*>& elements,CRuntimeClass* pRC);
	bool writeElements(slim::XmlNode* pNode,const CString& elementTag,CArray<CInfoBase*,CInfoBase*>& elements);

	CInfoBase* GetParent();
	void SetParent(CInfoBase* pParent);

protected:
	CInfoBase* m_parent;
};


#endif //INFOITEM_H


