/*************************************************
* \brief 该文件定义该工程的一些定义
* \note
* \author 刘增艺
* \date   2017.03.28
**************************************************/
#ifndef SN3D_ALGORITHM_ALGORITHMLZY_DEFINES_H
#define SN3D_ALGORITHM_ALGORITHMLZY_DEFINES_H

#include "Cloud.h"
#include "TriMesh.h"
#include "RangeData.h"
#include "Image.h"
#include "BaseTriMesh.h"
#include "BoundingBox.h"
#include "PointImage.h"
#include <string>
#include <iostream>
#include <fstream>

#define CalibParemBinaryRead 1  //基于二进制读取为1 基于文本格式读取为0

namespace Sn3DAlgorithm
{

	//标定数据
	struct CameraCalibParam
	{
 		CameraCalibParam()
 		{
 			SetZero();
 		}
 		//参数归零
 		void SetZero()
 		{
 			fc.setZero();
 			cc.setZero();
 			kc.setZero();
 			rot.setIdentity();
 			tran.setZero();
 			skew = 0;
 		}
 

		RetVal read_calidata_txt(const std::string&loadPath , const Eigen::Vector2i&camearSize)
		{
			FILE *fl = fopen(loadPath.c_str(), "r");
			if (fl == NULL)
			{
				std::cout << "loadPath = " << loadPath << std::endl;
				std::cout << "can not run function read_calidata_txt because loadPath is invaild" << std::endl;
				return RetVal_ILLEGAL_INPUT;
			}
			double igore;
			int readCount = 0;
			readCount = fscanf(fl, "Focal Length:          fc = [%lf %lf]\n", &fc(0), &fc(1)); if (readCount != 2)return RetVal_ILLEGAL_INPUT;
			
			readCount = fscanf(fl, "                            [%lf %lf]\n", &igore, &igore); if (readCount != 2)return RetVal_ILLEGAL_INPUT;//fc_err[0],fc_err[1]

			readCount = fscanf(fl, "Principal point:       cc = [%lf %lf]\n", &cc(0), &cc(1)); if (readCount != 2)return RetVal_ILLEGAL_INPUT;

			readCount = fscanf(fl, "                            [%lf %lf\n\n]", &igore, &igore); if (readCount != 2)return RetVal_ILLEGAL_INPUT;//cc_err[0],cc_err[1]

			readCount = fscanf(fl, "alpha:              alpha = [%lf] [%lf]\n", &(skew), &igore); if (readCount != 2)return RetVal_ILLEGAL_INPUT;//alpha_err

			readCount = fscanf(fl, "Distortion:            kc = [%lf %lf %lf %lf %lf]\n", &kc(0), &kc(1), &kc(2), &kc(3), &kc(4)); if (readCount != 5)return RetVal_ILLEGAL_INPUT;

			readCount = fscanf(fl, "                            [%lf %lf %lf %lf %lf]\n", &igore, &igore, &igore, &igore, &igore); if (readCount != 5)return RetVal_ILLEGAL_INPUT; ///< kc error

			readCount = fscanf(fl, "Translation vector:     T = [%lf %lf %lf]\n", &tran(0), &tran(1), &tran(2)); if (readCount != 3) return RetVal_ILLEGAL_INPUT;

			readCount = fscanf(fl, "Rotation Matrix:        R = [%lf %lf %lf %lf %lf %lf %lf %lf %lf]\n",
				&rot(0, 0), &rot(0, 1), &rot(0, 2),
				&rot(1, 0), &rot(1, 1), &rot(1, 2),
				&rot(2, 0), &rot(2, 1), &rot(2, 2)); if (readCount != 9) return RetVal_ILLEGAL_INPUT;

			readCount = fscanf(fl, "Standard Error:     error = [%lf]\n", &igore); if (readCount != 1) return RetVal_ILLEGAL_INPUT;
			fclose(fl);
			generate_camera_matrix();
			imageSize = camearSize;
			return RetVal_OK;
		}


		///<生成相机矩阵
		void generate_camera_matrix()
		{
			cameraMatrix(0, 0) = fc(0);  cameraMatrix(0, 1) = skew;  cameraMatrix(0, 2) = cc(0);
			cameraMatrix(1, 0) = 0;  cameraMatrix(1, 1) = fc(1);  cameraMatrix(1, 2) = cc(1);
			cameraMatrix(2, 0) = 0;  cameraMatrix(2, 1) = 0;  cameraMatrix(2, 2) = 1;
		}

