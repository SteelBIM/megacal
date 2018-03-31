#pragma once
#include "IGenerateText.h"
/** @class  ICalculationReport
 *  @brief	�������ӿ��ļ�
 *  @author Eric
 *  @date	2016/05/31
 *  ------------------------------------------------------------
 *  �汾��ʷ       ע��                       ����
 *  ------------------------------------------------------------
 *  @version v1.0  ��ʼ�汾                   2016/05/31
 *  ------------------------------------------------------------
 *  @note:  -  
 */

namespace sd
{
enum SdReportSetupOption
{//���������ÿ�����
	sdReportSetupOpt_None=0,					//��

	sdReportSetupOpt_Cover=1<<0,				//��ʾ����ҳ
	sdReportSetupOpt_Content=1<<1,				//��ʾ����ҳ
	sdReportSetupOpt_Layout=1<<2,				//��ʾ����ҳ
	sdReportSetupOpt_Table=1<<3,				//��ʾ���ҳ
	sdReportSetupOpt_HeaderFooter=1<<4,			//��ʾҳüҳ��ҳ
	sdReportSetupOpt_Font=1<<5,					//��ʾ����ҳ
	sdReportSetupOpt_Graphic=1<<6,				//��ʾͼ��ҳ
	sdReportSetupOpt_Output=1<<7,				//��ʾ�����ʽҳ

	sdReportSetupOpt_Template=1<<8,				//��ʾģ����
	sdReportSetupOpt_BuildButton=1<<9,			//��ʾ"���ɼ�����"��ť

	sdReportSetupOpt_All=0xFFFF,				//ȫ��
};

enum SdReportBuildOption
{//���������ɿ�����
	sdReportBuildOpt_None=0,					//��
	sdReportBuildOpt_ShowProgress=1<<0,			//��ʾ������
	sdReportBuildOpt_Open=1<<1,					//���ɺ��Զ���

	sdReportBuildOpt_TitleAlignLeft=1<<2,		//����������루Ĭ�Ͼ���)
	sdReportBuildOpt_Pdf_VectorGraphic=1<<3,	//PDF������ʸ��ͼ��ÿͼ��ռһҳ)
	
	sdReportBuildOpt_Default= sdReportBuildOpt_ShowProgress | sdReportBuildOpt_Open 
		| sdReportBuildOpt_TitleAlignLeft | sdReportBuildOpt_Pdf_VectorGraphic,
												//Ĭ�Ϸ�ʽ
};

enum OPTION_TYPE
{//��������ͼ������(����Сͼ����ʾ�����ͼ������)
	OPTION_NONE,				//0. ��
	OPTION_TEXT=1<<0,			//1. ����(���)
	OPTION_LINE_CHART=1<<1,		//2. ����ͼ
	OPTION_PIE_CHART=1<<2,		//4. ��ͼ
	OPTION_HISTOGRAM=1<<3,		//8. ��״ͼ
	OPTION_GRAPHIC=1<<4,		//16.Tͼ
	OPTION_AUTO_SPLIT=1<<5,		//32.�Զ���ͼ
};



enum CATALOG_LEVEL
{//Ŀ¼����
	LEVEL_1 = 0,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4
};

enum FILE_TYPE
{//�ļ���ʽ
	FILE_WORD,
	FILE_PDF,
	FILE_TXT,
};

typedef struct tagCALC_ITEM_OPTION
{//����
	CATALOG_LEVEL eLevel;	//Ŀ¼����
}CALC_ITEM_OPTION,*PCALC_ITEM_OPTION;

enum CALC_ITEM
{
	ITEM_NONE,			//��
	FRONT_COVER,		//����
	DESIGN_BASIS,		//�������
	SOFTWARE,			//�������
};


struct CHAPTER_SECTION
{//�½���Ϣ
	unsigned level_1;
	unsigned level_2;
	unsigned level_3;
	unsigned level_4;

	CHAPTER_SECTION()
	{
		reset();
	}

	void reset()
	{
		level_1=0;
		level_2=0;
		level_3=0;
		level_4=0;
	}

