// MyRc4.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./MyRc4.h"
#include <iostream>
#include <fstream>

using namespace std;

// we just have to separate input from output here
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

void useRc4(char* option, unsigned char* key, int sizeOfKey)
{
	MyRc4 rc4 = MyRc4();
	rc4.setKey(key, sizeOfKey);
	if (strcmp(option, "-e"))
	{
		cout << "Your file is about to be encrypted" << endl;
		//rc4.encrypt(inputTxt, outputTxt, sizeOfInput);
		cout << "Your file got encrypted" << endl;
	}
	else if (strcmp(option, "-d"))
	{
		cout << "Your file is about to be decrypted" << endl;
		//rc4.encrypt(inputTxt, outputTxt, sizeOfInput);
		cout << "Your file is got decrypted" << endl;
	}
}

int main(int argc, char** argv)
{
	if(argc != 6)
	{
		cout << "This RC4 can be used by using these option" << endl;
		cout << "main {-e|-d} NB_THREADS INPUT_FILE OUTPUT_FILE KEY" << endl;
		return -1;
	}
	char* option = argv[1];
	int nbOfThread = atoi(argv[2]);
	// get the content of files
	const char* inputFile = reinterpret_cast<const char*>(argv[3]);
	const char* outputTxt = argv[4];
	std::ifstream ifs(inputFile);
	const std::string inputContent((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	unsigned char* key = reinterpret_cast<unsigned char*>(argv[5]);
	int sizeOfKey = strlen(reinterpret_cast<char*>(key));
	int sizeOfInput = inputContent.length();
	// rc4 is simetric, we just keep the option for the sake of it
	useRc4(option, key, sizeOfKey);
	return 0;
}
