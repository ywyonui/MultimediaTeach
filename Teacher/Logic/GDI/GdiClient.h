#pragma once

#include <windows.h>
#define	LPBMIH				LPBITMAPINFOHEADER

struct GdiDS
{
	char	*pDIBitmap;

	char	*pDIB;
	char	*pStartDIB;

	int		iGridX;
	int		iGridY;

	int		iWidth1;
	int		iWidth2;
	int		iHeight1;
	int		iHeight2;

	LPBITMAPINFOHEADER	lpBitmapIH;

	BOOL	fDIBitmap;
};

struct GdiList
{
	struct	GdiDS	Gdi;
	struct	GdiList	*pNext;
};

struct CommandDS
{
	char	szElement[81];
};

struct CommandList
{
	struct CommandDS	Command;
	struct CommandList	*pNext;
};


class CGdiClient
{
private:
	CGdiClient();
	~CGdiClient();

public:
	static CGdiClient& GetInstance();

public:
	BOOL	bSendOnly = FALSE;
	BOOL	fSending = FALSE;
	BOOL	fReceiving = FALSE;
	DWORD	iLen;
	char	*pDIBitmap;
	int		iScreenWidth, iScreenHeight;
	int		iWidth, iHeight, iWidthX, iHeightY;
	int		iWidth1, iWidth2, iHeight1, iHeight2;
	int		iCompressionLevel;
	BOOL	fDIBitmap = FALSE;
	LPBMIH	lpBitmapIH;
	struct	sockaddr_in	server;
	SOCKET	sClient;
	BOOL	fRefreshing = FALSE;
	BOOL	fConnected = FALSE;
	int		nGridX = 8, nGridY = 4;
	BOOL	fGridX = FALSE, fGridY = FALSE;
	WORD	bmBitsPixel = 4;

	struct	GdiList		GdiStart;
	struct	GdiList		*pGdiNode;


public:
	struct	GdiList	*Add_Gdi(struct GdiList *pNode, struct GdiDS Gdi);
	void			Clear_Gdi(struct GdiList *pStart);

	struct CommandList	*Add_Command(struct CommandList *pNode, struct CommandDS Command);
	void		Clear_Command(struct CommandList *pStart);

public:
	void InitGrids();

	void GetResolution();

	void UpdateRegionalScreen(HWND hWnd, SOCKET sClient, char* pCommond, BOOL fFirstTime);

	DWORD Receive(SOCKET Socket, char *pData, DWORD iLength);

	int Send(SOCKET Socket, char *szSend);

	int Recv(SOCKET Socket, char *szRecv, int iRecvLen);


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

