
#ifndef _CPhaseInterpolation_H
#define _CPhaseInterpolation_H

#include "opencv2/opencv.hpp"
#include "cv.h"
#include"./DataIO/DataIO.h"
#include "cuda/commonGpuTool.h"

class CPhaseInterpolation
{
public:
	CPhaseInterpolation(void);
	virtual ~CPhaseInterpolation(void);

	ScanType scan_type_bino;                                                                  // ˫Ŀ���ֿ�ɨ������ 
	                                                                                          // ����Ķ�Ӧ������
	double *gl_xa,*gl_ya,*gl_xb,*gl_yb;                                                       // ����id�Ķ�Ӧ������

	double *gl_bTable, *gl_phTable;                                                           // �ֿ��м���
	
	int *gl_iTable, *gl_jTable;

	int *gl_IdlistImage,*gl_Mask;

	double* gl_pta;                                                                           // DLP��Ӧ��������� add by ly
	
	double* gl_ptb;

	int gl_Sample_Nums;                                                                       // ȫ�ֵĲ�������
	
	int gl_PointsNums;

	int gl_build_inter;
    
	bool is_Debug;                                                                            // �Ƿ����debugģʽ(���������)

public:
	bool LoadUndistortTable(const char *name);                                                // ���ұ����� ��ͶӰģʽ

	bool ReleaseUndistortTable();                                                             // ���ұ��ͷ�
	
	bool UnwarpPhase(int w, int h, double* phLeft,unsigned char *pLeft,                       // ������λ����
		double *phRight,unsigned char *pRight );
    
	void UvnInit(int *uvN, int *uvBegin, int *uvEnd);                                         // �ֿ����ݳ�ʼ��
   
	void GetUVn(int *uvN);                                                                    // ȷ��ÿ���ֿ�ĵ���

	void GetUVBegin(int *uvN, int *uvBegin);                                                  // ����������ʼ��

	void GetUVEnd(int *uvBegin, int *uvEnd, double *bTable,                                   // ����������ֹ��
		double *phTable, int *iTable, int *jTable);
	
	void OuttestGpu(double *uvN, double *uvBegin);

	void OuttestCPU(double *uvN, double *uvBegin);                                            // �м����������

	void PhaseRebuildFast_CPU();                                                              // ����

	bool StartFind();                                                                         // ������Ӧ�����̿�ʼ

	void SetFrqStrip(double frqStrip);                                                        // �趨ģ��ֿ����
	
	bool uvCenter();                                                                          // ȷ������b��phi�ֿ�����ߴ�����ĵ�

	void PointsImageGPU(double*& X, double*& Y, double*& Z, int *&idListImage, int &count,
		const std::vector<float3> &PointData);

protected:
	int uv(int u, int v)                                                                      // ��ά������һά
	{ 
		return u*m_nv+v;
	};

	bool uvInBound(int u,int v)
	{
		return (u>=0 && u<m_nu && v>=0 && v<m_nv);
	};

	int ij(int i, int j)
	{ 
		return i*m_w +j;
	};

	int round( double x);                                                                     // ��������

	int getUL(int i, int j);                                                                  // ͨ���������꣬Ѱ��С�� ����
	int getVL(int i, int j);
	int getUR(int i, int j);
	int getVR(int i, int j);

	void CheckCorresPointsForword(int id, int minij, double *phTable,                         // ������������������Ƿ���ڸ���ƥ��ĵ�
		int *iTable, int *jTable, int *iRigh, int *jRigh);

	void CheckCorresPointsBackword(int id, int minij, double *phTable,                        // �ڸ���������������Ƿ���ڸ���ƥ��ĵ�
		int *iTable, int *jTable, int *iRigh, int *jRigh);

	bool CheckPhaseLeft(const int &id, const int &k);                                         // �����������ȷ���õ��Ƿ���Ч�������������Ч���������Ч�������ӵ�����

	void CorresFindRough(int *uvN, int *iLeft, int *jLeft, int *iRigh, int *jRigh,            // ����������������Ķ�Ӧ������
		int *uvBegin, int *uvEnd, double *bTable,
		double *phTable, int *iTable, int *jTable);

