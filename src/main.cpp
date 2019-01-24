/******
	main.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
*******/

#include "./main.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <sstream>
#include <cassert>

using namespace std;

MyRc4::MyRc4()
{
}

void MyRc4::prga(unsigned char* plaintext, unsigned char* cipher, int begin, int ending)
{
	for (int k = begin; k < ending; k++)
	{
		prgaIndexA = (prgaIndexA + 1) % 256;
		prgaIndexB = (prgaIndexB + sbox[prgaIndexA]) % 256;
		swap(sbox, prgaIndexA, prgaIndexB);
		cipher[k] = sbox[(sbox[prgaIndexA] + sbox[prgaIndexB]) % 256] ^ plaintext[k];
	}
}

void MyRc4::prga(char* plaintext, char* cipher, int begin, int ending)
{
	prga(reinterpret_cast<unsigned char *>(plaintext), reinterpret_cast<unsigned char*>(cipher), begin, ending);
}

void MyRc4::encrypt(char* plaintext, char* ciphertext, int begin, int ending)
{
	prga(plaintext, ciphertext, begin, ending);
}

void MyRc4::encrypt(unsigned char* plaintext, unsigned char* ciphertext, int begin, int ending)
{
	prga(plaintext, ciphertext, begin, ending);
}

void MyRc4::swap(unsigned char data[], int i, int j)
{
	unsigned char temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}

void MyRc4::ksa(unsigned char* key)
{
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		j = (j + sbox[i] + key[i % sizeKey]) % 256;
		swap(sbox, i, j);
	}
}

void MyRc4::setKey(unsigned char k[], int size)
{
	prgaIndexA = 0;
	prgaIndexB = 0;
	sizeKey = size;

	for (int i = 0; i < 256; i++)
	{
		sbox[i] = static_cast<unsigned char>(i);
	}

	ksa(k);
}

void rc4Par(unsigned char* key, int sizeOfKey, char* inputContent, char* outputTxt, int beginLimit, int endLimit)
{
	MyRc4 rc4 = MyRc4();
	rc4.setKey(key, sizeOfKey);
	rc4.encrypt(inputContent, outputTxt, beginLimit, endLimit);
}

void useRc4(char* option, unsigned char* key, string inputFile, int nbThreads)
{
	clock_t begin = clock();
	nbThreads = 1;
	char* outputFile;
	int sizeOfKey = strlen(reinterpret_cast<char*>(key));
	int sizeOfInput = inputFile.length();

	char* outputTxt = new char[sizeOfInput];

	for (int i = 0; i < sizeOfInput; i++)
	{
		outputTxt[i] = ' ';
	}
	string optionInfo = ((strcmp(option, "-e")) == 0) ? "encrypted" : "decrypted";

	cout << "Your file is about to be " << optionInfo << endl;
	char* inputTxt = const_cast<char*>(inputFile.c_str());
	int beginLimit = 0, endLimit = 0;

	for (int i = 0; i < nbThreads; i++)
	{
		beginLimit = ((i == 0) ? 0 : (sizeOfInput / nbThreads) * i);
		endLimit = ((i == nbThreads - 1) ? sizeOfInput : (sizeOfInput / nbThreads) * (i + 1));
		thread myThread(rc4Par, key, sizeOfKey, inputTxt, outputTxt, beginLimit, endLimit);
		myThread.join();
	}

	ofstream out(outputFile);
	out << outputTxt;
	out.close();

	clock_t end = clock();

	cout << "Your file got " << optionInfo << endl;
	cout << "Elapsed time = " << (double)end - begin << endl;
}


/**
 * mkdir build
 * cd build
 * cmake ..
 * make
 * */
int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "This RC4 can be used by using these option :" << endl;
		cout << "main {-e|-d} NB_THREADS KEY INPUT_FILE" << endl;
		return -1;
	}

	char* option = argv[1];
	int nbOfThread = atoi(argv[2]);
	unsigned char* key = (unsigned char*)argv[3]; // (unsigned char*)"ESGI";
	char* inputFilePath = (char*)(argv[4]); //reinterpret_cast<_>
	ifstream ifs(inputFilePath);
	string inputFile((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
	char* outputFile = argv[4];

	useRc4(option, key, inputFile, nbOfThread);

	return 0;
}
