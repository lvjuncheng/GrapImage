#include "stdafx.h"

//#include "XCamera/Include/Raw2Rgb.h"

#include "ImageConvert.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

void CImageConvert::ImageNearestScale( BYTE *destData,int destW, int destH, BYTE *srcData, int srcW, int srcH,BOOL bRGB)
{
	int bytesPixel = 1; 
	if( bRGB )//RGB
		bytesPixel = 3;

	double rx = srcW/(double)destW;
	double ry = srcH/(double)destH;
	BYTE *srcLine = srcData;
	int i,j;
	for ( j=0; j<destH; j++ )
	{
		for ( i=0; i<destW; i++ )
		{
			int xoff = ( (int)( rx*i ) )* bytesPixel;
			BYTE *p = srcLine + xoff;
			*destData++ = *p++;
			if( bRGB )
			{
				*destData++ = *p++;
				*destData++ = *p++;
			}
		}
		srcLine = srcData + ( (int)( ry*j ) )* srcW*bytesPixel;
	}
}
void CImageConvert::ImageCopyClip( BYTE *destData,int destW, int destH, 
								BYTE *srcData, int srcW, int srcH,
								int posX, int posY,BOOL bRGB )
{
	int bytesPixel = 1; 
	if( bRGB )//RGB
		bytesPixel = 3;
	int destSize = bytesPixel*destW*destH;
	memset(destData,0,destSize);

	// ÿ��Ҫ��������
	int cpLinePixel = destW<(srcW-posX) ? destW : (srcW-posX);
	int cpLineCount = destH<(srcH-posY) ? destH : (srcH-posY);
	if( cpLinePixel <= 0 || cpLineCount<=0 || 
		posX<0 || posY<0 || srcW<= posX || srcH<=posY )
		return;

	BYTE *destLine = destData;
	BYTE *srcLine = srcData + (srcW*posY + posX)*bytesPixel;
	int j;
	for ( j=0; j<cpLineCount; j++ )
	{
		memcpy(destLine,srcLine,cpLinePixel*bytesPixel);
		destLine += destW*bytesPixel;
		srcLine  += srcW*bytesPixel;
	}
}
void CImageConvert::ImageFlip(BYTE *dataRGB,int w, int h)
{
	int lineByte = w*3;
	BYTE *lineData = new BYTE[lineByte];
	BYTE *p1;
	BYTE *p2;
	for (int j=0; j<h/2; j++)
	{
		//��j�����h-j�л���
		p1 = dataRGB+j*lineByte;
		p2 = dataRGB+(h-1-j)*lineByte;
		memcpy(lineData,p1,lineByte);
		memcpy(p1,p2,lineByte);
		memcpy(p2,lineData,lineByte);
	}
	delete[] lineData;
}

void CImageConvert::RedFullImage(BYTE *dataRGB,int w, int h)
{
	// �����ĵط���ʾΪ��ɫ��
	BYTE * data = dataRGB;
	BYTE r,g,b;
	for (int j=0; j<h; j++)
	{
		for (int i=0; i<w; i++)
		{
			r = *data;
			g = *(data+1);
			b = *(data+2);
			if( r==255 && g==255 && b==255)
			{
				*data = 0;
				*(data+1) =0;
			}
			data=data+3;
		}
	}
}