		///<找到像平面上某个像素位置看到图像在不带畸变情况下应该被看到的位置
		void remove_dis(Eigen::Vector2d&originPosition , Eigen::Vector2d&dstPosition)
		{
			///<从像素坐标系变换到像平面坐标系（f归一化坐标系）
			Eigen::Vector2d posIn , posInCopy;
			posIn(0) = (originPosition(0) - cc(0))/fc(0);
			posIn(1) = (originPosition(1) - cc(1))/fc(1);
			posIn(0) -= skew*posIn(1);

			posInCopy = posIn;
			double dis(0) , radialDis(0);
			Eigen::Vector2d deltaDis;
			for (int interId = 0; interId < 20; ++interId)
			{
				dis = posIn(0)*posIn(0) + posIn(1)*posIn(1);
				radialDis = 1.0 + kc(0)*dis + kc(1)*dis*dis + kc(4)*dis*dis*dis;///<径向畸变
				deltaDis(0) = 2 * kc(2)*posIn(0)*posIn(1) + kc(3)*(dis + 2 * posIn(0)*posIn(0));///<切向畸变
				deltaDis(1) = kc(2)*(dis + 2 * posIn(1)*posIn(1)) + 2 * kc(3)*posIn(0)*posIn(1);
				posIn(0) = (posInCopy(0) - deltaDis(0)) / radialDis;
				posIn(1) = (posInCopy(1) - deltaDis(1)) / radialDis;
			}
			dstPosition(0) = fc(0)*(posIn(0) + posIn(1)*skew) + cc(0);
			dstPosition(1) = fc(1)*posIn(1) + cc(1);
		}


		void add_dis(const Eigen::Vector2d&originPosition, Eigen::Vector2d&dstPosition)
		{
			///<从像素坐标系变换到像平面坐标系（f归一化坐标系）
			Eigen::Vector2d posIn, posInCopy;
			posIn(0) = (originPosition(0) - cc(0)) / fc(0);
			posIn(1) = (originPosition(1) - cc(1)) / fc(1);
			posIn(0) -= skew*posIn(1);

			posInCopy = posIn;
			double dis(0), radialDis(0);
			Eigen::Vector2d deltaDis;
			for (int interId = 0; interId < 20; ++interId)
			{
				dis = posIn(0)*posIn(0) + posIn(1)*posIn(1);
				radialDis = 1.0 + kc(0)*dis + kc(1)*dis*dis + kc(4)*dis*dis*dis;///<径向畸变
				deltaDis(0) = 2 * kc(2)*posIn(0)*posIn(1) + kc(3)*(dis + 2 * posIn(0)*posIn(0));
				deltaDis(1) = kc(2)*(dis + 2 * posIn(1)*posIn(1)) + 2 * kc(3)*posIn(0)*posIn(1);
				posIn[0] = posInCopy[0] * radialDis + deltaDis(0);
				posIn[1] = posInCopy[1] * radialDis + deltaDis(1);
			}
			dstPosition(0) = fc(0)*(posIn(0) + posIn(1)*skew) + cc(0);
			dstPosition(1) = fc(1)*posIn(1) + cc(1);
		}

		void count_remove_dis_lookTable()
		{
			imageRemoveDisLookTable.resize(imageSize(0)*imageSize(1), Eigen::Vector2(0, 0));
			Eigen::Vector2d sourcePixPos , dstPixPos;
			int Id = -1;
			Eigen::Vector4d rangeLimit = Eigen::Vector4d(0.5, float(imageSize(0)) - 0.5 , 0.5 , float(imageSize(1)) - 0.5);
			for (int heightId = 0; heightId < imageSize(1); ++heightId)
			{
				for (int widthId = 0; widthId < imageSize(0); ++widthId)
				{
					++Id;
					sourcePixPos = Eigen::Vector2d(double(widthId) + 0.5 , double(heightId) + 0.5);
					add_dis(sourcePixPos, dstPixPos);
					if (dstPixPos(0) < rangeLimit(0) || dstPixPos(0) > rangeLimit(1) || dstPixPos(1) < rangeLimit(2) || dstPixPos(1) > rangeLimit(3))
					{
						imageRemoveDisLookTable[Id] = Eigen::Vector2(-1 , -1);
					}
					imageRemoveDisLookTable[Id] = dstPixPos.cast<float>();
				}
			}
		}



