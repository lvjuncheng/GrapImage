#ifndef _ImageConvert_H
#define _ImageConvert_H

class CImageConvert
{
public:

	static void ImageNearestScale( BYTE *destData,int destW, int destH, 
		BYTE *srcData, int srcW, int srcH, BOOL bRGB = TRUE);
	static void ImageCopyClip( BYTE *destData,int destW, int destH, 
		BYTE *srcData, int srcW, int srcH, int posX, int posY, BOOL bRGB = TRUE);
	static void ImageFlip(BYTE *dataRGB,int w, int h);
	static void RedFullImage(BYTE *dataRGB,int w, int h);		// ���͵ط��ú�ɫ��ʾ

	static BOOL SaveBMP( char *name, BYTE* rgb24Data, int w, int h, BOOL bFlip =TRUE);
	//static BOOL LoadBMP( char *name, BYTE* rgb24Data, int w, int h);
	//static void Bayer2Rgb( BYTE* bayerData, BYTE* rgb24Data, int w, int h);
	
};

#endif