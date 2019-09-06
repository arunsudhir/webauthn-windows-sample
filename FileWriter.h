#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <WinBio.h>

class FileWriter
{
public:
	void WriteCharToFile(LPCWSTR fileName);
	void WriteToFile(LPCWSTR fileName, LPVOID data);
	void DisplayError(LPCWSTR lpszFunction);
	WINBIO_IDENTITY ReadFromFile(LPCWSTR fileName);
};