 		//打印参数值
 		void PrintCalibResult() const 
 		{
 			std::cout << "fc : " << fc.transpose() << std::endl;
 			std::cout << "cc : " << cc.transpose() << std::endl;
 			std::cout << "skew : " << skew << std::endl;
 			std::cout << "kc : " << kc.transpose() << std::endl;
 			std::cout << "rot : " << rot << std::endl;
 			std::cout << "tran : " << tran.transpose() << std::endl;
 		}
 
 		Eigen::Vector2d fc;            //横纵方向的等效焦距
 		Eigen::Vector2d cc;            //主点坐标
 		Eigen::Matrix<double, 5, 1> kc; //畸变参数	
 		Eigen::Matrix3d rot;           //旋转矩阵
 		Eigen::Vector3d tran;          //平移矩阵
		Eigen::Matrix3d cameraMatrix;  ///<相机矩阵
		Eigen::Vector2i imageSize;     ///<相机分辨率
		std::vector<Eigen::Vector2>imageRemoveDisLookTable;///<图片去畸变查找表,记录了原始图片插值源
 		double skew;

	};


	//光平面参数
	struct MltLinesPlaneParam
	{
 		MltLinesPlaneParam()
 		{
 			vecPlaneEquation.clear();
 			vecDecodeRangeLookTable.clear();
 		}
		std::vector<std::vector<Eigen::Vector4f>> vecPlaneEquation;         // 光平面方程
		std::vector<std::vector<Eigen::Vector4i>> vecDecodeRangeLookTable;   // 第一第二幅联合解码的查找表
		//输出光平面参数
		const void printf_out() const
		{
			int planeNum = vecPlaneEquation.size();
			for (int planeId = 0; planeId < planeNum; ++planeId)
			{
				std::cout << "DepthImage index = " << planeId + 1<< std::endl;
				for (int iD = 0; iD < vecPlaneEquation[planeId].size(); ++iD)
				{
					const Eigen::Vector4f& pe = vecPlaneEquation[planeId][iD];
					if (abs(pe[0]) < 1e-6 && abs(pe[1]) < 1e-6 && abs(pe[2]) < 1e-6  && abs(pe[3]) < 1e-6)
					{
						continue;	
					}
					std::cout << "vecPlaneEquation = " << iD << " " << pe(0) << "\t" << pe(1) << "\t" << pe(2) << "\t" << pe(3) << std::endl;	
				}
			}
			int lookTableNum = vecDecodeRangeLookTable.size();
			std::cout << "look Table" << std::endl;
			for (int lookTableId = 0; lookTableId < lookTableNum; ++lookTableId)
			{
				for (int iD = 0; iD < vecDecodeRangeLookTable[lookTableId].size(); ++iD)
				{
					std::cout << "vecPlaneEquation = " << vecDecodeRangeLookTable[lookTableId][iD](0) << "\t" << vecDecodeRangeLookTable[lookTableId][iD](1) << "\t" << vecDecodeRangeLookTable[lookTableId][iD](2) << "\t" << vecDecodeRangeLookTable[lookTableId][iD](3) << std::endl;
				}
			}
		}
	};

	//标定参数
 	struct DeviceCalibParam
	{
   		CameraCalibParam    camCalibParam;   //相机标定参数
 		MltLinesPlaneParam  mltLineParam;    //光平面标定参数
		//输出标定参数
		void printf_out() const
		{
			//输出相机标定参数
			camCalibParam.PrintCalibResult();
			//输出线平面标定参数
			mltLineParam.printf_out();
		}

