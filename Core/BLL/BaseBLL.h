#pragma once
/***************************************************************************************/
/**
	文件名称:	BaseBll.h	
	创建时间:	2016/10/22	11:00:47
	作    者:	ywyonui
	功能说明:	业务逻辑层的基类（ BLL : Business Logic Layer ）
**/
/***************************************************************************************/

// 定义宏，用于DLL中的函数导出
#ifndef BLL_API
#define BLL_API __declspec(dllexport)
#endif

namespace BLL
{
	/**
		类    型 :	基类
		功能说明 :	业务逻辑层基类
	*/
	class BLL_API CBaseBLL
	{
	public:
		CBaseBLL();
		~CBaseBLL();

#pragma region 公共变量
	public:

#pragma endregion 公共变量


#pragma region 私有变量
	private:

#pragma endregion 私有变量


#pragma region 公共接口
	public:
		/*************************************************************
		函数名称:	Serialize
		参数说明:	void
		返 回 值:	void
		功能说明:	序列化数据
		*************************************************************/
		virtual void Serialize();

		/*************************************************************
		函数名称:	Deserialize
		参数说明:	void
		返 回 值:	void
		功能说明:	反序列化数据
		*************************************************************/
		virtual void Deserialize();
#pragma endregion 公共接口


#pragma region 私有接口
	private:

#pragma endregion 私有接口

	};
}