#include "StdAfx.h"
#include "ConfigGen.h"
#include "./json/json.h"
#include "ICalculationReport.h"
#include "CfgSvr.h"
 

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <intrin.h>
#include "IGenerateText.h"
#include "Pk_MainOutPut.h"
#include "ParagraphMacroHeader.h"
#include "GReport.h"
#include "GPara.h"

using namespace slim;

IMPLEMENT_DYNAMIC(CConfigGenCalc,CInfoBase)
CConfigGenCalc::CConfigGenCalc(void)
{
	m_ver = 1;
	m_Title = _T("CALCULATION REPORT");
	m_ProjNum = _T("No.1");
	m_ProjName = _T("X_X_X PROJECT");
	m_Computer = _T("Mrs/Mr");
	m_MajorOfficer = _T("MajorOfficer");
	 
	CTime t =  CTime::GetCurrentTime();
	m_Date.Format(_T("%d-%02d-%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
	m_Checker			= _T("XXX Designer"); 
	m_Company			= _T("XXXX Company Ltd"); 
	m_Sketch			= _T("C:\\Users\\Pictures\\A.jpg"); 
}


CConfigGenCalc::~CConfigGenCalc(void)
{
}

void CConfigGenCalc::WriteToVersion(XmlNode *ndVersion)
{
	ndVersion->setInt(m_ver);
}

void CConfigGenCalc::WriteToReportOption()
{
	
	const TCHAR* marker_ReportOption =_T( "ReportOption");

	const TCHAR* marker_Version =_T( "Version");

	const TCHAR* marker_Cover =_T( "Cover");
	const TCHAR* marker_Font =_T( "Font");
	const TCHAR* marker_General =_T( "General");
	const TCHAR* marker_HeaderFooter =_T( "HeaderFooter");

	const TCHAR* marker_Layout =_T( "Layout");
	const TCHAR* marker_Table =_T( "Table");
	const TCHAR* marker_NumStyle =_T( "NumStyle");
	const TCHAR* marker_AllItems =_T( "AllItems");
	const TCHAR* marker_SelectedItems =_T( "SelectedItems");
	const TCHAR* marker_Setup =_T( "Setup");
 

	XmlNode*  ndReportOption =  m_doc.addChild(marker_ReportOption);



	XmlNode*  ndVersion = ndReportOption->addChild(marker_Version);
	XmlNode*  ndConver	=  ndReportOption->addChild(marker_Cover);
	XmlNode*  ndFont	=  ndReportOption->addChild(marker_Font);

	XmlNode*  ndGeneral1			=ndReportOption->addChild(marker_General);
	XmlNode*  ndGeneral2			=ndReportOption->addChild(marker_General);
	XmlNode*  ndHeaderFooter	=ndReportOption->addChild(marker_HeaderFooter);
	XmlNode*  ndLayout			=ndReportOption->addChild(marker_Layout);
	
	XmlNode*  ndTable 			=ndReportOption->addChild(marker_Table );
	XmlNode*  ndNumStyle 		=ndReportOption->addChild(marker_NumStyle );
	XmlNode*  ndAllItems 		=ndReportOption->addChild(marker_AllItems );

	XmlNode*  ndSelectedItems 	=ndReportOption->addChild(marker_SelectedItems );
	XmlNode*  ndSetup 			=ndReportOption->addChild(marker_Setup );

	WriteToVersion(ndVersion);
	WriteToCover(ndConver);
	WriteToFont(ndFont);
	WriteToGeneral1(ndGeneral1);
	WriteToGeneral2(ndGeneral2);

	WriteToHeaderFooter(ndHeaderFooter);

	WriteToLayout(ndLayout);
	WriteToTable(ndTable);
	WriteToNumStyle(ndNumStyle);

	WriteToAllItems(ndAllItems);
	WriteToSelectedItems(ndSelectedItems);
	WriteToSetup(ndSetup);
}
bool CConfigGenCalc::ReadFromReportOption()
{
	const TCHAR* marker_ReportOption =_T("ReportOption");

	const TCHAR* marker_Layout =_T("Layout");

	XmlNode*  ndReportOption =  m_doc.findChild(marker_ReportOption);
	if(nullptr == ndReportOption)
	{
		return false;
	}

	XmlNode*  ndLayout = ndReportOption->findChild(marker_Layout);
	if (nullptr == ndLayout)
	{
		return false;
	}

	if(false == ReadFromLayout(ndLayout))
	{
		return false;
	}

	return true;
}

void CConfigGenCalc::WriteToCover( XmlNode* ndCover)
{
	const TCHAR* output =_T( "output");
	const TCHAR* fontname = _T("fontname");
	const TCHAR* fontheight =_T( "fontheight");
	const TCHAR* underline =_T( "underline");
	const TCHAR* delline =_T( "delline");
	const TCHAR* bold = _T("bold");
	const TCHAR* color = _T("color");
	const TCHAR* autobuild = _T("autobuild");

	 XmlNode * nd = ndCover->addChild("Title");
	 nd->addAttribute(output,true);
	 nd->addAttribute(fontname,_T("黑体"));
	 nd->addAttribute(fontheight,48);
	 nd->addAttribute(underline,false);
	 nd->addAttribute(delline,false);
	 nd->addAttribute(bold,true);
	 nd->addAttribute(color,0);
	 nd->setString(m_Title);

// 	 nd = ndCover->addChild("ProjNum");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_ProjNum);
// 
// 	 nd = ndCover->addChild("ProjName");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_ProjName);
// 
// 	 nd = ndCover->addChild("Computer");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_Computer);
// 
// 	 nd = ndCover->addChild("MajorOfficer");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_MajorOfficer);
// 
// 	 nd = ndCover->addChild("Date");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_Date);
// 
// 	 nd = ndCover->addChild("Checker");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("宋体"));
// 	 nd->addAttribute(fontheight,14);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_Checker);
// 	 
// 	 nd = ndCover->addChild("Company");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(fontname,_T("黑体"));
// 	 nd->addAttribute(fontheight,22);
// 	 nd->addAttribute(underline,false);
// 	 nd->addAttribute(delline,false);
// 	 nd->addAttribute(bold,true);
// 	 nd->addAttribute(color,0);
// 	 nd->setString(m_Company);
// 
// 	 nd = ndCover->addChild("Sketch");
// 	 nd->addAttribute(output,true);
// 	 nd->addAttribute(autobuild,false);
// 	 nd->setString(m_Sketch);
}

void CConfigGenCalc::WriteToFont( slim::XmlNode *ndFont )
{
	const TCHAR* FirstLevel =_T( "FirstLevel");
	const TCHAR* SecondLevel =_T( "SecondLevel");
	const TCHAR* ThirdLevel =_T( "ThirdLevel");
	const TCHAR* Content =_T( "Content");
	const TCHAR* TableHeader =_T( "TableHeader");
	const TCHAR* TableContent =_T( "TableContent");
	const TCHAR* Header =_T( "Header");
	const TCHAR* Footer =_T( "Footer");
	const TCHAR* GraphicTitle =_T( "GraphicTitle");


	const TCHAR* fontname =_T( "fontname");
	const TCHAR* fontheight =_T( "fontheight");
	const TCHAR* underline =_T( "underline");
	const TCHAR* bold =_T( "bold");

	
	XmlNode * nd = ndFont->addChild(FirstLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,16);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(SecondLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(ThirdLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(Content);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(TableHeader);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(TableHeader);
	nd->addAttribute(fontname,_T("黑体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(TableContent);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(Header);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(Footer);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(GraphicTitle);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);
	
}

void CConfigGenCalc::WriteToGeneral1( slim::XmlNode *ndGeneral1 )
{
	XmlNode * nd = ndGeneral1->addChild("Output");
	nd->addAttribute("style",0);//0 word 1 pdf

}

void CConfigGenCalc::WriteToGeneral2( slim::XmlNode *ndGeneral2 )
{
	XmlNode * nd = ndGeneral2->addChild("ColorScheme");
	nd->addAttribute("val",1);
}

void CConfigGenCalc::WriteToHeaderFooter( slim::XmlNode *ndHeaderFooter )
{
	const TCHAR* Header =_T( "Header");
	const TCHAR* text =_T( "text");
	const TCHAR* align =_T( "align");
	const TCHAR* logo =_T( "logo");
	const TCHAR* use =_T( "use");
	const TCHAR* leftifalignsame =_T( "leftifalignsame");

	const TCHAR* Footer =_T( "Footer");
	const TCHAR* type =_T( "type");

	XmlNode * ndHeader = ndHeaderFooter->addChild("Header");
	XmlNode *  ndtext= ndHeader->addChild(text);
	ndtext->addAttribute(align,2);
	ndtext->setString(m_PageHeader);

	XmlNode *  ndlogo=  ndHeader->addChild(logo);
	ndlogo->addAttribute(align,1);
	ndlogo->addAttribute(use,false);
	ndlogo->addAttribute(leftifalignsame,true);



	XmlNode* ndFooter = ndHeaderFooter->addChild(Footer);
	ndFooter->addAttribute(type,2);
	ndFooter->addAttribute(align,2);

}

void CConfigGenCalc::WriteToLayout( slim::XmlNode *ndLayout )
{
	const TCHAR* TextPart =_T( "TextPart");
	const TCHAR* Page =_T( "Page");
	const TCHAR* Direction =_T( "Direction");
	const TCHAR* Margin =_T( "Margin");
	const TCHAR* left =_T( "left");

	const TCHAR* right =_T( "right");
	const TCHAR* top =_T( "top");
	const TCHAR* bottom =_T( "bottom");
	const TCHAR* ColNum =_T( "ColNum");

	const TCHAR* GraphicPart =_T( "GraphicPart");


	XmlNode* nd=nullptr;

	XmlNode * ndTextPart = ndLayout->addChild(TextPart);
	nd = ndTextPart->addChild(Page);
	nd->setString(_T("A4"));
	nd = ndTextPart->addChild(Direction);
	nd->setString(_T("Veri"));
	nd = ndTextPart->addChild(Margin);
	nd->addAttribute(left,25);
	nd->addAttribute(right,25);
	nd->addAttribute(top,32);
	nd->addAttribute(bottom,32);
	nd = ndTextPart->addChild(ColNum);
	nd->setInt(1);

	XmlNode * ndGraphicPart = ndLayout->addChild(GraphicPart);
	nd = ndGraphicPart->addChild(Page);
	nd->setString(_T("A4"));
	nd = ndGraphicPart->addChild(Direction);
	nd->setString(_T("Veri"));
	nd = ndGraphicPart->addChild(Margin);
	nd->addAttribute(left,25);
	nd->addAttribute(right,15);
	nd->addAttribute(top,10);
	nd->addAttribute(bottom,8);
	nd = ndGraphicPart->addChild(ColNum);
	nd->setInt(1);

}
bool CConfigGenCalc::ReadFromLayout(slim::XmlNode *ndLayout)
{
	const TCHAR* TextPart =_T( "TextPart");
	const TCHAR* Page =_T( "Page");
	const TCHAR* Direction =_T( "Direction");
	const TCHAR* Margin =_T( "Margin");
	const TCHAR* left =_T( "left");

	const TCHAR* right =_T( "right");
	const TCHAR* top =_T( "top");
	const TCHAR* bottom =_T( "bottom");
	const TCHAR* ColNum =_T( "ColNum");

	const TCHAR* GraphicPart =_T( "GraphicPart");


	XmlNode * ndTextPart = ndLayout->findChild(TextPart);
	XmlNode * ndGraphPart = ndLayout->findChild(GraphicPart);

	XmlNode *ndPart ;
	LayoutSub *layout ;
	XmlNode *nd;

	ndPart = ndTextPart;
	layout = &textLayOut;
	readSubItemString(ndPart,Page,layout->Page);
	readSubItemString(ndPart,Direction,layout->Direction);
	readSubItemInt(ndPart,ColNum,layout->ColNum);

	nd = ndPart->findChild(Margin);
	readAttrInt(nd,left,layout->MarginL);
	readAttrInt(nd,right,layout->MarginR);
	readAttrInt(nd,top,layout->MarginT);
	readAttrInt(nd,bottom,layout->MarginB);

	///
	ndPart = ndGraphPart;
	layout = &graphLayOut;

	readSubItemString(ndPart,Page,layout->Page);
	readSubItemString(ndPart,Direction,layout->Direction);
	readSubItemInt(ndPart,ColNum,layout->ColNum);

	nd = ndPart->findChild(Margin);
	readAttrInt(nd,left,layout->MarginL);
	readAttrInt(nd,right,layout->MarginR);
	readAttrInt(nd,top,layout->MarginT);
	readAttrInt(nd,bottom,layout->MarginB);


	return true;
}
void CConfigGenCalc::WriteToTable( slim::XmlNode *ndTable )
{
	const TCHAR* Border =_T( "Border");
	const TCHAR* style =_T( "style");
	const TCHAR* Title =_T( "Title");
	const TCHAR* align =_T( "align");

	XmlNode* nd;
	nd = ndTable->addChild(Border);
	nd->addAttribute(style,0);

	nd = ndTable->addChild(Title);
	nd->addAttribute(align,2);
}

void CConfigGenCalc::WriteToNumStyle( slim::XmlNode *ndNumStyle )
{
	const TCHAR* FirstLevel =_T( "FirstLevel");
	const TCHAR* SecondLevel =_T( "SecondLevel");
	const TCHAR* ThirdLevel =_T( "ThirdLevel");

	const TCHAR* type =_T( "type");
	const TCHAR* prefix =_T( "prefix");
	const TCHAR* suffix =_T( "suffix");

	XmlNode* ndFirstLevel =  ndNumStyle->addChild(FirstLevel);
	ndFirstLevel->addAttribute(type,2);
	ndFirstLevel->addAttribute(prefix,_T(""));
	ndFirstLevel->addAttribute(suffix,_T("."));

	XmlNode* ndSecondLevel =  ndNumStyle->addChild(SecondLevel);
	ndSecondLevel->addAttribute(type,1);
	ndSecondLevel->addAttribute(prefix,_T(""));
	ndSecondLevel->addAttribute(suffix,_T("."));

	XmlNode* ndThirdLevel =  ndNumStyle->addChild(ThirdLevel);
	ndThirdLevel->addAttribute(type,1);
	ndThirdLevel->addAttribute(prefix,_T("("));
	ndThirdLevel->addAttribute(suffix,_T(")."));




}

void CConfigGenCalc::WriteToAllItems( slim::XmlNode *ndItems )
{
	const TCHAR* Node =_T( "Node");
	const TCHAR* type =_T( "type");
	const TCHAR* key =_T( "key");

	const TCHAR* checked =_T( "checked");
	const TCHAR* text =_T( "text");
	const TCHAR* style =_T( "style");

	const TCHAR* sel =_T( "sel");


	const TCHAR* branch =_T( "branch"); 
	const TCHAR* graphic =_T( "graphic");
	 
	XmlNode* oneNode  ;
	XmlNode* oneBranch  ;

	
	if(GPara::m_DesnPara->ifCustomSection){	//任意截面
		//第一章
		{
			oneBranch = ndItems->addChild(Node);
			oneBranch->addAttribute(type,branch);
			oneBranch->addAttribute(key,0);
			oneBranch->addAttribute(checked,true);
			oneBranch->addAttribute(text,_T("PARAMETERS"));
			oneBranch->addAttribute(style,0);
			oneBranch->addAttribute(sel,0);

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_GENERAL_PARAMETERS);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("General parameters"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,0);
			}

// 			{
// 				oneNode = oneBranch->addChild(Node);
// 				oneNode->addAttribute(type,text);
// 				oneNode->addAttribute(key,STS_MEGA_CROSS_SECTION);
// 				oneNode->addAttribute(checked,true);
// 				oneNode->addAttribute(text,_T("Cross-section"));
// 				oneNode->addAttribute(style,0);
// 				oneNode->addAttribute(sel,1);
// 			}


			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_Material_properties);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Material properties"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

/*			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_Creep_coefficient_concrete);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Creep coefficient of the concrete"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}
*/
			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_Load_cases);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Internal force"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_Calculation_parameters);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Calculation parameters"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_Load_combinations);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Load combinations"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

		}

	//第三章
		{
			oneBranch = ndItems->addChild(Node);
			oneBranch->addAttribute(type,branch);
			oneBranch->addAttribute(key,0);
			oneBranch->addAttribute(checked,true);
			oneBranch->addAttribute(text,_T("ULTIMATE LIMIT STATES (ULS)"));
			oneBranch->addAttribute(style,0);
			oneBranch->addAttribute(sel,0);

// 			{
// 				oneNode = oneBranch->addChild(Node);
// 				oneNode->addAttribute(type,text);
// 				oneNode->addAttribute(key,STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N2);
// 				oneNode->addAttribute(checked,true);
// 				oneNode->addAttribute(text,_T("Resistance of the cross-section to combined actions M-N(without buckling)"));
// 				oneNode->addAttribute(style,0);
// 				oneNode->addAttribute(sel,1);
// 			}

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,graphic);
				oneNode->addAttribute(key,STS_STRUCT_FIBER_2);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("N-M Curve(without length effect)"));
				oneNode->addAttribute(style,32);
				oneNode->addAttribute(sel,32);
			}

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,graphic);
				oneNode->addAttribute(key,STS_STRUCT_FIBER_1);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("N-M Curve(with length effect)"));
				oneNode->addAttribute(style,32);
				oneNode->addAttribute(sel,32);
			}
		}
		//第四章
		{
			oneBranch = ndItems->addChild(Node);
			oneBranch->addAttribute(type,branch);
			oneBranch->addAttribute(key,0);
			oneBranch->addAttribute(checked,true);
			oneBranch->addAttribute(text,_T("ULTIMATE LIMIT STATES (ULS)"));
			oneBranch->addAttribute(style,0);
			oneBranch->addAttribute(sel,0);

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_SYNTHESIS_OF_VERIFICATIONS2);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Synthesis of verifications"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

		}
	}
	else{		//预定义截面
	//第一章
	{
		oneBranch = ndItems->addChild(Node);
		oneBranch->addAttribute(type,branch);
		oneBranch->addAttribute(key,0);
		oneBranch->addAttribute(checked,true);
		oneBranch->addAttribute(text,_T("PARAMETERS"));
		oneBranch->addAttribute(style,0);
		oneBranch->addAttribute(sel,0);

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_CODES);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Code/Standard"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,0);
		}
		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_GENERAL_PARAMETERS);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("General parameters"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,0);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,graphic);
			oneNode->addAttribute(key,STS_STRUCT_SECTION);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Figure of section"));
			oneNode->addAttribute(style,32);
			oneNode->addAttribute(sel,32);
		}
		
		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_CROSS_SECTION);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Cross-section"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}


		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_Material_properties);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Material properties"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_Creep_coefficient_concrete);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Creep coefficient of the concrete"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_Loadinfo);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Load case"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_Load_cases);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Internal force"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_Calculation_parameters);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Calculation parameters"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

 		{
 			oneNode = oneBranch->addChild(Node);
 			oneNode->addAttribute(type,text);
 			oneNode->addAttribute(key,STS_MEGA_Load_combinations);
 			oneNode->addAttribute(checked,true);
 			oneNode->addAttribute(text,_T("Load combinations"));
 			oneNode->addAttribute(style,0);
 			oneNode->addAttribute(sel,1);
 		}
		
	}


	//第三章
	{
		oneBranch = ndItems->addChild(Node);
		oneBranch->addAttribute(type,branch);
		oneBranch->addAttribute(key,0);
		oneBranch->addAttribute(checked,true);
		oneBranch->addAttribute(text,_T("ULTIMATE LIMIT STATES (ULS)"));
		oneBranch->addAttribute(style,0);
		oneBranch->addAttribute(sel,0);


		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_AXIAL);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Resistance of the cross-section to axial force"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,0);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_SHEAR_RESISTANCE);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Resistance of the cross-section to shear resistance"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}



		bool isFiber = GPara::m_DesnPara->ifFiber;
		if (isFiber)
		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,graphic);
			oneNode->addAttribute(key,STS_STRUCT_FIBER_2);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("N-M Curve(without length effect)"));
			oneNode->addAttribute(style,32);
			oneNode->addAttribute(sel,32);
		}
		else
		{
			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_RESISTANCE_OF_THE_CROSS_SECTION_TO_COMBINED_ACTIONS_M_N);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("Resistance of the cross-section to combined actions M-N(without length effect)"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,1);
			}

			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,graphic);
				oneNode->addAttribute(key,STS_STRUCT_PDM_2);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("N-M Curve(without length effect)"));
				oneNode->addAttribute(style,32);
				oneNode->addAttribute(sel,32);
			}
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_BUCKLING_RESISTANCE_TO_AXIAL_FORCE);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Buckling resistance to axial force"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}
		if(isFiber){
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,graphic);
			oneNode->addAttribute(key,STS_STRUCT_FIBER_1);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("N-M Curve(with length effect)"));
			oneNode->addAttribute(style,32);
			oneNode->addAttribute(sel,32);
		}
		else{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,graphic);
			oneNode->addAttribute(key,STS_STRUCT_PDM_1);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("N-M Curve(with length effect)"));
			oneNode->addAttribute(style,32);
			oneNode->addAttribute(sel,32);
		}


		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_AMPLIFICATION_OF_DESIGN_MOMENT_OF_COMBINED_ACTION_M_N_CONSIDERING);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Amplification of design moment of combined action M-N considering flexural buckling"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}


