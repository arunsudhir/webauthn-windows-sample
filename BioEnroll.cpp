#include "BioEnroll.h"
#include <Windows.h>
#include <Stdio.h>
#include <Conio.h>
#include <Winbio.h>


HRESULT BioEnroll::EnrollSysPool(
	BOOL discardEnrollment,
	WINBIO_BIOMETRIC_SUBTYPE subFactor)
{
	HRESULT hr = S_OK;
	WINBIO_IDENTITY identity = { 0 };
	WINBIO_SESSION_HANDLE sessionHandle = NULL;
	WINBIO_UNIT_ID unitId = 0;
	WINBIO_REJECT_DETAIL rejectDetail = 0;
	BOOLEAN isNewTemplate = TRUE;

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
		wprintf_s(L"\n WinBioOpenSession failed. ");
		wprintf_s(L"hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Locate a sensor.
	wprintf_s(L"\n Swipe your finger on the sensor...\n");
	hr = WinBioLocateSensor(sessionHandle, &unitId);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioLocateSensor failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Begin the enrollment sequence. 
	wprintf_s(L"\n Starting enrollment sequence...\n");
	hr = WinBioEnrollBegin(
		sessionHandle,      // Handle to open biometric session
		subFactor,          // Finger to create template for
		unitId              // Biometric unit ID
	);
	if (FAILED(hr))
	{
		wprintf_s(L"\n WinBioEnrollBegin failed. hr = 0x%x\n", hr);
		goto e_Exit;
	}

	// Capture enrollment information by swiping the sensor with
	// the finger identified by the subFactor argument in the 
	// WinBioEnrollBegin function.
	for (int swipeCount = 1;; ++swipeCount)
	{
		wprintf_s(L"\n Swipe the sensor to capture %s sample.",
			(swipeCount == 1) ? L"the first" : L"another");

		hr = WinBioEnrollCapture(
			sessionHandle,  // Handle to open biometric session
			&rejectDetail   // [out] Failure information
		);

		wprintf_s(L"\n Sample %d captured from unit number %d.",
			swipeCount,
			unitId);

		if (hr == WINBIO_I_MORE_DATA)
		{
			wprintf_s(L"\n    More data required.\n");
			continue;
		}
		if (FAILED(hr))
		{
			if (hr == WINBIO_E_BAD_CAPTURE)
			{
				wprintf_s(L"\n  Error: Bad capture; reason: %d",
					rejectDetail);
				continue;
			}
			else
			{
				wprintf_s(L"\n WinBioEnrollCapture failed. hr = 0x%x", hr);
				goto e_Exit;
			}
		}
		else
		{
			wprintf_s(L"\n    Template completed.\n");
			break;
		}
	}

	// Discard the enrollment if the appropriate flag is set.
	// Commit the enrollment if it is not discarded.
	if (discardEnrollment == TRUE)
	{
		wprintf_s(L"\n Discarding enrollment...\n\n");
		hr = WinBioEnrollDiscard(sessionHandle);
		if (FAILED(hr))
		{
			wprintf_s(L"\n WinBioLocateSensor failed. hr = 0x%x\n", hr);
		}
		goto e_Exit;
	}
	else
	{
		wprintf_s(L"\n Committing enrollment...\n");
		hr = WinBioEnrollCommit(
			sessionHandle,      // Handle to open biometric session
			&identity,          // WINBIO_IDENTITY object for the user
			&isNewTemplate);    // Is this a new template

		if (FAILED(hr))
		{
			wprintf_s(L"\n WinBioEnrollCommit failed. hr = 0x%x\n", hr);
			goto e_Exit;
		}
	}


e_Exit:
	if (sessionHandle != NULL)
	{
		WinBioCloseSession(sessionHandle);
		sessionHandle = NULL;
	}

	wprintf_s(L" Press any key to continue...");
	_getch();

	return hr;
}
