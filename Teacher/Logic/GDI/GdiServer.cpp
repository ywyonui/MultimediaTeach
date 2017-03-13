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

// ���һ��Ԫ�ص�GDI����

CGdiServer& CGdiServer::GetInstance()
{
	static CGdiServer hInstance;
	return hInstance;
}

struct GdiServerList*	CGdiServer::Add_Gdi(struct GdiServerList *pNode, struct GdiServerDS Gdi)
{
	// ��ӵ������ĩβ
	if (pNode->pNext = (struct GdiServerList *)malloc(sizeof(struct GdiServerList)))
	{
		pNode = pNode->pNext;

		// �����������
		pNode->Gdi.iGridX = Gdi.iGridX;
		pNode->Gdi.iGridY = Gdi.iGridY;

		// ��������ľ�������
		pNode->Gdi.iWidth1 = Gdi.iWidth1;
		pNode->Gdi.iWidth2 = Gdi.iWidth2;
		pNode->Gdi.iHeight1 = Gdi.iHeight1;
		pNode->Gdi.iHeight2 = Gdi.iHeight2;

		// ����DIB��ɫ�����ɫ��
		pNode->Gdi.nColors = Gdi.nColors;

		// ����DIB��Ϣͷ���ֽ���
		pNode->Gdi.dwBitMapHeader = Gdi.dwBitMapHeader;

		// ����λͼ���Ⱥ���ʼ����
		pNode->Gdi.dwLen = Gdi.dwLen;
		pNode->Gdi.dwCompress = Gdi.dwCompress;
		pNode->Gdi.iStartPos = Gdi.iStartPos;

		//����DIB
		pNode->Gdi.DIBitmap = Gdi.DIBitmap;

		// ����DIB��Ϣͷ
		pNode->Gdi.BMIH = Gdi.BMIH;

		// ����DIB��Ϣͷ��ָ��
		pNode->Gdi.lpBMIH = Gdi.lpBMIH;

		// ���������װ���豸���
		pNode->Gdi.hMemDC = Gdi.hMemDC;

		// ���������λͼ���
		pNode->Gdi.hDIBitmap = Gdi.hDIBitmap;

		// ������ѹ��DIB��ָ��
		pNode->Gdi.pDIB = Gdi.pDIB;

		//����ָ������DIB�仯�Ĳ��ֵ�ָ��
		pNode->Gdi.pDIBChange = Gdi.pDIBChange;

		//����ָ��ѹ�������DIB��ָ��
		pNode->Gdi.pDIBCompress = Gdi.pDIBCompress;

		//����ָ��ȫ������λͼָ��
		pNode->Gdi.pDIBitmap = Gdi.pDIBitmap;

		// ����λͼ��־
		pNode->Gdi.fDIBitmap = Gdi.fDIBitmap;
		pNode->Gdi.fChange = Gdi.fChange;

		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

// ��ȫ���GDI����
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


// ��ʼ����ʾ����
void CGdiServer::InitDisplay(HWND hWnd, DWORD dwSocket)
{
	m_dwSocket = dwSocket;

	struct	GdiServerDS	Gdi;
	int		iWidthX, iHeightY, nGrid;
	int		iXGrid, iYGrid, iLoop;

	GdiStart.pNext = NULL;
	pGdiNode = &GdiStart;
	hDDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	// �õ���Ļ�Ŀ�Ⱥ͸߶�
	iWidth = GetDeviceCaps(hDDC, HORZRES);
	iHeight = GetDeviceCaps(hDDC, VERTRES);

	// �ָ���Ļ��X��������
	if (nGridX > 0)
		iWidthX = iWidth / nGridX;
	else
		iWidthX = iWidth;

	// �ָ���Ļ��Y��������
	if (nGridY > 0)
		iHeightY = iHeight / nGridY;
	else
		iHeightY = iHeight;

	// Ϊ���񴴽������λͼ��Memory DC
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

				// �����������GDI��Ϣ
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

//�õ��������ʾλͼ
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

	// �õ�����ľ�������
	iWidth1 = pGdiNode->Gdi.iWidth1;
	iWidth2 = pGdiNode->Gdi.iWidth2;
	iHeight1 = pGdiNode->Gdi.iHeight1;
	iHeight2 = pGdiNode->Gdi.iHeight2;

	BitBlt(pGdiNode->Gdi.hMemDC, 0, 0, iWidth2, iHeight2, hDDC, iWidth1, iHeight1, SRCCOPY);

	//��DDBת����DIB
	// �õ�����λͼ��Ϣ
	GetObject(pGdiNode->Gdi.hDIBitmap, sizeof(BITMAP), &(pGdiNode->Gdi.DIBitmap));
	// ������ɫģʽ
	pGdiNode->Gdi.DIBitmap.bmBitsPixel = bmBitsPixel;
	// ��ʼ��λͼ��Ϣͷ
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

	//�����������ɫ��
	pGdiNode->Gdi.nColors = 1 << pGdiNode->Gdi.BMIH.biBitCount;
	if (pGdiNode->Gdi.nColors > 256)
		pGdiNode->Gdi.nColors = 0;
	// ����Ϊλͼ��Ϣͷ�Ͳ�ɫ������ڴ�����ĳߴ�
	pGdiNode->Gdi.dwLen = (DWORD)(sizeof(BITMAPINFOHEADER) + pGdiNode->Gdi.nColors * sizeof(RGBQUAD));

	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = (BYTE *)malloc(pGdiNode->Gdi.dwLen);
		pStartDIB = pGdiNode->Gdi.pDIB;
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	*(pGdiNode->Gdi.lpBMIH) = pGdiNode->Gdi.BMIH;
	GetDIBits(hNullDC, pGdiNode->Gdi.hDIBitmap, 0L, (DWORD)pGdiNode->Gdi.BMIH.biHeight, (LPBYTE)NULL, (LPBITMAPINFO)pGdiNode->Gdi.lpBMIH, DIB_RGB_COLORS);

	// �Լ���ĳߴ���������λͼ��Ϣͷ
	pGdiNode->Gdi.BMIH = *(pGdiNode->Gdi.lpBMIH);
	// ����λͼ��Ϣͷ�Ĵ�С
	pGdiNode->Gdi.dwBitMapHeader = pGdiNode->Gdi.dwLen;
	pGdiNode->Gdi.dwLen += (DWORD)(pGdiNode->Gdi.BMIH.biSizeImage);
	if (!pGdiNode->Gdi.fDIBitmap)
	{
		pGdiNode->Gdi.pDIB = pStartDIB;
		pGdiNode->Gdi.pDIB = (BYTE *)realloc(pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);
	}

	pGdiNode->Gdi.lpBMIH = (LPBMIH)pGdiNode->Gdi.pDIB;
	bGotBits = GetDIBits(hNullDC, pGdiNode->Gdi.hDIBitmap, 0L, (DWORD)pGdiNode->Gdi.BMIH.biHeight, (LPBYTE)pGdiNode->Gdi.lpBMIH + (pGdiNode->Gdi.BMIH.biSize + pGdiNode->Gdi.nColors * sizeof(RGBQUAD)), (LPBITMAPINFO)pGdiNode->Gdi.lpBMIH, DIB_RGB_COLORS);

	// �Ƚ�����DIBS֮ǰ��֮��ı仯
	if (pGdiNode->Gdi.fDIBitmap)
	{
		dwLen1 = pGdiNode->Gdi.dwLen;
		dwBitMapHeader1 = pGdiNode->Gdi.dwBitMapHeader;
		pDIBitmap1 = pGdiNode->Gdi.pDIBitmap;
		pDIB1 = pGdiNode->Gdi.pDIB;
		fChange1 = pGdiNode->Gdi.fChange;

		// ÿ��4���ֽڵıȽ�����λͼ,ע��Ҫ����λͼ��ͷ��
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

		//���ñ仯״̬
		pGdiNode->Gdi.fChange = fChange1;
		// Ϊ��������DIB�Ĳ����һ���µ�DIB
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

			//������ȫ�������DIB
			memblast(pGdiNode->Gdi.pDIBitmap, pGdiNode->Gdi.pDIB, pGdiNode->Gdi.dwLen);
			// ����ѹ������
			pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen - pGdiNode->Gdi.dwBitMapHeader;
			// �������ݵ���ʼλ
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

		// ����Ҫ�ٷ����ڴ�
		pGdiNode->Gdi.fDIBitmap = TRUE;
		pGdiNode->Gdi.fChange = TRUE;

		pGdiNode->Gdi.dwCompress = pGdiNode->Gdi.dwLen;
		pGdiNode->Gdi.iStartPos = 0;
	}

	return pGdiNode->Gdi.fChange;
}

// ����Resolution���ͻ��ˣ������ΰѿ�߷����ͻ���
void CGdiServer::SendResolution()
{
	char	szMessage[81];
	DWORD	iSent, iRecv;

	// ������Ļ���
	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "%d", iWidth);
	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

	// ����ȷ��
	memset(szMessage, '\0', sizeof(szMessage));
	iRecv = recv(m_dwSocket, szMessage, 81, 0);
	szMessage[iRecv] = '\0';

	// ������Ļ�ĸ߶�
	memset(szMessage, '\0', sizeof(szMessage));
	sprintf_s(szMessage, "%d", iHeight);
	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

	// ����ȷ��
	memset(szMessage, '\0', sizeof(szMessage));
	iRecv = recv(m_dwSocket, szMessage, 81, 0);
	szMessage[iRecv] = '\0';

}

// ͨ��socket����������ʾλͼ
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