// 		if (GPara::m_DesnPara->ifFiber)
// 		{
//  			oneBranch = ndItems->addChild(Node);
//  			oneBranch->addAttribute(type,branch);
//  			oneBranch->addAttribute(key,0);
//  			oneBranch->addAttribute(checked,true);
//  			oneBranch->addAttribute(text,_T("N-M Curve"));
//  			oneBranch->addAttribute(style,32);
//  			oneBranch->addAttribute(sel,0);
// 
// 
// 			oneNode = oneBranch->addChild(Node);
// 			oneNode->addAttribute(type,graphic);
// 			oneNode->addAttribute(key,STS_STRUCT_FIBER_1);
// 			oneNode->addAttribute(checked,true);
// 			oneNode->addAttribute(text,_T("N-M Curve of the Fiber Model"));
// 			oneNode->addAttribute(style,32);
// 			oneNode->addAttribute(sel,32);
// 		}
// 		else
// 		{
//  			oneBranch = ndItems->addChild(Node);
//  			oneBranch->addAttribute(type,branch);
//  			oneBranch->addAttribute(key,0);
//  			oneBranch->addAttribute(checked,true);
//  			oneBranch->addAttribute(text,_T("N-M Curve"));
//  			oneBranch->addAttribute(style,32);
//  			oneBranch->addAttribute(sel,0);
// 
// 
// 			oneNode = oneBranch->addChild(Node);
// 			oneNode->addAttribute(type,graphic);
// 			oneNode->addAttribute(key,STS_STRUCT_PDM_1);
// 			oneNode->addAttribute(checked,true);
// 			oneNode->addAttribute(text,_T("N-M Curve of the PDM Method"));
// 			oneNode->addAttribute(style,32);
// 			oneNode->addAttribute(sel,32);
// 		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_INTERFACE_SHEAR_CONNECTION);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Interface shear connection"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_SYNTHESIS_OF_VERIFICATIONS);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("Synthesis of verifications"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,1);
		}

