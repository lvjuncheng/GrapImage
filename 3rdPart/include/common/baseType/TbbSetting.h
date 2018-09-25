/*********************************************
 * CUPʹ���ʼ��㡢TBB��������
 * \author ��Сƽ 
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
    //���ϵͳCPUʹ����
    class SN3D_COMMON_CLASS Sn3dCPURate
    {
    public:  
        //CPUʹ����
        static double  get_cpu_use_rate();
        //CPU������
        static double  get_cpu_idle_rate();
        //CPU���к���
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
        //CPUʹ����
        static double  get_cpu_use_rate();
        //CPU������
        static double  get_cpu_idle_rate();
        //CPU���к���
        static int     get_cpu_idle_core();
    };
#endif

    //TBB��������
    class SN3D_COMMON_CLASS Sn3dTbbSetting
    {
    public:
        ~Sn3dTbbSetting(){_tbbInit.terminate();}
        Sn3dTbbSetting(int numThread);
        //����һ�������� procs-reserveNum
        void set_reverse_thread(const int reserveNum);
        //���ݿ������Զ�����
        void set_auto_thread();
        //��ʼ��
        void initialize(const int numThread);
    private:
        tbb::task_scheduler_init _tbbInit;
        bool _initialized;
    };
}
#endif//SN3D_TBB_SETTING_H

