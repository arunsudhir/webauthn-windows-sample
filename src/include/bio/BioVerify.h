#pragma once
#include <Windows.h>
#include <Winbio.h>

class BioVerify
{
public:
	HRESULT Verify(WINBIO_BIOMETRIC_SUBTYPE subFactor);
	HRESULT DeleteTemplate(WINBIO_BIOMETRIC_SUBTYPE subFactor);
	HRESULT VerifyOld(WINBIO_BIOMETRIC_SUBTYPE subFactor);
};