// BOOL CImageConvert::SaveBMP( char *szName, BYTE* rgb24Data, int w, int h, BOOL bFlip)
// {
// 	//BOOL bFlip=TRUE;
// 
// 	BITMAPINFO *bmpInfo;		//BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
// 	//BYTE *m_imageBuffer;		//Bayerת���󻺳���
// 	char chBmpBuf[2048];		//BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
// 
// 	//	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
// 	bmpInfo								= (BITMAPINFO *)chBmpBuf;
// 	//	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
// 	bmpInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
// 	//	ͼ���ȣ�һ��Ϊ������ڿ��
// 	bmpInfo->bmiHeader.biWidth			= w;
// 	//	ͼ���ȣ�һ��Ϊ������ڸ߶�
// 	bmpInfo->bmiHeader.biHeight			= h;
// 
// 	/*
// 	*	��������һ����ͬ��
// 	*	���ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��
// 	*/
// 	bmpInfo->bmiHeader.biPlanes			= 1;
// 	bmpInfo->bmiHeader.biBitCount		= 24;
// 	bmpInfo->bmiHeader.biCompression	= BI_RGB;
// 	bmpInfo->bmiHeader.biSizeImage		= 0;
// 	bmpInfo->bmiHeader.biXPelsPerMeter	= 0;
// 	bmpInfo->bmiHeader.biYPelsPerMeter	= 0;
// 	bmpInfo->bmiHeader.biClrUsed		= 0;
// 	bmpInfo->bmiHeader.biClrImportant	= 0;
// 
// 
// 	BITMAPFILEHEADER bfh	= {0};
// 	DWORD dwImageSize		= 0;
// 	DWORD dwBytesRead		= 0;
// 	BOOL bRVal				= TRUE;
// 
// 	dwImageSize		=  w * h * 3;		//����ͼ���С������Ƶ������ں���Ƶ��ʽȷ��
// 
// 	bfh.bfType		= (WORD)'M' << 8 | 'B';			//�����ļ�����
// 	bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);	//�����ļ�ͷ��С
// 
// 	bfh.bfSize		= bfh.bfOffBits + dwImageSize;	//�ļ���С
// 
// 	HANDLE hFile = ::CreateFile((LPCSTR)szName,
// 		GENERIC_WRITE ,
// 		0,
// 		NULL,
// 		CREATE_ALWAYS,	FILE_ATTRIBUTE_NORMAL,
// 		NULL
// 		);
// 	if (hFile == INVALID_HANDLE_VALUE) 
// 	{
// 		bRVal = FALSE;
// 	}
// 	else
// 	{
// 		::WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL );
// 		::WriteFile(hFile, bmpInfo, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL );
// 		if( bFlip)
// 		{
// 			BYTE *newData = new BYTE[dwImageSize];
// 			memcpy(newData,rgb24Data,dwImageSize);
// 			ImageFlip(newData, w, h);
// 			::WriteFile(hFile, newData, dwImageSize, &dwBytesRead, NULL );
// 			delete[] newData;
// 		}
// 		else
// 			::WriteFile(hFile, rgb24Data, dwImageSize, &dwBytesRead, NULL );
// 
// 		CloseHandle(hFile);
// 	}
// 	return bRVal;
// }

BOOL CImageConvert::SaveBMP( char *szName, BYTE* rgb24Data, int w, int h, BOOL bFlip)
{
	//BOOL bFlip=TRUE;

	BITMAPINFO *bmpInfo;		//BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	//BYTE *m_imageBuffer;		//Bayerת���󻺳���
	char chBmpBuf[2048];		//BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����

	//	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
	bmpInfo								= (BITMAPINFO *)chBmpBuf;
	//	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
	bmpInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	//	ͼ���ȣ�һ��Ϊ������ڿ��
	bmpInfo->bmiHeader.biWidth			= w;
	//	ͼ���ȣ�һ��Ϊ������ڸ߶�
	bmpInfo->bmiHeader.biHeight			= h;

	/*
	*	��������һ����ͬ��
	*	���ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��
	*/
	bmpInfo->bmiHeader.biPlanes			= 1;
	bmpInfo->bmiHeader.biBitCount		= 8;
	bmpInfo->bmiHeader.biCompression	= BI_RGB;
	bmpInfo->bmiHeader.biSizeImage		= 0;
	bmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	bmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	bmpInfo->bmiHeader.biClrUsed		= 0;
	bmpInfo->bmiHeader.biClrImportant	= 0;


	BITMAPFILEHEADER bfh	= {0};
	RGBQUAD   rgbQuad[256];         //�����ɫ��
	for(int i = 0; i < 256; i++)   
	{   
		rgbQuad[i].rgbBlue  =   (BYTE)i;   
		rgbQuad[i].rgbGreen =   (BYTE)i;   
		rgbQuad[i].rgbRed   =   (BYTE)i;   
		rgbQuad[i].rgbReserved   =   0;   
	}   

	DWORD dwImageSize		= 0;
	DWORD dwBytesRead		= 0;
	BOOL bRVal				= TRUE;

	dwImageSize		=  w * h;		//����ͼ���С������Ƶ������ں���Ƶ��ʽȷ��

	bfh.bfType		= (WORD)'M' << 8 | 'B';			//�����ļ�����
	bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;	//�����ļ�ͷ��С

	bfh.bfSize		= bfh.bfOffBits + dwImageSize;	//�ļ���С

	HANDLE hFile = ::CreateFileA((LPCSTR)szName,
		GENERIC_WRITE ,
		0,
		NULL,
		CREATE_ALWAYS,	FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		bRVal = FALSE;
	}
	else
	{
		::WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, bmpInfo, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, (LPSTR)rgbQuad, sizeof(RGBQUAD)*256, &dwBytesRead, NULL);
		if( bFlip)
		{
			BYTE *newData = new BYTE[dwImageSize];
			memcpy(newData,rgb24Data,dwImageSize * sizeof(BYTE));
			ImageFlip(newData, w, h);
			::WriteFile(hFile, newData, dwImageSize, &dwBytesRead, NULL );
			delete[] newData;
		}
		else
			::WriteFile(hFile, rgb24Data, dwImageSize, &dwBytesRead, NULL );

		CloseHandle(hFile);
	}
	return bRVal;
}