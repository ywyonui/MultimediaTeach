#pragma once
#include <windows.h>
#include <vector>

struct ST_GdiServerDS
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
	// GDI区域结构
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
	
	BOOL	fDIBitmap = FALSE;	// 是否指向全局位图
	BOOL	fChange = FALSE;	// 是否发生了改变

	WORD	bmBitsPixel = 4;
	DWORD	dwLen;
	DWORD	dwCompress;
	DWORD	dwSendLen;

	DWORD	m_dwSocket;
	DWORD	m_dwListen;
	int		m_nPort;
	int		m_nClientCount;		// 客户端计数器，只需要等待指定客户端个数连接到当前服务，就可以退出监听了
	sockaddr_in m_addrServer;	// 服务器地址
	
	HWND	m_hWnd;

public:
#pragma region 外部调用
	static CGdiServer& GetInstance();

	// 初始化
	bool Init(HWND hWnd);
	// 关闭
	void Exit();

#pragma endregion 外部调用
	

protected:
#pragma region 内部实现
	bool InitServer();

	void InitDisplay();

	void ExitServer();

	void ExitDisplay();

	//得到区域的显示位图
	int GetRegionDisplay();

	//传输数据到客户端
	BOOL Transmit(DWORD dwClientSocket, BYTE *pData, DWORD dwLength);

	// 发送Resolution到客户端
	void SendResolution(DWORD dwClientSocket);

	// 通过socket发送区域显示位图
	int SendRegionDisplay(DWORD dwClientSocket);

	int SelectProtocols(DWORD dwSetFlags, DWORD dwNotSetFlags, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength, WSAPROTOCOL_INFO *pProtocol);

#pragma endregion 内部实现
	ST_GdiServerList *Add_Gdi(ST_GdiServerList *pNode, ST_GdiServerDS Gdi);
	void Clear_Gdi();


#pragma region 监听函数
	// 监听函数
	static DWORD WINAPI ListenThread(LPVOID lpParam);

	//
	static DWORD WINAPI ClientThread(LPVOID lpParam);
#pragma endregion 监听函数



};


