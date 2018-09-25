#include "stdafx.h"

//#include "..//sn3DBuild//ConfigFile.h"
#include "ImageTex.h"

#include <math.h>
#include <stdio.h>

// startPhase ��ʼ��λ
// times  ������
#define M_PI       3.14159265358979323846
void CreatePhaseImage( AUX_RGBImageRec *image, double startPhase, double times = 2.0)
{
	int w = image->sizeX;
	int h = image->sizeY;
	double M2 = 256.0/2; //��ֵһ��
	memset( image->data, 0, 3*w*h*sizeof(unsigned char) );
	double step = 2.0*M_PI*times/image->sizeY;
	for( int i=0; i<w; i++)
	{
		double sinN = sin( startPhase + step*i );
		int color = (int)(M2*sinN+M2);
		if( color>255)
			color = 255;
		if( color<0 )
			color = 0;
		for( int j=0; j<h; j++)
		{
			unsigned char *data = image->data+j*w*3+i*3;
			*(data + 0 ) = color;
			*(data + 1 ) = color;
			*(data + 2 ) = color;
		}
	}
}

void ImageTex::GetTexture(GLuint &tex,  AUX_RGBImageRec *image, double startPhase, double times )
{
	CreatePhaseImage( image,startPhase,times );
	glGenTextures(1, &tex);					// ��������

	// ʹ������λͼ�������� �ĵ�������
	glBindTexture(GL_TEXTURE_2D, tex);
	// ��������
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
}

void ImageTex::BuildTexture( ConfigFile *config)
{
	if( imageTexs )
		delete[] imageTexs;
	count = config->FrameCount();
	imageTexs  = new GLuint[count];
	int nFrame = 0;
	for( int i=0; i<config->stepNum; i++ )
	{
		for( int j=0; j<config->step->moveNum; j++)
		{
			double start = j*M_PI*2/config->step[i].moveNum;
			double time = 1.0*config->width/config->step[i].pixelWidth;
			GetTexture(imageTexs[nFrame], &data, start, time );
			nFrame++;
		}
	}
}
GLubyte rasters[12] = {
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc,
	0xfc, 0xc0, 0xc0, 0xc0, 0xff, 0xff};

void ImageTex::Draw( )
{
	if( stop )
		return;

	CreatePhaseImage( &data,0,1 );
	glDrawPixels( data.sizeX,data.sizeY,GL_RGB,GL_UNSIGNED_BYTE,data.data);

	return;
//  	glEnable(GL_TEXTURE_2D);					// ʹ�ö�ά����
//  	glBindTexture(GL_TEXTURE_2D, imageTexs[curFrame]);
// 	glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����
// 	glTranslatef(0.0f,0.0f,-1.0f);
// 	glBegin(GL_QUADS);									//  ����������
// 	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, 1.0f, 0.0f);					// ����
// 	glTexCoord2f(1.0f, 0.0f);glVertex3f( 1.0f, 1.0f, 0.0f);					// ����
// 	glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f,-1.0f, 0.0f);					// ����
// 	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f,-1.0f, 0.0f);					// ����
// 	glEnd();	
}