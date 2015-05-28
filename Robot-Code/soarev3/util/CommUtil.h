/*
 * CommUtil.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 *  Contains useful functions for dealing with communication and networking
 *    Mostly packing and unpacking buffers and dealing with binary
 */

#ifndef COMMUTIL_H_
#define COMMUTIL_H_

#include <vector>
#include <iostream>

typedef std::vector<unsigned int> IntBuffer;
typedef IntBuffer::iterator IntBufferIt;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

using namespace std;

// Print the given unsigned int in binary to standard out
inline void printBinary32(uint i){
	for(int d = 31; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}

// Print the given unsigned short in binary to standard out
inline void printBinary16(ushort i){
	for(int d = 15; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}

// Print the given unsigned character in binary to standard out
inline void printBinary8(uchar i){
	for(int d = 7; d >= 0; d--){
		cout << (i & (1 << d) ? "1" : "0");
	}
	cout << endl;
}


// Fills params with integers extracted from the buffer
//  size is size in bytes
void unpackBuffer(const char* buffer, const uint size, IntBuffer& params);

// returns a chunk of memory (buffer) and its size in bytes
//   (Code that calls this is responsible for the memory)
void packBuffer(IntBuffer& params, char*& buffer, uint& size);

// Given two unsigned shorts, packs them into a single unsigned integer
inline uint packShorts(ushort a, ushort b){
	return (a << 16) | b;
}

// Given an unsigned integer, unpacks it into two unsigned shorts a and b
//   (same order as packShorts)
inline void unpackShorts(uint i, ushort& a, ushort& b){
	a = (i >> 16) & 0xffff;
	b = i & 0xffff;
}

// Given 4 unsigned bytes, packs them into a signel unsigned integer
inline uint packBytes(uchar a, uchar b, uchar c, uchar d){
	return (a << 24) | (b << 16) | (c << 8) | d;
}

// Given a pointer to an array of unsigned characters, packs the first 4
//   into a single unsigned integer
inline uint packBytes(uchar* a){
	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

// Given an unsigned integer i, unpacks it into 4 unsigned bytes
//   (same order as packBytes)
inline void unpackBytes(uint i, uchar& a, uchar& b, uchar& c, uchar& d){
	a = (i >> 24) & 0xff;
	b = (i >> 16) & 0xff;
	c = (i >> 8) & 0xff;
	d = i & 0xff;
}

// Given an unsigned integer i, and a pointer to an unsigned byte array of at least size 4,
//   Unpacks the int and fills the first 4 slots in the array (same order as packBytes)
inline void unpackBytes(uint i, uchar* a){
	a[0] = (i >> 24) & 0xff;
	a[1] = (i >> 16) & 0xff;
	a[2] = (i >> 8) & 0xff;
	a[3] = i & 0xff;
}

#endif /* COMMUTIL_H_ */