// 		{
// 			oneNode = oneBranch->addChild(Node);
// 			oneNode->addAttribute(type,text);
// 			oneNode->addAttribute(key,STS_MEGA_SYNTHESIS_OF_VERIFICATIONS);
// 			oneNode->addAttribute(checked,true);
// 			oneNode->addAttribute(text,_T("Cross-section"));
// 			oneNode->addAttribute(style,0);
// 			oneNode->addAttribute(sel,1);
// 		}

	}


	//第四章
	{
		oneBranch = ndItems->addChild(Node);
		oneBranch->addAttribute(type,branch);
		oneBranch->addAttribute(key,0);
		oneBranch->addAttribute(checked,true);
		oneBranch->addAttribute(text,_T("SERVICEABILITY LIMIT STATES"));
		oneBranch->addAttribute(style,0);
		oneBranch->addAttribute(sel,0);

// 		{
// 			oneNode = oneBranch->addChild(Node);
// 			oneNode->addAttribute(type,text);
// 			oneNode->addAttribute(key,STS_MEGA_INTERNAL_FORCES);
// 			oneNode->addAttribute(checked,true);
// 			oneNode->addAttribute(text,_T("INTERNAL FORCES"));
// 			oneNode->addAttribute(style,0);
// 			oneNode->addAttribute(sel,0);
// 		}
		if(GPara::m_DesnPara->ifCrackCheck)
		{
			oneNode = oneBranch->addChild(Node);
			oneNode->addAttribute(type,text);
			oneNode->addAttribute(key,STS_MEGA_CRACK_CHECK);
			oneNode->addAttribute(checked,true);
			oneNode->addAttribute(text,_T("MEGA CRACK CHECK"));
			oneNode->addAttribute(style,0);
			oneNode->addAttribute(sel,0);
		}

		int sectkind = GPara::m_Sect_in->m_kind;
		if(sectkind==1 || sectkind==3 || sectkind==4 || sectkind==6 ||sectkind==7){
			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_MEMBER_CHECK1);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("MEMBER CHECK"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,0);
			}
		}
		else if(sectkind==2 ||sectkind==5){
			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_MEMBER_CHECK2);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("MEMBER CHECK"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,0);
			}
		}
		else{
			{
				oneNode = oneBranch->addChild(Node);
				oneNode->addAttribute(type,text);
				oneNode->addAttribute(key,STS_MEGA_MEMBER_CHECK3);
				oneNode->addAttribute(checked,true);
				oneNode->addAttribute(text,_T("MEMBER CHECK"));
				oneNode->addAttribute(style,0);
				oneNode->addAttribute(sel,0);
			}
		}

	}

	}





