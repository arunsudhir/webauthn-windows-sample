// WebAuthnTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WebConnection.h"
#include <Windows.h>
#include "json.h"
#include "webauthn.h"
#include "Utils.h"
#include "BioEnroll.h"
#include "BioVerify.h"
#include "FileWriter.h"
#include "BioDelete.h"

using namespace std;

int main()
{
	WebConnection conn;
	Utils utils;
	string retJson, challenge, requestId, rpId, rpName, userId, userName;
	LPCWSTR url = L"localhost";
	LPCWSTR object = L"/v1/register";
	LPCSTR data = "username=reno&displayName=Reno&requireResidentKey=false&credentialNickname=reno";
	
	//Connect to FIDO2 server on https://localhost:8080
	retJson = conn.GetDataFromUrl(url, 8080, object, data);
	json_value* val = json_parse(retJson.c_str(), retJson.size());
	

	//Parse all the needed values
	challenge = val->u.object.values[1].value->u.object.values[3].value->u.object.values[2].value->u.string.ptr;
	requestId = val->u.object.values[1].value->u.object.values[2].value->u.string.ptr;
	rpId = val->u.object.values[1].value->u.object.values[3].value->u.object.values[0].value->u.object.values[1].value->u.string.ptr;
	rpName = val->u.object.values[1].value->u.object.values[3].value->u.object.values[0].value->u.object.values[0].value->u.string.ptr;
    
	BOOL* isTpmAvailable = (BOOL*) 1;

	//Enroll fingerprint (commented because fingerprint already enrolled)
	BioEnroll bioenroll;
    bioenroll.EnrollSysPool(FALSE, WINBIO_ANSI_381_POS_RH_LITTLE_FINGER);

	//BioVerify del;
	//del.DeleteTemplate(WINBIO_ANSI_381_POS_RH_INDEX_FINGER);
	BioVerify verify;
	verify.Verify(WINBIO_ANSI_381_POS_RH_LITTLE_FINGER);
	verify.DeleteTemplate(WINBIO_ANSI_381_POS_RH_INDEX_FINGER);

	//FileWriter writer;
	//writer.WriteToFile(L"C:\\testfile.txt");

	//DWORD hrd = WebAuthNGetApiVersionNumber();
	//WebAuthNIsUserVerifyingPlatformAuthenticatorAvailable(isTpmAvailable);
	
	/*
	if (isTpmAvailable) {
		//Pass them onto webauthn to call TPM 2.0 based biometric auth
		// Relying Party details
		WEBAUTHN_RP_ENTITY_INFORMATION rpInfo;
		rpInfo.dwVersion = WEBAUTHN_RP_ENTITY_INFORMATION_CURRENT_VERSION;
		rpInfo.pwszId = utils.str2ws(rpId).c_str();
		rpInfo.pwszName = utils.str2ws(rpName).c_str();


		//user details
		WEBAUTHN_USER_ENTITY_INFORMATION  userInfo;

		userInfo.cbId = 600;
		userInfo.dwVersion = WEBAUTHN_USER_ENTITY_INFORMATION_CURRENT_VERSION;
		userInfo.pwszName = L"reno";


		//manually make the clientdataJSON with the following fields
		//type: webauth.create or webauthn.get
		// clientExtensions = {}
		// challenge = the challenge we got above
		// origin = the rp origin we got above
		
	}*/
}


