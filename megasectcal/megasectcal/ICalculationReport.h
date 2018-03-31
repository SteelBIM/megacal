#pragma once
#include "IGenerateText.h"
/** @class  ICalculationReport
 *  @brief	计算书库接口文件
 *  @author Eric
 *  @date	2016/05/31
 *  ------------------------------------------------------------
 *  版本历史       注释                       日期
 *  ------------------------------------------------------------
 *  @version v1.0  初始版本                   2016/05/31
 *  ------------------------------------------------------------
 *  @note:  -  
 */

namespace sd
{
enum SdReportSetupOption
{//计算书设置控制项
	sdReportSetupOpt_None=0,					//无

	sdReportSetupOpt_Cover=1<<0,				//显示封面页
	sdReportSetupOpt_Content=1<<1,				//显示内容页
	sdReportSetupOpt_Layout=1<<2,				//显示布局页
	sdReportSetupOpt_Table=1<<3,				//显示表格页
	sdReportSetupOpt_HeaderFooter=1<<4,			//显示页眉页脚页
	sdReportSetupOpt_Font=1<<5,					//显示字体页
	sdReportSetupOpt_Graphic=1<<6,				//显示图形页
	sdReportSetupOpt_Output=1<<7,				//显示输出方式页

	sdReportSetupOpt_Template=1<<8,				//显示模板项
	sdReportSetupOpt_BuildButton=1<<9,			//显示"生成计算书"按钮

	sdReportSetupOpt_All=0xFFFF,				//全部
};

enum SdReportBuildOption
{//计算书生成控制项
	sdReportBuildOpt_None=0,					//无
	sdReportBuildOpt_ShowProgress=1<<0,			//显示进度条
	sdReportBuildOpt_Open=1<<1,					//生成后自动打开

	sdReportBuildOpt_TitleAlignLeft=1<<2,		//各标题左对齐（默认居中)
	sdReportBuildOpt_Pdf_VectorGraphic=1<<3,	//PDF中生成矢量图（每图独占一页)
	
