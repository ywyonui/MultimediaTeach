#pragma once
#include <windows.h>
#define	LPBMIH				LPBITMAPINFOHEADER

struct GdiServerDS
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

struct GdiServerList
{
	struct	GdiServerDS	Gdi;
	struct	GdiServerList	*pNext;
};

class CGdiServer
{
private:
	CGdiServer();
	~CGdiServer();

public:
	static CGdiServer& GetInstance();

public:
	// GDI����ṹ
	struct	GdiServerList	GdiStart;
	struct	GdiServerList	*pGdiNode;

public:
	int		nGridX = 8;
	int		nGridY = 2;
	DWORD	dwLen, dwCompress, dwSendLen;
	int		iCompressionLevel = 10;
	HWND	hServerWnd;
	HDC		hDDC = NULL, hMemDC, hNullDC;
	BOOL	fChange = FALSE;
	int		iWidth, iHeight;
	WORD	bmBitsPixel = 4;
	// ָ��ȫ��λͼ
	BOOL	fDIBitmap = FALSE;

	DWORD	m_dwSocket;

public:
	struct	GdiServerList	*Add_Gdi(struct GdiServerList *pNode, struct GdiServerDS Gdi);
	void			Clear_Gdi(struct GdiServerList *pStart);

	void InitDisplay(HWND hWnd, DWORD dwSocket);

	void ExitDisplay();

	//�õ��������ʾλͼ
	int GetRegionDisplay(HWND hWnd);

	// ����Resolution���ͻ���
	void SendResolution();

	// ͨ��socket����������ʾλͼ
	int SendRegionDisplay(HWND hWnd);

	//�������ݵ��ͻ���
	BOOL Transmit(BYTE *pData, DWORD dwLength);

public:
	static void memblast(void* dest, void* src, DWORD count)
	{
		DWORD	iCount;

		__asm
		{
			MOV		ECX, count
				SHR		ECX, 2
				SHL		ECX, 2
				MOV		iCount, ECX

				MOV		ESI, src
				MOV		EDI, dest
				MOV		ECX, iCount
				SHR		ECX, 2
				REP		MOVSD

				MOV		ECX, count
				MOV		EAX, iCount
				SUB		ECX, EAX

				JZ		Exit

				MOV		ESI, src
				ADD		ESI, EAX
				MOV		EDI, dest
				ADD		EDI, EAX
				REP		MOVSB
			Exit :
		}
	}

};


