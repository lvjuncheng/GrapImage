
#ifndef _DebugConsole_H
#define _DebugConsole_H
///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (C), 2009-20??, Shining 3D Tech. Co., Ltd.
//
/// @file	DebugConsole.h
/// @brief	define marco: show console window in Win32 Application
///         FILE_DEBUG_CONSOLE  
///         CREATE_DEBUG_CONSOLE
///         RELEASE_DEBUG_CONSOLE
///         
/// @author wuzq
/// @version 1.0
/// @date 2012-01-04
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <locale.h>

#define FILE_DEBUG_CONSOLE \
	FILE* fpDebugOut = NULL; \
	FILE* fpDebugIn = NULL; //\
	//FILE* fpDebugErr = NULL; 

#define CREATE_DEBUG_CONSOLE \
	if( !AllocConsole() ) \
	MessageBox(NULL, _T("控制台生成失败。"), NULL, 0); \
	fpDebugOut = freopen("CON","w", stdout); \
	fpDebugIn = freopen("CON", "r", stdin); 

/*

_tfreopen_s(&fpDebugOut, _T("CONOUT$"),_T("w"), stdout); \
_tfreopen_s(&fpDebugIn, _T("CONIN$"), _T("r"), stdin); \
_tfreopen_s(&fpDebugErr, _T("CONOUT$"),_T("w"), stderr); \
*/

#define RELEASE_DEBUG_CONSOLE \
	fclose(fpDebugOut); \
	fclose(fpDebugIn); \
	FreeConsole();//\
	//fclose(fpDebugErr); \

#endif //_DebugConsole_H





