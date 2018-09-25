
/***********************************************
 * Range�ļ���д
 * \author ��Сƽ
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
#define SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
#include "../baseType/RangeData.h"
#include "../baseType/Cloud.h"
#include "../baseType/SimpRangeData.h"
#include <string>

namespace Sn3DAlgorithm
{
    ///������Ϣ
    class SN3D_COMMON_API GeometryInfo
    {
    public:
        /// ģ����Ϣ
        uint vn;
        size_t matrixByte;
        size_t vertexByte;
        size_t normalByte;
        size_t colorByte;
        size_t texByte;

        /// �ļ���Ϣ
        /// ���ݴ洢˳��1 matrix 2 �������� 3 ���㷨�� 4 ������ɫ 5 ��������
        size_t firstMatrix;	///<�任���� firstMatrix=fisrtGeometry
        size_t firstVertex; ///<��������
        size_t firstNormal; ///<���㷨��
        size_t firstColor;	///<������ɫ
        size_t firstTex;	///<��������

    public:
        GeometryInfo();
        void SetInfo(Sn3DAlgorithm::RangeData&  data);
        size_t GetAllSize();
        size_t SetFirstByte(size_t fisrtGeometry);
        void SetLength();
    };

    /// ���ͼ������Ϣ
    class SN3D_COMMON_API RangeTopologyInfo
    {
    public:
        /// ģ����Ϣ
        uint width;
        uint height;
        uint refPtNum;
        double cameraDir[3];///<�ϰ汾ʹ�ã������λ��ȡ��
        double meadistance; ///<����

        size_t cameraByte;	///<���λ��
        size_t tableByte;	///<UV����
        size_t flagByte;	///<Cell��־λ
        size_t refPtByte;	///<��־�� refPtByte = refIDByte + refPosByte+refweightByte

        size_t refIDByte;
        size_t refPosByte;
        size_t refweightByte;

        /// �ļ���Ϣ
        size_t firstCamera;
        size_t firstTable;
        size_t firstFlag;
        size_t firstRefPt;

        /// �ļ���ʽ
        bool isOrderedPoint;

    public:
        RangeTopologyInfo();
        void SetInfo(Sn3DAlgorithm::RangeData&  range);
        size_t SetFistByte(size_t fisrtTopology);
        void   SetRefPtLength();
        void SetLength(int vn);
    };

    ///RangeData��д
    class SN3D_COMMON_API CRangeDataIO
    {
    public:
        CRangeDataIO(void);
        ~CRangeDataIO(void);


    public:
        ///��ȡrge�ļ���������RangeImage��
        static int read_rge(const std::string&  fileName, RangeData &range);
        static int read_rge(const std::wstring& fileName, RangeData &range);
        ///��RangeImageд��rge�ļ���
        static int write_rge(const std::string&  fileName, const RangeData& range);
        static int write_rge(const std::wstring& fileName, const RangeData& range);
        ///����RangeImage����תƽ�ƾ���
        static int update_rge_transform(const std::wstring& rgeName, const RangeData& range);
        static int update_rge_transform(const std::wstring& rgeName, const RigidMatrix& rigidMat);
        ///����RangeImage�ı�־��
        static int update_rge_refpoints(const std::wstring& rgeName, const RangeData& range);

		//! read srge
		static int read_srge(const std::string& file_name, SimpRangeData &rge);
		static int write_srge(const std::string& file_name, const SimpRangeData &rge);
    }; 

}
#endif //SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
