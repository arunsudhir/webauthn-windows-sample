#include "bio/Utils.h"
#include <string>
#include <Windows.h>

std::wstring Utils::str2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//------------------------------------------------------------------------
// The following function retrieves the identity of the current user.
// This is a helper function and is not part of the Windows Biometric
// Framework API.
//
HRESULT Utils::GetCurrentUserIdentity(__inout PWINBIO_IDENTITY Identity)
{
	// Declare variables.
	HRESULT hr = S_OK;
	HANDLE tokenHandle = NULL;
	DWORD bytesReturned = 0;
	struct {
		TOKEN_USER tokenUser;
		BYTE buffer[SECURITY_MAX_SID_SIZE];
	} tokenInfoBuffer;

	// Zero the input identity and specify the type.
	ZeroMemory(Identity, sizeof(WINBIO_IDENTITY));
	Identity->Type = WINBIO_ID_TYPE_NULL;

	// Open the access token associated with the
	// current process
	if (!OpenProcessToken(
		GetCurrentProcess(),            // Process handle
		TOKEN_READ,                     // Read access only
		&tokenHandle))                  // Access token handle
	{
		DWORD win32Status = GetLastError();
		wprintf_s(L"Cannot open token handle: %d\n", win32Status);
		hr = HRESULT_FROM_WIN32(win32Status);
		goto e_Exit;
	}

	// Zero the tokenInfoBuffer structure.
	ZeroMemory(&tokenInfoBuffer, sizeof(tokenInfoBuffer));

	// Retrieve information about the access token. In this case,
	// retrieve a SID.
	if (!GetTokenInformation(
		tokenHandle,                    // Access token handle
		TokenUser,                      // User for the token
		&tokenInfoBuffer.tokenUser,     // Buffer to fill
		sizeof(tokenInfoBuffer),        // Size of the buffer
		&bytesReturned))                // Size needed
	{
		DWORD win32Status = GetLastError();
		wprintf_s(L"Cannot query token information: %d\n", win32Status);
		hr = HRESULT_FROM_WIN32(win32Status);
		goto e_Exit;
	}

	// Copy the SID from the tokenInfoBuffer structure to the
	// WINBIO_IDENTITY structure. 
	CopySid(
		SECURITY_MAX_SID_SIZE,
		Identity->Value.AccountSid.Data,
		tokenInfoBuffer.tokenUser.User.Sid
	);

	// Specify the size of the SID and assign WINBIO_ID_TYPE_SID
	// to the type member of the WINBIO_IDENTITY structure.
	Identity->Value.AccountSid.Size = GetLengthSid(tokenInfoBuffer.tokenUser.User.Sid);
	Identity->Type = WINBIO_ID_TYPE_SID;

e_Exit:

	if (tokenHandle != NULL)
	{
		CloseHandle(tokenHandle);
	}

	return hr;
}
