/**
 *\file simple_svg.h
 *\brief 绘制svg图片文件的接口
 *	
 *	内置实现了以下图形
 *	1. 环形百分比
 *	2. 柱状分布图
 *	3. 流速状态图
 *
 *\date 2017/07/04
 */

#ifndef __SIMPLE_SVG_H_2017_07_04__
#define	__SIMPLE_SVG_H_2017_07_04__

/**
 *\enum SVG_STATUS
 *\brief SVG状态
 */
enum svg_status{
	
	SVG_SUCCESS = 0x00,	///< 成功
	SVG_FAILED = 0x10,	///< 失败
	SVG_HANDLE_VALID = 0x01,	///< svg句柄可用
	SVG_HANDLE_INVALID = 0x11,	///< svg句柄不可用
};

/**
 *\struct st_svg_position
 *\brief 绘制图形的时候 坐标 和 视图大小
 */
typedef struct svg_position{
	
	unsigned int	x_viewport;	///< 左上角 x 坐标值
	unsigned int	y_viewport;	///< 左上角 y 坐标值
	unsigned int	width_viewport;		///< 宽度
	unsigned int	height_viewport;	///< 高度
	unsigned int	x_viewbox;	///< 视图 x 坐标
	unsigned int	y_viewbox;	///< y 坐标
	unsigned int	width_viewbox;	///<
	unsigned int	height_viewbox;	///<
}st_svg_position;
 
/**
 *\struct st_svg_handler
 *\brief 操作svg的句柄结构【结构的具体实现建议隐藏掉】
 */
typedef struct svg_handler{
	
	xmlDocPtr		xml_doc;	///< svg图片采用libxml2.0库操作， xml文档的指针
	unsigned int	status;		///< 表示当前引用的句柄状态
}st_svg_handler;

/**
 *\brief 初始化svg句柄
 *\param[in,out] handler 操作svg图片的句柄
 *\retval SVG_SUCCESS 初始化成功
 *\retval SVG_FAILED 初始化失败
 */
int init_svg_handler(st_svg_handler *handler);

/**
 *\brief 销毁svg句柄
 *\param[in,out] handler 操作svg图片的句柄
 *\retval SVG_SUCCESS 成功注销
 *\retval SVG_FAILED 注销失败
 */
int destroy_svg_handler(st_svg_handler *handler);

/**
 *\brief 绘制环形百分比图
 */
int draw_circle_percentage(st_svg_handler *handler);

#endif//__SIMPLE_SVG_H_2017_07_04__