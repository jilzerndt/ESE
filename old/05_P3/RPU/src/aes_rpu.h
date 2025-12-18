/****************************************************************************************
 * @file
 * @brief See aes_rpu.c
 ***************************************************************************************/

#ifndef AES_RPU_H
#define AES_RPU_H

/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <stdint.h>

/****************************************************************************************
 * Functions
 ***************************************************************************************/
void AES_RPU_encrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length);
void AES_RPU_decrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length);
int AES_RPU_start(char *firmware);
int AES_RPU_stop(char *firmware);

#endif  /* AES_RPU_H */