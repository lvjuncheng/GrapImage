/*********************************************
 * 定义错误,警告输出的宏定义
 * \author 张健 
 *********************************************/

#ifndef SN3D_ALGORITHM_COMMON_PRINT_ERROR_H
#define SN3D_ALGORITHM_COMMON_PRINT_ERROR_H

#include <iostream>
//用于输出带颜色的信息
#ifdef _WIN32
//#define NOGDI
#include <windows.h>
#define Sn_Set_Color(c) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c)
#else
//todo : linux内改变终端输出颜色的宏定义
#define Sn_Set_Color(c) 
#endif
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

//用于输出error、warning信息
//4:FOREGROUND_RED; 6:FOREGROUND_GREEN|FOREGROUND_RED; 7:FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED
#ifndef Sn_Error
#define Sn_Error(i)   { Sn_Set_Color(4); std::cout << __FILE__ << " " << __LINE__ << " Error: " << i << std::endl;    Sn_Set_Color(7); }
#endif

#ifndef Sn_wError
#define Sn_wError(i)  { Sn_Set_Color(4); std::wcout << __FILE__ << " " << __LINE__ << " Error: " << i << std::endl;   Sn_Set_Color(7); }
#endif

#ifndef Sn_Warning
#define Sn_Warning(i) { Sn_Set_Color(6); std::cout << __FILE__ << " " << __LINE__ << " Warning: " << i << std::endl;  Sn_Set_Color(7); }
#endif

#ifndef Sn_wWarning
#define Sn_wWarning(i){ Sn_Set_Color(6); std::wcout << __FILE__ << " " << __LINE__ << " Warning: " << i << std::endl; Sn_Set_Color(7); }
#endif

#endif //SN3D_ALGORITHM_COMMON_PRINT_ERROR_H
