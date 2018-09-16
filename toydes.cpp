#include <string>
#include <vector>
#include "toydes.h"

std::string Toydes::cts(unsigned int c, unsigned int length) {
	std::string s = std::string(length, '0');
	for (unsigned int i = 0; i < length; i++) {
		if ((c>>i)&1) s[length-1-i] = '1';
	}
	return s;
}

unsigned int Toydes::sbox(unsigned int in, unsigned int s[4][4]) {
	unsigned int tmp1 = ((in & 0x8) >> 2) + (in & 0x1);
	unsigned int tmp2 = (in & 0x6) >> 1;
	return s[tmp1][tmp2];
}

unsigned int Toydes::p4t4(unsigned int in){
	unsigned int out = 0;
	std::vector<unsigned int> k = {2,4,3,1};
	for (unsigned int i = 0; i < 4; i++) {
		out|=((in>>(4-k[i]))&1)<<(3-i);
	}
	return out;
}

unsigned int Toydes::r4t4(unsigned int in){
	unsigned int out = 0;
	std::vector<unsigned int> k = {4,1,3,2};
	for (unsigned int i = 0; i < 4; i++) {
		out|=((in>>(4-k[i]))&1)<<(3-i);
	}
	return out;
}

unsigned int Toydes::p4t8(unsigned int in) {
	unsigned int out = 0;
	std::vector<unsigned int> k = {4,1,2,3,2,3,4,1};
	for (unsigned int i = 0; i < 8; i++) {
		out|=((in>>(4-k[i]))&1)<<(7-i);
	}
	return out;
}

unsigned int Toydes::p8t8(unsigned int in) {
	unsigned int out = 0;
	std::vector<unsigned int> k = {2,6,3,1,4,8,5,7};
	for (unsigned int i = 0; i < 8; i++) {
		out|=((in>>(8-k[i]))&1)<<(7-i);
	}
	return out;
}

unsigned int Toydes::r8t8(unsigned int in) {
	unsigned int out = 0;
	std::vector<unsigned int> k = {4,1,3,5,7,2,8,6};
	for (unsigned int i = 0; i < 8; i++) {
		out|=((in>>(8-k[i]))&1)<<(7-i);
	}
	return out;
}

unsigned int Toydes::p10t8(unsigned int in) {
	unsigned int out = 0;
	std::vector<unsigned int> k = {6,3,7,4,8,5,10,9};
	for (unsigned int i = 0; i < 8; i++) {
		out|=((in>>(10-k[i]))&1)<<(7-i);
	}
	return out;
}

unsigned int Toydes::p10t10(unsigned int in) {
	int out = 0;
	std::vector<unsigned int> k = {3,5,2,7,4,10,1,9,8,6};
	for (unsigned int i = 0; i < 10; i++) {
		out|=((in>>(10-k[i]))&1)<<(9-i);
	}
	return out;
}

unsigned int Toydes::f(unsigned int in, unsigned int k) {
	unsigned int out = p4t8(in);
	out = out^k;
	unsigned int l = (out>>4)&0x0f;
	unsigned int r = out&0x0f;
	l = sbox(l, s1);
	r = sbox(r, s2);
	out = r + ((l&0x03)<<2);
	return p4t4(out);
}

std::vector<unsigned int> Toydes::keyman(unsigned int key){
	unsigned int k = p10t10(key);
	unsigned int kl = (k>>5)&0x1f;
	unsigned int kr = k&0x1f;
	kl = ((kl<<1)&0x1e) + ((kl>>4)&0x1);
	kr = ((kr<<1)&0x1e) + ((kr>>4)&0x1);
	unsigned int k1 = (kl<<5) + kr;
	k1 = p10t8(k1);
	kl = ((kl<<1)&0x1e) + ((kl>>4)&0x1);
	kr = ((kr<<1)&0x1e) + ((kr>>4)&0x1);
	unsigned int k2 = (kl<<5) + kr;
	k2 = p10t8(k2);
	std::vector<unsigned int> keys = {k1, k2};
	return keys;
}

std::vector<unsigned int> Toydes::r(std::vector<unsigned int> k) {
	std::vector<unsigned int> r(k.size());
	for (unsigned int i = 0; i < k.size(); i++) {
		r[k[i]-1] = i+1;
	}
	return r;
}

unsigned char Toydes::encryptByte(unsigned int msg, unsigned int key) {
	unsigned int cyp = p8t8(msg);
	std::vector<unsigned int> keys = keyman(key);
	unsigned int l0 = (cyp>>4)&0x0f;
	unsigned int r0 = cyp&0x0f;
	unsigned int l1 = l0^f(r0, keys[0]);
	unsigned int r1 = f(l1, keys[1])^r0;
	cyp = r1 + (l1<<4);
	cyp = r8t8(cyp);
	return cyp;
}

unsigned char Toydes::decryptByte(unsigned int cyp, unsigned int key) {
	unsigned int msg = p8t8(cyp);
	std::vector<unsigned int> keys = keyman(key);
	unsigned int l1 = (msg>>4)&0x0f;
	unsigned int r1 = msg&0x0f;
	unsigned int r0 = r1^f(l1, keys[1]);
	unsigned int l0 = f(r0, keys[0])^l1;
	msg = r0 + (l0<<4);
	msg = r8t8(msg);
	return msg;
}