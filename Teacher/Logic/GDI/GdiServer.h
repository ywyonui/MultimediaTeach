#pragma once
#include <windows.h>
#include <vector>

struct ST_GdiServerDS
{
	// ��������
	int		iGridX;
	int		iGridY;

	// �����������
	int		iWidth1;
	int		iWidth2;
	int		iHeight1;
	int		iHeight2;

	// �豸�޹�λͼ(DIB)��ɫ���е�ɫ�ʵ���Ŀ
	int		nColors;

	// DIB��Ϣͷ���ֽ���
	DWORD	dwBitMapHeader;

	// DIB ���Ⱥ���ʼ����
	DWORD	dwLen;
	DWORD	dwCompress;
	DWORD	iStartPos;

	// DIB
	BITMAP	DIBitmap;

	// DIB ��Ϣͷ
	BITMAPINFOHEADER	BMIH;

	// DIB��Ϣͷ��ָ��
	LPBITMAPINFOHEADER	lpBMIH;

	// ������豸���HDC
	HDC		hMemDC;

	// �����λͼ���
	HBITMAP	hDIBitmap;

	// ��ѹ����DIB����ָ��
	BYTE	*pDIB;

	// ָ������DIB�ı仯��ָ��
	BYTE	*pDIBChange;
	BYTE	*pDIBChangeStart;

	// ָ��ѹ�������DIBָ��
	BYTE	*pDIBCompress;

	// ָ��ȫ�������DIB
	BYTE	*pDIBitmap;

	// DIB��־
	BOOL	fDIBitmap;
	BOOL	fChange;

};

struct ST_GdiServerList
{
	struct	ST_GdiServerDS	Gdi;
	struct	ST_GdiServerList	*pNext;
};

class CGdiServer
{
private:
	CGdiServer();
	~CGdiServer();

public:
	// GDI����ṹ
	struct	ST_GdiServerList	GdiStart;
	struct	ST_GdiServerList	*pGdiNode = NULL;

public:
	int		nGridX = 8;
	int		nGridY = 2;
	int		iCompressionLevel = 10;

	int		iWidth;
	int		iHeight;

	HDC		hDDC = NULL;
	HDC		hMemDC = NULL;
	HDC		hNullDC = NULL;
	
	BOOL	fDIBitmap = FALSE;	// �Ƿ�ָ��ȫ��λͼ
	BOOL	fChange = FALSE;	// �Ƿ����˸ı�

	WORD	bmBitsPixel = 4;
	DWORD	dwLen;
	DWORD	dwCompress;
	DWORD	dwSendLen;

	DWORD	m_dwSocket;
	DWORD	m_dwListen;
	int		m_nPort;
	int		m_nClientCount;		// �ͻ��˼�������ֻ��Ҫ�ȴ�ָ���ͻ��˸������ӵ���ǰ���񣬾Ϳ����˳�������
	sockaddr_in m_addrServer;	// ��������ַ
	
	HWND	m_hWnd;

public:
#pragma region �ⲿ����
	static CGdiServer& GetInstance();

	// ��ʼ��
	bool Init(HWND hWnd);
	// �ر�
	void Exit();

#pragma endregion �ⲿ����
	

protected:
#pragma region �ڲ�ʵ��
	bool InitServer();

	void InitDisplay();

	void ExitServer();

	void ExitDisplay();

	//�õ��������ʾλͼ
	int GetRegionDisplay();

	//�������ݵ��ͻ���
	BOOL Transmit(DWORD dwClientSocket, BYTE *pData, DWORD dwLength);

	// ����Resolution���ͻ���
	void SendResolution(DWORD dwClientSocket);

	// ͨ��socket����������ʾλͼ
	int SendRegionDisplay(DWORD dwClientSocket);

	int SelectProtocols(DWORD dwSetFlags, DWORD dwNotSetFlags, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength, WSAPROTOCOL_INFO *pProtocol);

#pragma endregion �ڲ�ʵ��
	ST_GdiServerList *Add_Gdi(ST_GdiServerList *pNode, ST_GdiServerDS Gdi);
	void Clear_Gdi();


#pragma region ��������
	// ��������
	static DWORD WINAPI ListenThread(LPVOID lpParam);

	//
	static DWORD WINAPI ClientThread(LPVOID lpParam);
#pragma endregion ��������



};


