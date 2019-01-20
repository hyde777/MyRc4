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

MyRc4::MyRc4() {}

// we just have to separate input from output here
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

void MyRc4::encrypt(unsigned char* plaintext, unsigned char * ciphertext, int begin, int ending) 
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

void useRc4(char* option, unsigned char* key, string inputFile, const char* outputFile, int nbThreads) 
{
	clock_t begin = clock();

	const int sizeOfKey = strlen(reinterpret_cast<char*>(key));
	const int sizeOfInput = inputFile.length();
	char* outputTxt = new char[sizeOfInput];

	for (int i = 0; i < sizeOfInput; i++)
	{
		outputTxt[i] = ' ';
	}
	string optionInfo;

	if (strcmp(option, "-e") == 0)
	{
		optionInfo = "encrypted";
	}
	else if (strcmp(option, "-d") == 0)
	{
		optionInfo = "decrypted";
	}
	else 
	{
		cout << "Option error... {-e|-d}" << endl;
		return;
	}	

	cout << "Your file is about to be " << optionInfo << endl;
	char* inputTxt = const_cast<char*>(inputFile.c_str());
	int beginLimit = 0, endLimit = 0;

	// BUG: Ne crypte que la 1ere ligne et la portion diviser par le nombre de thread...
	for (int i = 0; i < nbThreads; i++) 
	{
		beginLimit = ((i == 0) ? 0 : (sizeOfInput / nbThreads) * i);
		endLimit = ((i == nbThreads - 1) ? sizeOfInput : (sizeOfInput / nbThreads) * (i + 1));
		thread myThread(rc4Par, key, sizeOfKey, inputTxt, outputTxt, beginLimit, endLimit);
		myThread.join();
	}

	cout << "Your file got " << optionInfo << endl;

	ofstream out(outputFile + optionInfo);
	out << outputTxt;
	out.close();

	clock_t end = clock();

	double elapsed = end - begin;
	cout << "Elapsed time = " << elapsed << endl;
}

//-e 2 .\doc\testfile.txt .\doc\testfile.rc4.txt
//-d 2 .\doc\testfile.rc4.txt .\doc\testfile.rc4.decrypt.txt
int main(int argc, char** argv) 
{
	if(argc != 5) 
	{
		cout << "This RC4 can be used by using these option" << endl;
		cout << "main {-e|-d} NB_THREADS INPUT_FILE OUTPUT_FILE" << endl;
		return -1;
	}

	// get the option
	char* option = argv[1];

	// get nb thread
	int nbOfThread = atoi(argv[2]);

	// get input file name
	const char* inputFilePath = reinterpret_cast<const char*>(argv[3]);
	ifstream ifs(inputFilePath);
	const string inputFile((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

	// get output file name
	const char* outputFile = argv[4];

	// the constant key
	unsigned char* key = (unsigned char*)"ESGI";

	// rc4 is simetric, we just keep the option for the sake of it
	useRc4(option, key, inputFile, outputFile, nbOfThread);

	return 0;
}
