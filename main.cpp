#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "toydes.h"

int main(int argc, char *argv[]) {
	unsigned int in;
	int key;
	unsigned char tmp;
	std::string name;
	Toydes t = Toydes();
	std::cout << "Message => ";
	std::cin >> in;
	/*
	std::ifstream infile;
	infile.open("t.txt", std::ios::binary | std::ios::in);
	if (!infile.is_open())
	{
		std::cout << "Error opening file";
		return -1;
	}
	*/
	std::cout << "Input key => ";
	std::cin >> key;
	key = key&0x3ff;
	/*
	std::ofstream enfile;
	enfile.open("o.txt", std::ios::binary | std::ios::out);
	*/
	std::cout << "Encrypting: " << t.cts(in, 8) << std::endl << "Key: " << t.cts(key, 10) << std::endl;
	unsigned int cyp = t.encryptByte(in, key);
	unsigned int msg = t.decryptByte(cyp, key);
	std::cout << "Encrypted: " << t.cts(cyp, 8) << std::endl << "Decrypted: " << t.cts(msg, 8) << std::endl;
	/*
	while(!infile.eof())
	{
		infile.read(&in, 1);
		if (infile.eof()) break;
		tmp = t.encryptByte(in, key);
		enfile.write(reinterpret_cast<char*>(&tmp), 1);
	}
	infile.close();
	enfile.close();

	std::ifstream cypfile;
	cypfile.open("o.txt", std::ios::binary | std::ios::in);

	std::ofstream defile;
	defile.open("d.txt", std::ios::binary | std::ios::out);

	while(!cypfile.eof())
	{
		cypfile.read(&in, 1);
		if (cypfile.eof()) break;
		tmp = t.decryptByte(in, key);
		defile.write(reinterpret_cast<char*>(&tmp), 1);
	}
	cypfile.close();
	defile.close();
	*/
	return 1;
}