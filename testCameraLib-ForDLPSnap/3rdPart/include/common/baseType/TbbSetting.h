/*********************************************
 * CUP使用率计算、TBB并行设置
 * \author 向小平 
 *********************************************/

#ifndef SN3D_TBB_SETTING_H
#define SN3D_TBB_SETTING_H  

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#endif
#include <tbb/tbb.h>

namespace Sn3DAlgorithm
{

#ifdef _WIN32
    //获得系统CPU使用率
    class SN3D_COMMON_CLASS Sn3dCPURate
    {
    public:  
        //CPU使用率
        static double  get_cpu_use_rate();
        //CPU空闲率
        static double  get_cpu_idle_rate();
        //CPU空闲核数
        static int     get_cpu_idle_core();
         
    private:
        static bool    initialize();
        static double filetime_to_double(const FILETIME&);
        static bool   m_initialized;
        static double m_oldIdleFTime;
        static double m_oldKernelFTime;
        static double m_oldUserFTime;
        static int    m_numberOfProcessors;
    };
#else
    class SN3D_COMMON_CLASS Sn3dCPURate
    {
    public:
        //CPU使用率
        static double  get_cpu_use_rate();
        //CPU空闲率
        static double  get_cpu_idle_rate();
        //CPU空闲核数
        static int     get_cpu_idle_core();
    };
#endif

    //TBB并行设置
    class SN3D_COMMON_CLASS Sn3dTbbSetting
    {
    public:
        ~Sn3dTbbSetting(){_tbbInit.terminate();}
        Sn3dTbbSetting(int numThread);
        //保留一定核心数 procs-reserveNum
        void set_reverse_thread(const int reserveNum);
        //根据空闲率自动设置
        void set_auto_thread();
        //初始化
        void initialize(const int numThread);
    private:
        tbb::task_scheduler_init _tbbInit;
        bool _initialized;
    };
}
#endif//SN3D_TBB_SETTING_H