	// ָ��GDI�������ʼλ
	iUpdates = 0;
	pGdiNode = GdiStart.pNext;
	while (pGdiNode)
	{
		//Ϊÿ������õ�������ʾ
		fSend = GetRegionDisplay(hWnd);
		// ������Ҫ���͵ı仯����ʾ�������Ŀ
		if (fSend)
			iUpdates++;
		// �ƶ�����һ���ڵ�
		pGdiNode = pGdiNode->pNext;
	}

	// ���͵��ͻ�����Ҫ���µ���ʾ�������Ŀ
 	memset(szMessage, '\0', sizeof(szMessage));
 	sprintf_s(szMessage, "%d", iUpdates);
 	iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

 	// ����ȷ��
 	memset(szMessage, '\0', sizeof(szMessage));
 	iRecv = recv(m_dwSocket, szMessage, 81, 0);
 	szMessage[iRecv] = '\0';

	if (iUpdates > 0)
	{
		// ָ��GDI�������ʼλ
		pGdiNode = GdiStart.pNext;
		while (pGdiNode)
		{
			// ������淢���˱仯������DIB
			if (pGdiNode->Gdi.fChange)
			{
				int		iCompressions = 1;
				//��ѹ��
#pragma region ѹ��
				if (iCompressionLevel == 0)
				{
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress);
					memblast(pGdiNode->Gdi.pDIBCompress, pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress);
					dwSendLen = pGdiNode->Gdi.dwCompress;
				}
				if (iCompressionLevel == 10) // �������������ѹ��
				{
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress + 1536);
					// ���ɻ������ֽ����ֵ�
					wTreeSize = HuffmanDictionary(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, &dwByteTree[0], &dwCodes[0]);
					// ʹ�û�����ѹ������ѹ��ͼƬ
					dwSendLen = HuffmanCompress(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, wTreeSize, &dwByteTree[0], &dwCodes[0], pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 11) // ��������ѹ������
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
						// ���ɻ������ֽ����ֵ�
						wTreeSize = HuffmanDictionary(pTempDIB, pGdiNode->Gdi.dwCompress, &dwByteTree[0], &dwCodes[0]);
						// ����ѹ������
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
				else if (iCompressionLevel == 12) // Run Length����
				{
					//Ϊ����������ѹ���ռ�
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length����ͼ��
					dwSendLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, pGdiNode->Gdi.pDIBCompress);
				}
				else if (iCompressionLevel == 13) // Run Length&Huffman����
				{
					pTempDIB = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					pGdiNode->Gdi.pDIBCompress = (BYTE *)malloc(pGdiNode->Gdi.dwCompress * 3 + 4);
					// Run Length ����ͼ��
					dwCompLen = RunLengthEncode(pGdiNode->Gdi.pDIBChange, pGdiNode->Gdi.dwCompress, pTempDIB);
					// ���ɻ������ֽ������ֵ�
					wTreeSize = HuffmanDictionary(pTempDIB, dwCompLen, &dwByteTree[0], &dwCodes[0]);
					// ʹ�û�����ѹ��Run Lenght�����ͼ��
					dwSendLen = HuffmanCompress(pTempDIB, dwCompLen, wTreeSize, &dwByteTree[0], &dwCodes[0], pGdiNode->Gdi.pDIBCompress);
					// �ͷ���ʱ��DIB
					free(pTempDIB);
				}
#pragma endregion ѹ��

				// ����λͼ������Ϣ
				memset(szMessage, '\0', sizeof(szMessage));
				sprintf_s(szMessage, "%d;%d;%d;%ld;%ld;%ld;",
					iCompressions,
					pGdiNode->Gdi.iGridX, pGdiNode->Gdi.iGridY,
					pGdiNode->Gdi.iStartPos,
					pGdiNode->Gdi.dwCompress, dwSendLen);
				// ���Ϳ�����Ϣ
				iSent = Transmit((BYTE*)szMessage, strlen(szMessage));

				// ����ȷ��
				memset(szMessage, '\0', sizeof(szMessage));
				iRecv = recv(m_dwSocket, szMessage, 81, 0);
				szMessage[iRecv] = '\0';

				// ����ѹ����DIB
				fTransmit = Transmit(pGdiNode->Gdi.pDIBCompress, dwSendLen);

				// �ͷ�ѹ����DIB
				free(pGdiNode->Gdi.pDIBCompress);
 				// ����ȷ��
				memset(szMessage, '\0', sizeof(szMessage));
				iRecv = recv(m_dwSocket, szMessage, 81, 0);
				szMessage[iRecv] = '\0';
			}
			pGdiNode = pGdiNode->pNext;
		}
	}
	return iUpdates;
}

//�������ݵ��ͻ���
BOOL CGdiServer::Transmit(BYTE *pData, DWORD dwLength)
{
	WSAOVERLAPPED	olSend;
	WSAEVENT	gheventOlSock;
	WSAEVENT	eventArray[2];
	WSABUF		buffSend;
	DWORD		dwRet, dwNumBytes, dwFlags;
	int			nWSAError;
//	BYTE		szError[81];

	// Ϊ������ɴ���һ���ź��¼�
	gheventOlSock = WSACreateEvent();
	eventArray[0] = gheventOlSock;

	// ��ʼ���ص����͵Ľṹ
	ZeroMemory(&olSend, sizeof(WSAOVERLAPPED));
	// Ϊ�����ص��ṹ����һ���ź�ʱ��
	olSend.hEvent = gheventOlSock;
	buffSend.len = dwLength;
	buffSend.buf = (char*)pData;
	// �������ͣ�ֱ��dwSendLen���ֽڱ��������
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
		// ����gheventOlSock
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

	// �ر��ź��¼�
	WSACloseEvent(gheventOlSock);
	return TRUE;
}