	void add(CATALOG_LEVEL eLevel)
	{
		switch(eLevel)
		{
		case LEVEL_1:
			{
				level_1++;
				level_2=level_3=level_4=0;
				break;
			}
		case LEVEL_2:
			{
				level_2++;;
				level_3=level_4=0;
				break;
			}
		case LEVEL_3:
			{
				level_3++;
				level_4=0;
				break;
			}
		case LEVEL_4:
			{
				level_4++;
				break;
			}
		}
	}
};

typedef struct tagSD_POINT
{//����λ��
	int x;
	int y;
	tagSD_POINT()
	{
		x=y=0;
	}
}SD_POINT,*PSD_POINT;

#define NODE_KEY unsigned

typedef struct tagREPORT_ITEM
{//��������
	NODE_KEY key;
	const char* pText;
	const char* pJsonOption;		//������
									//�ı��� Key: "text_opt"
									//����ͼ Key: "line_chart_opt"
									//��ͼ   Key: "pie_chart_opt"
									//���磺 {"text_opt":{"������Ϣ":"��","������Ϣ":"��"}}


	CHAPTER_SECTION chapterSection;	//�½ں�

	//����
	LOGFONT fontContent;			//����
	LOGFONT fontTableHeader;		//��ͷ
	LOGFONT fontTableContent;		//�������
	LOGFONT fontGraphicTitle;		//ͼ��

	//ͼ����ɫ
	int colorScheme;				//��ɫ���� 0:��ɫ 1:�ڰ�

	//���
	SdTabBorderStyle tableStyle;	//�����ʽ
	SdAlign tableTitleAlign;		//��������뷽ʽ

	OPTION_TYPE optionType;			//������(�ı�������ͼ����״ͼ��)

	tagREPORT_ITEM()
	{
		key=0;
		colorScheme=0;
		pText=NULL;
		pJsonOption=NULL;
		tableStyle=sdTabBorderHeadWide_NoneSide;
		tableTitleAlign=sdAlignCenter;
		optionType=OPTION_NONE;
	}

}REPORT_ITEM,*PREPORT_ITEM;

typedef struct tagGRAPHIC_ITEM
{//Tͼ���������
	NODE_KEY key;
	const char* pTFileName;					//ԭʼTͼ�ļ���
	std::vector<const char*> vctEmfFile;	//�ָ�õ�emf�ļ���
	std::vector<const char*> vctTitleName;	//�ָ�õĸ�������

	CHAPTER_SECTION chapterSection;			//�½ں�
	LOGFONT fontGraphicTitle;				//ͼ������

	int pageWidth;							//ֽ�ſ�
	int pageHeight;							//ֽ�Ÿ�
}GRAPHIC_ITEM,*PGRAPHIC_ITEM;



typedef bool (* FUN_REPORT_ITEM)(IGenerateText* ,const REPORT_ITEM&);

typedef bool (* FUN_GRAPHIC_ITEM)(IGenerateText* ,const GRAPHIC_ITEM&);

typedef bool (* FUN_ONCLICK_ITEM)(NODE_KEY,OPTION_TYPE,SD_POINT);
//�ڵ��ţ�Сͼ�����ͣ������λ��




class ICalculationReport
{
public:
	virtual ~ICalculationReport(){};
public:
	/**
	* @breif  ָ�������ļ�xml�ļ�
	* @param  sXmlFile - xml�ļ���(��·��)
	* @note	  �����ָ��,Ĭ�ϲ��� dllģ������·��\config\CalculationReport.xml�ļ� 
	*/
	virtual void setConfigXml(const char* sXmlFile)=0;

	/**
	* @breif  ���ü������ʽ
	* @param  eSetupOpt - ���ÿ�����
	* @param  eBuildOpt - ���ɿ�����
	* @param  sReportPathName - ���ɵ��ļ�����(��·��������Ҫ����׺��.docx,.txt, .pdf), �����ָ����������Ϊ��ʱĿ¼\������.***
	* @note   �����û����������������ֱ�ӵ�����ɼ����飬���Դ˴��ṩ���ɿ�����
	*/
	virtual void setupOption(SdReportSetupOption eSetupOpt=sdReportSetupOpt_All,
		SdReportBuildOption eBuildOpt=sdReportBuildOpt_Default,const char *sReportPathName=NULL)=0;

	/**
	* @breif  ���ɼ�����
	* @param  opt - ������
	*/
	virtual bool buildReport(SdReportBuildOption opt=sdReportBuildOpt_Default)=0;
	