// 	{
// 		oneBranch = ndItems->addChild(Node);
// 		oneBranch->addAttribute(type,branch);
// 		oneBranch->addAttribute(key,0);
// 		oneBranch->addAttribute(checked,true);
// 		oneBranch->addAttribute(text,_T("荷载与计算结果简图"));
// 		oneBranch->addAttribute(style,32);
// 		oneBranch->addAttribute(sel,0);
// 
// 		oneNode = oneBranch->addChild(Node);
// 		oneNode->addAttribute(type,graphic);
// 		oneNode->addAttribute(key,STS_STRUCT_DRAWING_VIEW);
// 		oneNode->addAttribute(checked,true);
// 		oneNode->addAttribute(text,_T("结构简图"));
// 		oneNode->addAttribute(style,32);
// 		oneNode->addAttribute(sel,32);
// 
// 		oneNode = oneBranch->addChild(Node);
// 		oneNode->addAttribute(type,graphic);
// 		oneNode->addAttribute(key,STS_STRUCT_DRAWING_LOAD);
// 		oneNode->addAttribute(checked,true);
// 		oneNode->addAttribute(text,_T("荷载简图"));
// 		oneNode->addAttribute(style,32);
// 		oneNode->addAttribute(sel,32);
// 
// 		oneNode = oneBranch->addChild(Node);
// 		oneNode->addAttribute(type,graphic);
// 		oneNode->addAttribute(key,STS_STRUCT_DRAWING_STREE_SCALE);
// 		oneNode->addAttribute(checked,true);
// 		oneNode->addAttribute(text,_T("应力比图"));
// 		oneNode->addAttribute(style,32);
// 		oneNode->addAttribute(sel,32);
// 
// 		oneNode = oneBranch->addChild(Node);
// 		oneNode->addAttribute(type,graphic);
// 		oneNode->addAttribute(key,STS_STRUCT_DRAWING_STREE);
// 		oneNode->addAttribute(checked,true);
// 		oneNode->addAttribute(text,_T("内力图"));
// 		oneNode->addAttribute(style,32);
// 		oneNode->addAttribute(sel,32);
// 
// 		
// 
// 		oneNode = oneBranch->addChild(Node);
// 		oneNode->addAttribute(type,graphic);
// 		oneNode->addAttribute(key,STS_STRUCT_DRAWING_DISP);
// 		oneNode->addAttribute(checked,true);
// 		oneNode->addAttribute(text,_T("位移图"));
// 		oneNode->addAttribute(style,32);
// 		oneNode->addAttribute(sel,32);
// 	}
	

}

void CConfigGenCalc::WriteToSelectedItems( slim::XmlNode *ndSelectedItems )
{
	WriteToAllItems( ndSelectedItems );
}

