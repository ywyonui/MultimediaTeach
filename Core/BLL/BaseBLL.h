#pragma once
/***************************************************************************************/
/**
	�ļ�����:	BaseBll.h	
	����ʱ��:	2016/10/22	11:00:47
	��    ��:	ywyonui
	����˵��:	ҵ���߼���Ļ��ࣨ BLL : Business Logic Layer ��
**/
/***************************************************************************************/

// ����꣬����DLL�еĺ�������
#ifndef BLL_API
#define BLL_API __declspec(dllexport)
#endif

namespace BLL
{
	/**
		��    �� :	����
		����˵�� :	ҵ���߼������
	*/
	class BLL_API CBaseBLL
	{
	public:
		CBaseBLL();
		~CBaseBLL();

#pragma region ��������
	public:

#pragma endregion ��������


#pragma region ˽�б���
	private:

#pragma endregion ˽�б���


#pragma region �����ӿ�
	public:
		/*************************************************************
		��������:	Serialize
		����˵��:	void
		�� �� ֵ:	void
		����˵��:	���л�����
		*************************************************************/
		virtual void Serialize();

		/*************************************************************
		��������:	Deserialize
		����˵��:	void
		�� �� ֵ:	void
		����˵��:	�����л�����
		*************************************************************/
		virtual void Deserialize();
#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
	private:

#pragma endregion ˽�нӿ�

	};
}