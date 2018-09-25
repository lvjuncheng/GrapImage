/***************************************************
 * 定义些常用的功能函数
 * \author 张健
 ***************************************************/

#ifndef SN3D_ALGORITHM_COMMON_COMMON_TOOL_H
#define SN3D_ALGORITHM_COMMON_COMMON_TOOL_H

#include "globalDef.h"
#include <sstream>
#include <sys/timeb.h>
#include "ctime"
#ifdef _WIN32
	//#define NOGDI
	#include <windows.h>
	#include <psapi.h>
#endif
#include <vector>
#include "./baseType/Traits.h"


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


namespace Sn3DAlgorithm
{
	///不同类型数据之间互相转化
	template <typename T, typename U>
	inline int convert_form(const U& input, T& output)
	{
		std::stringstream ss;
		if(!(ss << input))
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		if(!(ss >> output))
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		std::string s;
		ss >> s;
		if(s.size())
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		return 0;
	}
	///不同类型数据之间的相互转化，使用wstringstream
	template <typename T, typename U>
	inline int convert_form_using_wchar(const U& input, T& output)
	{
		std::wstringstream ss;
		if(!(ss << input))
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		if(!(ss >> output))
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		std::wstring s;
		ss >> s;
		if(s.size())
		{
			Sn_Cout1("input data is illage");
			return 1;
		}
		return 0;
	}

	inline std::string wstring_to_string(const std::wstring& wstr)
	{
		std::string	strText(wstr.begin(), wstr.end());
		return strText;
	}

	inline std::wstring string_to_wstring(const std::string &str)
	{
		std::wstring wstr(str.length(), L' ');
		std::copy(str.begin(), str.end(), wstr.begin());
		return wstr;
	}

	///得到系统时间，返回的是以毫秒为单位的整数
	inline long long get_system_time()
	{
		timeb t;
		ftime(&t);
		return 1000*t.time + t.millitm;
	}


	///在0到num-1中选择k个数。k相对于num是比较大的
	void SN3D_COMMON_CLASS randomly_select_more(int num, int k, std::vector<int>& picked);
	///在0到num-1中选择k个数。k相对于num是比较小的
	void SN3D_COMMON_CLASS randomly_select_less(int num, int k, std::vector<int>& picked);
	///在0到num-1中选择k个数。k个数中可以有重复的。
	void SN3D_COMMON_CLASS multi_randomly_select(int num, int k, std::vector<int>& picked);

#ifdef _WIN32
    ///统计执行时间, 计算代码段的执行时间
    ///call: rest(), code, microsecond()
    class SN3D_COMMON_CLASS CQueryPerformance
    {
    public:
        CQueryPerformance();
        ~CQueryPerformance(){}
        ///重新计时
        void   reset();
        ///获取从计时到当前执行时间(微秒)
        long long  microsecond();
        ///打印执行时间,info为函数、代码段提示信息。
        void   print_spend_time(const char *info);

    private:
        LARGE_INTEGER frequency;
        LARGE_INTEGER start;
    };    
#else
    class SN3D_COMMON_CLASS CQueryPerformance
    {
    public:
        CQueryPerformance();
        ~CQueryPerformance(){}
        ///重新计时
        void   reset();
        ///获取从计时到当前执行时间(微秒)
        long long  microsecond();
        ///打印执行时间,info为函数、代码段提示信息。
        void   print_spend_time(const char *info);
    private:
      clock_t start,end;
    };
#endif //WIN32


	void SN3D_COMMON_CLASS count_time_begin();
	void SN3D_COMMON_CLASS count_time_end(const std::string&costName);
	void SN3D_COMMON_CLASS count_time_end(const std::string&costName, float&timeCost);
	void SN3D_COMMON_CLASS count_time_end_wm(const std::string&costName);
	void SN3D_COMMON_CLASS count_time_end_wm(float&timeCount);
	void SN3D_COMMON_CLASS count_time_end_wm(const std::string&costName, float&timeCost);
	

	///<为了得到奇数
	inline int get_odd(const int input)
	{
		return(input | 1);
	}

	///<该函数用于计算以某个体块为中心，range为半径每个体块的偏移量,确保输入的range是个奇数
	inline void get_shift(std::vector<Eigen::Vector3i>&shiftXYZ, const int range)
	{
		if (range < 1)
		{
			std::cout << "get_shift fail because range < 1" << std::endl;
			shiftXYZ.clear();
			return;
		}
		int rangeCopy = get_odd(range);
		shiftXYZ.resize(rangeCopy*rangeCopy*rangeCopy, Eigen::Vector3i(0, 0, 0));
		for (int vId = 0; vId < shiftXYZ.size(); vId++)
		{
			shiftXYZ[vId](2) = (vId / (rangeCopy*rangeCopy)) - (rangeCopy >> 1);
			int zY = vId % (rangeCopy*rangeCopy);
			shiftXYZ[vId](1) = zY / rangeCopy - (rangeCopy >> 1);
			shiftXYZ[vId](0) = zY % rangeCopy - (rangeCopy >> 1);
		}
	}

    ///统计函数体执行时间
    ///在对象析构时统计时间
    class SN3D_COMMON_CLASS CQPMethod
    {
    public:
        explicit CQPMethod(char* info)
        {
            printf("%s ......\n", info);
            strcpy(m_info, info);
            query.reset();
        }
        ~CQPMethod(){ query.print_spend_time(m_info); }

