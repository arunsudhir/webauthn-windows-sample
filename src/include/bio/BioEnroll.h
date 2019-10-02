#include <Windows.h>
#include <Stdio.h>
#include <Conio.h>
#include <Winbio.h>
#pragma comment(lib, "winbio.lib")

class BioEnroll
{
public:
	HRESULT EnrollSysPool(
		BOOL discardEnrollment,
		WINBIO_BIOMETRIC_SUBTYPE subFactor);

	
};

