/****************************************************************************************
 * @file
 * @brief csu-based AES encryption
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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <linux/if_alg.h>
#include <time.h>
#include "aes_csu.h"

/* SETTINGS */
#define AES_KEY_LENGTH 32


/****************************************************************************************
 * Local Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief CSU-based AES encryption
 * @param enc_dir[in]	encryption or decryption (ALG_OP_ENCRYPT, ALG_OP_DECRYPT)
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain/Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
static void AES_CSU_xcrypt_buffer(int enc_dir, const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	int ret;
	int sd;
	int fd;
	/* msghdr structure to send data with settings to kernel */
	struct msghdr msg = {};

	/* buffer for op, iv and aad data length, used for msghdr (msg) */
	char cbuf[CMSG_SPACE(4) + CMSG_SPACE(20)] = {0};
	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);

	/* pointer for elements from cbuf */
	struct cmsghdr *cmsg;


	/* prepare buffer to send data to kernel */
	struct af_alg_iv *ivp;
	struct iovec iov = {buf, length};


	/*  create socket of type AF_ALG */
    // STUDENTS TASK: open a socket
	sd = socket(AF_ALG, SOCK_SEQPACKET, 0);
	
	if (sd == -1) {
		printf("socket failed: %s\n", strerror(errno));
		return;
	}

	/* create structure and select encryption algorithm */
	// STUDENTS TASK: define struct sockaddr_alg with the type 'skcipher' and name 'cbc(aes)'
	struct sockaddr_alg sa = {
		.salg_family = AF_ALG,
		.salg_type   = "skcipher",
		.salg_name   = "cbc(aes)"
	};

	/* bind socket to selected algorithm */
	// STUDENTS TASK: set the crypto algorithm
	ret = bind(sd, (struct sockaddr *) &sa, sizeof(sa));

	if (ret == -1) {
		printf("bind failed: %s\n", strerror(errno));
		close(sd);
		return;
	}

	/* set key for aes encryption */
	// STUDENTS TASK: set the key and key length
	ret = setsockopt(sd, SOL_ALG, ALG_SET_KEY, key, AES_KEY_LENGTH);

	if (ret == -1) {
		printf("setsockopt failed: %s\n", strerror(errno));
		close(sd);
		return;
	}

	/* get a file descriptor for data transfer */
	fd = accept(sd, NULL, 0);
	if (fd == -1) {
		printf("accept failed: %s\n", strerror(errno));
		close(sd);
		return;
	}
	close(sd);


	/* settings for encryption:
	* Header for ancillary data objects in msg_control buffer.
	* Used for additional information with/about a datagram
	* not expressible by flags.  The format is a sequence
	* of message elements headed by cmsghdr structures.
	*/
	cmsg = CMSG_FIRSTHDR(&msg);				// get pointer to first cmsg_header (data object in msg_control buffer 'msg')
	cmsg->cmsg_level = SOL_ALG;
	cmsg->cmsg_type = ALG_SET_OP;
	cmsg->cmsg_len = CMSG_LEN(4);
	*(__u32 *)CMSG_DATA(cmsg) = enc_dir;	// copy message content into data structure
	cmsg = CMSG_NXTHDR(&msg, cmsg);			// get pointer to next cmsg_header
	cmsg->cmsg_level = SOL_ALG;
	cmsg->cmsg_type = ALG_SET_IV;
	cmsg->cmsg_len = CMSG_LEN(20);
	ivp = (void *)CMSG_DATA(cmsg);			// copy iv content into data structure
	ivp->ivlen = 16;

	/* set iv for cbc algorithm */
	memcpy(ivp->iv, iv, 16);
	iov.iov_len = length;
	/* set pointer to plaintext/ciphertext buffer */
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	struct timespec time_start, time_stop;
	clock_gettime(CLOCK_REALTIME, &time_start);
	/* send data to encrypt/decrypt */
	// STUDENTS TASK: send message buffer through socket
	ret = sendmsg(fd, &msg, 0);

	if (ret == -1) {
		printf("sendmsg failed: %s\n", strerror(errno));
		close(fd);
		return;
	}

	/* get encrypted/decrypted data */
	ret = read(fd, buf, length);
	if (ret == -1) {
		printf("read failed: %s\n", strerror(errno));
		close(fd);
		return;
	}

	clock_gettime(CLOCK_REALTIME, &time_stop);
	float time = (time_stop.tv_sec + time_stop.tv_nsec*1e-9)
         - (time_start.tv_sec + time_start.tv_nsec*1e-9);

	printf("Time taken: %f seconds\n", time);

	close(fd);
}


/****************************************************************************************
 * Global Functions
 ***************************************************************************************/

/****************************************************************************************
 * @brief CSU-based AES encryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Plain text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_CSU_encrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_CSU_xcrypt_buffer(ALG_OP_ENCRYPT, key, iv, buf, length);
}

/****************************************************************************************
 * @brief CSU-based AES decryption
 * @param key[in]		Key
 * @param iv[in]		Initialization vector
 * @param buf[in/out]	Cipher text
 * @param length[in]	Length of text (must be divisible by 16byte)
 ***************************************************************************************/
void AES_CSU_decrypt_buffer(const uint8_t* key, const uint8_t* iv, uint8_t* buf, size_t length) {
	AES_CSU_xcrypt_buffer(ALG_OP_DECRYPT, key, iv, buf, length);
}