	/**
	* @breif  ��������
	* @param  sXmlFile - xml�ļ���(��·��)
	* @retrn  �����Ƿ�ɹ�
	* @note	  �����ָ��,Ĭ�ϲ��õ�ǰ�򿪵������ļ��� 
	*/
	virtual bool saveConfigXml(const char* sXmlFile)=0;

public:
	/**
	* @breif  չʾ��ص�����ע��
	* @param  FUN_REPORT_ITEM - չʾĳ��
	* @note   ���ɼ���������У�ÿ��(����Tͼ��)չʾ���ص��˺������û��ڴ˺����б�д��������ɴ���
	*/
	virtual void registeReportCallback(FUN_REPORT_ITEM)=0;

	/**
	* @breif  ͼ�Σ�Tͼ)��ص�����ע��
	* @param  FUN_REPORT_ITEM - չʾĳ��
	* @note   ���ɼ���������У�ͼ�Σ�Tͼ)��չʾ���ص��˺������û��ڴ˺����б�д��������ɴ�
	*         ����û�δע�ᣬ���߻ص���������false, �򽫵����м�����÷�������ָ�õ�Tͼ��
	*         ����û��ڻص��������Ѵ�����Tͼ���룬������true,���м�������ٲ��ͼ��
	*/
	virtual void registeGraphicCallback(FUN_GRAPHIC_ITEM)=0;

	/**
	* @breif  �����������¼�����ע��
	* @param  FUN_ONCLICK_ITEM - �����¼���Ӧ����
	* @note   ����������ϵ�ĳ��ͼ��ʱ�������ô˺���. ���δע��˺��������ߺ�������false,�򶼻�����������ô��ڡ�
	*         ����ص���������true,��ʾ�û��Ѿ������˴��¼����򲻻ᵯ�����ô���
	*/
	virtual void registeOnClickItemCallback(FUN_ONCLICK_ITEM)=0;

	/**
	* @breif  �޸�ĳ������
	* @param  NODE_KEY - ������Keyֵ
	* @param  OPTION_TYPE - ���������
	* @note   pJsonOption - ���������ݣ�Json��ʽ)
	*/
	virtual bool modifyNodeOption(NODE_KEY,OPTION_TYPE,const char* pJsonOption)=0;

	/**
	* @breif  ��ȡĳ������
	* @param  NODE_KEY - ������Keyֵ
	* @param  OPTION_TYPE - ���������
	* @retrn  ���������ݣ�Json��ʽ)
	*/
	virtual const char* getNodeOption(NODE_KEY,OPTION_TYPE)=0;
public:
	/** 
	 * @brief Tͼתemfͼ
	 * @para  sTFileName - Tͼ�ļ�(��·��)
	 * @para  sEmfFile - Emfʸ��ͼ�ļ�(��·��)
	 */
	virtual bool transformTtoEmf(const char* sTFile,const char* sEmfFile)=0;

	/** 
	 * @brief Tͼתjpgͼ
	 * @para  sTFileName - Tͼ�ļ�(��·��)
	 * @para  sEmfFile - jpgͼƬ�ļ�(��·��)
	 */
	virtual bool transformTtoJpg(const char* sTFile,const char* sJpgFile)=0;

	/** 
	 * @brief TͼתPdfʸ��ͼ
	 * @para  sTFileName - Tͼ�ļ�(��·��)
	 * @para  sPdfFile - PDF�ļ�(��·��)
	 * @note  ���ɵ�pdfͼ���ļ��������ڲ��뵽��һ��pdf�ļ���
	 */
	virtual bool transformTtoPdf(const char* sTFile,const char* sPdfFile)=0;

	/** 
	 * @brief ��ȡTͼ�ı߽緶Χ
	 * @para  sTFileName - Tͼ�ļ�(��·��)
	 * @para  [���] leftBottomPt - ���½�����
	 * @para  [���] rightTopPt - ���Ͻ�����
	 */
	virtual bool CalcTRange(const char* sTFile, SD_POINT& leftBottomPt/*�������*/, SD_POINT& rightTopPt/*�������*/ )=0;

public:
	/** 
	 * @brief �����ǰ�����ʽ
	 * @note  word,pdf��txt
	 */
	virtual FILE_TYPE getCurrentFileType()=0;

};
}