/*	
 * File: BinaryFile.cpp
 * Project: DUtils library
 * Author: Dorian Galvez-Lopez
 * Date: April 2010
 * Description: reads and writes binary files in network byte order.
 *    Manages endianness and data size automatically.
 * License: see the LICENSE.txt file
 *
 */

#include "FileModes.h"
#include "BinaryFile.h"

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
// Microsoft Visual Studio does not ship stdint.h until 2010 version
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int32 uint64_t;
#else
#include <stdint.h>
#endif


using namespace DUtils;

BinaryFile::BinaryFile(void): m_is_little_endian(-1)
{
	setEndianness();
}

BinaryFile::~BinaryFile(void)
{
	Close();
}

void BinaryFile::Close()
{
	if(m_f.is_open()) m_f.close();
}

BinaryFile::BinaryFile(const char *filename, const FILE_MODES mode)
{
	Init(filename, mode);
}

BinaryFile::BinaryFile(const std::string &filename, const FILE_MODES mode)
{
	Init(filename.c_str(), mode);
}

void BinaryFile::Init(const char *filename, const FILE_MODES mode)
{
	m_is_little_endian = -1;
	setEndianness();
	
	if(mode & READ){
		OpenForReading(filename);
	}else if((mode & WRITE) && (mode & APPEND)){
		OpenForAppending(filename);
	}else if(mode & WRITE){
		OpenForWriting(filename);
	}else{
		throw DException("Wrong access mode");
	}
}

void BinaryFile::OpenForReading(const char *filename)
{
	Close();

  m_f.open(filename, std::ios::in | std::ios::binary);
	if(!m_f.is_open()){
    throw DException(std::string("Cannot open ") + filename + " for reading");
	}else{
		m_mode = READ;
	}
}

void BinaryFile::OpenForWriting(const char *filename)
{
	Close();
	
  m_f.open(filename, std::ios::out | std::ios::binary);
	if(!m_f.is_open()){
    throw DException(std::string("Cannot open ") + filename + " for writing");
	}else{
		m_mode = WRITE;
	}
}

void BinaryFile::OpenForAppending(const char *filename)
{
	Close();

  m_f.open(filename, std::ios::out | std::ios::app | std::ios::binary);
	if(!m_f.is_open()){
    throw DException(std::string("Cannot open ") + filename + " for writing at the end");
	}else{
		m_mode = DUtils::FILE_MODES(WRITE | APPEND);
	}
}

void BinaryFile::DiscardBytes(int count)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & READ){
		m_f.ignore(count);
	}else
		throw DException("Wrong access mode");
}

inline bool BinaryFile::Eof()
{
	return(!m_f.is_open() || m_f.eof());
}

unsigned int BinaryFile::BytesRead()
{
	if(m_mode & READ){
		return (unsigned int)m_f.tellg();
	}else
		throw DException("Wrong access mode");
}

BinaryFile& BinaryFile::operator<< (char v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & WRITE){
		m_f.write(&v, 1);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator<< (int v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & WRITE){
		uint32_t w = htonl(v);
		m_f.write((const char *)&w, 4);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator<< (float v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & WRITE){
		hton_f(v, m_aux);
		m_f.write(m_aux, 4);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator<< (double v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & WRITE){
		hton_d(v, m_aux);
		m_f.write(m_aux, 8);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator>>(char &v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & READ){
		m_f.read(&v, 1);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator>>(int &v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & READ){
		m_f.read(m_aux, 4);
		uint32_t *w = ((uint32_t*)&m_aux[0]);
		v = (int)htonl(*w);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator>>(float &v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & READ){
		m_f.read(m_aux, 4);
		v = ntoh_f(m_aux);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}

BinaryFile& BinaryFile::operator>>(double &v)
{
	if(!m_f.is_open()) throw DException("File is not open");

	if(m_mode & READ){
		m_f.read(m_aux, 8);
		v = ntoh_d(m_aux);
	}else
		throw DException("Wrong access mode");
	
	return *this;
}


void BinaryFile::hton_f(float v, char buf[8]) const
{
	unsigned char *w = (unsigned char *)&v;

	// network order is big endian
	if(isLittleEndian()){
		buf[0] = w[3];
		buf[1] = w[2];
		buf[2] = w[1];
		buf[3] = w[0];
	}else{
		buf[0] = w[0];
		buf[1] = w[1];
		buf[2] = w[2];
		buf[3] = w[3];
	}
}

float BinaryFile::ntoh_f(char buf[8]) const
{
	float v;
	unsigned char *w = (unsigned char*)&v;

	// network order is big endian
	if(isLittleEndian()){
		w[3] = buf[0];
		w[2] = buf[1];
		w[1] = buf[2];
		w[0] = buf[3];
	}else{
		w[0] = buf[0];
		w[1] = buf[1];
		w[2] = buf[2];
		w[3] = buf[3];
	}

	return v;
}

void BinaryFile::hton_d(double v, char buf[8]) const
{
	unsigned char *w = (unsigned char *)&v;

	// network order is big endian
	if(isLittleEndian()){
		buf[0] = w[7];
		buf[1] = w[6];
		buf[2] = w[5];
		buf[3] = w[4];
		buf[4] = w[3];
		buf[5] = w[2];
		buf[6] = w[1];
		buf[7] = w[0];
	}else{
		buf[0] = w[0];
		buf[1] = w[1];
		buf[2] = w[2];
		buf[3] = w[3];
		buf[4] = w[4];
		buf[5] = w[5];
		buf[6] = w[6];
		buf[7] = w[7];
	}
}

double BinaryFile::ntoh_d(char buf[8]) const
{
	double v;
	unsigned char *w = (unsigned char*)&v;

	// network order is big endian
	if(isLittleEndian()){
		w[7] = buf[0];
		w[6] = buf[1];
		w[5] = buf[2];
		w[4] = buf[3];
		w[3] = buf[4];
		w[2] = buf[5];
		w[1] = buf[6];
		w[0] = buf[7];
	}else{
		w[0] = buf[0];
		w[1] = buf[1];
		w[2] = buf[2];
		w[3] = buf[3];
		w[4] = buf[4];
		w[5] = buf[5];
		w[6] = buf[6];
		w[7] = buf[7];
	}

	return v;
}

void BinaryFile::setEndianness()
{
	if(m_is_little_endian == -1){
		char SwapTest[2] = { 1, 0 }; 
		short *p = (short *) SwapTest;
		m_is_little_endian = (*p == 1 ? 1 : 0);
	}
}
	
