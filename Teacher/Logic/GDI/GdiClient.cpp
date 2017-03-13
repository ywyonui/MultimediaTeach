#include "stdafx.h"
#include "GdiClient.h"
#include <windows.h>
#include "GdiServer.h"
#include "HuffCompress.h"
#include "RLE.h"
#include "cstdio"
#include "cstdlib"

#define REFRESH_MSG			"REFRESH"

CGdiClient::CGdiClient()
{

}

CGdiClient::~CGdiClient()
{

}

CGdiClient& CGdiClient::GetInstance()
{
	static CGdiClient hInstance;
	return hInstance;
}

struct GdiList* CGdiClient::Add_Gdi(struct GdiList *pNode, struct GdiDS Gdi)
{
	if (pNode->pNext = (struct GdiList *)malloc(sizeof(struct GdiList)))
	{
		pNode = pNode->pNext;
		pNode->Gdi.pDIBitmap = Gdi.pDIBitmap;

		pNode->Gdi.pDIB = NULL;
		pNode->Gdi.pStartDIB = NULL;

		pNode->Gdi.iGridX = Gdi.iGridX;
		pNode->Gdi.iGridY = Gdi.iGridY;

		pNode->Gdi.iWidth1 = Gdi.iWidth1;
		pNode->Gdi.iWidth2 = Gdi.iWidth2;
		pNode->Gdi.iHeight1 = Gdi.iHeight1;
		pNode->Gdi.iHeight2 = Gdi.iHeight2;
		pNode->Gdi.lpBitmapIH = Gdi.lpBitmapIH;
		pNode->Gdi.fDIBitmap = Gdi.fDIBitmap;
		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

void CGdiClient::Clear_Gdi(struct GdiList *pStart)
{
	struct	GdiList	*pPrev;
	struct	GdiList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		if (pNode->Gdi.fDIBitmap)
		{
			free(pNode->Gdi.pDIBitmap);
			free(pNode->Gdi.pDIB);
		}
		free(pNode);
	}
}

struct CommandList	* CGdiClient::Add_Command(struct CommandList *pNode, struct CommandDS Command)
{
	if (pNode->pNext = (struct CommandList *)malloc(sizeof(struct CommandList)))
	{
		pNode = pNode->pNext;
		strcpy_s(pNode->Command.szElement, Command.szElement);
		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

void CGdiClient::Clear_Command(struct CommandList *pStart)
{
	struct	CommandList	*pPrev;
	struct	CommandList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		free(pNode);
	}
}

void CGdiClient::InitGrids()
{
	int		iLoop, jLoop, nGrid;
	struct	GdiDS	Gdi;

	Gdi.fDIBitmap = FALSE;
	GdiStart.pNext = NULL;
	pGdiNode = &GdiStart;

	if ((nGridX > 0) && (nGridY > 0))
	{
		for (iLoop = 0; iLoop < nGridX; iLoop++)
		{
			for (jLoop = 0; jLoop < nGridY; jLoop++)
			{
				Gdi.iGridX = iLoop;
				Gdi.iGridY = jLoop;

				pGdiNode = Add_Gdi(pGdiNode, Gdi);
			}
		}
	}
	else
	{
		nGrid = max(nGridX, nGridY);
		for (iLoop = 0; iLoop < nGrid; iLoop++)
		{
			if (nGridX > 0)
				Gdi.iGridX = iLoop;
			else
				Gdi.iGridX = 0;

			if (nGridY > 0)
				Gdi.iGridY = iLoop;
			else
				Gdi.iGridY = 0;

			pGdiNode = Add_Gdi(pGdiNode, Gdi);
		}
	}

	if (nGridX > 0)
		iWidthX = iWidth / nGridX;
	else
		iWidthX = iWidth;

	if (nGridY > 0)
		iHeightY = iHeight / nGridY;
	else
		iHeightY = iHeight;
}

void CGdiClient::GetResolution()
{
	int		iSend, iRecv;
	char	szMessage[81];
	char	szRecv[81];

	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "RESOLUTION\0");
	iSend = Send(sClient, szMessage);

	iRecv = Recv(sClient, szRecv, 80);
	iWidth = atoi(szRecv);

	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "OK\0");
	iSend = Send(sClient, szMessage);

	iRecv = Recv(sClient, szRecv, 80);
	iHeight = atoi(szRecv);

	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "OK\0");
	iSend = Send(sClient, szMessage);
}

