#include "fido2/fido.h"
#include "fido2/extern.h"
#include "fido2/types.h"
#include "fido2/FidoLogger.h"
#include "fido2/register.h"
#include <conio.h>
#include <string>
#include <iostream>
#include "fido2/Base64Utils.h"
#include "windows.h"
#include <openssl/sha.h>
using namespace std;

void registerWebAuthn(const char* challenge, const char* userId) {

	bool		 rk = false;
	bool		 uv = false;
	fido_cred_t* cred = NULL;
	FidoLogger logger;
	Base64Utils b64Utils;
	int		 coseType = COSE_ES256;
	int		 ext = 0;
	int		 retCode;
	const char* cmdType = "webauthn.create";
	static const unsigned char clientDataHashOld[32] = {
	0xf9, 0x64, 0x57, 0xe7, 0x2d, 0x97, 0xf6, 0xbb,
	0xdd, 0xd7, 0xfb, 0x06, 0x37, 0x62, 0xea, 0x26,
	0x20, 0x44, 0x8e, 0x69, 0x7c, 0x03, 0xf2, 0x31,
	0x2f, 0x99, 0xdc, 0xaf, 0x3e, 0x8a, 0x91, 0x6b,
	};
	// make a (mostly empty) credential
	if ((cred = fido_cred_new()) == NULL) {
		printf("error!");
	}
	
	//initialize logging
	logger.log_init();

	/* coseType */
	retCode = fido_cred_set_type(cred, coseType);
	if (retCode != FIDO_OK)
		logger.log_debug("fido_cred_set_type: %s (0x%x)", fido_strerr(retCode), retCode);

	
	const char* clientData = "{ \"type\": \"webauthn.create\", \"challenge\": \"challengehere\", \"origin\": \"http://localhost\" }";
	unsigned char* clientDataHash;
	clientDataHash = convertToUnsignedAndHash(clientData);
	/*unsigned char* val = new unsigned char[strlen(clientData) + 1];
	strcpy_s((char*)val, strlen(clientData) + 1, clientData);
	unsigned char clientDataHash[32];
	SHA256(val, (sizeof(val)/ sizeof(unsigned char*)), clientDataHash);
	*/

	const char* user_Id = userId;
	unsigned char* userIdHash;
	userIdHash = convertToUnsignedAndHash(user_Id);

	/* client data hash */
	retCode = fido_cred_set_clientdata_hash(cred, clientDataHash, sizeof(clientDataHash));
	if (retCode != FIDO_OK)
		logger.log_debug("fido_cred_set_clientdata_hash: %s (0x%x)",
			fido_strerr(retCode), retCode);

	/* relying party */
	retCode = fido_cred_set_rp(cred, "localhost", "localhost");
	if (retCode != FIDO_OK)
		logger.log_debug("fido_cred_set_rp: %s (0x%x)", fido_strerr(retCode), retCode);

	/* user */
	retCode = fido_cred_set_user(cred, userIdHash, sizeof(userIdHash), "john smith",
		"jsmith", NULL);
	if (retCode != FIDO_OK)
		logger.log_debug("fido_cred_set_user: %s (0x%x)", fido_strerr(retCode), retCode);

	/* extensions */
	retCode = fido_cred_set_extensions(cred, ext);
	if (retCode != FIDO_OK)
		logger.log_debug("fido_cred_set_extensions: %s (0x%x)", fido_strerr(retCode), retCode);

	/* resident key */
	if (rk && (retCode = fido_cred_set_rk(cred, FIDO_OPT_TRUE)) != FIDO_OK)
		logger.log_debug("fido_cred_set_rk: %s (0x%x)", fido_strerr(retCode), retCode);

	/* user verification */
	if (uv && (retCode = fido_cred_set_uv(cred, FIDO_OPT_TRUE)) != FIDO_OK)
		logger.log_debug("fido_cred_set_uv: %s (0x%x)", fido_strerr(retCode), retCode);

	retCode = fido_dev_make_cred(cred);

	/* Some MBS to w_Str stuff (mostly win32)
	int expectedSize = MultiByteToWideChar(CP_UTF8, 0, clientData, -1, NULL, 0);
	LPWSTR convertedUnicodeString = new WCHAR[expectedSize];
	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, clientData, strlen(clientData), convertedUnicodeString, expectedSize);
	const unsigned char* clientdata = convertedUnicodeString;*/
	_getch();

}

unsigned char* convertToUnsignedAndHash(const char* data) {
	//char* clientData = "{ \"type\": \"webauthn.create\", \"challenge\": \"challengehere\", \"origin\": \"http://localhost\" }";
	unsigned char* val = new unsigned char[strlen(data) + 1];
	strcpy_s((char*)val, strlen(data) + 1, data);
	unsigned char hash[32];
	SHA256(val, (sizeof(val) / sizeof(unsigned char*)), hash);
	return hash;
}