	bool CorresFindRefine(int *iLeft, int *jLeft, int *iRigh, int *jRigh,                     // ��ϸ�����������ؼ��������
		double *xl, double *yl, double *xr, double *yr);

	void GenerateIdListImage(int *PMask, double *xl, double *yl, double *xr, double *yr);     // ������ά���Ӧ��id

	bool build(	int *uvN, int *uvBegin, int *uvEnd, double *bTable,                           // ���� ��Ӧ����������
		double *phTable, int *iTable, int *jTable);

	void GetMaxMinb(double &MinValue, double &MaxValue);                                      // ȷ������b��phi�ֿ�����ߴ�����ĵ�

	void GetMaxMinPhase(double &MinValue, double &MaxValue);

	int maskLegalCount( unsigned  char * mask, int count);                                    // ��Ч��������ѯ
	
	bool biInterpolationRight(int idLeft,int i1,int j1 , int i2, int j2,                      // ˫���Բ�ֵ��������ͼ����idLeft������ͼ����[���Ͻǣ�i1��j1�������½ǣ�i2��j2��]�е�����������
		double &rx, double &ry);
	
	bool biInterpolationLeft(int idRigh,int i1,int j1 , int i2, int j2,                       // ˫���Բ�ֵ��������ͼ����idRigh������ͼ����[���Ͻǣ�i1��j1�������½ǣ�i2��j2��]�е�����������
		double &lx, double &ly);

	bool allocUndistortTable(int count);                                                      // �������ұ�ռ�
	
	bool freeUndistortTable();                                                                // �ͷŲ��ұ�����

public:
	//---------------------------------------------------------------------------
	unsigned char* m_imgMaskR;                                                                // �ؽ���Ч�㣬�ò����ɽ���λ�õ�����ЧΪ1����ЧΪ0
	
	unsigned char* m_imgMaskL;
	
	double* m_imgPhiR;
	
	double* m_imgPhiL;

	int m_w;
	int m_h;
	int m_wl;
	int m_hl;
	int m_wr;
	int m_hr;
	int m_sizeLeft;
	int m_sizeRight;

	int m_nu;                                                                                  // ��С������꣬m_nu���������m_nv�������
	int m_nv;
	int m_cu;                                                                                  // ����
	int m_cv;
                                                                                               // ͼ����Ϣ
	double m_proWidth;		                                                                   // ͶӰͼ����
	double m_frqStrip;		                                                                   // ��������

	double m_reSlope;                                              
	double m_reTh;                                                                             // ���Ʒֿ���ֵ�ĵ���
	double m_Th;

	//---------------------------------------------------------------------------
	// ���ұ�����

	double *m_xUnLeft;
	double *m_yUnLeft;

	double *m_xUnRight;
	double *m_yUnRight;

	double *m_aRightL;                                                                         // ��ͼ���Ӧ�ļ���
	double *m_bRightL;

	double *m_aLeftR;                                                                          // ��ͼ���Ӧ�ļ���
	double *m_bLeftR;

	double *m_bRightE;	                                                                       // ͨ�����߼���bֵ
	double *m_bLeftE;

	int m_size;
	bool m_hasTable;
};

inline int CPhaseInterpolation::round( double x)//��������
{ 
	if(x-int(x) > 0.5 )
		return (int)(x+0.5);
	return (int)x;
}

//�������u v
inline int CPhaseInterpolation::getUL(int i, int j)
{
	return round( m_bRightL[ij(i,j)] * m_reSlope ) + m_cu;
}
inline int CPhaseInterpolation::getVL(int i, int j)
{
	return round( m_imgPhiL[ij(i,j)] * m_reTh ) + m_cv;
}
//�������u v
inline int CPhaseInterpolation::getUR(int i, int j)
{
	return round( m_bRightE[ij(i,j)] * m_reSlope ) + m_cu;
}
inline int CPhaseInterpolation::getVR(int i, int j)
{
	return round( m_imgPhiR[ij(i,j)] * m_reTh ) + m_cv;
}

#endif // _CPhaseInterpolation_H