// 有MsgHelperMain中接收到Display消息开始调用，第一次接收到的还包括了CommandList
void CGdiClient::UpdateRegionalScreen(HWND hWnd, SOCKET sClient, char* pMessage, BOOL fFirstTime)
{
	LPBMIH	lpBMIH;
	char	*pDIB;
	char	*pDIBCompress;
	char	*pStartDIB;
	DWORD	iSent, iRecv;
	DWORD	iCompLen, iNewLen, iRLELen, iStartPos;
	int		iGridX, iGridY;
	int		iUpdates, iLoop, iCompressions, jLoop;
	char	szMessage[81];

	struct	CommandList	CommandStart;
	struct	CommandList	*pCommandNode;
	struct	CommandDS	Command;
	char				*pDest;
	int					iLoc1, iLoc2, nChar;

	char				*pTempDIB;

	// 修改执行顺序



	if (!fRefreshing)
	{
		CommandStart.pNext = NULL;
 		fRefreshing = TRUE;
		strcpy_s(szMessage, REFRESH_MSG);
		iSent = Send(sClient, szMessage);
		memset(szMessage, '\0', sizeof(szMessage));
		iRecv = Recv(sClient, szMessage, 80);
		if (iRecv == SOCKET_ERROR || iRecv == 0)
		{
		//	AfxMessageBox(L"NO DATA or SOCKET ERROR", L"Client", MB_OK);
		}	
		iUpdates = atoi(szMessage);
		strcpy_s(szMessage, "OK\0");
		iSent = Send(sClient, szMessage);


		if (iUpdates > 0)
		{
			pGdiNode = GdiStart.pNext;
			for (iLoop = 0; iLoop < iUpdates; iLoop++)
			{
				pCommandNode = &CommandStart;
				memset(szMessage, '\0', sizeof(szMessage));
				iRecv = Recv(sClient, szMessage, 80);
				if (iRecv == SOCKET_ERROR || iRecv == 0)
				{
				//	AfxMessageBox(L"NO DATA or SOCKET ERROR", L"Client", MB_OK);
				}
				iLoc1 = 0;
				while (pDest = strchr(szMessage, ';'))
				{
					iLoc2 = pDest - szMessage;
					nChar = iLoc2 - iLoc1;
					memset(Command.szElement, '\0', sizeof(Command.szElement));
					strncpy_s(Command.szElement, &szMessage[iLoc1], nChar);
					pCommandNode = Add_Command(pCommandNode, Command);
					memset(&szMessage[iLoc2], ':', 1);
					iLoc1 = iLoc2 + 1;
				}

				pCommandNode = CommandStart.pNext;
				if (pCommandNode)
				{
					iCompressions = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iGridX = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;
					iGridY = atoi(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iStartPos = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iLen = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;

					iCompLen = atol(pCommandNode->Command.szElement);
					pCommandNode = pCommandNode->pNext;
				}

				if (iLen > (int)iCompLen)
					pDIBCompress = (char *)malloc(iLen);
				else
					pDIBCompress = (char *)malloc(iCompLen);

				while (pGdiNode)
				{
					if ((pGdiNode->Gdi.iGridX == iGridX) &&
						(pGdiNode->Gdi.iGridY == iGridY))
					{
						if (!pGdiNode->Gdi.fDIBitmap)
						{
							pGdiNode->Gdi.pDIBitmap = (char*)malloc(iLen);
							memset(pGdiNode->Gdi.pDIBitmap, 0, iLen);

							pGdiNode->Gdi.lpBitmapIH = (LPBMIH)pGdiNode->Gdi.pDIBitmap;
							pGdiNode->Gdi.iWidth1 = iGridX * iWidthX;
							pGdiNode->Gdi.iWidth2 = iGridX * iWidthX + iWidthX;
							pGdiNode->Gdi.iHeight1 = iGridY * iHeightY;
							pGdiNode->Gdi.iHeight2 = iGridY * iHeightY + iHeightY;

							iScreenWidth = iWidth;
							iScreenHeight = iHeight;

							pGdiNode->Gdi.pDIB = (char *)malloc(iLen);
							pGdiNode->Gdi.pStartDIB = pGdiNode->Gdi.pDIB;
						}

						strcpy_s(szMessage, "OK\0");
						iSent = Send(sClient, szMessage);
						pStartDIB = pDIBCompress;
						Receive(sClient, pDIBCompress, iCompLen);
						pDIBCompress = pStartDIB;
						pStartDIB = pGdiNode->Gdi.pStartDIB;

						if (iCompressionLevel == 0)
						{
							memblast(pGdiNode->Gdi.pDIB, pDIBCompress, iLen);
							iNewLen = iLen;
						}
						else if (iCompressionLevel == 10)
						{
							iNewLen = HuffmanUnCompress((BYTE *)pDIBCompress, (BYTE *)pGdiNode->Gdi.pDIB);
						}
						else if (iCompressionLevel == 11)
						{
							for (jLoop = 0; jLoop < iCompressions; jLoop++)
							{
								iNewLen = HuffmanUnCompress((BYTE *)pDIBCompress, (BYTE *)pGdiNode->Gdi.pDIB);
								if ((jLoop + 1) < iCompressions)
									memblast(pDIBCompress, pGdiNode->Gdi.pDIB, iNewLen);
							}
						}
						else if (iCompressionLevel == 12)
						{
							iNewLen = RunLengthDecode((BYTE *)pDIBCompress, (BYTE *)pGdiNode->Gdi.pDIB);
						}
						else if (iCompressionLevel == 13)
						{
							char	*pTempDIB;
							iLen = HuffmanGetSize((BYTE *)pDIBCompress);
							pTempDIB = (char *)malloc(iLen);
							iRLELen = HuffmanUnCompress((BYTE *)pDIBCompress, (BYTE *)pTempDIB);
							iNewLen = RunLengthDecode((BYTE *)pTempDIB, (BYTE *)pGdiNode->Gdi.pDIB);

							free(pTempDIB);
						}

						pGdiNode->Gdi.pDIB = pGdiNode->Gdi.pStartDIB;
						lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;

						if (fFirstTime)
						{
							pTempDIB = pGdiNode->Gdi.pDIBitmap;
							memblast(&pGdiNode->Gdi.pDIBitmap[iStartPos], pGdiNode->Gdi.pDIB, iNewLen);

							pGdiNode->Gdi.fDIBitmap = TRUE;
						}
						else
						{
							pTempDIB = pGdiNode->Gdi.pDIBitmap;
							pDIB = pGdiNode->Gdi.pDIB;

							__asm
							{
								MOV		ECX, iNewLen
									SHR		ECX, 2
									MOV		EDI, pTempDIB
									ADD		EDI, iStartPos
									MOV		ESI, pDIB
								Addition :
								LODSD
									ADD[EDI], EAX
									ADD		EDI, 4
									DEC		ECX
									JNZ		Addition
							}
						}

						Clear_Command(&CommandStart);
						strcpy_s(szMessage, "OK\0");
						iSent = Send(sClient, szMessage);
						break;
					}
					pGdiNode = pGdiNode->pNext;
				}

				free(pDIBCompress);
			}

			if (fFirstTime)
				fDIBitmap = TRUE;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		fRefreshing = FALSE;
	}
}

// 接收图片数据
DWORD CGdiClient::Receive(SOCKET Socket, char *pData, DWORD iLength)
{
	WSAOVERLAPPED	olRecv;
	WSAEVENT	gheventOlSock;
	WSAEVENT	eventArray[2];

	WSABUF		buffRecv;
	DWORD		dwRet, dwNumBytes, dwFlags;
	int			nWSAError;
	DWORD		iNumBytes = 0;

//	char		szError[81];

	gheventOlSock = WSACreateEvent();
	eventArray[0] = gheventOlSock;

	ZeroMemory(&olRecv, sizeof(WSAOVERLAPPED));
	olRecv.hEvent = gheventOlSock;
	buffRecv.len = iLength;
	buffRecv.buf = pData;

	fReceiving = TRUE;

	while (TRUE)
	{
		dwFlags = 0;
		dwNumBytes = 0;

		if ((dwRet = WSARecv(Socket, &buffRecv, 1, &dwNumBytes, &dwFlags, &olRecv, NULL)) == SOCKET_ERROR)
		{
			nWSAError = WSAGetLastError();
			if (nWSAError != ERROR_IO_PENDING)
			{
			//	sprintf_s(szError, "WSARecv failed with error %d\n", nWSAError);
			//	MessageBox(NULL, szError, "Client", MB_OK);
				return SOCKET_ERROR;
			}
		}

		if (WSAWaitForMultipleEvents(1, eventArray, FALSE, WSA_INFINITE, FALSE) == WSA_WAIT_FAILED)
		{
		//	sprintf_s(szError, "WSAWaitForMultipleEvents failed %d\n", WSAGetLastError());
		//	MessageBox(NULL, szError, "Client", MB_OK);
		}
		WSAResetEvent(eventArray[0]);
		if (WSAGetOverlappedResult(Socket, &olRecv, &dwNumBytes, FALSE, &dwFlags) == TRUE)
		{
			if (dwNumBytes == 0)
				break;
		}
		else
		{
		//	sprintf_s(szError, "WSAGetOverlappedResult failed with error %d\n", WSAGetLastError());
		//	MessageBox(NULL, szError, "Client", MB_OK);
			return SOCKET_ERROR;
		}
		buffRecv.len -= dwNumBytes;
		iNumBytes = iNumBytes + dwNumBytes;
		if (buffRecv.len == 0)
			break;
		else
			buffRecv.buf += dwNumBytes;
	}
	fReceiving = FALSE;
	return iNumBytes;
}

int CGdiClient::Send(SOCKET Socket, char *szSend)
{
	int		iSent;
	fSending = TRUE;
	iSent = send(Socket, szSend, strlen(szSend), 0);
	fSending = FALSE;
	return iSent;
}

int CGdiClient::Recv(SOCKET Socket, char *szRecv, int iRecvLen)
{
	int		iRecv;
	fReceiving = TRUE;
	iRecv = recv(Socket, szRecv, iRecvLen, 0);
	if (iRecv == SOCKET_ERROR || iRecv == 0)
//		MessageBox(NULL, "NO DATA or SOCKET ERROR", "Client", MB_OK);
	szRecv[iRecv] = '\0';
	fReceiving = FALSE;
	return iRecv;
}
