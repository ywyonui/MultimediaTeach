#include "stdafx.h"
#include <windows.h>
#include "GdiServer.h"
#include "GdiClient.h"
#include "HuffCompress.h"
#include "RLE.h"
#include "cstdio"
#include "cstdlib"

CGdiServer::CGdiServer()
{

}

CGdiServer::~CGdiServer()
{

}

// 添加一个元素到GDI链表

CGdiServer& CGdiServer::GetInstance()
{
	static CGdiServer hInstance;
	return hInstance;
}

struct GdiServerList*	CGdiServer::Add_Gdi(struct GdiServerList *pNode, struct GdiServerDS Gdi)
{
	// 添加到链表的末尾
	if (pNode->pNext = (struct GdiServerList *)malloc(sizeof(struct GdiServerList)))
	{
		pNode = pNode->pNext;

		// 添加网格坐标
		pNode->Gdi.iGridX = Gdi.iGridX;
		pNode->Gdi.iGridY = Gdi.iGridY;

		// 设置区域的矩形坐标
		pNode->Gdi.iWidth1 = Gdi.iWidth1;
		pNode->Gdi.iWidth2 = Gdi.iWidth2;
		pNode->Gdi.iHeight1 = Gdi.iHeight1;
		pNode->Gdi.iHeight2 = Gdi.iHeight2;

		// 设置DIB颜色表的颜色数
		pNode->Gdi.nColors = Gdi.nColors;

		// 设置DIB信息头的字节数
		pNode->Gdi.dwBitMapHeader = Gdi.dwBitMapHeader;

		// 设置位图长度和起始坐标
		pNode->Gdi.dwLen = Gdi.dwLen;
		pNode->Gdi.dwCompress = Gdi.dwCompress;
		pNode->Gdi.iStartPos = Gdi.iStartPos;

		//设置DIB
		pNode->Gdi.DIBitmap = Gdi.DIBitmap;

		// 设置DIB信息头
		pNode->Gdi.BMIH = Gdi.BMIH;

		// 设置DIB信息头的指针
		pNode->Gdi.lpBMIH = Gdi.lpBMIH;

		// 设置区域的装置设备句柄
		pNode->Gdi.hMemDC = Gdi.hMemDC;

		// 设置区域的位图句柄
		pNode->Gdi.hDIBitmap = Gdi.hDIBitmap;

		// 区域无压缩DIB的指针
		pNode->Gdi.pDIB = Gdi.pDIB;

		//设置指向区域DIB变化的部分的指针
		pNode->Gdi.pDIBChange = Gdi.pDIBChange;

		//设置指向压缩区域的DIB的指针
		pNode->Gdi.pDIBCompress = Gdi.pDIBCompress;

		//设置指向全局区域位图指针
		pNode->Gdi.pDIBitmap = Gdi.pDIBitmap;

		// 区域位图标志
		pNode->Gdi.fDIBitmap = Gdi.fDIBitmap;
		pNode->Gdi.fChange = Gdi.fChange;

		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

// 完全清楚GDI链表
void CGdiServer::Clear_Gdi(struct GdiServerList *pStart)
{
	struct	GdiServerList	*pPrev;
	struct	GdiServerList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		DeleteDC(pNode->Gdi.hMemDC);
		DeleteObject(pNode->Gdi.hDIBitmap);
		if (pNode->Gdi.fDIBitmap)
		{
			free(pNode->Gdi.pDIBitmap);
			free(pNode->Gdi.pDIB);
			free(pNode->Gdi.pDIBChangeStart);
		}
		free(pNode);
	}
}


// 初始化显示变量
void CGdiServer::InitDisplay(HWND hWnd, DWORD dwSocket)
{
	m_dwSocket = dwSocket;

	struct	GdiServerDS	Gdi;
	int		iWidthX, iHeightY, nGrid;
	int		iXGrid, iYGrid, iLoop;

	GdiStart.pNext = NULL;
	pGdiNode = &GdiStart;
	hDDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	// 得到屏幕的宽度和高度
	iWidth = GetDeviceCaps(hDDC, HORZRES);
	iHeight = GetDeviceCaps(hDDC, VERTRES);

	// 分割屏幕成X网格区域
	if (nGridX > 0)
		iWidthX = iWidth / nGridX;
	else
		iWidthX = iWidth;

	// 分割屏幕成Y网格区域
	if (nGridY > 0)
		iHeightY = iHeight / nGridY;
	else
		iHeightY = iHeight;

	// 为网格创建区域的位图和Memory DC
	if ((nGridX > 0) && (nGridY > 0))
	{
		for (iXGrid = 0; iXGrid < nGridX; iXGrid++)
		{
			for (iYGrid = 0; iYGrid < nGridY; iYGrid++)
			{
				Gdi.fChange = FALSE;
				Gdi.fDIBitmap = FALSE;

				Gdi.iGridX = iXGrid;
				Gdi.iGridY = iYGrid;

				Gdi.iWidth1 = iXGrid * iWidthX;
				Gdi.iWidth2 = iXGrid * iWidthX + iWidthX;
				Gdi.iHeight1 = iYGrid * iHeightY;
				Gdi.iHeight2 = iYGrid * iHeightY + iHeightY;

				Gdi.hMemDC = CreateCompatibleDC(hDDC);
				Gdi.hDIBitmap = CreateCompatibleBitmap(hDDC, iWidthX, iHeightY);
				SelectObject(Gdi.hMemDC, Gdi.hDIBitmap);

				// 往队列中添加GDI信息
				pGdiNode = Add_Gdi(pGdiNode, Gdi);
			}
		}
	}
	else
	{
		nGrid = max(nGridX, nGridY);
		for (iLoop = 0; iLoop < nGrid; iLoop++)
		{
			Gdi.fChange = FALSE;
			Gdi.fDIBitmap = FALSE;

			if (nGridX > 0)
			{
				Gdi.iGridX = iLoop;
				Gdi.iWidth1 = iLoop * iWidthX;
				Gdi.iWidth2 = iLoop * iWidthX + iWidthX;
			}
			else
			{
				Gdi.iGridX = 0;
				Gdi.iWidth1 = 0;
				Gdi.iWidth2 = iWidthX;
			}

			if (nGridY > 0)
			{
				Gdi.iGridY = iLoop;
				Gdi.iHeight1 = iLoop * iHeightY;
				Gdi.iHeight2 = iLoop * iHeightY + iHeightY;
			}
			else
			{
				Gdi.iGridY = 0;
				Gdi.iHeight1 = 0;
				Gdi.iHeight2 = iHeightY;
			}

			Gdi.hMemDC = CreateCompatibleDC(hDDC);
			Gdi.hDIBitmap = CreateCompatibleBitmap(hDDC, iWidthX, iHeightY);
			SelectObject(Gdi.hMemDC, Gdi.hDIBitmap);
			pGdiNode = Add_Gdi(pGdiNode, Gdi);
		}
	}
	hNullDC = GetDC(NULL);
}


void CGdiServer::ExitDisplay()
{
	DeleteDC(hDDC);
	DeleteDC(hNullDC);
	Clear_Gdi(pGdiNode);
}

//得到区域的显示位图
int CGdiServer::GetRegionDisplay(HWND hWnd)
{
	int		iWidth1, iWidth2, iHeight1, iHeight2;
	BOOL	bGotBits;
	DWORD	iLength;
	BYTE	*pStartDIB = NULL;

	DWORD	dwLen1;
	DWORD	dwBitMapHeader1;
	BYTE	*pDIBitmap1;
	BYTE	*pDIB1;
	int		fChange1;
	BYTE	*pDIBChange1;

	// 得到区域的矩形坐标
	iWidth1 = pGdiNode->Gdi.iWidth1;
	iWidth2 = pGdiNode->Gdi.iWidth2;
	iHeight1 = pGdiNode->Gdi.iHeight1;
	iHeight2 = pGdiNode->Gdi.iHeight2;

	BitBlt(pGdiNode->Gdi.hMemDC, 0, 0, iWidth2, iHeight2, hDDC, iWidth1, iHeight1, SRCCOPY);

	//将DDB转化成DIB
	// 得到区域位图信息
	GetObject(pGdiNode->Gdi.hDIBitmap, sizeof(BITMAP), &(pGdiNode->Gdi.DIBitmap));
	// 设置颜色模式
	pGdiNode->Gdi.DIBitmap.bmBitsPixel = bmBitsPixel;
	// 初始化位图信息头
	pGdiNode->Gdi.BMIH.biSize = sizeof(BITMAPINFOHEADER);
	pGdiNode->Gdi.BMIH.biWidth = pGdiNode->Gdi.DIBitmap.bmWidth;
	pGdiNode->Gdi.BMIH.biHeight = pGdiNode->Gdi.DIBitmap.bmHeight;
	pGdiNode->Gdi.BMIH.biPlanes = 1;
	pGdiNode->Gdi.BMIH.biBitCount = (WORD)pGdiNode->Gdi.DIBitmap.bmPlanes * (WORD)pGdiNode->Gdi.DIBitmap.bmBitsPixel;
	pGdiNode->Gdi.BMIH.biCompression = BI_RGB;
	pGdiNode->Gdi.BMIH.biSizeImage = 0;
	pGdiNode->Gdi.BMIH.biXPelsPerMeter = 0;
	pGdiNode->Gdi.BMIH.biYPelsPerMeter = 0;
	pGdiNode->Gdi.BMIH.biClrUsed = 0;
	pGdiNode->Gdi.BMIH.biClrImportant = 0;

	//设置区域的颜色数
	pGdiNode->Gdi.nColors = 1 << pGdiNode->Gdi.BMIH.biBitCount;
	if (pGdiNode->Gdi.nColors > 256)
		pGdiNode->Gdi.nColors = 0;
	// 计算为位图信息头和彩色表分配内存所需的尺寸
	pGdiNode->Gdi.dwLen = (DWORD)(sizeof(BITMAPINFOHEADER) + pGdiNode->Gdi.nColors * sizeof(RGBQUAD));

	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = (BYTE *)malloc(pGdiNode->Gdi.dwLen);
		pStartDIB = pGdiNode->Gdi.pDIB;
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	*(pGdiNode->Gdi.lpBMIH) = pGdiNode->Gdi.BMIH;
	GetDIBits(hNullDC, pGdiNode->Gdi.hDIBitmap, 0L, (DWORD)pGdiNode->Gdi.BMIH.biHeight, (LPBYTE)NULL, (LPBITMAPINFO)pGdiNode->Gdi.lpBMIH, DIB_RGB_COLORS);

	// 以计算的尺寸设置区域位图信息头
	pGdiNode->Gdi.BMIH = *(pGdiNode->Gdi.lpBMIH);
	// 保存位图信息头的大小
	pGdiNode->Gdi.dwBitMapHeader = pGdiNode->Gdi.dwLen;
	pGdiNode->Gdi.dwLen += (DWORD)(pGdiNode->Gdi.BMIH.biSizeImage);
	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = pStartDIB;
		pGdiNode->Gdi.pDIB = (BYTE *)realloc(pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	bGotBits = GetDIBits(hNullDC, pGdiNode->Gdi.hDIBitmap, 0L, (DWORD)pGdiNode->Gdi.BMIH.biHeight, (LPBYTE)pGdiNode->Gdi.lpBMIH + (pGdiNode->Gdi.BMIH.biSize + pGdiNode->Gdi.nColors * sizeof(RGBQUAD)), (LPBITMAPINFO)pGdiNode->Gdi.lpBMIH, DIB_RGB_COLORS);

	// 比较区域DIBS之前和之后的变化
	if (pGdiNode->Gdi.fDIBitmap)
	{
		dwLen1 = pGdiNode->Gdi.dwLen;
		dwBitMapHeader1 = pGdiNode->Gdi.dwBitMapHeader;
		pDIBitmap1 = pGdiNode->Gdi.pDIBitmap;
		pDIB1 = pGdiNode->Gdi.pDIB;
		fChange1 = pGdiNode->Gdi.fChange;

		// 每次4个字节的比较两副位图,注意要跳过位图的头部
		__asm
		{
			MOV		ECX, dwLen1
				SUB		ECX, dwBitMapHeader1
				SHR		ECX, 2
				MOV		EDX, dwBitMapHeader1
				MOV		ESI, pDIBitmap1
				ADD		ESI, EDX
				MOV		EDI, pDIB1
				ADD		EDI, EDX
				REP		CMPSD
				JNZ		SetFlagRegion1
				MOV		fChange1, FALSE
				JMP		ExitRegion1
			SetFlagRegion1 :
			MOV		fChange1, TRUE
			ExitRegion1 :
		}

		//设置变化状态
		pGdiNode->Gdi.fChange = fChange1;
		// 为两副区域DIB的差别建立一个新的DIB
		if (pGdiNode->Gdi.fChange)
		{
			DWORD		iZeros = 0;
			iLength = (pGdiNode->Gdi.dwLen - pGdiNode->Gdi.dwBitMapHeader);
			pGdiNode->Gdi.pDIBChange = pGdiNode->Gdi.pDIBChangeStart;
			pDIBChange1 = pGdiNode->Gdi.pDIBChange;

			__asm
			{
				MOV		ECX, iLength
					SHR		ECX, 2
					MOV		EDI, pDIBChange1
					MOV		ESI, pDIB1
					ADD		ESI, dwBitMapHeader1
					MOV		EDX, pDIBitmap1
					ADD		EDX, dwBitMapHeader1
				SubtractRegion :
				LODSD
					SUB		EAX, [EDX]
					ADD		EDX, 4
					STOSD
					DEC		ECX
					JNZ		SubtractRegion
			}

			//拷贝到全局区域的DIB
			memblast(pGdiNode->Gdi.pDIBitmap, pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);
			// 设置压缩长度
			pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen - pGdiNode->Gdi.dwBitMapHeader;
			// 设置数据的起始位
			pGdiNode->Gdi.iStartPos = pGdiNode->Gdi.dwBitMapHeader;
		}
	}
	else
	{
		iLength = (pGdiNode->Gdi.dwLen);
		pGdiNode->Gdi.pDIBitmap = (BYTE *)malloc(iLength);
		pGdiNode->Gdi.pDIBChange = (BYTE *)malloc(iLength);
		pGdiNode->Gdi.pDIBChangeStart = pGdiNode->Gdi.pDIBChange;
		memblast(pGdiNode->Gdi.pDIBitmap, pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);
		memblast(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);

		// 不需要再分配内存
		pGdiNode->Gdi.fDIBitmap = TRUE;
		pGdiNode->Gdi.fChange = TRUE;

		pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen;
		pGdiNode->Gdi.iStartPos = 0;
	}

	return pGdiNode->Gdi.fChange;
}

// 发送Resolution到客户端，分两次把宽高发给客户端
void CGdiServer::SendResolution()
{
	char	szMessage[81];
	DWORD	iSent, iRecv;

	// 建立屏幕宽度
	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "%d", iWidth);
	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

	// 接收确认
	memset(szMessage, '\0', sizeof(szMessage));
	iRecv = recv(m_dwSocket, szMessage, 81, 0);
	szMessage[iRecv] = '\0';

	// 建立屏幕的高度
	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "%d", iHeight);
	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