void CConfigGenCalc::WriteToSetup( slim::XmlNode *ndSetup )
{
 	const TCHAR* SetupItem =_T( "SetupItem");
 	const TCHAR* key =_T( "key");
 	const TCHAR* text =_T( "text");
 	const TCHAR* option =_T( "option");
 
 	CString STR;
 

	///////////////////////////
	Json::Value  valFiberNm;
	for (int i=0;i<T_FiberNM.GetSize();i++)
	{
	 	Option(valFiberNm,i+1,T_FiberNM[i].filename,
	 		T_FiberNM[i].bottom,T_FiberNM[i].left,T_FiberNM[i].right,T_FiberNM[i].top,
	 		T_FiberNM[i].caption);
	 
	}
	STR = valFiberNm.toStyledString().c_str();
	STR.Replace('\"','$');
	 
	XmlNode *item = ndSetup->addChild(SetupItem);
	item->addAttribute(key,STS_STRUCT_FIBER_1);
	item->addAttribute(text,_T("Fiber N-M"));
	item->addAttribute(option,STR);


	Json::Value  valPDMNm;
	for (int i=0;i< T_PDMNM.GetSize() ;i++)
	{
		Option(valPDMNm,i+1,T_PDMNM[i].filename,
			T_PDMNM[i].bottom,T_PDMNM[i].left,T_PDMNM[i].right,T_PDMNM[i].top,
			T_PDMNM[i].caption);

	}
	STR = valPDMNm.toStyledString().c_str();
	STR.Replace('\"','$');

	item = ndSetup->addChild(SetupItem);
	item->addAttribute(key,STS_STRUCT_PDM_1);
	item->addAttribute(text,_T("PDM N-M"));
	item->addAttribute(option,STR);


	/////
	Json::Value  valSection;
	for (int i=0;i< T_Section.GetSize() ;i++)
	{
		Option(valSection,i+1,T_Section[i].filename,
			T_Section[i].bottom,T_Section[i].left,T_Section[i].right,T_Section[i].top,
			T_Section[i].caption);

	}
	STR = valSection.toStyledString().c_str();
	STR.Replace('\"','$');

	item = ndSetup->addChild(SetupItem);
	item->addAttribute(key,STS_STRUCT_SECTION);
	item->addAttribute(text,_T("Section"));
	item->addAttribute(option,STR);
	/////

	Json::Value  valFiberNm_2;
	for (int i=0;i<T_FiberNM_2.GetSize();i++)
	{
		Option(valFiberNm_2,i+1,T_FiberNM_2[i].filename,
			T_FiberNM_2[i].bottom,T_FiberNM_2[i].left,T_FiberNM_2[i].right,T_FiberNM_2[i].top,
			T_FiberNM_2[i].caption);

	}
	STR = valFiberNm_2.toStyledString().c_str();
	STR.Replace('\"','$');

	item = ndSetup->addChild(SetupItem);
	item->addAttribute(key,STS_STRUCT_FIBER_2);
	item->addAttribute(text,_T("Fiber N-M(without length effect)"));
	item->addAttribute(option,STR);


	Json::Value  valPDMNm_2;
	for (int i=0;i< T_PDMNM_2.GetSize() ;i++)
	{
		Option(valPDMNm_2,i+1,T_PDMNM_2[i].filename,
			T_PDMNM_2[i].bottom,T_PDMNM_2[i].left,T_PDMNM_2[i].right,T_PDMNM_2[i].top,
			T_PDMNM_2[i].caption);

	}
	STR = valPDMNm_2.toStyledString().c_str();
	STR.Replace('\"','$');

	item = ndSetup->addChild(SetupItem);
	item->addAttribute(key,STS_STRUCT_PDM_2);
	item->addAttribute(text,_T("PDM N-M(without length effect)"));
	item->addAttribute(option,STR);
// 	///////////////////////////
// 	Json::Value  valview;
// 	for (int i=0;i<T_View.GetSize();i++)
// 	{
// 		Option(valview,i+1,T_View[i].filename,
// 			T_View[i].bottom,T_View[i].left,T_View[i].right,T_View[i].top,
// 			T_View[i].caption);
// 
// 	}
// 	STR = valview.toStyledString().c_str();
// 	STR.Replace('\"','$');
// 
// 	XmlNode *item = ndSetup->addChild(SetupItem);
// 	item->addAttribute(key,STS_STRUCT_DRAWING_VIEW);
// 	item->addAttribute(text,_T("结构简图"));
// 	item->addAttribute(option,STR);
// 
// 	///////////////////////////
// 	Json::Value  valLoads;
// 	for (int i=0;i<T_Loads.GetSize();i++)
// 	{
// 		Option(valLoads,i+1,T_Loads[i].filename,
// 			T_Loads[i].bottom,T_Loads[i].left,T_Loads[i].right,T_Loads[i].top,
// 			T_Loads[i].caption);
// 
// 	}
// 	STR = valLoads.toStyledString().c_str();
// 	STR.Replace('\"','$');
// 
// 	item = ndSetup->addChild(SetupItem);
// 	item->addAttribute(key,STS_STRUCT_DRAWING_LOAD);
// 	item->addAttribute(text,_T("荷载简图"));
// 	item->addAttribute(option,STR);
// 
// 	///////////////////////////
// 	Json::Value  valStressSca;
// 	STR = "";
// 	for (int i=0;i<T_StressScale.GetSize();i++)
// 	{
// 		Option(valStressSca,i+1,T_StressScale[i].filename,
// 			T_StressScale[i].bottom,T_StressScale[i].left,T_StressScale[i].right,T_StressScale[i].top,
// 			T_StressScale[i].caption);
// 
// 	}
// 	STR = valStressSca.toStyledString().c_str();
// 	STR.Replace('\"','$');
// 
// 	item = ndSetup->addChild(SetupItem);
// 	item->addAttribute(key,STS_STRUCT_DRAWING_STREE_SCALE);
// 	item->addAttribute(text,_T("应力比图"));
// 	item->addAttribute(option,STR);
// 
// 	///////////////////////////
// 	Json::Value  valStress;
// 	STR = "";
// 	for (int i=0;i<T_Stress.GetSize();i++)
// 	{
// 		Option(valStress,i+1,T_Stress[i].filename,
// 			T_Stress[i].bottom,T_Stress[i].left,T_Stress[i].right,T_Stress[i].top,
// 			T_Stress[i].caption);
// 
// 	}
// 	STR = valStress.toStyledString().c_str();
// 	STR.Replace('\"','$');
// 
// 	item = ndSetup->addChild(SetupItem);
// 	item->addAttribute(key,STS_STRUCT_DRAWING_STREE);
// 	item->addAttribute(text,_T("内力图"));
// 	item->addAttribute(option,STR);
// 
// 	
// 
// 
// 	///////////////////////////
// 	Json::Value  valDisp;
// 	STR = "";
// 	for (int i=0;i<T_Disp.GetSize();i++)
// 	{
// 		Option(valDisp,i+1,T_Disp[i].filename,
// 			T_Disp[i].bottom,T_Disp[i].left,T_Disp[i].right,T_Disp[i].top,
// 			T_Disp[i].caption);
// 
// 	}
// 	STR = valDisp.toStyledString().c_str();
// 	STR.Replace('\"','$');
// 
// 	item = ndSetup->addChild(SetupItem);
// 	item->addAttribute(key,STS_STRUCT_DRAWING_DISP);
// 	item->addAttribute(text,_T("位移图"));
// 	item->addAttribute(option,STR);
	
}

CString CConfigGenCalc::Option(Json::Value&  val,int idx, CString pathfile,double bottom,double left,double right,double top,
	CString drawname,
	bool forceDrawNum,bool forceFontHeight,bool forceFontZoomout,bool foreFontZoomoutMult,int minFontSize)
{
	//Json::Value  val;

	
	CString strFilePath;
	strFilePath.Format("@%04d@%s" ,idx,pathfile);

	Json::Value oneRange;
	oneRange["bottom"] = bottom;
	oneRange["left"] = left;
	oneRange["right"] = right;
	oneRange["top"] = top;


	val[strFilePath]["边界"].append(oneRange);
	val[strFilePath]["强制图数"] =forceDrawNum?"是":"否";
	val[strFilePath]["强制字高"] =forceFontHeight?"是":"否";
	val[strFilePath]["强制字体放大"] =forceFontZoomout?"是":"否";
	val[strFilePath]["图名"] = drawname.GetBuffer();
	val[strFilePath]["字体放大倍数"] =foreFontZoomoutMult;
	val[strFilePath]["最小字高"] =minFontSize;

	CString str1 ( val.toStyledString().c_str());

	return str1;
}

