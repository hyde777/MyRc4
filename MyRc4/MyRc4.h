#pragma once
#ifndef MyRc4_H_
#define MyRc4_H_

/**
* RC4 Encryptor utility for decrypting Strings
* @brief Utility to RC4 encrypt bytes
*/

class MyRc4 {

public:
	void setKey(unsigned char k[], int size);
	void encrypt(unsigned char* in, unsigned char* out, int begin, int ending);
	void encrypt(char* in, char* out, int begin, int ending);

	MyRc4();

protected:
	void ksa(unsigned char * key);
	void swap(unsigned char data[], int i, int j);
	void prga(unsigned char * plaintext, unsigned char * cipher, int begin, int ending);
	void prga(char * plaintext, char * cipher, int begin, int ending);

	unsigned char sbox[256];
	int sizeKey, prgaIndexA, prgaIndexB;
};

#endif /* ARC4_H_ */