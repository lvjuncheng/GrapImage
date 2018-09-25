/***************************************************
 * ����Щ���õĹ��ܺ���
 * \author �Ž�
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
	///��ͬ��������֮�以��ת��
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
	///��ͬ��������֮����໥ת����ʹ��wstringstream
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

	///�õ�ϵͳʱ�䣬���ص����Ժ���Ϊ��λ������
	inline long long get_system_time()
	{
		timeb t;
		ftime(&t);
		return 1000*t.time + t.millitm;
	}


	///��0��num-1��ѡ��k������k�����num�ǱȽϴ��
	void SN3D_COMMON_CLASS randomly_select_more(int num, int k, std::vector<int>& picked);
	///��0��num-1��ѡ��k������k�����num�ǱȽ�С��
	void SN3D_COMMON_CLASS randomly_select_less(int num, int k, std::vector<int>& picked);
	///��0��num-1��ѡ��k������k�����п������ظ��ġ�
	void SN3D_COMMON_CLASS multi_randomly_select(int num, int k, std::vector<int>& picked);

#ifdef _WIN32
    ///ͳ��ִ��ʱ��, �������ε�ִ��ʱ��
    ///call: rest(), code, microsecond()
    class SN3D_COMMON_CLASS CQueryPerformance
    {
    public:
        CQueryPerformance();
        ~CQueryPerformance(){}
        ///���¼�ʱ
        void   reset();
        ///��ȡ�Ӽ�ʱ����ǰִ��ʱ��(΢��)
        long long  microsecond();
        ///��ӡִ��ʱ��,infoΪ�������������ʾ��Ϣ��
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
        ///���¼�ʱ
        void   reset();
        ///��ȡ�Ӽ�ʱ����ǰִ��ʱ��(΢��)
        long long  microsecond();
        ///��ӡִ��ʱ��,infoΪ�������������ʾ��Ϣ��
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
	

	///<Ϊ�˵õ�����
	inline int get_odd(const int input)
	{
		return(input | 1);
	}

	///<�ú������ڼ�����ĳ�����Ϊ���ģ�rangeΪ�뾶ÿ������ƫ����,ȷ�������range�Ǹ�����
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

    ///ͳ�ƺ�����ִ��ʱ��
    ///�ڶ�������ʱͳ��ʱ��
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


	///�ж�a�Ƿ��ǺϷ�ֵ,��a���ܵ���QNAN,INF,IND
	inline bool is_value_legal(double a)
	{
		return (a >= -std::numeric_limits<double>::max()) && (a <= std::numeric_limits<double>::max());
	}
	///�ж�a�Ƿ��ǺϷ�ֵ,��a���ܵ���QNAN,INF,IND
	inline bool is_value_legal(float a)
	{
		return (a >= -std::numeric_limits<float>::max()) && (a <= std::numeric_limits<float>::max());
	}
	///�ж�normal�Ƿ��ǺϷ�ֵ, normal�ĳ���Ҫ����1
	inline bool is_normal_legal(const Eigen::Vector3d& normal)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(normal[i])) return false;
		}
		if(std::fabs(normal.norm() - 1.0) < Sn_F_EPS || normal.norm() < Sn_F_EPS) return true;
		return false;
	}
	///�ж�normal�Ƿ��ǺϷ�ֵ, normal�ĳ���Ҫ����1����0
	inline bool is_normal_legal(const Eigen::Vector3f& normal)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(normal[i])) return false;
		}
		if(std::fabs(normal.norm() - 1.0) < Sn_F_EPS || normal.norm() < Sn_F_EPS) return true;
		return false;
	}
	///�ж������Ƿ��ǺϷ�ֵ
	inline bool is_coord_legal(const Eigen::Vector3d& coord)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(coord[i])) return false;
		}
		return true;
	}
	///�ж������Ƿ��ǺϷ�ֵ
	inline bool is_coord_legal(const Eigen::Vector3f& coord)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(!is_value_legal(coord[i])) return false;
		}
		return true;
	}
	///�ж���ɫ�Ƿ��ǺϷ�ֵ, ��ɫ��ȡֵ��Χ��[0,1]
	inline bool is_color_legal(const Eigen::Vector3d& color)
	{
		for(int i = 0; i < 3; ++i)
		{
			if((color[i] < 0.0) || (color[i] > 1.0)) return false;
		}
		return true;
	}
	///�ж���ɫ�Ƿ��ǺϷ�ֵ, ��ɫ��ȡֵ��Χ��[0,1]
	inline bool is_color_legal(const Eigen::Vector3f& color)
	{
		for(int i = 0; i < 3; ++i)
		{
			if((color[i] < 0.0) || (color[i] > 1.0)) return false;
		}
		return true;
	}

#ifdef _WIN32
    ///��ǰ���̵��ڴ�����ͳ��
    ///ͳ������CheckMemory֮����ڴ��������ͷ�
    class SN3D_COMMON_CLASS CProcessMemoryInfo
    {
typedef PROCESS_MEMORY_COUNTERS PMC;

    public:
        ~CProcessMemoryInfo();
        CProcessMemoryInfo();
        //����ͳ��
        void reset();
        //��鵱ǰ�ڴ�
        void check_memory_info(char *checkPoint="");

    private:
        HANDLE m_hProcess;
        PMC m_basePMC;//ԭʼ�ڴ���Ϣ
        PMC m_lastPMC;//�ϴ��ڴ���Ϣ
        PMC m_currPMC;//��ǰ�ڴ���Ϣ

    private:
        void print_memory_info();
    };
#else
    class SN3D_COMMON_CLASS CProcessMemoryInfo
    {
    public:
        ~CProcessMemoryInfo();
        CProcessMemoryInfo();
        //����ͳ��
        void reset();
        //��鵱ǰ�ڴ�
        void check_memory_info(char *checkPoint = "");

    private:
        void print_memory_info();
    };
#endif
	/** \brief ���ļ��ж������ݣ����һ��float��vector,oneDisparity��������֪��С�����Ѿ���ǰ����ÿռ��vector
	*/
	void SN3D_COMMON_CLASS read_float_vector(const std::string&sourcePath, std::vector<float>&oneDisparity);

	/** \brief ��һ��vector<float>д�����ļ���ȥ
	*/
	void SN3D_COMMON_CLASS write_float_vector(const std::string&outputPath,const std::vector<float>&oneDisparity);

	/** \brief ��·��filePath�ֽ���ļ���·��dirName���ļ���fileName
	 *  \return �ֽ���ȷ���򷵻�0
	 */
	int SN3D_COMMON_CLASS split_filePath(const std::string& filePath, std::string& dirName, std::string& fileName);

	///����ļ���fileName�е�ǰ׺prefix�ͺ�׺suffix
	int SN3D_COMMON_CLASS split_fileName(const std::string& fileName, std::string& prefix, std::string& suffix);

	/** \brief ʹ���ض��ַ���string�ֽ�ɶ��string��
	 *  \param[in] isReserveNull �Ƿ�������Ϊ0���ַ���
	 */
	void SN3D_COMMON_CLASS split_by_character(const std::string& input, char splitChar, std::vector<std::string>& ouput, bool isReserveNull = false);

	///�����������ֵ
	inline float round(float v) { return v>=0.0f?floorf(v+0.5f):ceilf(v-0.5f); }
	///�����������ֵ
	inline double round(double v) { return v>=0.0?floor(v+0.5):ceil(v-0.5); }
	///������תΪstring
	std::string SN3D_COMMON_API num_to_string(int num);
	///������תΪstring������������Ҫָ�����ȣ�decimalPlaces��
	std::string SN3D_COMMON_API num_to_string(Scalar num, int decimalPlaces);

    ///true:OpenMP���У�false:TBB����
    extern bool SN3D_COMMON_API g_useOpenmpParallel;
}//namespace
#endif //SN3D_ALGORITHM_COMMON_COMMON_TOOL_H
