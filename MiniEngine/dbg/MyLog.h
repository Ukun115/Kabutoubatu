/*!
 *@brief		ログ
 */

#pragma once

/*!
 *@brief	ログ出力
 */
namespace nsKabutoubatu
{
	static inline void Log(const char* format, ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf(log, format, va);
		OutputDebugStringA(log);
		va_end(va);
	}
	static inline void LogW(const wchar_t* format, ...)
	{
		static wchar_t log[1024 * 10];
		va_list va;
		va_start(va, format);
		_vswprintf(log, format, va);
		OutputDebugStringW(log);
		va_end(va);
	}
}

#ifdef MY_DEBUG
	#define K2_LOG( format, ... )	Log(format, __VA_ARGS__)
	#define K2_LOG_W( format, ... )	LogW(format, __VA_ARGS__)
#else // _DEBUG
#define K2_LOG( format, ... )
#define K2_LOG_W( format, ... )
#endif // _DEBUG
