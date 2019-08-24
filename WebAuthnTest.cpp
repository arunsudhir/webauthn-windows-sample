// WebAuthnTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WebConnection.h"
#include <Windows.h>
#include "json.h"

int main()
{
	WebConnection conn;
	std::string retJson, challenge, requestId, rpId, rpName, userId, userName;
	LPCWSTR url = L"localhost";
	LPCWSTR object = L"/v1/register";
	LPCSTR data = "username=reno&displayName=Reno&requireResidentKey=false&credentialNickname=reno";
	
	retJson = conn.GetDataFromUrl(url, 8080, object, data).c_str();
	json_value* val = json_parse(retJson.c_str(), retJson.size());
	

	//Parse all the needed values
	challenge = val->u.object.values[1].value->u.object.values[3].value->u.object.values[2].value->u.string.ptr;
	requestId = val->u.object.values[1].value->u.object.values[2].value->u.string.ptr;
	rpId = val->u.object.values[1].value->u.object.values[3].value->u.object.values[0].value->u.object.values[1].value->u.string.ptr;
	rpName = val->u.object.values[1].value->u.object.values[3].value->u.object.values[0].value->u.object.values[0].value->u.string.ptr;
    
	//Pass them onto webauthn to call TPM 2.0 based biometric auth

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


