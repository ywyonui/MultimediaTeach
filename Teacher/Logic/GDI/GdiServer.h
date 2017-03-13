#pragma once
#include <windows.h>
#define	LPBMIH				LPBITMAPINFOHEADER

struct GdiServerDS
{
	// 网格坐标
	int		iGridX;
	int		iGridY;

	// 网格矩形区域
	int		iWidth1;
	int		iWidth2;
	int		iHeight1;
	int		iHeight2;

	// 设备无关位图(DIB)颜色表中的色彩的数目
	int		nColors;

	// DIB信息头的字节数
	DWORD	dwBitMapHeader;

	// DIB 长度和起始坐标
	DWORD	dwLen;
	DWORD	dwCompress;
	DWORD	iStartPos;

	// DIB
	BITMAP	DIBitmap;

	// DIB 信息头
	BITMAPINFOHEADER	BMIH;

	// DIB信息头的指针
	LPBITMAPINFOHEADER	lpBMIH;

	// 区域的设备句柄HDC
	HDC		hMemDC;

	// 区域的位图句柄
	HBITMAP	hDIBitmap;

	// 无压缩的DIB区域指针
	BYTE	*pDIB;

	// 指向区域DIB的变化的指针
	BYTE	*pDIBChange;
	BYTE	*pDIBChangeStart;

	// 指向压缩区域的DIB指针
	BYTE	*pDIBCompress;

	// 指向全局区域的DIB
	BYTE	*pDIBitmap;

	// DIB标志
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
	// GDI区域结构
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
	// 指向全局位图
	BOOL	fDIBitmap = FALSE;

	DWORD	m_dwSocket;

public:
	struct	GdiServerList	*Add_Gdi(struct GdiServerList *pNode, struct GdiServerDS Gdi);
	void			Clear_Gdi(struct GdiServerList *pStart);

	void InitDisplay(HWND hWnd, DWORD dwSocket);

	void ExitDisplay();

	//得到区域的显示位图
	int GetRegionDisplay(HWND hWnd);

	// 发送Resolution到客户端
	void SendResolution();

	// 通过socket发送区域显示位图
	int SendRegionDisplay(HWND hWnd);

	//传输数据到客户端
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


