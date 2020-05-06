#pragma once
#include <wrl.h>
#include <locale>
#include <codecvt>

// Helper macros

#define Property(type, var) \
private: \
    type _##var; \
public: \
    type Get##var() { return _##var; }\
    void Set##var(type val) { _##var = val; }

// Various helper functions

using namespace std;

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw exception();
	}
}

//-------------------------------------------------------------------------------------------

// Utility functions to convert from wstring to string and back
// Copied from https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string

inline wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

inline string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

inline void ErrorDialog(const wstring& title, const wstring& content, const std::exception& exception)
{
	int o = MessageBoxEx(
		0, (content + L"\n\nAbort: Close this application.\nRetry: Debug this application.\nIgnore: Continue without generating normals.").c_str(),
		title.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR, 0
	);

	switch (o)
	{
	case IDABORT:
		exit(~0);
		break;

	case IDRETRY:
		throw exception;
		break;

	case IDIGNORE: default:
		break;
	}
}

inline void Output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(s2ws(szBuff).c_str());
}