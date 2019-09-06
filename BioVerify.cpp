#include "BioVerify.h"
#include <Windows.h>
#include <Stdio.h>
#include <Conio.h>
#include <Winbio.h>
#include "FileWriter.h"

HRESULT BioVerify::Verify(WINBIO_BIOMETRIC_SUBTYPE subFactor)
{
	HRESULT hr = S_OK;
	WINBIO_SESSION_HANDLE sessionHandle = NULL;
	WINBIO_UNIT_ID unitId = 0;
	WINBIO_REJECT_DETAIL rejectDetail = 0;
	WINBIO_IDENTITY identity = { 0 };
	BOOLEAN match = FALSE;

	// Find the identity of the user.
	/*hr = GetCurrentUserIdentity(&identity);

	if (FAILED(hr))
	{
		wprintf_s(L"\n User identity not found. hr = 0x%x\n", hr);
		goto e_Exit;
	}*/
	FileWriter file;
	identity = file.ReadFromFile(L"C:\\testbio.txt");

	// Connect to the system pool. 
	hr = WinBioOpenSession(
		WINBIO_TYPE_FINGERPRINT,    // Service provider
		WINBIO_POOL_SYSTEM,         // Pool type
		WINBIO_FLAG_DEFAULT,        // Configuration and access
		NULL,                       // Array of biometric unit IDs
		0,                          // Count of biometric unit IDs
		NULL,                       // Database ID
		&sessionHandle              // [out] Session handle
	);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioOpenSession failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Verify a biometric sample.
	wprintf_s(L"\n Calling WinBioVerify - Swipe finger on sensor...\n");
	hr = WinBioVerify(
		sessionHandle,
		&identity,
		subFactor,
		&unitId,
		&match,
		&rejectDetail
	);
	wprintf_s(L"\n Swipe processed - Unit ID: %d\n", unitId);
	if (FAILED(hr))
	{
		if (hr == WINBIO_E_NO_MATCH)
		{
			wprintf_s(L"\n- NO MATCH - identity verification failed.\n");
		}
		else if (hr == WINBIO_E_BAD_CAPTURE)
		{
			wprintf_s(L"\n- Bad capture; reason: %d\n", rejectDetail);
		}
		else
		{
			wprintf_s(L"\n WinBioVerify failed. hr = 0x%x\n", hr);
		}
		goto e_Exit;
	}
	wprintf_s(L"\n Fingerprint verified:\n", unitId);


e_Exit:
	if (sessionHandle != NULL)
	{
		WinBioCloseSession(sessionHandle);
		sessionHandle = NULL;
	}

	wprintf_s(L"\n Press any key to exit...");
	_getch();

	return hr;
}

HRESULT BioVerify::DeleteTemplate(WINBIO_BIOMETRIC_SUBTYPE subFactor)
{
	HRESULT hr = S_OK;
	WINBIO_IDENTITY identity = { 0 };
	WINBIO_SESSION_HANDLE sessionHandle = NULL;
	WINBIO_UNIT_ID unitId = 0;

	// Find the identity of the user.
	FileWriter file;
	identity = file.ReadFromFile(L"C:\\testbio.txt");

	// Connect to the system pool. 
	//
	hr = WinBioOpenSession(
		WINBIO_TYPE_FINGERPRINT,    // Service provider
		WINBIO_POOL_SYSTEM,         // Pool type
		WINBIO_FLAG_DEFAULT,        // Configuration and access
		NULL,                       // Array of biometric unit IDs
		0,                          // Count of biometric unit IDs
		NULL,                       // Database ID
		&sessionHandle              // [out] Session handle
	);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioEnumBiometricUnits failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Locate the sensor.
	//
	wprintf_s(L"\n Swipe your finger on the sensor...\n");
	hr = WinBioLocateSensor(sessionHandle, &unitId);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioLocateSensor failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Delete the template identified by the subFactor argument.
	//
	hr = WinBioDeleteTemplate(
		sessionHandle,
		unitId,
		&identity,
		subFactor
	);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioDeleteTemplate failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

e_Exit:
	if (sessionHandle != NULL)
	{
		WinBioCloseSession(sessionHandle);
		sessionHandle = NULL;
	}

	wprintf_s(L"Press any key to exit...");
	_getch();

	return hr;
}


HRESULT BioVerify::VerifyOld(WINBIO_BIOMETRIC_SUBTYPE subFactor)
{
	HRESULT hr = S_OK;
	WINBIO_SESSION_HANDLE sessionHandle = NULL;
	WINBIO_UNIT_ID unitId = 0;
	WINBIO_REJECT_DETAIL rejectDetail = 0;
	WINBIO_IDENTITY identity = { 0 };
	BOOLEAN match = FALSE;

	/* **WARNING INCLUDE THE IDENTITY FUNC AND UNCOMMENT BEOFRE USING
	//Find the identity of the user.
	identity = FileWriter::
	hr = GetCurrentUserIdentity(&identity);
	if (FAILED(hr))
	{
		wprintf_s(L"\n User identity not found. hr = 0x%x\n", hr);
		goto e_Exit;
	}
	*/
	// Connect to the system pool. 
	hr = WinBioOpenSession(
		WINBIO_TYPE_FINGERPRINT,    // Service provider
		WINBIO_POOL_SYSTEM,         // Pool type
		WINBIO_FLAG_DEFAULT,        // Configuration and access
		NULL,                       // Array of biometric unit IDs
		0,                          // Count of biometric unit IDs
		NULL,                       // Database ID
		&sessionHandle              // [out] Session handle
	);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioOpenSession failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Verify a biometric sample.
	wprintf_s(L"\n Calling WinBioVerify - Swipe finger on sensor...\n");
	hr = WinBioVerify(
		sessionHandle,
		&identity,
		subFactor,
		&unitId,
		&match,
		&rejectDetail
	);
	wprintf_s(L"\n Swipe processed - Unit ID: %d\n", unitId);
	if (FAILED(hr))
	{
		if (hr == WINBIO_E_NO_MATCH)
		{
			wprintf_s(L"\n- NO MATCH - identity verification failed.\n");
		}
		else if (hr == WINBIO_E_BAD_CAPTURE)
		{
			wprintf_s(L"\n- Bad capture; reason: %d\n", rejectDetail);
		}
		else
		{
			wprintf_s(L"\n WinBioVerify failed. hr = 0x%x\n", hr);
		}
		goto e_Exit;
	}
	wprintf_s(L"\n Fingerprint verified:\n", unitId);


e_Exit:
	if (sessionHandle != NULL)
	{
		WinBioCloseSession(sessionHandle);
		sessionHandle = NULL;
	}

	wprintf_s(L"\n Press any key to exit...");
	_getch();

	return hr;
}