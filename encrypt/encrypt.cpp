// encrypt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "encrypt.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/rsa.h"
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#define SESSION_KEY_LENGTH 32

ENCRYPT_EXPORTS int gen_aes_encrypted_minutiae(const unsigned char *minutiae, const unsigned char *iv, const unsigned char *key, unsigned char *encrypted_minutiae)
{
	int ret;
	mbedtls_aes_context aes;
	unsigned char cbc_iv[16];
	unsigned int i;

	memset(cbc_iv, 0, 16);
	memcpy(cbc_iv, iv, 16);
	memset(encrypted_minutiae, 0, 512);

	mbedtls_aes_init(&aes);

	if ((ret = mbedtls_aes_setkey_enc(&aes, key, 256)) != 0) goto exit;

	if ((ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 512, cbc_iv, minutiae, encrypted_minutiae)) != 0) goto exit;

exit:
	mbedtls_aes_free(&aes);

	return (ret);
}

ENCRYPT_EXPORTS int gen_rsa_encrypted_skey(const unsigned char *session_key, const char *rsa_key_path, unsigned char *encrypted_session_key)
{
	FILE *f;
	errno_t err;
	int return_val, exit_val;
	size_t i;
	mbedtls_pk_context pk;
	mbedtls_rsa_context rsa;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	unsigned char input[1024];
	unsigned char buf[512];
	const char *pers = "rsa_encrypt";
	mbedtls_mpi N, E;

	exit_val = MBEDTLS_EXIT_SUCCESS;

	mbedtls_mpi_init(&N); mbedtls_mpi_init(&E);
	mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	return_val = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
		&entropy, (const unsigned char *)pers,
		strlen(pers));
	if (return_val != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		goto exit;
	}

	mbedtls_pk_init(&pk);

	if ((return_val = mbedtls_pk_parse_public_keyfile(&pk, rsa_key_path)) != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		goto exit;
	}

	/* access to an RSA context inside a PK context */
	rsa = *mbedtls_pk_rsa(pk);

	memcpy(input, session_key, SESSION_KEY_LENGTH);

	/*
	* Calculate the RSA encryption of the hash.
	*/

	return_val = mbedtls_rsa_pkcs1_encrypt(&rsa, mbedtls_ctr_drbg_random,
		&ctr_drbg, MBEDTLS_RSA_PUBLIC,
		SESSION_KEY_LENGTH, input, buf);
	if (return_val != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		goto exit;
	}

	memcpy(encrypted_session_key, buf, rsa.len);

exit:
	mbedtls_mpi_free(&N); mbedtls_mpi_free(&E);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	mbedtls_rsa_free(&rsa);

	return (exit_val);
}

ENCRYPT_EXPORTS int gen_random_bytes(size_t num_of_block, unsigned char *random_bytes)
{
	int i, k, ret;
	mbedtls_entropy_context entropy;
	unsigned char buf[16];
	unsigned char *ptr = random_bytes;

	mbedtls_entropy_init(&entropy);

	for (i = 0, k = num_of_block; i < k; i++)
	{
		ret = mbedtls_entropy_func(&entropy, buf, sizeof(buf));
		if (ret != 0)
		{
			goto cleanup;
		}

		memcpy(ptr, buf, sizeof(buf));
		ptr += sizeof(buf);
	}

	ret = 0;

cleanup:
	mbedtls_entropy_free(&entropy);

	return(ret);
}