    private:
        char m_info[512];
        CQueryPerformance query;
    };
    
#ifdef _WIN32
    #define CQPMETHOD CQPMethod cqp(__FUNCTION__);
#else
    #define CQPMETHOD CQPMethod cqp("");
#endif
    
    void SN3D_COMMON_CLASS count_time_begin();
    void SN3D_COMMON_CLASS count_time_end(const std::string&costName);
    void SN3D_COMMON_CLASS count_time_end_wm(const std::string&costName);
    void SN3D_COMMON_CLASS count_time_end(const std::string&costName, float&timeCost);


	///判断a是否是合法值,即a不能等于QNAN,INF,IND
	inline bool is_value_legal(double a)
	{
		return (a >= -std::numeric_limits<double>::max()) && (a <= std::numeric_limits<double>::max());
	}
	///判断a是否是合法值,即a不能等于QNAN,INF,IND
	inline bool is_value_legal(float a)
	{
		return (a >= -std::numeric_limits<float>::max()) && (a <= std::numeric_limits<float>::max());
	}
	///判断normal是否是合法值, normal的长度要等于1
	inline bool is_normal_legal(const Eigen::Vector3d& normal)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(normal[i])) return false;
		}
		if(std::fabs(normal.norm() - 1.0) < Sn_F_EPS || normal.norm() < Sn_F_EPS) return true;
		return false;
	}
	///判断normal是否是合法值, normal的长度要等于1或者0
	inline bool is_normal_legal(const Eigen::Vector3f& normal)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(normal[i])) return false;
		}
		if(std::fabs(normal.norm() - 1.0) < Sn_F_EPS || normal.norm() < Sn_F_EPS) return true;
		return false;
	}
	///判断坐标是否是合法值
	inline bool is_coord_legal(const Eigen::Vector3d& coord)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(coord[i])) return false;
		}
		return true;
	}
	///判断坐标是否是合法值
	inline bool is_coord_legal(const Eigen::Vector3f& coord)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(coord[i])) return false;
		}
		return true;
	}
	///判断颜色是否是合法值, 颜色的取值范围是[0,1]
	inline bool is_color_legal(const Eigen::Vector3d& color)
	{
		for(int i = 0; i < 3; ++i)
		{
			if((color[i] < 0.0) || (color[i] > 1.0)) return false;
		}
		return true;
	}
	///判断颜色是否是合法值, 颜色的取值范围是[0,1]
	inline bool is_color_legal(const Eigen::Vector3f& color)
	{
		for(int i = 0; i < 3; ++i)
		{
			if((color[i] < 0.0) || (color[i] > 1.0)) return false;
		}
		return true;
	}

#ifdef _WIN32
    ///当前进程的内存消耗统计
    ///统计两次CheckMemory之间的内存消耗与释放
    class SN3D_COMMON_CLASS CProcessMemoryInfo
    {
typedef PROCESS_MEMORY_COUNTERS PMC;

    public:
        ~CProcessMemoryInfo();
        CProcessMemoryInfo();
        //重新统计
        void reset();
        //检查当前内存
        void check_memory_info(char *checkPoint="");

    private:
        HANDLE m_hProcess;
        PMC m_basePMC;//原始内存信息
        PMC m_lastPMC;//上次内存信息
        PMC m_currPMC;//当前内存信息

    private:
        void print_memory_info();
    };
#else
    class SN3D_COMMON_CLASS CProcessMemoryInfo
    {
    public:
        ~CProcessMemoryInfo();
        CProcessMemoryInfo();
        //重新统计
        void reset();
        //检查当前内存
        void check_memory_info(char *checkPoint = "");

    private:
        void print_memory_info();
    };
#endif
	/** \brief 从文件中读入数据，填充一个float的vector,oneDisparity必须是已知大小，且已经提前分配好空间的vector
	*/
	void SN3D_COMMON_CLASS read_float_vector(const std::string&sourcePath, std::vector<float>&oneDisparity);

	/** \brief 将一个vector<float>写出到文件中去
	*/
	void SN3D_COMMON_CLASS write_float_vector(const std::string&outputPath,const std::vector<float>&oneDisparity);

	/** \brief 将路径filePath分解成文件夹路径dirName和文件名fileName
	 *  \return 分解正确，则返回0
	 */
	int SN3D_COMMON_CLASS split_filePath(const std::string& filePath, std::string& dirName, std::string& fileName);

	///获得文件名fileName中的前缀prefix和后缀suffix
	int SN3D_COMMON_CLASS split_fileName(const std::string& fileName, std::string& prefix, std::string& suffix);

	/** \brief 使用特定字符将string分解成多个string串
	 *  \param[in] isReserveNull 是否保留长度为0的字符串
	 */
	void SN3D_COMMON_CLASS split_by_character(const std::string& input, char splitChar, std::vector<std::string>& ouput, bool isReserveNull = false);

	///获得四舍五入值
	inline float round(float v) { return v>=0.0f?floorf(v+0.5f):ceilf(v-0.5f); }
	///获得四舍五入值
	inline double round(double v) { return v>=0.0?floor(v+0.5):ceil(v-0.5); }
	///将数字转为string
	std::string SN3D_COMMON_API num_to_string(int num);
	///将数字转为string，浮点数，需要指定精度（decimalPlaces）
	std::string SN3D_COMMON_API num_to_string(Scalar num, int decimalPlaces);

    ///true:OpenMP并行，false:TBB并行
    extern bool SN3D_COMMON_API g_useOpenmpParallel;
}//namespace
#endif //SN3D_ALGORITHM_COMMON_COMMON_TOOL_H
