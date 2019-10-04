void registerWebAuthn(const char* challenge, const char* userId);
void convertToUnsignedAndHash(const char* data, unsigned char (&hashVal)[32]);