	// 接收确认
	memset(szMessage, '\0', sizeof(szMessage));
	iRecv = recv(m_dwSocket, szMessage, 81, 0);
	szMessage[iRecv] = '\0';

}

// 通过socket发送区域显示位图
int CGdiServer::SendRegionDisplay(HWND hWnd)
{
	char	szMessage[81];
	DWORD	iSent, iRecv;
	int		fSend = FALSE;
	int		iUpdates;
	WORD	wTreeSize;
	DWORD	dwByteTree[768];
	DWORD	dwCodes[514];
	DWORD	dwCompLen, dwLastCompLen;
	BOOL	fTransmit;
	BYTE	*pTempDIB;
	DWORD	dwMinCompress;

	// 指向GDI链表的起始位
	iUpdates = 0;
	pGdiNode = GdiStart.pNext;
	while (pGdiNode)
	{
		//为每个网格得到区域显示
		fSend = GetRegionDisplay(hWnd);
		// 计算需要发送的变化的显示区域的数目
		if (fSend)
			iUpdates++;
		// 移动到下一个节点
		pGdiNode = pGdiNode->pNext;
	}

	// 发送到客户端需要更新的显示区域的数目
 	memset(szMessage, '\0', sizeof(szMessage));
 	sprintf_s(szMessage, "%d", iUpdates);
 	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

 	// 接收确认
 	memset(szMessage, '\0', sizeof(szMessage));
 	iRecv = recv(m_dwSocket, szMessage, 81, 0);
 	szMessage[iRecv] = '\0';

	if (iUpdates > 0)
	{
		// 指向GDI链表的起始位
		pGdiNode = GdiStart.pNext;
		while (pGdiNode)
		{
			// 如果桌面发生了变化，则发送DIB
			if (pGdiNode->Gdi.fChange)
			{
				int		iCompressions = 1;
				//无压缩
#pragma region 压缩
				if (iCompressionLevel == 0)
				{
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress);
					memblast(pGdiNode->Gdi.pDIBCompress, pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress);
					dwSendLen = pGdiNode->Gdi.dwCompress;
				}
				if (iCompressionLevel == 10) // 单遍霍夫曼编码压缩
				{
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress + 1536);
					// 生成霍夫曼字节树字典
					wTreeSize = HuffmanDictionary(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, &dwByteTree[0], &dwCodes[0]);
					// 使用霍夫曼压缩方法压缩图片
					dwSendLen = HuffmanCompress(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, wTreeSize, &dwByteTree[0], &dwCodes[0], pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 11) // 多遍霍夫曼压缩编码
				{
					dwMinCompress = pGdiNode->Gdi.dwCompress + 1536;
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(dwMinCompress);
					pTempDIB = (BYTE *)malloc(pGdiNode->Gdi.dwCompress);
					memblast(pTempDIB, pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress);
					iCompressions = 0;
					dwCompLen = pGdiNode->Gdi.dwCompress;
					dwLastCompLen = dwCompLen;
					for (;;)
					{
						// 生成霍夫曼字节树字典
						wTreeSize = HuffmanDictionary(pTempDIB, pGdiNode->Gdi.dwCompress, &dwByteTree[0], &dwCodes[0]);
						// 计算压缩长度
						dwCompLen = HuffmanCountCompress(pTempDIB, dwCompLen, wTreeSize, &dwCodes[0]);
						if (dwCompLen < dwMinCompress)
						{
							dwSendLen = HuffmanCompress(pTempDIB, dwLastCompLen, wTreeSize, &dwByteTree[0], &dwCodes[0], pGdiNode->Gdi.pDIBCompress);
							memblast(pTempDIB, pGdiNode->Gdi.pDIBCompress, dwSendLen);
							dwMinCompress = dwSendLen;
							dwLastCompLen = dwCompLen;
							iCompressions++;
						}
						else
							break;
					}
					free(pTempDIB);
				}
				else if (iCompressionLevel == 12) // Run Length编码
				{
					//为最坏的情况分配压缩空间
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length编码图象
					dwSendLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 13) // Run Length&Huffman编码
				{
					pTempDIB = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length 编码图象
					dwCompLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, pTempDIB);
					// 生成霍夫曼字节树的字典
					wTreeSize = HuffmanDictionary(pTempDIB, dwCompLen, &dwByteTree[0], &dwCodes[0]);
					// 使用霍夫曼压缩Run Lenght编码的图象
					dwSendLen = HuffmanCompress(pTempDIB, dwCompLen, wTreeSize, &dwByteTree[0], &dwCodes[0], pGdiNode->Gdi.pDIBCompress);
					// 释放临时的DIB
					free(pTempDIB);
				}
#pragma endregion 压缩

				// 建立位图控制消息
				memset(szMessage, '\0', sizeof(szMessage));
				sprintf_s(szMessage, "%d;%d;%d;%ld;%ld;%ld;",
					iCompressions,
					pGdiNode->Gdi.iGridX, pGdiNode->Gdi.iGridY,
					pGdiNode->Gdi.iStartPos,
					pGdiNode->Gdi.dwCompress, dwSendLen);
				// 发送控制消息
				iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

				// 接收确认
				memset(szMessage, '\0', sizeof(szMessage));
				iRecv = recv(m_dwSocket, szMessage, 81, 0);
				szMessage[iRecv] = '\0';

				// 发送压缩的DIB
				fTransmit = Transmit(pGdiNode->Gdi.pDIBCompress, dwSendLen);

				// 释放压缩的DIB
				free(pGdiNode->Gdi.pDIBCompress);
 				// 接收确认
				memset(szMessage, '\0', sizeof(szMessage));
				iRecv = recv(m_dwSocket, szMessage, 81, 0);
				szMessage[iRecv] = '\0';
			}
			pGdiNode = pGdiNode->pNext;
		}
	}
	return iUpdates;
}