		//加载标定参数
		bool load_calib_data(const std::string& sourcePath)
		{
			//读取标定数据
#if CalibParemBinaryRead  
			std::string path = sourcePath;
#else 
			std::string path = sourcePath + "CalibParam/";
#endif

			bool ret = ReadCamCalibParamTeeth(camCalibParam, 1, path);
			if(!ret)
			{
				std::cout << "load ReadCamCalibParamTeeth fail " << __LINE__ << std::endl;
				return false;
			}

#if CalibParemBinaryRead                         //二进制格式读取
			//二进制读取光平面方程
			ret = LoadPlaneEquationBinary(path);
			if (!ret)
			{
				std::cout << "LoadPlaneEquationBinary fail " << __LINE__ << std::endl;
				return false;
			}
	
#else                                           //文本格式读取
			//加载标定数据及生成编码范围查找表
			mltLineParam.vecPlaneEquation.clear();
			mltLineParam.vecPlaneEquation.resize(3);
			int maxPlaneNum = 76; //最大光平面个数
			int range = 20;
			path = sourcePath + "Index3PlaneParame.txt";
			ret = LoadPlaneParam(path, maxPlaneNum, mltLineParam.vecPlaneEquation[0]);
			if (!ret)
			{
				std::cout << "load Index3PlaneParame fail " << __LINE__ << std::endl;
				return false;
			}
			path = sourcePath + "Index4PlaneParame.txt";
			LoadPlaneParam(path, maxPlaneNum, mltLineParam.vecPlaneEquation[1]);
			if (!ret)
			{
				std::cout << "load Index4PlaneParame fail " << __LINE__ << std::endl;
				return false;
			}
			path = sourcePath + "Index5PlaneParame.txt";
			LoadPlaneParam(path, maxPlaneNum, mltLineParam.vecPlaneEquation[2]);
			if (!ret)
			{
				std::cout << "load Index5PlaneParame fail " << __LINE__ << std::endl;
				return false;
			}
			//加载深度查找表
			path = sourcePath + "DecodeRangeLookTable.txt";
			ret = CreateRangeLookTable(path, range, mltLineParam.vecDecodeRangeLookTable);
			if (!ret)
			{
				std::cout << "load DecodeRangeLookTable fail " << __LINE__ << std::endl;
				return false;
			}
#endif	
			return true;
	
		}

		//读取相机参数
		bool ReadCamCalibParamTeeth(CameraCalibParam& camCalibPar, int num, const std::string&ccFPath)
		{
			camCalibPar.SetZero();
			 
#if CalibParemBinaryRead                            //二进制读取
			//相机标定参数二进制读取	
			double rot[9];
			double tran[3];
			std::ifstream input;
			std::string path = ccFPath + "cameraCCF.bin";
			std::cout << path << std::endl;
			input.open(path.c_str(), std::ios::binary);
			if (input.fail())
			{
				std::cout << "load camera calibration param fail" << std::endl;
				return false;
			}
			//写出x与y方向的等效焦距
			input.read((char*)&camCalibPar.fc[0], sizeof(double)* 2);
			input.read((char*)&camCalibPar.cc[0], sizeof(double)* 2);
			input.read((char*)&camCalibPar.kc[0], sizeof(double)* 5);
			input.read((char*)&camCalibPar.skew, sizeof(double));
			input.read((char*)&rot[0], sizeof(double)* 9);
			input.read((char*)&tran[0], sizeof(double)* 3);
			Eigen::Map<Eigen::Matrix<double, 3, 3, Eigen::RowMajor>> rt_m(&rot[0], 3, 3);
			Eigen::Map<Eigen::Vector3d> rt_v(&tran[0], 3, 1);
			camCalibPar.rot = rt_m;
			camCalibPar.tran = rt_v;
			input.close();
			return true;
#else                                                 //文本格式读取
			double rot[9];
			double tran[3];
			std::string path = ccFPath + "LeftCCF.txt";
			std::cout << path << std::endl;
			//加载相机标定数据
			bool ret = ReadcaliparamText(path.c_str(), &camCalibPar.fc[0], &camCalibPar.cc[0], camCalibPar.skew, &camCalibPar.kc[0], tran, rot);
			if (!ret)
			{
				std::cout << "load camera calibration param fail" << std::endl;
				return false;
			}
			Eigen::Map<Eigen::Matrix<double, 3, 3, Eigen::RowMajor>> rt_m(&rot[0], 3, 3);
			Eigen::Map<Eigen::Vector3d> rt_v(&tran[0], 3, 1);
			camCalibPar.rot = rt_m;
			camCalibPar.tran = rt_v;
			return true;
#endif	
		}


