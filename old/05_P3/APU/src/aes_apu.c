/****************************************************************************************
 * @file
 * @brief APU-based AES encryption
 *
 * @note 
 *
 * --------------------------------------------------------------------------------------
 * @author  Flavio Felder, felf@zhaw.ch
 * @date	04.09.2023
 ***************************************************************************************/

/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "aes_apu.h"
#include "aes.h"

/****************************************************************************************
 * Variables
 ***************************************************************************************/
static struct AES_ctx ctx;

/****************************************************************************************
 * Global Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief APU-based AES encryption initialization
 * @param key[in]	Key
 * @param iv[in]	Initialization vector
 ***************************************************************************************/
void AES_APU_init_ctx_iv(const uint8_t* key, const uint8_t* iv) {
	AES_init_ctx_iv(&ctx, key, iv);
}

/****************************************************************************************
 * @brief APU-based AES encryption
 * @param buf[in]		Plain text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_APU_encrypt_buffer(uint8_t* buf, size_t length) {
	AES_CBC_encrypt_buffer(&ctx, buf, length);
}

/****************************************************************************************
 * @brief APU-based AES decryption
 * @param buf[in]		Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_APU_decrypt_buffer(uint8_t* buf, size_t length) {
	AES_CBC_decrypt_buffer(&ctx, buf, length);
}
