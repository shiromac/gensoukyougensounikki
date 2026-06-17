#pragma once


typedef char SByte;
typedef unsigned char UByte;
//tstring’è‹`
#include <tchar.h>
#include <string>
typedef std::basic_string<TCHAR> tstring;

//ioftstream’è‹`
#include <fstream>
#include <ios>
#ifdef __EMSCRIPTEN__
typedef ggn_tofstream oftstream;
typedef ggn_tifstream iftstream;
#else
typedef std::basic_ofstream<TCHAR> oftstream;
typedef std::basic_ifstream<TCHAR> iftstream;
#endif
