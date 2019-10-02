#pragma comment(lib, "winhttp.lib")
#include <Windows.h>
#include <winhttp.h>
#include <string>

class WebConnection
{
public:
	//data to send is a narrow string and not a wide string
	std::string GetDataFromUrl(LPCWSTR url, INTERNET_PORT port, LPCWSTR object, LPCSTR data);
};

