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

ENCRYPT_EXPORTS int gen_aes_encrypted_minutiae(const unsigned char *minutiae, const unsigned char *iv, const unsigned char * key, unsigned char *encrypted_minutiae)
{
	return 0;
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

	/* Print out the binary values */
	for (i = 0; i < 32; i++)
		mbedtls_printf("0x%02X,%s", session_key[i],
		(i + 1) % 4 == 0 ? "\r\n" : " ");

	mbedtls_printf("\n  . Encrypt data with RSA...\n");

	mbedtls_printf("\n  . Seeding the random number generator...");
	fflush(stdout);

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
		mbedtls_printf(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n",
			return_val);
		goto exit;
	}

	mbedtls_printf("\n  . Reading public key from '%s'", rsa_key_path);
	fflush(stdout);

	mbedtls_pk_init(&pk);

	if ((return_val = mbedtls_pk_parse_public_keyfile(&pk, rsa_key_path)) != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		mbedtls_printf(" failed\n  ! mbedtls_pk_parse_public_keyfile returned %d\n\n",
			return_val);
		goto exit;
	}

	/* access to an RSA context inside a PK context */
	rsa = *mbedtls_pk_rsa(pk);

	if (strlen((const char*)session_key) > 100)
	{
		mbedtls_printf(" Input data larger than 100 characters.\n\n");
		goto exit;
	}

	memcpy(input, session_key, strlen((const char*)session_key));

	/*
	* Calculate the RSA encryption of the hash.
	*/
	mbedtls_printf("\n  . Generating the RSA encrypted value\n\n");
	fflush(stdout);

	return_val = mbedtls_rsa_pkcs1_encrypt(&rsa, mbedtls_ctr_drbg_random,
		&ctr_drbg, MBEDTLS_RSA_PUBLIC,
		strlen((const char*)session_key), input, buf);
	if (return_val != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		mbedtls_printf(" failed\n  ! mbedtls_rsa_pkcs1_encrypt returned %d\n\n",
			return_val);
		goto exit;
	}

	/*
	* Write the signature into result-enc.txt
	*/
	if ((err = fopen_s(&f, "result-enc.txt", "wb")) != 0)
	{
		exit_val = MBEDTLS_EXIT_FAILURE;
		mbedtls_printf(" failed\n  ! Could not create %s\n\n", "result-enc.txt");
		goto exit;
	}

	for (i = 0; i < rsa.len; i++)
		mbedtls_fprintf(f, "0x%02X,%s", buf[i],
		(i + 1) % 16 == 0 ? "\r\n" : " ");

	for (i = 0; i < rsa.len; i++)
		mbedtls_printf("0x%02X,%s", buf[i],
		(i + 1) % 16 == 0 ? "\r\n" : " ");

	mbedtls_printf("\n\n");

	memcpy(encrypted_session_key, buf, rsa.len);
	for (i = 0; i < rsa.len; i++)
		mbedtls_printf("0x%02X,%s", *(encrypted_session_key + i),
		(i + 1) % 16 == 0 ? "\r\n" : " ");

	fclose(f);

	mbedtls_printf("\n  . Done (created \"%s\")\n\n", "result-enc.txt");

exit:
	mbedtls_mpi_free(&N); mbedtls_mpi_free(&E);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	mbedtls_rsa_free(&rsa);

	mbedtls_printf("\n  + Press Enter to exit this program.\n");
	fflush(stdout); getchar();

	return (exit_val);
}
