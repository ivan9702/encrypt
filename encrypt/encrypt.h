// encrypt.h - Contains declarations of encryption functions
#pragma once

#ifdef ENCRYPT_EXPORTS
#define ENCRYPT_EXPORTS __declspec(dllexport)
#else
#define ENCRYPT_EXPORTS __declspec(dllimport)
#endif

// Use mbedTLS.lib to implement encryption function in encrypt.dll

// Implement AES-256-CBC encryption
extern "C" ENCRYPT_EXPORTS int gen_aes_encrypted_minutiae(const unsigned char *minutiae,
	const unsigned char *iv, const unsigned char *key, unsigned char *encrypted_minutiae);

// Implement RSA encryption
extern "C" ENCRYPT_EXPORTS int gen_rsa_encrypted_skey(const unsigned char *session_key, 
	const char *rsa_key_path, unsigned char *encrypted_session_key);