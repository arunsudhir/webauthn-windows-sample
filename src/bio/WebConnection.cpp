#include "bio/WebConnection.h"
#include <winhttp.h>
#include <stdio.h>
#include <string>

	std::string WebConnection::GetDataFromUrl(LPCWSTR url, INTERNET_PORT port, LPCWSTR object, LPCSTR data)
	{
		std::string fullBuf;
		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		LPSTR pszOutBuffer;
		BOOL  bResults = FALSE;
		HINTERNET  hSession = NULL,
			hConnect = NULL,
			hRequest = NULL;
		
		// Use WinHttpOpen to obtain a session handle.
		hSession = WinHttpOpen(L"WinHTTP Example/1.0",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS, 0);

		// Specify an HTTP server.
		if (hSession)
			hConnect = WinHttpConnect(hSession, url,
				8080, 0);

		// Create an HTTP request handle.
		if (hConnect)
			hRequest = WinHttpOpenRequest(hConnect, L"POST", object,
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				WINHTTP_FLAG_BYPASS_PROXY_CACHE);

		LPCWSTR header = L"Content-Type: application/x-www-form-urlencoded\r\n";
		// Send a request.
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				header, -1L,
				(LPVOID) data,
				79,
				79, 0);


		// End the request.
		if (bResults)
			bResults = WinHttpReceiveResponse(hRequest, NULL);

		// Keep checking for data until there is nothing left.
		if (bResults)
		{
			do
			{
				// Check for available data.
				dwSize = 0;
				if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
					printf("Error %u in WinHttpQueryDataAvailable.\n",
						GetLastError());

				// Allocate space for the buffer.
				pszOutBuffer = new char[dwSize + 1];
				if (!pszOutBuffer)
				{
					printf("Out of memory\n");
					dwSize = 0;
				}
				else
				{
					// Read the data.
					ZeroMemory(pszOutBuffer, dwSize + 1);

					if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
						dwSize, &dwDownloaded))
						printf("Error %u in WinHttpReadData.\n", GetLastError());
					else {
						printf("%s", pszOutBuffer);
						fullBuf.append(pszOutBuffer);
					}
					// Free the memory allocated to the buffer.
					delete[] pszOutBuffer;
				}
			} while (dwSize > 0);
		}


		// Report any errors.
		if (!bResults)
			printf("Error %d has occurred.\n", GetLastError());

		// Close any open handles.
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);
		
		//const char *cstr = fullBuf.c_str();
		return fullBuf;
	}