		//二进制读取光平面方程
		bool LoadPlaneEquationBinary(std::string path)
		{
			//光平面方程二进制读取
			char fileName[256];
			sprintf_s(fileName, "PlaneEquationParam.bin");
			std::ifstream input;
			input.open(path + fileName, std::ios::binary);
			if (input.fail())
			{
				std::cout << "LoadPlaneEquationBinary fail" << std::endl;
				return false;
			}
	        //光平面方程参数初始化
			int allDepthImgNum = 3;
			int singlePlaneEquNum = 76;
			mltLineParam.vecPlaneEquation.clear();
			mltLineParam.vecPlaneEquation.resize(allDepthImgNum);
			mltLineParam.vecPlaneEquation[0].resize(singlePlaneEquNum);
			mltLineParam.vecPlaneEquation[1].resize(singlePlaneEquNum);
			mltLineParam.vecPlaneEquation[2].resize(singlePlaneEquNum);
			mltLineParam.vecDecodeRangeLookTable.clear();
			mltLineParam.vecDecodeRangeLookTable.resize(allDepthImgNum);
			mltLineParam.vecDecodeRangeLookTable[0].resize(5);
			mltLineParam.vecDecodeRangeLookTable[1].resize(5);
			mltLineParam.vecDecodeRangeLookTable[2].resize(10);


			//所有深度图的个数
			int nDepthImgNum = 0;
			input.read((char*)(&nDepthImgNum), sizeof(int));
			if (nDepthImgNum != allDepthImgNum)
			{
				std::cout << "nDepthImgNum != allDepthImgNum  and the nDepthImgNum = " << nDepthImgNum << std::endl;
				input.close();
				return false;
			}
			//读取所有深度图的平面方程
			for (int i = 0; i < nDepthImgNum; i++)
			{
				//读取当前幅深度图的光平面方程个数
				int nCurDepthImgIdxNum = 0;
				input.read((char*)(&nCurDepthImgIdxNum), sizeof(int));
				if (nCurDepthImgIdxNum != singlePlaneEquNum)
				{
					std::cout << "nCurDepthImgIdxNum != singlePlaneEquNum  and the nCurDepthImgIdxNum = " << nDepthImgNum << std::endl;
					input.close();
					return false;
				}
				//读取当面幅深度图的光平面方程
				for (int n = 0; n < nCurDepthImgIdxNum; n++)
				{
					input.read((char*)(&mltLineParam.vecPlaneEquation[i][n][0]), sizeof(Eigen::Vector4f));
				}
			}

			//读取最近最远的像素移动范围
			int nDepthImgPixselRangeNum = 0;
			//读取总的编码值个数3个 （编码为1、2、3）
			input.read((char*)(&nDepthImgPixselRangeNum), sizeof(int));
			if (nDepthImgPixselRangeNum != allDepthImgNum)
			{
				std::cout << "nDepthImgPixselRangeNum != allDepthImgNum  and the nDepthImgPixselRangeNum = " << nDepthImgPixselRangeNum << std::endl;
				input.close();
				return false;
			}
			//依次读取每个编码的像素range
			for (int i = 0; i < nDepthImgPixselRangeNum; i++)
			{
				//读取总的编码值个数3个 （编码为1、2、3）
				int nRangeNum = 0;
				input.read((char*)(&nRangeNum), sizeof(int));
				if (nRangeNum != mltLineParam.vecDecodeRangeLookTable[i].size())
				{
					std::cout << "nRangeNum != mltLineParam.vecDecodeRangeLookTable[i].size() and the nRangeNum = " << nRangeNum << std::endl;
					input.close();
				}
				//读取当前编码值的所有range
				for (int j = 0; j < nRangeNum; j++)
				{
					input.read((char*)(&mltLineParam.vecDecodeRangeLookTable[i][j][0]), sizeof(Eigen::Vector4f));
				}
			}
			input.close();
			//光平面二进制写出完毕
			return true;
		}


