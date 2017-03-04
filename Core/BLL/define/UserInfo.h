#pragma once
/***************************************************************************************/
/**
	�ļ�����:	UserInfo.h	
	����ʱ��:	2016/10/21	19:47:07
	��    ��:	ywyonui
	����˵��:	�û����ݶ��壬Ŀǰ�������Ψһ��Ҫ�����ݿ⽻����
**/
/***************************************************************************************/

#include <string>

/**
��    �� :	��½��Ϣ
����˵�� :	�û���������
*/
struct ST_LoginUserInfo
{
	char	strUserName[30];	//	�û���
	char	strUserPwd[30];		//	����

	ST_LoginUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strUserPwd, 0, 30);
	}

};

/**
	��    �� :	ע���û�����Ϣ
	����˵�� :	�û������ǳơ����롣����
*/
struct ST_RegUserInfo
{
	char	strUserName[30];	//	�û���
	char	strPetName[50];		//	�ǳ�
	char	strUserPwd[30];		//	����

	ST_RegUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strPetName, 0, 50);
		memset(strUserPwd, 0, 30);
	}
};

/**
	��    �� :	�û���ʾ��Ϣ
	����˵�� :	�û������ǳơ���ҵȡ�����
*/
struct ST_ShowUserInfo
{
	int		nUserID;			//	�û�ID
	char	strUserName[30];	//	�û���
	char	strPetName[50];		//	�ǳ�
	int		nMoney;				//	���
	int		nIndexDesk;			//  ���Ӻ�
	int		nIndexSeat;			//  ��λ��
	long	nSocketID;			//	��¼��ǰ�ͻ��˵�SocketID����½��ʱ�򱣴�

	ST_ShowUserInfo()
	{
		nUserID = 0;
		memset(strUserName, 0, 30);
		memset(strPetName, 0, 50);
		nMoney = 0;
		nIndexDesk = -1;
		nIndexSeat = 0;
		nSocketID = 0;
	}
};


