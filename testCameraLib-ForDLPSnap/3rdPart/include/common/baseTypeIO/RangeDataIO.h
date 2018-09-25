
/***********************************************
 * Range文件读写
 * \author 向小平
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
#define SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
#include "../baseType/RangeData.h"
#include "../baseType/Cloud.h"
#include "../baseType/SimpRangeData.h"
#include <string>

namespace Sn3DAlgorithm
{
    ///顶点信息
    class SN3D_COMMON_API GeometryInfo
    {
    public:
        /// 模型信息
        uint vn;
        size_t matrixByte;
        size_t vertexByte;
        size_t normalByte;
        size_t colorByte;
        size_t texByte;

        /// 文件信息
        /// 数据存储顺序，1 matrix 2 顶点坐标 3 顶点法向 4 顶点颜色 5 顶点纹理
        size_t firstMatrix;	///<变换矩阵 firstMatrix=fisrtGeometry
        size_t firstVertex; ///<顶点坐标
        size_t firstNormal; ///<顶点法向
        size_t firstColor;	///<顶点颜色
        size_t firstTex;	///<顶点纹理

    public:
        GeometryInfo();
        void SetInfo(Sn3DAlgorithm::RangeData&  data);
        size_t GetAllSize();
        size_t SetFirstByte(size_t fisrtGeometry);
        void SetLength();
    };

    /// 深度图拓扑信息
    class SN3D_COMMON_API RangeTopologyInfo
    {
    public:
        /// 模型信息
        uint width;
        uint height;
        uint refPtNum;
        double cameraDir[3];///<老版本使用，被相机位置取代
        double meadistance; ///<点间距

        size_t cameraByte;	///<相机位置
        size_t tableByte;	///<UV坐标
        size_t flagByte;	///<Cell标志位
        size_t refPtByte;	///<标志点 refPtByte = refIDByte + refPosByte+refweightByte

        size_t refIDByte;
        size_t refPosByte;
        size_t refweightByte;

        /// 文件信息
        size_t firstCamera;
        size_t firstTable;
        size_t firstFlag;
        size_t firstRefPt;

        /// 文件形式
        bool isOrderedPoint;

    public:
        RangeTopologyInfo();
        void SetInfo(Sn3DAlgorithm::RangeData&  range);
        size_t SetFistByte(size_t fisrtTopology);
        void   SetRefPtLength();
        void SetLength(int vn);
    };

    ///RangeData读写
    class SN3D_COMMON_API CRangeDataIO
    {
    public:
        CRangeDataIO(void);
        ~CRangeDataIO(void);


    public:
        ///读取rge文件，保存在RangeImage中
        static int read_rge(const std::string&  fileName, RangeData &range);
        static int read_rge(const std::wstring& fileName, RangeData &range);
        ///将RangeImage写入rge文件中
        static int write_rge(const std::string&  fileName, const RangeData& range);
        static int write_rge(const std::wstring& fileName, const RangeData& range);
        ///更新RangeImage的旋转平移矩阵
        static int update_rge_transform(const std::wstring& rgeName, const RangeData& range);
        static int update_rge_transform(const std::wstring& rgeName, const RigidMatrix& rigidMat);
        ///更新RangeImage的标志点
        static int update_rge_refpoints(const std::wstring& rgeName, const RangeData& range);

		//! read srge
		static int read_srge(const std::string& file_name, SimpRangeData &rge);
		static int write_srge(const std::string& file_name, const SimpRangeData &rge);
    }; 

}
#endif //SN3D_ALGORITHM_COMMON_RANGE_DATA_IO_H
