// encrypt.h - Contains declarations of encryption functions
#pragma once

#ifdef ENCRYPT_EXPORTS
#define ENCRYPT_EXPORTS __declspec(dllexport)
#else
#define ENCRYPT_EXPORTS __declspec(dllimport)
#endif

// Use mbedTLS.lib to implement encryption function in encrypt.dll

/**
* \brief          Implement AES-256-CBC encryption
*                 Refer to the 'mbedtls_aes_*' APIs in
*                 '\mbedtls\library\aes.c'
*                 '\mbedtls\include\mbedtls\aes.h'
*
* \param minutiae            Pointer to the minutiae binary with 512 bytes length
* \param iv                  Initial vector for AES-256-CBC algorithm
* \param key                 Pointer to AES-256 key (key length: 256 bits) 
* \param encrypted_minutiae  Pointer to the encrypted minutiae
*
* \return         0 on success, or 1 on failure.
*/
extern "C" ENCRYPT_EXPORTS int gen_aes_encrypted_minutiae(const unsigned char *minutiae,
	const unsigned char *iv, const unsigned char *key, unsigned char *encrypted_minutiae);

/**
* \brief          Implement RSA encryption
*                 Refer to
*                 '\mbedtls\programs\pkey\rsa_encrypt.c' - for RSAES-PKCS1-v1_5 operation
*                 '\mbedtls\programs\pkey\pk_encrypt.c'  - for extracting rsa context from PEM-formatted public key
*
* \param session_key            Pointer to AES-256 key (key length: 256 bits)
* \param rsa_key_path           Path to RSA public key file
*                               PEM format,
*                               starts with '-----BEGIN PUBLIC KEY-----',
*                               ends with '-----END PUBLIC KEY-----'
* \param encrypted_session_key  Pointer to encryption result
*
* \return         0 on success, or 1 on failure.
*/
extern "C" ENCRYPT_EXPORTS int gen_rsa_encrypted_skey(const unsigned char *session_key, 
	const char *rsa_key_path, unsigned char *encrypted_session_key);

/**
* \brief          Generate random bytes for iv or session key
*                 Refer to
*                 '\mbedtls\programs\random\gen_entropy.c'
*
*                 Total # of bytes = num_of_block * (16 bytes per block)
*                 e.g., 2 blocks * 16 bytes per block = 32 bytes
*
* \param num_of_block     Expected number of block
* \param random_bytes     Pointer to the content of expected random bytes
*
* \return         0 on success, or 1 on failure.
*/
extern "C" ENCRYPT_EXPORTS int gen_random_bytes(size_t num_of_block, unsigned char *random_bytes);