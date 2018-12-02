// MyRc4.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./MyRc4.h"
#include <iostream>

using namespace std;

void  MyRc4::prga(unsigned char * plaintext, unsigned char * cipher, int size) {
	for (int k = 0;k < size;k++) {
		prgaIndexA = (prgaIndexA + 1) % 256;
		prgaIndexB = (prgaIndexB + sbox[prgaIndexA]) % 256;
		swap(sbox, prgaIndexA, prgaIndexB);
		cipher[k] = sbox[(sbox[prgaIndexA] + sbox[prgaIndexB]) % 256] ^ plaintext[k];
	}
}

void  MyRc4::prga(char * plaintext, char * cipher, int size) {
	prga((unsigned char *)plaintext, (unsigned char *)cipher, size);
}

void MyRc4::encrypt(char * plaintext, char * ciphertext, int size) {
	prga(plaintext, ciphertext, size);
}
void MyRc4::encrypt(unsigned char * plaintext, unsigned char * ciphertext, int size) {
	prga(plaintext, ciphertext, size);
}
MyRc4::MyRc4() {
}
void MyRc4::swap(unsigned char data[], int i, int j) {
	unsigned char temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}
void MyRc4::ksa(unsigned char * key) {
	int j = 0;
	for (int i = 0;i < 256;i++) {
		j = (j + sbox[i] + key[i% sizeKey]) % 256;
		swap(sbox, i, j);
	}
}
void MyRc4::setKey(unsigned char  k[], int size) {
	prgaIndexA = 0;
	prgaIndexB = 0;
	sizeKey = size;
	for (int i = 0;i < 256;i++) {
		sbox[i] = (unsigned char)i;
	}
	ksa(k);
}

int main(int argc, char** argv)
{
	if(argc != 6)
	{
		cout << "This RC4 can be used by using these option" << endl;
		cout << "main {-e|-d} NB_THREADS INPUT_FILE OUTPUT_FILE KEY" << endl;
		return -1;
	}
	int nbOfThread = atoi(argv[2]);
	unsigned char* inputTxt = reinterpret_cast<unsigned char*>(argv[3]);
	char* outputTxt = argv[4];
	unsigned char* key = reinterpret_cast<unsigned char*>(argv[5]);
	int size = 0;

	// rc4 is simetric, we jst keep the option for the sake of it
	MyRc4 rc4 = MyRc4();
	if (strcmp(argv[1], "-e"))
	{
		cout << "Your file is about to be encrypted" << endl;
		//rc4.encrypt(inputTxt, key, size);
		cout << "Your file got encrypted" << endl;
	}
	else if (strcmp(argv[1], "-d"))
	{
		cout << "Your file is about to be decrypted" << endl;
		//rc4.encrypt(inputTxt, key, size);
		cout << "Your file is got decrypted" << endl;
	}
	return 0;
}