bool CConfigGenCalc::saveToFile( CString& strFileName )
{
	WriteToReportOption();

	{
		CString path;
		::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer(-1);
		path+= _T("\\");

		CString path2 = path;

		CFileFind tempFind;

		CString strAllfile;
		strAllfile.Format(_T("%s*"),CPk_MainOutPut::configXmlPrefix);
		BOOL isFinded = tempFind.FindFile(strAllfile);

		CString file0,file1,file2;
		file0.Format(("%s.xml"),CPk_MainOutPut::configXmlPrefix);
		file1.Format(("%s1.xml"),CPk_MainOutPut::configXmlPrefix);
		file2.Format(("%s2.xml"),CPk_MainOutPut::configXmlPrefix);
		file0.MakeUpper();
		file1.MakeUpper();
		file2.MakeUpper();

		if (isFinded)
		{
			CString strFileDirName2;

			
			while(isFinded)
			{
				isFinded = tempFind.FindNextFile();
				if (!tempFind.IsDots())
				{
					strFileDirName2 = tempFind.GetFilePath();

					strFileDirName2.MakeUpper();
					if (strFileDirName2.Find(file1)>=0)
					{
						CString strdel;
						::GetCurrentDirectory(MAX_PATH,strdel.GetBuffer(MAX_PATH));
						strdel.ReleaseBuffer(-1);
						strdel+= _T("\\");
						strdel+= file1;
						DeleteFile(strdel);

						strFileName =  file2;
						break;
					}
					if (strFileDirName2.Find(file2)>=0)
					{
						CString strdel;
						::GetCurrentDirectory(MAX_PATH,strdel.GetBuffer(MAX_PATH));
						strdel.ReleaseBuffer(-1);
						strdel+= _T("\\");
						strdel+= file2;
						DeleteFile(strdel);

						strFileName =  file1;
						break;
					}
					if (strFileDirName2.Find(file0)>=0)
					{
						CString strdel;
						::GetCurrentDirectory(MAX_PATH,strdel.GetBuffer(MAX_PATH));
						strdel.ReleaseBuffer(-1);
						strdel+= _T("\\");
						strdel+= file0;
						DeleteFile(strdel);

						strFileName =  file1;
						break;
					}
				}
			}
		}
		else
		{
			strFileName =  file1;
		}
		
	}
	CString path;
	{
		::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer(-1);
		path+= _T("\\");
		path+= strFileName;
	}

	m_doc.save(path,Encode::ANSI);

	CStdioFile file;
	file.Open(path,CFile::modeReadWrite);
	file.Seek(0,CFile::begin);
	file.WriteString(_T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n<ReportOption>"));
	file.Close();

	return true;

}

bool CConfigGenCalc::readFromFile(CString strFileName)
{
	CString path;
	{
		::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer(-1);
		path+= _T("\\");
		path+= strFileName;
	}
	bool bRead =  m_doc.loadFromFile(path);
	if (false == bRead)
	{
		return false;
	}

	return ReadFromReportOption(); 

}
void CConfigGenCalc::init()
{
	clear();

}
void CConfigGenCalc::clear()
{
	m_ver = 0;
	m_Title			 = _T("");
	m_ProjNum		 = _T("");
	m_ProjName		 = _T("");
	m_Computer		 = _T("");
	m_MajorOfficer	 = _T("");
	m_Date			 = _T("");
	m_Checker		 = _T("");
	m_Company		 = _T("");
	m_Sketch		 = _T("");
	m_PageHeader	 = _T("");

	m_strPathT = "";
// 	T_Loads.RemoveAll();
// 	T_Stress.RemoveAll();
// 	T_Disp.RemoveAll();
}

void CConfigGenCalc::addT(CArray<ConfigDrawT>& arr, CString filename ,
	CString caption )
{
	filename = m_strPathT + filename;
	if (_taccess_s(filename,0)==0)
	{
		sd::SD_POINT pt1,pt2;
		GReport::g_pReport->CalcTRange(filename, pt1,pt2 );

		ConfigDrawT oneT;
		oneT.filename = filename;
		oneT.caption = caption;


		int drWidth = abs(pt1.x - pt2.x);
		int drHeight = abs(pt1.y - pt2.y);

		double exScaleWid = 10;
		double exScaleHeight= 20;

		oneT.left = pt1.x - drWidth * (exScaleWid / 100);
		oneT.bottom = pt1.y - drHeight * (exScaleHeight / 100)  ;
		
		oneT.right = pt2.x + drWidth * (exScaleWid / 100);
		oneT.top = pt2.y + drHeight * (exScaleHeight / 100);

		arr.Add(oneT); 
	}
	
}
void CConfigGenCalc::addT0001(CArray<ConfigDrawT>& arr, CString filename ,
	CString caption )
{
	filename = m_strPathT + filename;
	if (_taccess_s(filename,0)==0)
	{
		sd::SD_POINT pt1,pt2;
		GReport::g_pReport->CalcTRange(filename, pt1,pt2 );

		ConfigDrawT oneT;
		oneT.filename = filename;
		oneT.caption = caption;


		int drWidth = abs(pt1.x - pt2.x);
		int drHeight = abs(pt1.y - pt2.y);

		double exScaleWid = 10;
		double exScaleHeight= 20;

		oneT.left = pt1.x - drWidth * (exScaleWid / 100);
		oneT.bottom = pt1.y - drHeight * (exScaleHeight / 100)-2  ;

		oneT.right = pt2.x + drWidth * (exScaleWid / 100);
		oneT.top = pt2.y + drHeight * (exScaleHeight / 100)+2;

		arr.Add(oneT); 
	}
}
// void CConfigGenCalc::addViewT( CString filename ,CString caption )
// {
// 	addT(T_View,filename,caption );
// }
// void CConfigGenCalc::addLoadsT( CString filename ,CString caption )
// {
// 	addT(T_Loads,filename,caption );
// }
// 
// void CConfigGenCalc::addStressT( CString filename ,CString caption )
// {
// 	addT0001(T_Stress,filename,caption );
// }
// void CConfigGenCalc::addStressScaleT(CString filename ,CString caption)
// {
// 	addT0001(T_StressScale,filename,caption );
// }
// void CConfigGenCalc::addDispT( CString filename ,CString caption )
// {
// 	
// 	addT0001(T_Disp,filename,caption );
// }
void CConfigGenCalc::addFiberNM_T(CString filename ,CString caption)
{
	addT(T_FiberNM,filename,caption );
}
void CConfigGenCalc::addPDMNM_T(CString filename ,CString caption)
{
	addT(T_PDMNM,filename,caption );
}

void CConfigGenCalc::addFiberNM_T2(CString filename ,CString caption)
{
	addT(T_FiberNM_2,filename,caption );
}
void CConfigGenCalc::addPDMNM_T2(CString filename ,CString caption)
{
	addT(T_PDMNM_2,filename,caption );
}

void CConfigGenCalc::addSection_T(CString filename ,CString caption)
{
	addT(T_Section,filename,caption );
}

void CConfigGenCalc::setPathT( CString str )
{
	CString path;
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer(-1);
	path+= _T("\\");
	if (str.IsEmpty())
	{
		m_strPathT = path;
	}
	else
	{
		m_strPathT = path + str;
		CfgSvrUser::FixPathStr(m_strPathT);
	}
	
}

bool CConfigGenCalc::DeleteCalculationReportXml()
{
	CString strFile;
	CString pathWork;
	{
		::GetCurrentDirectory(MAX_PATH,pathWork.GetBuffer(MAX_PATH));
		pathWork.ReleaseBuffer(-1);
		pathWork+= _T("\\");
	}
	strFile = pathWork + CalculationReportXml;
	::DeleteFile(strFile);
	return true;
}

const TCHAR* CConfigGenCalc::CalculationReportXml = _T("CalculationReport.XML");
const TCHAR* CConfigGenCalc::XML_FILE_NAME = _T("MjCalcReport.xml");








////////////////////////JC CONFIG///////////////////////////////

using namespace slim;

IMPLEMENT_DYNAMIC(CConfigGenJC,CInfoBase)
	CConfigGenJC::CConfigGenJC(void)
{
	m_ver = 1;
	m_Title = _T("刚架基础计算书");
	m_ProjNum = _T("No.1");
	m_ProjName = _T("XXX项目");
	m_Computer = _T("XXX设计师");
	m_MajorOfficer = _T("XXX总工");

	CTime t =  CTime::GetCurrentTime();
	m_Date.Format(_T("%d-%02d-%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
	m_Checker			= _T("XXX设计师"); 
	m_Company			= _T("XXXX设计研究院"); 
	m_Sketch			= _T("C:\\Users\\Eric\\Pictures\\效果图.jpg"); 
}


CConfigGenJC::~CConfigGenJC(void)
{
}

void CConfigGenJC::Version(XmlNode *ndVersion)
{
	ndVersion->setInt(m_ver);
}

void CConfigGenJC::ReportOption()
{

	const TCHAR* marker_ReportOption =_T( "ReportOption");

	const TCHAR* marker_Version =_T( "Version");

	const TCHAR* marker_Cover =_T( "Cover");
	const TCHAR* marker_Font =_T( "Font");
	const TCHAR* marker_General =_T( "General");
	const TCHAR* marker_HeaderFooter =_T( "HeaderFooter");

	const TCHAR* marker_Layout =_T( "Layout");
	const TCHAR* marker_Table =_T( "Table");
	const TCHAR* marker_NumStyle =_T( "NumStyle");
	const TCHAR* marker_AllItems =_T( "AllItems");
	const TCHAR* marker_SelectedItems =_T( "SelectedItems");
	const TCHAR* marker_Setup =_T( "Setup");


	XmlNode*  ndReportOption =  doc.addChild(marker_ReportOption);



	XmlNode*  ndVersion = ndReportOption->addChild(marker_Version);
	XmlNode*  ndConver	=  ndReportOption->addChild(marker_Cover);
	XmlNode*  ndFont	=  ndReportOption->addChild(marker_Font);

	XmlNode*  ndGeneral1			=ndReportOption->addChild(marker_General);
	XmlNode*  ndGeneral2			=ndReportOption->addChild(marker_General);
	XmlNode*  ndHeaderFooter	=ndReportOption->addChild(marker_HeaderFooter);
	XmlNode*  ndLayout			=ndReportOption->addChild(marker_Layout);

	XmlNode*  ndTable 			=ndReportOption->addChild(marker_Table );
	XmlNode*  ndNumStyle 		=ndReportOption->addChild(marker_NumStyle );
	XmlNode*  ndAllItems 		=ndReportOption->addChild(marker_AllItems );

	XmlNode*  ndSelectedItems 	=ndReportOption->addChild(marker_SelectedItems );
	XmlNode*  ndSetup 			=ndReportOption->addChild(marker_Setup );

	Version(ndVersion);
	Cover(ndConver);
	Font(ndFont);
	General1(ndGeneral1);
	General2(ndGeneral2);

	HeaderFooter(ndHeaderFooter);

	Layout(ndLayout);
	Table(ndTable);
	NumStyle(ndNumStyle);

	AllItems(ndAllItems);
	SelectedItems(ndSelectedItems);
	Setup(ndSetup);
}

void CConfigGenJC::Cover( XmlNode* ndCover)
{
	const TCHAR* output =_T( "output");
	const TCHAR* fontname = _T("fontname");
	const TCHAR* fontheight =_T( "fontheight");
	const TCHAR* underline =_T( "underline");
	const TCHAR* delline =_T( "delline");
	const TCHAR* bold = _T("bold");
	const TCHAR* color = _T("color");
	const TCHAR* autobuild = _T("autobuild");

	XmlNode * nd = ndCover->addChild("Title");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("黑体"));
	nd->addAttribute(fontheight,48);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_Title);

	nd = ndCover->addChild("ProjNum");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_ProjNum);

	nd = ndCover->addChild("ProjName");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_ProjName);

	nd = ndCover->addChild("Computer");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_Computer);

	nd = ndCover->addChild("MajorOfficer");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_MajorOfficer);

	nd = ndCover->addChild("Date");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_Date);

	nd = ndCover->addChild("Checker");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_Checker);

	nd = ndCover->addChild("Company");
	nd->addAttribute(output,true);
	nd->addAttribute(fontname,_T("黑体"));
	nd->addAttribute(fontheight,22);
	nd->addAttribute(underline,false);
	nd->addAttribute(delline,false);
	nd->addAttribute(bold,true);
	nd->addAttribute(color,0);
	nd->setString(m_Company);

	nd = ndCover->addChild("Sketch");
	nd->addAttribute(output,true);
	nd->addAttribute(autobuild,false);
	nd->setString(m_Sketch);
}

