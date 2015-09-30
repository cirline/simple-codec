#ifndef __S_C_BASE64_H__
#define __S_C_BASE64_H__

unsigned char * base64_encode(const unsigned char * in_code, long len);
unsigned char * base64_decode(unsigned char * ciphertext);

#endif