	sdReportBuildOpt_Default= sdReportBuildOpt_ShowProgress | sdReportBuildOpt_Open 
		| sdReportBuildOpt_TitleAlignLeft | sdReportBuildOpt_Pdf_VectorGraphic,
												//默认方式
};

enum OPTION_TYPE
{//内容项上图标类型(控制小图标显示及输出图形内容)
	OPTION_NONE,				//0. 无
	OPTION_TEXT=1<<0,			//1. 文字(表格)
	OPTION_LINE_CHART=1<<1,		//2. 折线图
	OPTION_PIE_CHART=1<<2,		//4. 饼图
	OPTION_HISTOGRAM=1<<3,		//8. 柱状图
	OPTION_GRAPHIC=1<<4,		//16.T图
	OPTION_AUTO_SPLIT=1<<5,		//32.自动分图
};



enum CATALOG_LEVEL
{//目录级别
	LEVEL_1 = 0,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4
};

enum FILE_TYPE
{//文件格式
	FILE_WORD,
	FILE_PDF,
	FILE_TXT,
};

typedef struct tagCALC_ITEM_OPTION
{//设置
	CATALOG_LEVEL eLevel;	//目录级别
}CALC_ITEM_OPTION,*PCALC_ITEM_OPTION;

enum CALC_ITEM
{
	ITEM_NONE,			//无
	FRONT_COVER,		//封面
	DESIGN_BASIS,		//设计依据
	SOFTWARE,			//计算软件
};


struct CHAPTER_SECTION
{//章节信息
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
{//坐标位置
	int x;
	int y;
	tagSD_POINT()
	{
		x=y=0;
	}
}SD_POINT,*PSD_POINT;

#define NODE_KEY unsigned

typedef struct tagREPORT_ITEM
{//单项设置
	NODE_KEY key;
	const char* pText;
	const char* pJsonOption;		//设置项
									//文本项 Key: "text_opt"
									//折线图 Key: "line_chart_opt"
									//饼图   Key: "pie_chart_opt"
									//例如： {"text_opt":{"基本信息":"是","绝密信息":"是"}}


	CHAPTER_SECTION chapterSection;	//章节号

	//字体
	LOGFONT fontContent;			//正文
	LOGFONT fontTableHeader;		//表头
	LOGFONT fontTableContent;		//表格正文
	LOGFONT fontGraphicTitle;		//图名

	//图形颜色
	int colorScheme;				//颜色方案 0:彩色 1:黑白

	//表格
	SdTabBorderStyle tableStyle;	//表格样式
	SdAlign tableTitleAlign;		//表格标题对齐方式

	OPTION_TYPE optionType;			//设置项(文本、折线图或柱状图等)

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
{//T图项输出设置
	NODE_KEY key;
	const char* pTFileName;					//原始T图文件名
	std::vector<const char*> vctEmfFile;	//分割好的emf文件名
	std::vector<const char*> vctTitleName;	//分割好的各标题名

	CHAPTER_SECTION chapterSection;			//章节号
	LOGFONT fontGraphicTitle;				//图名字体

	int pageWidth;							//纸张宽
	int pageHeight;							//纸张高
}GRAPHIC_ITEM,*PGRAPHIC_ITEM;



typedef bool (* FUN_REPORT_ITEM)(IGenerateText* ,const REPORT_ITEM&);

typedef bool (* FUN_GRAPHIC_ITEM)(IGenerateText* ,const GRAPHIC_ITEM&);

typedef bool (* FUN_ONCLICK_ITEM)(NODE_KEY,OPTION_TYPE,SD_POINT);
//节点编号，小图标类型，鼠标点击位置




class ICalculationReport
{
public:
	virtual ~ICalculationReport(){};
public:
	/**
	* @breif  指定配置文件xml文件
	* @param  sXmlFile - xml文件名(含路径)
	* @note	  如果不指定,默认采用 dll模块所在路径\config\CalculationReport.xml文件 
	*/
	virtual void setConfigXml(const char* sXmlFile)=0;

	/**
	* @breif  设置计算书格式
	* @param  eSetupOpt - 设置控制项
	* @param  eBuildOpt - 生成控制项
	* @param  sReportPathName - 生成的文件名称(含路径，但不要带后缀如.docx,.txt, .pdf), 如果不指定，则生成为临时目录\计算书.***
	* @note   由于用户可能在设置面板中直接点击生成计算书，所以此处提供生成控制项
	*/
	virtual void setupOption(SdReportSetupOption eSetupOpt=sdReportSetupOpt_All,
		SdReportBuildOption eBuildOpt=sdReportBuildOpt_Default,const char *sReportPathName=NULL)=0;

	/**
	* @breif  生成计算书
	* @param  opt - 控制项
	*/
	virtual bool buildReport(SdReportBuildOption opt=sdReportBuildOpt_Default)=0;
	
	/**
	* @breif  保存配置
	* @param  sXmlFile - xml文件名(含路径)
	* @retrn  保存是否成功
	* @note	  如果不指定,默认采用当前打开的配置文件名 
	*/
	virtual bool saveConfigXml(const char* sXmlFile)=0;

public:
	/**
	* @breif  展示项回调函数注册
	* @param  FUN_REPORT_ITEM - 展示某项
	* @note   生成计算书过程中，每项(不含T图项)展示将回调此函数。用户在此函数中编写各项的生成代码
	*/
	virtual void registeReportCallback(FUN_REPORT_ITEM)=0;

	/**
	* @breif  图形（T图)项回调函数注册
	* @param  FUN_REPORT_ITEM - 展示某项
	* @note   生成计算书过程中，图形（T图)项展示将回调此函数。用户在此函数中编写各项的生成代
	*         如果用户未注册，或者回调函数返回false, 则将调用中间件内置方法插入分割好的T图。
	*         如果用户在回调函数中已处理了T图插入，并返回true,则中间件将不再插此图。
	*/
	virtual void registeGraphicCallback(FUN_GRAPHIC_ITEM)=0;

	/**
	* @breif  单击内容项事件函数注册
	* @param  FUN_ONCLICK_ITEM - 单击事件反应函数
	* @note   点击内容项上的某项图标时，将调用此函数. 如果未注册此函数，或者函数返回false,则都会继续弹出内置窗口。
	*         如果回调函数返回true,表示用户已经处理了此事件，则不会弹出内置窗口
	*/
	virtual void registeOnClickItemCallback(FUN_ONCLICK_ITEM)=0;

	/**
	* @breif  修改某项设置
	* @param  NODE_KEY - 内容项Key值
	* @param  OPTION_TYPE - 设置项类别
	* @note   pJsonOption - 设置项内容（Json格式)
	*/
	virtual bool modifyNodeOption(NODE_KEY,OPTION_TYPE,const char* pJsonOption)=0;

	/**
	* @breif  获取某项设置
	* @param  NODE_KEY - 内容项Key值
	* @param  OPTION_TYPE - 设置项类别
	* @retrn  设置项内容（Json格式)
	*/
	virtual const char* getNodeOption(NODE_KEY,OPTION_TYPE)=0;
public:
	/** 
	 * @brief T图转emf图
	 * @para  sTFileName - T图文件(含路径)
	 * @para  sEmfFile - Emf矢量图文件(含路径)
	 */
	virtual bool transformTtoEmf(const char* sTFile,const char* sEmfFile)=0;

	/** 
	 * @brief T图转jpg图
	 * @para  sTFileName - T图文件(含路径)
	 * @para  sEmfFile - jpg图片文件(含路径)
	 */
	virtual bool transformTtoJpg(const char* sTFile,const char* sJpgFile)=0;

	/** 
	 * @brief T图转Pdf矢量图
	 * @para  sTFileName - T图文件(含路径)
	 * @para  sPdfFile - PDF文件(含路径)
	 * @note  生成的pdf图形文件可以用于插入到另一个pdf文件中
	 */
	virtual bool transformTtoPdf(const char* sTFile,const char* sPdfFile)=0;

	/** 
	 * @brief 获取T图的边界范围
	 * @para  sTFileName - T图文件(含路径)
	 * @para  [输出] leftBottomPt - 左下角坐标
	 * @para  [输出] rightTopPt - 右上角坐标
	 */
	virtual bool CalcTRange(const char* sTFile, SD_POINT& leftBottomPt/*输出参数*/, SD_POINT& rightTopPt/*输出参数*/ )=0;

public:
	/** 
	 * @brief 获出当前输出方式
	 * @note  word,pdf或txt
	 */
	virtual FILE_TYPE getCurrentFileType()=0;

};
}