void CConfigGenJC::Font( slim::XmlNode *ndFont )
{
	const TCHAR* FirstLevel =_T( "FirstLevel");
	const TCHAR* SecondLevel =_T( "SecondLevel");
	const TCHAR* ThirdLevel =_T( "ThirdLevel");
	const TCHAR* Content =_T( "Content");
	const TCHAR* TableHeader =_T( "TableHeader");
	const TCHAR* TableContent =_T( "TableContent");
	const TCHAR* Header =_T( "Header");
	const TCHAR* Footer =_T( "Footer");
	const TCHAR* GraphicTitle =_T( "GraphicTitle");


	const TCHAR* fontname =_T( "fontname");
	const TCHAR* fontheight =_T( "fontheight");
	const TCHAR* underline =_T( "underline");
	const TCHAR* bold =_T( "bold");


	XmlNode * nd = ndFont->addChild(FirstLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,16);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(SecondLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,14);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(ThirdLevel);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(Content);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(TableHeader);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(TableHeader);
	nd->addAttribute(fontname,_T("黑体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,true);

	nd = ndFont->addChild(TableContent);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(Header);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(Footer);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

	nd = ndFont->addChild(GraphicTitle);
	nd->addAttribute(fontname,_T("宋体"));
	nd->addAttribute(fontheight,12);
	nd->addAttribute(underline,false);
	nd->addAttribute(bold,false);

}

void CConfigGenJC::General1( slim::XmlNode *ndGeneral1 )
{
	XmlNode * nd = ndGeneral1->addChild("Output");
	nd->addAttribute("style",0);

}

void CConfigGenJC::General2( slim::XmlNode *ndGeneral2 )
{
	XmlNode * nd = ndGeneral2->addChild("ColorScheme");
	nd->addAttribute("val",1);
}

void CConfigGenJC::HeaderFooter( slim::XmlNode *ndHeaderFooter )
{
	const TCHAR* Header =_T( "Header");
	const TCHAR* text =_T( "text");
	const TCHAR* align =_T( "align");
	const TCHAR* logo =_T( "logo");
	const TCHAR* use =_T( "use");
	const TCHAR* leftifalignsame =_T( "leftifalignsame");

	const TCHAR* Footer =_T( "Footer");
	const TCHAR* type =_T( "type");

	XmlNode * ndHeader = ndHeaderFooter->addChild("Header");
	XmlNode *  ndtext= ndHeader->addChild(text);
	ndtext->addAttribute(align,2);
	ndtext->setString(m_PageHeader);

	XmlNode *  ndlogo=  ndHeader->addChild(logo);
	ndlogo->addAttribute(align,1);
	ndlogo->addAttribute(use,false);
	ndlogo->addAttribute(leftifalignsame,true);



	XmlNode* ndFooter = ndHeaderFooter->addChild(Footer);
	ndFooter->addAttribute(type,2);
	ndFooter->addAttribute(align,2);

}

void CConfigGenJC::Layout( slim::XmlNode *ndLayout )
{
	const TCHAR* TextPart =_T( "TextPart");
	const TCHAR* Page =_T( "Page");
	const TCHAR* Direction =_T( "Direction");
	const TCHAR* Margin =_T( "Margin");
	const TCHAR* left =_T( "left");

	const TCHAR* right =_T( "right");
	const TCHAR* top =_T( "top");
	const TCHAR* bottom =_T( "bottom");
	const TCHAR* ColNum =_T( "ColNum");

	const TCHAR* GraphicPart =_T( "GraphicPart");


	XmlNode* nd=nullptr;

	XmlNode * ndTextPart = ndLayout->addChild(TextPart);
	nd = ndTextPart->addChild(Page);
	nd->setString(_T("A4"));
	nd = ndTextPart->addChild(Direction);
	nd->setString(_T("Veri"));
	nd = ndTextPart->addChild(Margin);
	nd->addAttribute(left,25);
	nd->addAttribute(right,25);
	nd->addAttribute(top,32);
	nd->addAttribute(bottom,32);
	nd = ndTextPart->addChild(ColNum);
	nd->setInt(1);

	XmlNode * ndGraphicPart = ndLayout->addChild(GraphicPart);
	nd = ndGraphicPart->addChild(Page);
	nd->setString(_T("A4"));
	nd = ndGraphicPart->addChild(Direction);
	nd->setString(_T("Hori"));
	nd = ndGraphicPart->addChild(Margin);
	nd->addAttribute(left,25);
	nd->addAttribute(right,15);
	nd->addAttribute(top,10);
	nd->addAttribute(bottom,8);
	nd = ndGraphicPart->addChild(ColNum);
	nd->setInt(1);

}

void CConfigGenJC::Table( slim::XmlNode *ndTable )
{
	const TCHAR* Border =_T( "Border");
	const TCHAR* style =_T( "style");
	const TCHAR* Title =_T( "Title");
	const TCHAR* align =_T( "align");

	XmlNode* nd;
	nd = ndTable->addChild(Border);
	nd->addAttribute(style,0);

	nd = ndTable->addChild(Title);
	nd->addAttribute(align,2);
}

void CConfigGenJC::NumStyle( slim::XmlNode *ndNumStyle )
{
	const TCHAR* FirstLevel =_T( "FirstLevel");
	const TCHAR* SecondLevel =_T( "SecondLevel");
	const TCHAR* ThirdLevel =_T( "ThirdLevel");

	const TCHAR* type =_T( "type");
	const TCHAR* prefix =_T( "prefix");
	const TCHAR* suffix =_T( "suffix");

	XmlNode* ndFirstLevel =  ndNumStyle->addChild(FirstLevel);
	ndFirstLevel->addAttribute(type,2);
	ndFirstLevel->addAttribute(prefix,_T(""));
	ndFirstLevel->addAttribute(suffix,_T("."));

	XmlNode* ndSecondLevel =  ndNumStyle->addChild(SecondLevel);
	ndSecondLevel->addAttribute(type,1);
	ndSecondLevel->addAttribute(prefix,_T(""));
	ndSecondLevel->addAttribute(suffix,_T("."));

	XmlNode* ndThirdLevel =  ndNumStyle->addChild(ThirdLevel);
	ndThirdLevel->addAttribute(type,1);
	ndThirdLevel->addAttribute(prefix,_T("("));
	ndThirdLevel->addAttribute(suffix,_T(")."));



}

void CConfigGenJC::AllItems( slim::XmlNode *ndItems )
{
	const TCHAR* Node =_T( "Node");
	const TCHAR* type =_T( "type");
	const TCHAR* key =_T( "key");

	const TCHAR* checked =_T( "checked");
	const TCHAR* text =_T( "text");
	const TCHAR* style =_T( "style");

	const TCHAR* sel =_T( "sel");


	const TCHAR* branch =_T( "branch"); 
	const TCHAR* graphic =_T( "graphic");

	XmlNode* oneNode  ;
	XmlNode* oneBranch  ;
	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,410);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("设计依据"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,1);

	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,415);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("计算软件信息"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,1);

	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,420);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("示意图"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,1);

	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,435);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("荷载效应组合"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,0);

	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,440);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("基础计算结果"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,0);

	oneNode = ndItems->addChild(Node);
	oneNode->addAttribute(type,text);
	oneNode->addAttribute(key,450);
	oneNode->addAttribute(checked,true);
	oneNode->addAttribute(text,_T("附录"));
	oneNode->addAttribute(style,0);
	oneNode->addAttribute(sel,0);

}

