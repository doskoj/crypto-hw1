#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include "toydes.h"

int main (int argc, char* argv[])
{
	Toydes t = Toydes();
	//int key = atoi(argv[1])&0x3ff;
	int difftable[16][4] = {0,0,0,0};
	int tmp;
	int in;
	int out;
	std::cout << "Enter input XOR => ";
	std::cin >> in;
	std::cout << "Enter output XOR => ";
	std::cin >> out;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			tmp = t.sbox(i, t.s1)^t.sbox(j, t.s1);
			difftable[i^j][tmp]++;
			if ((tmp == out) && ((i^j) == in)){
				std::cout << "Pair: " << i << ", " << j << std::endl;
			}
		}
	}
	for (int k = 0; k < 16; k++)
	{
		for (int l = 0; l < 4; l++)
		{
			for (int m = 0; m < 1 - (int)log10(difftable[k][l] + 1); m++) std::cout << " ";
			std::cout <<  difftable[k][l] << " ";
		}
		std::cout << std::endl;
	}
}