		//读取文本格式的标定数据
		bool ReadcaliparamText(const std::string&filename, double *fc, double* cc, double &alpha, double *Distort, double *T, double *R)
		{
			FILE* file;
			if ((file = fopen(filename.c_str(), "r")) == NULL)
			{
				std::cout << "read calib fail" << std::endl;
				return false;
			}
			char firstchar;
			int i = 0;
			while (fscanf(file, "%c", &firstchar) != EOF)
			{
				if (firstchar == '[')
				{
					i++;
					if (1 == i)
					{
						fscanf(file, "%lf %lf", &(fc[0]), &(fc[1]));
					}
					if (3 == i)
					{
						fscanf(file, "%lf %lf", &(cc[0]), &(cc[1]));
					}
					if (5 == i)
					{
						fscanf(file, "%lf", &(alpha));
					}
					if (7 == i)
					{
						fscanf(file, "%lf  %lf  %lf  %lf  %lf", &(Distort[0]), &(Distort[1]), 	&(Distort[2]), &(Distort[3]), &(Distort[4]));

					}
					if (9 == i)
					{
						fscanf(file, "%lf  %lf  %lf", &(T[0]), &(T[1]), &(T[2]));
					}
					if (10 == i)
					{
						fscanf(file, "%lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf", &(R[0]), &(R[1]),
							&(R[2]), &(R[3]), &(R[4]), &(R[5]), &(R[6]), &(R[7]), &(R[8]));
					}
				}
			}
			return true;
		}

		


		//加载光平面
		bool LoadPlaneParam(const std::string& str, int num, std::vector<Eigen::Vector4f>& vecPlaneParam)
		{
			vecPlaneParam.clear();
			FILE* fileRT;
			if ((fileRT = fopen(str.c_str(), "r")) == NULL)
			{
				std::cout << str.c_str() << "LoadPlaneParam fail" << std::endl;
				return false;
			}
			Eigen::Vector4f pt;
			for (int i = 0; i < num; i++)
			{
				fscanf(fileRT, "%f %f %f %f", &(pt[0]), &(pt[1]), &(pt[2]), &(pt[3]));
				vecPlaneParam.push_back(pt);
			}
			fclose(fileRT);
			if (vecPlaneParam.size() < 1)
			{
				std::cout << "vecPlaneParam.size() < 1 in LoadPlaneParam function " << std::endl;
				return false;
			}
			return true;
		}

		//加载编码范围查找表数据
		bool LoadPlaneParam(const std::string& str, int num, std::vector<Eigen::Vector4i>& vecPlaneParam)
		{
			vecPlaneParam.clear();
			FILE* fileRT;
			if ((fileRT = fopen(str.c_str(), "r")) == NULL)
			{
				std::cout << str.c_str() << "LoadPlaneParam fail" << std::endl;
				return false;
			}
			Eigen::Vector4i pt;
			for (int i = 0; i < num; i++)
			{
				fscanf(fileRT, "%d %d %d %d", &(pt[0]), &(pt[1]), &(pt[2]), &(pt[3]));
				vecPlaneParam.push_back(pt);
			}
			fclose(fileRT);
			return true;
		}


		//生成范围查找表
		bool CreateRangeLookTable(const std::string& str, int num, std::vector<std::vector<Eigen::Vector4i>>& vecDecodeRange)
		{
			vecDecodeRange.clear();
			vecDecodeRange.resize(3);
			vecDecodeRange[0].resize(5);
			vecDecodeRange[1].resize(5);
			vecDecodeRange[2].resize(10);
			//加载编码范围查找表数据
			std::vector<Eigen::Vector4i> vecPlaneParam;
			bool ret = LoadPlaneParam(str, num, vecPlaneParam);
			if (!ret || vecPlaneParam.size() != num)
			{
				std::cout << "vecPlaneParam.size() != num " << __LINE__<<std::endl;
				return false;
			}
			for (int i = 0; i < 5; i++)
			{
				vecDecodeRange[0][i] = vecPlaneParam[i];
				vecDecodeRange[1][i] = vecPlaneParam[i + 5];
			}
			for (int i = 0; i < 10; i++)
			{
				vecDecodeRange[2][i] = vecPlaneParam[i + 10];
			}
			return true;
		}

 	};


}// namespace Sn3DAlgorithm

#endif //SN3DALGORITHM_ALGORITHMLZY_ALGORITHMLZY_DEFINES_H
