#pragma once


#include <d3dx9.h>
#include "../gameMainSystem/filemanage/tstring_ph.h"
#include <vector>



using namespace std;


class expandedString;


#define DEFAULT_COLOR (0xFFFFFFFF)


class StyleString
{
protected:
	tstring conclete_tstr_;
public:
	const tstring& conclete_tstr() const{return conclete_tstr_;};
public:
	const TCHAR* c_str() const{return conclete_tstr().c_str();};
	int length(){return conclete_tstr().length();};
public:
	StyleString(void);
	//コピーコンストラクタ
	StyleString(const StyleString& str);
	StyleString(const tstring & str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	StyleString(const TCHAR* str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);

public:
	virtual ~StyleString(void);

public:
	//代入
	virtual StyleString&operator =(const StyleString& str);
	virtual StyleString&operator =(const tstring& str);
	virtual StyleString&operator =(const TCHAR* str);
	virtual StyleString&operator =(const int str);
	virtual StyleString&operator =(const double str);

	//文字列を足す
	virtual void addstring(const StyleString& str);
	virtual void addstring(const tstring& str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const TCHAR* str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const int str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const double str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	//書式を指定する
	virtual void addstring(const int figure,const TCHAR* form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const double figure,const TCHAR* form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const int figure,const tstring& form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);
	virtual void addstring(const double figure,const tstring& form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = 0);

	//文字列を足す
	virtual StyleString operator +(const StyleString& bstr);
	virtual StyleString operator +(const tstring& bstr);
	virtual StyleString operator +(const TCHAR* bstr);
	virtual StyleString operator +(const int bstr);
	virtual StyleString operator +(const double bstr);

	//文字列を足す
	virtual StyleString&operator +=(const StyleString& str);
	virtual StyleString&operator +=(const tstring& str);
	virtual StyleString&operator +=(const TCHAR* str);
	virtual StyleString&operator +=(const int str);
	virtual StyleString&operator +=(const double str);

	bool operator==(const StyleString& other) const;
    bool operator!=(const StyleString& other) const;


	virtual void clear();

	vector<int> sector;
	vector<unsigned long> colors;
	vector<D3DXVECTOR2> sizes;
	vector<unsigned> flags;

	//色がデフォルトか
	virtual bool isColorDefault();
	//同色で一致させる
	virtual void setSameColor(unsigned long color);
	//同サイズで一致させる
	virtual void setSameSize(D3DXVECTOR2 size);
	//同サイズで一致させる
	virtual void setSameSize(double sizeX, double sizeY);
	//同フラグで一致させる
	virtual void setSameFlag(unsigned flag);

	vector<expandedString> exportString();

	bool isFlagContent(unsigned int flagMask);

	//改行を空白に置換したString
	StyleString straightString();
};

//addstringの方が高速(高速に行うならaddstringを使え！)
StyleString setStyle(const tstring & str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);
StyleString setStyle(const TCHAR* str, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);
StyleString setStyle(int figure, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);
StyleString setStyle(double figure, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);

//書式を指定する
StyleString setStyle(int figure,const TCHAR* form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);
StyleString setStyle(double figure,const TCHAR* form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);
StyleString setStyle(double figure,const tstring& form, unsigned long color = DEFAULT_COLOR, float sizeX = 1.0f, float sizeY = 1.0f, unsigned flag = NULL);



StyleString operator +( const tstring & fstr, const StyleString & bstr );
StyleString operator +( const TCHAR* fstr, const StyleString & bstr );
StyleString operator +( const int & fstr, const StyleString & bstr );
StyleString operator +( const double & fstr, const StyleString & bstr );


//StyleString operator +( const TCHAR* fstr, const TCHAR* bstr );

//#define STYLES_BOLD (1<<0)
//#define STYLES_ITALIC (1<<1)
#define STYLES_FLAG_MESSAGE_CLEAR (1<<0)
#define STYLES_POSX_FLOOR (20)
#define STYLES_POSX_FLAG (0x800<<STYLES_POSX_FLOOR)
#define STYLES_POSX_MASK (0x7FF<<STYLES_POSX_FLOOR)
#define STYLES_POSY_FLOOR (8)
#define STYLES_POSY_FLAG (0x800<<STYLES_POSY_FLOOR)
#define STYLES_POSY_MASK (0x7FF<<STYLES_POSY_FLOOR)
//00000000 00000000 00000000 00000000

class expandedString:
	public tstring
{
public:
	expandedString(void);
public:
	virtual ~expandedString(void);
	//コピーコンストラクタ
	expandedString(const expandedString& str);

	unsigned long color;
	D3DXVECTOR2 size;
	unsigned long flag;
};



//文字列を分割します。
//２バイト文字に対する保障なし！
//空要素を飛ばします
void split(const tstring& str, const TCHAR* delim, vector<tstring>& output);

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<tstring>>& output);

void split(const tstring& str, const TCHAR* delim, vector<int>& output,const tstring& decodename = tstring(_T("")));

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<int>>& output,const tstring& decodename = tstring(_T("")));

void split_hex(const tstring& str, const TCHAR* delim, vector<int>& output,const tstring& decodename = tstring(_T("")));

void split_hex(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<int>>& output,const tstring& decodename = tstring(_T("")));

void split(const tstring& str, const TCHAR* delim, vector<double>& output,const tstring& decodename = tstring(_T("")));

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<double>>& output,const tstring& decodename = tstring(_T("")));

int tstrtoh(const tstring& str,const tstring& decodename = tstring(_T("")), const int line = 0);
int tstrtoi(const tstring& str,const tstring& decodename = tstring(_T("")), const int line = 0);
double tstrtod(const tstring& str,const tstring& decodename = tstring(_T("")), const int line = 0);


tstring trimstring(const tstring& s);
void trimstring(const tstring& s, tstring & output);