//传输数据到客户端
BOOL CGdiServer::Transmit(BYTE *pData, DWORD dwLength)
{
	WSAOVERLAPPED	olSend;
	WSAEVENT	gheventOlSock;
	WSAEVENT	eventArray[2];
	WSABUF		buffSend;
	DWORD		dwRet, dwNumBytes, dwFlags;
	int			nWSAError;
//	BYTE		szError[81];

	// 为发送完成创建一个信号事件
	gheventOlSock = WSACreateEvent();
	eventArray[0] = gheventOlSock;

	// 初始化重叠发送的结构
	ZeroMemory(&olSend, sizeof(WSAOVERLAPPED));
	// 为发送重叠结构创建一个信号时间
	olSend.hEvent = gheventOlSock;
	buffSend.len = dwLength;
	buffSend.buf = (char*)pData;
	// 持续发送，直到dwSendLen个字节被发送完成
	while (TRUE)
	{
		if ((dwRet = WSASend(m_dwSocket, &buffSend, 1, &dwNumBytes, 0, &olSend, NULL)) == SOCKET_ERROR)
		{
			nWSAError = WSAGetLastError();
			if (nWSAError != ERROR_IO_PENDING)
			{
// 				sprintf_s(szError, "WSASend failed with error %d\n", nWSAError);
// 				MessageBox(NULL, szError, "Server", MB_OK);
			}
		}

		if (WSAWaitForMultipleEvents(1, eventArray, FALSE, WSA_INFINITE, FALSE) == WSA_WAIT_FAILED)
		{
// 			sprintf_s(szError, "WSAWaitForMultipleEvents failed %d\n", WSAGetLastError());
// 			MessageBox(NULL, szError, "Server", MB_OK);
		}
		// 重置gheventOlSock
		WSAResetEvent(eventArray[0]);
		if (WSAGetOverlappedResult(m_dwSocket, &olSend, &dwNumBytes, FALSE, &dwFlags) == FALSE)
		{
// 			sprintf_s(szError, "WSAGetOverlappedResult failed with error %d\n", WSAGetLastError());
// 			MessageBox(NULL, szError, "Server", MB_OK);
		}
		buffSend.len -= dwNumBytes;
		if (buffSend.len == 0)
			break;
		else
			buffSend.buf += dwNumBytes;
	}

	// 关闭信号事件
	WSACloseEvent(gheventOlSock);
	return TRUE;
}