void CConfigGenJC::SelectedItems( slim::XmlNode *ndSelectedItems )
{
	AllItems( ndSelectedItems );
}

void CConfigGenJC::Setup( slim::XmlNode *ndSetup )
{
	 
}

CString CConfigGenJC::Option(Json::Value&  val,int idx, CString pathfile,double bottom,double left,double right,double top,
	CString drawname,
	bool forceDrawNum,bool forceFontHeight,bool forceFontZoomout,bool foreFontZoomoutMult,int minFontSize)
{
	//Json::Value  val;


	CString strFilePath;
	strFilePath.Format("@%04d@%s" ,idx,pathfile);

	Json::Value oneRange;
	oneRange["bottom"] = bottom;
	oneRange["left"] = left;
	oneRange["right"] = right;
	oneRange["top"] = top;


	val[strFilePath]["边界"].append(oneRange);
	val[strFilePath]["强制图数"] =forceDrawNum?"是":"否";
	val[strFilePath]["强制字高"] =forceFontHeight?"是":"否";
	val[strFilePath]["强制字体放大"] =forceFontZoomout?"是":"否";
	val[strFilePath]["图名"] = drawname.GetBuffer();
	val[strFilePath]["字体放大倍数"] =foreFontZoomoutMult;
	val[strFilePath]["最小字高"] =minFontSize;

	CString str1 ( val.toStyledString().c_str());

	return str1;
}

bool CConfigGenJC::saveTo( CString strFileName )
{
	ReportOption();

	CString path;
	{
		::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
		path.ReleaseBuffer(-1);
		path+= _T("\\");
		path+= strFileName;
	}

	doc.save(path,Encode::ANSI);

	CStdioFile file;
	file.Open(path,CFile::modeReadWrite);
	file.Seek(0,CFile::begin);
	file.WriteString(_T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n<ReportOption>"));
	file.Close();

	return true;

}
void CConfigGenJC::init()
{
	clear();

}
void CConfigGenJC::clear()
{
	m_ver = 0;
	m_Title			 = _T("");
	m_ProjNum		 = _T("");
	m_ProjName		 = _T("");
	m_Computer		 = _T("");
	m_MajorOfficer	 = _T("");
	m_Date			 = _T("");
	m_Checker		 = _T("");
	m_Company		 = _T("");
	m_Sketch		 = _T("");
	m_PageHeader	 = _T("");

	m_strPathT = "";
	T_Loads.RemoveAll();
	T_Stress.RemoveAll();
	T_Disp.RemoveAll();
}

 

const TCHAR* CConfigGenJC::XML_FILE_NAME = _T("MjJcReport.xml");



bool PrintPageSize::GetSizeA(int Type,bool bVertical,int& Width , int& Height)
{
	int a = 0,b = 0;
	switch (Type)
	{
	case A0:
		a = 841 ;
		b = 1189;
		break;
	case A1:
		a = 594;
		b = 841;
		break;
	case A2:
		a = 420;
		b = 594;
		break;
	case A3:
		a = 297;
		b = 420;
		break;
	case A4:
		a = 210;
		b = 297;
		break;
	case A5:
		a = 148;
		b = 210;
		break;
	default:
		return false;
	}

	if (bVertical)
	{
		Width = a;
		Height = b;
	}
	else
	{
		Width = b;
		Height = a;
	}

	return true;

}

bool PrintPageSize::GetSizeA( CString Type,bool bVertical,int& Width,int& Height)
{
	int ty = -1;
	if (0 == Type.CompareNoCase("A0"))
	{
		ty = 0;
	}
	else if (0 == Type.CompareNoCase("A1"))
	{
		ty = 1;
	}
	else if (0 == Type.CompareNoCase("A2"))
	{
		ty = 2;
	}
	else if (0 == Type.CompareNoCase("A3"))
	{
		ty = 3;
	}
	else if (0 == Type.CompareNoCase("A4"))
	{
		ty = 4;
	}
	else if (0 == Type.CompareNoCase("A5"))
	{
		ty = 5;
	}
	return GetSizeA(ty,bVertical,Width,Height);

}
