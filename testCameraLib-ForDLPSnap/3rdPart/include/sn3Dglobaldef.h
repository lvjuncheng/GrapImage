// ====================================================================================================================================================
// File name:	sn3Dglobaldef.h
//
// Copyright (C), 2009-2011, Shining 3D Tech. Co., Ltd.
//
// Description:	ȫ�ֶ��塣�������͡���ѧ�����������㡢����ֵ����ȵĶ��塣���е���д���붼Ӧ���Ȱ������ļ���������ʹ�ñ��ļ��еĶ��塣����������ʹ�ñ��ļ�
//              ��û�еĶ��壬Ӧ����������뵽���ļ���ȥ���Է������ʹ�á�
// 
// Histroy:		
// ======================================================================================================================================================

#ifndef __sn3Dglobaldef_H__
#define __sn3Dglobaldef_H__

// ======================================================================================================================================================
// ����
typedef	unsigned char byte;
typedef short int16;            // 16 bit integer
typedef unsigned short uint16;  // 16 bit unsigned integer
typedef int int32;              // 32 bit integer
typedef unsigned int uint;      // 32 bit unsigned integer
typedef uint uint32;            // 32 bit unsigned integer
typedef long long int64;

//=====================================================================================================================================================
// UNICODE
#include <tchar.h>
#ifndef tstring
	#ifdef _UNICODE
		#define  tstring wstring
	#else
		#define  tstring string
	#endif
#endif
#ifdef _UNICODE
#define  tistringstream wistringstream
#define  tostringstream wostringstream
#define  tcout			wcout
#else
#define  tistringstream istringstream
#define  tostringstream ostringstream
#define  tcout			cout
#endif
// ======================================================================================================================================================
// ��ѧ����
const double ZERO_TOLERANCE = 1.0e-12;                  // ������Ƚ�ֵ��С�ڴ�ֵ��Ϊ������Ϊ0
const double DEGREES_TO_RADIANS = 0.01745329251994329576923690768489;       // �Ƕ�ת���ȳ˷�����
const double RADIANS_TO_DEGREES = 57.295779513082320876798154814105;     // ����ת�Ƕȳ˷�����

const float PI_FLOAT = 3.1415926535897932f;                     // �����ͦ�
const double PI = 3.1415926535897932384626433832795;            // ˫���ȸ����ͦ�
const double SN_2PI = 6.283185307179586476925286766559;         // 2*��
const double PI_OVER_2 = 1.5707963267948966192313216916398;     // ��/2
const double PI_OVER_3 = 1.0471975511965977461542144610932;     // ��/3
const double SN_2PI_OVER_3 = 2.0943951023931954923084289221863; // 2*��/3
const double PI_OVER_4 = 0.78539816339744830961566084581988;    // ��/4
const double SN_3PI_OVER_4 = 2.3561944901923449288469825374596; // 3*��/4
const double PI_OVER_6 = 0.52359877559829887307710723054658;    // ��/6
const double SIN_PI_OVER_12 = 0.2588190451025207623488990;      // sin(��/12)
const double COS_PI_OVER_12 = 0.9659258262890682867497433;      // cos(��/12)
const double SN_SQRT2 = 1.414213562373095048801689;         // ����2
const double SN_SQRT3 = 1.732050807568877293527446;         // ����3
const double SQRT3_OVER_2 = 0.8660254037844386467637230;    // (����3)/2
const double SN_1_OVER_SQRT2 = 0.7071067811865475244008445; // 1/(����2)

const double SN_LOG2 = 0.6931471805599453094172321;
const double SN_LOG10 = 2.302585092994045684017991;

const double DOUBLE_MAX = 1.7976931348623158e+308;      // ���ɱ�ʾ��˫���ȸ������ֵ
const double DOUBLE_MIN = 2.22507385850720200e-308;     // ��С�ɱ�ʾ��˫���ȸ������ֵ

const double EPSILON = 2.2204460492503131e-16;
const double SQRT_EPSILON = 1.490116119385000000e-8;

const double UNSET_VALUE = -1.23432101234321e+308;      // Ϊ˫���ȸ�������ʼ����ֵ�������ڴ�ֵ�������ñ���δ�����㼰��ֵ��
const float UNSET_FLOAT = -1.234321e+38f;               // Ϊ��������ʼ����ֵ�������ڴ�ֵ�������ñ���δ�����㼰��ֵ��


// ======================================================================================================================================================
// ������
#define ArrayCount(a) (sizeof(a)/sizeof((a)[0]))

// ======================================================================================================================================================
// ����ֵ����
#define ERR_SUCCESS             0x0     //�ɹ��ķ���ֵ
#define ERR_PARA_NULL_POINTER   0x101   //����Ĳ�����Ӧ��Ϊ��ָ��
#define ERR_PARA_NEGATIVE       0x102   //����Ĳ�����Ӧ��Ϊ����
#define ERR_PARA_OVER_LIMIT     0x103   //����Ĳ��������˺Ϸ��ķ�Χ
#define ERR_PARA_LOGIC          0x104   //����Ĳ������߼���ͻ
#define ERR_PARA_UNDEFINED      0x105   //һ��Ĳ����������
#define ERR_MEM_NULL_POINTER    0x201   //��ͼ��ָ����з���
#define ERR_MEM_LEAK            0x202   //û���㹻���ڴ�ռ乩ʹ��
#define ERR_MEM_OVER_LIMIT      0x203   //�ڴ�ķǷ�Խ�����
#define ERR_MEM_UNDEFINED	    0x204   //һ����ڴ���ʴ���
#define ERR_CAL_DIV_BY_ZERO     0x301   //���ֱ�0������
#define ERR_CAL_NO_ENDING       0x302   //�㷨������������ѭ��
#define ERR_CAL_OVER_LIMIT	    0x303   //�㷨���м�ֵ��Խ�Ϸ���Χ
#define ERR_CAL_UNDEFINED	    0x304   //һ����㷨����
#define ERR_OPER_DATA_NOT_READY	0x401   //Ҫ����������/����û��׼�����
#define ERR_OPER_STEP_NOT_READY	0x402   //�������ǰ��Ĳ���
#define ERR_OPER_NO_LISENCE	    0x403   //��ģ��δ�������������ܿ�
#define ERR_OPER_VERSION	    0x404   //��ģ��汾��ƥ�䣬�����°�װ
#define ERR_OPER_UNDEFINED      0x405   //һ��Ĳ�������
#define ERR_UNDEFINED           0x1001  //�޷�����Ĵ���

#endif