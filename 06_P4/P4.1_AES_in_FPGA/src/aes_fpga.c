/****************************************************************************************
 * @file
 * @brief FPGA-based AES encryption
 *
 * @note Usage sequence:
 * 1. Load the key to be used into the key registers.
 * 2. Set the key length and encryption/decryption. Currently, only 256-bit keys are supported.
 * 3. Initialize key expansion.
 * 4. Wait for the READY bit in the status register.
 * 5. Write the IV to the IV registers.
 * 6. Write the cleartext (or ciphertext) block to the block registers.
 * 7. Start block processing by writing setting the NEXT bit in the control register.
 * 8. Wait for the VALID bit in the status register.
 * 9. Read out the ciphertext block from the result registers.
 *
 * --------------------------------------------------------------------------------------
 * @author  Flavio Felder, felf@zhaw.ch
 * @author  Tobias Welti, welo@zhaw.ch
 * @date	05.11.2023
 ***************************************************************************************/

/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "aes_fpga.h"

/****************************************************************************************
 * Defines
 ***************************************************************************************/
#define AES_BASE_ADDR		(0x80020000)	//system.dts apb@80020000

#define AES_CTRL_REG			(0x08)			//control register
#define AES_STATUS_REG			(0x09)			//status register
#define AES_CONFIG_REG			(0x0a)			//config register
#define AES_KEY_REG(X)			(0x10+X)		//..0x17 key register for each 32bit data
#define AES_BLOCK_REG(X)		(0x20+X)		//..0x23 block register
#define AES_RESULT_REG(X)		(0x30+X)		//..0x33 result register
#define AES_IV_REG(X)			(0x40+X)		//..0x43 initialization vector register

#define CTRL_INIT_BIT			(0)
#define CTRL_NEXT_BIT			(1)
#define CONFIG_ENCDEC_BIT		(0)
#define CONFIG_KEYLEN_BIT		(1)
#define STATUS_READY_BIT		(0)
#define STATUS_VALID_BIT		(1)

#define AES_128_BIT_KEY			(0)
#define AES_256_BIT_KEY			(1)
#define AES_ENC					(1)
#define AES_DEC					(0)

#define KEY_SIZE				(32)
#define IV_SIZE					(16)

#define DATA_BUS_SIZE_IN_BITS	(32)
#define CHAR_SIZE_IN_BITS		(8)
#define DATA_BUS_SIZE			(DATA_BUS_SIZE_IN_BITS/CHAR_SIZE_IN_BITS)

static void *virtual_aes_base;

/****************************************************************************************
 * Local Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief engine-based AES encryption
 * @param enc[in]		Set 1 for encryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain/Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
static void AES_FPGA_xcrypt_buffer(int enc, const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	int  m_mfd;

	// Open Memory as a virtual file
    if ((m_mfd = open("/dev/mem", O_RDWR)) < 0) {
        printf("FAILED open /dev/mem\n");
		return;
    }
	
	// Request a pointer for access to the AES region
    virtual_aes_base = (void *)mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ|PROT_WRITE, MAP_SHARED, m_mfd, AES_BASE_ADDR);
	close(m_mfd);
	
    if (virtual_aes_base == MAP_FAILED) {
        printf("FAILED virtual_aes_base\n");
		return;
    }

	// Registers are 32 bit, cast the pointer accordingly
	uint32_t *aes_cbc_reg = (uint32_t *) (virtual_aes_base);

	// APB is word oriented, cast function parameters
	uint32_t *key_word = (uint32_t *)(key);
	uint32_t *iv_word = (uint32_t *)(iv);
	uint32_t *buf_word = (uint32_t *)(buf);
	size_t length_word = length >> 2;

	// Load the key
	for (uint32_t i = 0; i < KEY_SIZE/DATA_BUS_SIZE; i++) {
		aes_cbc_reg[AES_KEY_REG(i)] = key_word[i];
		/* Kommentar an die Aufmerksamen: der Schlüssel wird in main.c byteweise
		definiert, und zwar so, dass key[0] dem höchsten Byte entspricht, das an 
		den AES-Block übergeben werden muss. */
	}

	// set key length and select encryption or decryption operation in the configuration register
	// register is write-only, can't read and modify!
	// STUDENTS TASK
	uint32_t config = 0;

    // Encrypt / Decrypt
    if (enc == AES_ENC) {
        config |= (1u << CONFIG_ENCDEC_BIT);    // 1 = Encrypt
    } else {
        // 0 = Decrypt => Bit bleibt 0
    }

    config |= (AES_256_BIT_KEY << CONFIG_KEYLEN_BIT);

    aes_cbc_reg[AES_CONFIG_REG] = config;

	// initialize key expansion
	// STUDENTS TASK
	aes_cbc_reg[AES_CTRL_REG] = (1u << CTRL_INIT_BIT);

	// Auf Ready-Bit im Status-Register warten
    while ( (aes_cbc_reg[AES_STATUS_REG] & (1u << STATUS_READY_BIT)) == 0 ) {
        // busy wait
    }

	// Write the plaintext (or ciphertext) block to the block registers
	if (length > 16) {
		printf("FAILED text too long: %li\n", length);
		length = 16;
	}
	// STUDENTS TASK
	for (uint32_t i = 0; i < (16u / DATA_BUS_SIZE); i++) {
        aes_cbc_reg[AES_BLOCK_REG(i)] = buf_word[i];
    }

	// Write the IV to the IV registers
	// STUDENTS TASK
	for (uint32_t i = 0; i < (IV_SIZE / DATA_BUS_SIZE); i++) {
        aes_cbc_reg[AES_IV_REG(i)] = iv_word[i];
    }

	// Start block processing
	// STUDENTS TASK
	aes_cbc_reg[AES_CTRL_REG] = (1u << CTRL_NEXT_BIT);

	// wait for valid flag
	// STUDENTS TASK
	while ( (aes_cbc_reg[AES_STATUS_REG] & (1u << STATUS_VALID_BIT)) == 0 ) {
        // busy wait
    }

	//Read out the ciphertext block from the result registers
	// STUDENTS TASK
	for (uint32_t i = 0; i < (16u / DATA_BUS_SIZE); i++) {
        buf_word[i] = aes_cbc_reg[AES_RESULT_REG(i)];
    }
}

/****************************************************************************************
 * Global Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief FPGA-based AES encryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_FPGA_encrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_FPGA_xcrypt_buffer(AES_ENC, key, iv, buf, length);
}

/****************************************************************************************
 * @brief FPGA-based AES decryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_FPGA_decrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_FPGA_xcrypt_buffer(AES_DEC, key, iv, buf, length);
}
