
#include"gl/glaux.h"			// GLaux库的头文件
//class ConfigFile;
class ImageTex
{

public:
	ImageTex()
		:imageTexs(NULL)
		,count(0)
	{
//		data.sizeX = 1024;
//		data.sizeY = 1024;
//		data.data = new unsigned char[data.sizeX*data.sizeY*3]; //RGB
		stop = 1;
		curFrame = 0;
	}
	~ImageTex()
	{
		delete[] data.data;
	}
	static ImageTex *GetInstance()
	{
		static ImageTex inst;
		return &inst;
	}
	
	GLuint *imageTexs;
	GLuint whiteTex;
	int count;
	int curFrame;
	int stop;
	AUX_RGBImageRec data;
	void BuildTexture( ConfigFile *config);
	void Draw( );
protected:
	void GetTexture(GLuint &tex,  AUX_RGBImageRec *image, double startPhase, double times = 1.0 );
};
