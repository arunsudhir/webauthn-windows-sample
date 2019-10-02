#pragma once
#include <string>
#include <Windows.h>
#include <WinBio.h>

class Utils
{
public:
	std::wstring str2ws(const std::string& s);
	HRESULT GetCurrentUserIdentity(__inout PWINBIO_IDENTITY Identity);
};

