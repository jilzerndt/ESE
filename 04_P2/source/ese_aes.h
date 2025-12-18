#ifndef _ESE_AES_H_
#define _ESE_AES_H_


/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <stdint.h>
#include <string.h>

/****************************************************************************************
 * Functions
 ***************************************************************************************/

/****************************************************************************************
 uint32_t runAES(char *message, size_t message_length);

 This function shall be used to perform the energy measurement of executing an AES encryption
 or decryption. 
 The function uses a string given through the input pointer *message. The message must 
 have a length of multiples of the AES key length in use, i.e., 
 e.g. 16B in case of AES128 is used. 

 The function returns an error (2) in case the message_length doesn't match this criterion.
 The function returns an error (1) in case the encryption or decryption of the message fails.
 The function returns (0), in case a single encryption and decryption run with AES worked
 without an error. 
 ***************************************************************************************/
uint32_t runAES(char *message, size_t message_length);

#endif  /* _ESE_AES_H_ */
