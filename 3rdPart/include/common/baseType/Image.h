/***********************************************
 * 定义图片类
 * \author 张健 
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_IMAGE_H
#define SN3D_ALGORITHM_COMMON_IMAGE_H

#include "Traits.h"

namespace Sn3DAlgorithm
{
	///图片类型
	enum ImageType
	{
		ImageType_Grey = 1, ///< 灰度图
		ImageType_RGB = 3   ///< RGB三通道图
	};

	/** \brief pixel类型的Traits
	 *  Type表示图片的类型,可以选择ImageType_Grey或者ImageType_RGB。
	 *  T表示存储图片的数据类型,可以选择float或者uchar。如果是float，范围[0,1)。如果是uchar，范围[0,255)
	 */
	template<int Type, typename T>
	class PixelTraits 
	{
	public:
		typedef Eigen::Matrix<T, Type, 1> PixelType;
	};

	template<typename T>
	class PixelTraits<ImageType_Grey, T> 
	{
	public:
		typedef T PixelType;
	};

	/** \brief 图片类
	 *  Type表示图片的类型,可以选择ImageType_Grey或者ImageType_RGB。
	 *  T表示存储图片的数据类型,可以选择float或者uchar。如果是float，范围[0,1)。如果是uchar，范围[0,255)
	 */
	template<int Type, typename T>
	class Image
	{
	public:
		typedef typename PixelTraits<Type, T>::PixelType PixelType;
		typedef Eigen::Matrix<PixelType, Eigen::Dynamic, Eigen::Dynamic> ImageData;

		////////////////////////////////////////////////////////构造函数和析构函数
		Image() {}
		Image(int h, int w) { set_size(h, w); }
		explicit Image(const ImageData& data): _data(data) {}

		////////////////////////////////////////////////////////height width channel
		void set_size(int h, int w) { _data.resize(h, w); }
		void set_constant(const PixelType& px) {
			int	nW	= get_width();
			int	nH	= get_height();
			for (int v = 0; v < nH; ++v) {
				for (int u = 0; u < nW; ++u)
					_data(v, u)	= px;
			}
		}
		int get_height() const { return _data.rows(); }
		int get_width() const { return _data.cols(); }

		///获得图片通道数目
		static int get_channel_num() { return Type; }

		///////////////////////////////////////////////////////access and modify
		const ImageData& get_data() const { return _data; }
		ImageData& get_data() { return _data; }
		///i代表高，j代表宽
		const PixelType& get_pixel(int i, int j) const { return _data(i, j); }
		PixelType& get_pixel(int i, int j) { return _data(i, j); }
		void set_pixel(int i, int j, const PixelType& p) {  _data(i, j) = p; }
		const PixelType& operator()(int i, int j) const { return _data(i, j); }
		PixelType& operator()(int i, int j) { return _data(i, j); }

	protected:
		ImageData _data;  ///< 图片的像素数据
	};

	typedef Image<ImageType_Grey, unsigned char> ImageGreyb;
	typedef Image<ImageType_Grey, float> ImageGreyf;
	typedef Image<ImageType_RGB, unsigned char> ImageRGBb;  ///< 三个通道分别是BGR
	typedef Image<ImageType_RGB, float> ImageRGBf;  ///< 三个通道分别是BGR

	//算法内部临时变量
	typedef Image<ImageType_Grey, double> ImageGreyd;
	typedef Image<ImageType_RGB, double> ImageRGBd;  ///< 三个通道分别是BGR

	/// 相机无畸变内参
	class SN3D_COMMON_API CameraInParam
	{
	public:
		void set_param(double fx, double fy, double cx, double cy) {
			_fx = fx;  _fy = fy; _cx = cx; _cy = cy;
		}
		double get_fx() const { return _fx; }
		double get_fy() const { return _fy; }
		double get_cx() const { return _cx; }
		double get_cy() const { return _cy; }

		///空间坐标转化为图像坐标
		inline void point_to_image(const Eigen::Vector3& coord, Eigen::Vector2& imageCoord) const;
		///图像坐标转化为空间坐标
		inline void image_to_point(const Eigen::Vector2& imageCoord, Scalar z, Eigen::Vector3& coord) const;

		inline void depth_to_point(const float depth, const Eigen::Vector2& pixId, Eigen::Vector3&point) const;

	protected:
		double _fx, _fy, _cx, _cy;
	};

	inline void CameraInParam::point_to_image(const Eigen::Vector3& coord, Eigen::Vector2& imageCoord) const
	{
		imageCoord[0] = coord[0]/coord[2]*_fx + _cx;
		imageCoord[1] = coord[1]/coord[2]*_fy + _cy;
	}

	inline void CameraInParam::image_to_point(const Eigen::Vector2& imageCoord, Scalar z, Eigen::Vector3& coord) const
	{
		coord[2] = z;
		coord[0] = (imageCoord[0] - _cx) / _fx * z;
		coord[1] = (imageCoord[1] - _cy) / _fy * z;
	}

	inline void CameraInParam::depth_to_point(const float depth, const Eigen::Vector2& pixId, Eigen::Vector3&point) const
	{
		point(0) = depth * (pixId(0) - _cx)/_fx;
		point(1) = depth * (pixId(1) - _cy)/_fy;
		point(2) = depth;
	}


}//namespace

#endif //SN3D_ALGORITHM_COMMON_IMAGE_H