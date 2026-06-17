#include "StyleString.h"
#include "debug.h"

StyleString::StyleString(void)
{

}

StyleString::~StyleString(void)
{

}

StyleString::StyleString(const tstring & str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	int length = str.length();
	conclete_tstr_.append(str);
	sector.push_back(length);
	colors.push_back(color);
	cRenderVector2 v(sizeX,sizeY);
	sizes.push_back(v);
	flags.push_back(flag);
}

//コピーコンストラクタ
StyleString::StyleString(const StyleString& str)
{
	conclete_tstr_.assign(str.conclete_tstr());
	sector = str.sector;
	colors = str.colors;
	sizes = str.sizes;
	flags = str.flags;
}
StyleString::StyleString(const TCHAR* str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(str, color, sizeX, sizeY, flag);
}



StyleString&StyleString::operator =(const StyleString& str)
{
	conclete_tstr_.assign(str.conclete_tstr());
	sector = str.sector;
	colors = str.colors;
	sizes = str.sizes;
	flags = str.flags;
	return *this;
}
StyleString&StyleString::operator =(const tstring& str)
{
	clear();
	addstring(str);
	return *this;
}
StyleString&StyleString::operator =(const TCHAR* str)
{
	clear();
	addstring((tstring)str);
	return *this;
}
StyleString&StyleString::operator =(const int str)
{
	clear();
	addstring(str);
	return *this;
}
StyleString&StyleString::operator =(const double str)
{
	clear();
	addstring(str);
	return *this;
}

StyleString&StyleString::operator +=(const StyleString& str)
{
	addstring(str);
	return *this;
}

StyleString&StyleString::operator +=(const tstring& str)
{
	addstring(str);
	return *this;
}
StyleString&StyleString::operator +=(const TCHAR* str)
{
	addstring(str);
	return *this;
}
StyleString&StyleString::operator +=(const int str)
{
	addstring(str);
	return *this;
}
StyleString&StyleString::operator +=(const double str)
{
	addstring(str);
	return *this;
}

//文字列をくわえる
void StyleString::addstring(const tstring& str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	int length = str.length();
	conclete_tstr_.append(str);
	sector.push_back(length);
	colors.push_back(color);
	cRenderVector2 v(sizeX,sizeY);
	sizes.push_back(v);
	flags.push_back(flag);
}
void StyleString::addstring(const StyleString& str)
{
	conclete_tstr_.append(str.conclete_tstr());
	sector.insert(sector.end(),str.sector.begin(),str.sector.end());
	colors.insert(colors.end(),str.colors.begin(),str.colors.end());
	sizes.insert(sizes.end(),str.sizes.begin(),str.sizes.end());
	flags.insert(flags.end(),str.flags.begin(),str.flags.end());
}
void StyleString::addstring(const TCHAR* str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring((tstring)str, color, sizeX, sizeY, flag);
}
void StyleString::addstring(const int str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, color, sizeX, sizeY, flag));
}
void StyleString::addstring(const double str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, color, sizeX, sizeY, flag));
}
//書式を指定する
void StyleString::addstring(const int str, const TCHAR* form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, form, color, sizeX, sizeY, flag));
}
void StyleString::addstring(const double str, const TCHAR* form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, form, color, sizeX, sizeY, flag));
}
void StyleString::addstring(const int str, const tstring& form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, form.c_str(), color, sizeX, sizeY, flag));
}
void StyleString::addstring(const double str, const tstring& form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	addstring(setStyle(str, form.c_str(), color, sizeX, sizeY, flag));
}

StyleString StyleString::operator +(const StyleString& bstr)
{
	StyleString sstr = *this;
	sstr.addstring(bstr);
    return sstr;
}


StyleString StyleString::operator +(const tstring& bstr)
{
	StyleString sstr = *this;
	sstr.addstring(bstr);
    return sstr;
}

StyleString StyleString::operator +(const TCHAR* bstr)
{
	StyleString sstr = *this;
	sstr.addstring(bstr);
    return sstr;
}
StyleString StyleString::operator +(const int bstr)
{
	StyleString sstr = *this;
	sstr.addstring(bstr);
    return sstr;
}
StyleString StyleString::operator +(const double bstr)
{
	StyleString sstr = *this;
	sstr.addstring(bstr);
    return sstr;
}

bool StyleString::operator==(const StyleString& other) const
{
	return conclete_tstr() == other.conclete_tstr();
}
bool StyleString::operator!=(const StyleString& other) const
{
	return conclete_tstr() != other.conclete_tstr();
}


StyleString operator +( const tstring & fstr, const StyleString & bstr )
{
	StyleString sstr;
	sstr = fstr;
	sstr.addstring(bstr);
    return sstr;
}
StyleString operator +( const TCHAR* fstr, const StyleString & bstr )
{
	StyleString sstr;
	sstr = fstr;
	sstr.addstring(bstr);
    return sstr;
}
StyleString operator +( const int & fstr, const StyleString & bstr )
{
	StyleString sstr;
	sstr = fstr;
	sstr.addstring(bstr);
    return sstr;
}
StyleString operator +( const double & fstr, const StyleString & bstr )
{
	StyleString sstr;
	sstr = fstr;
	sstr.addstring(bstr);
    return sstr;
}

/*//クラスじゃないから無理
StyleString operator +( const TCHAR* fstr, const TCHAR* bstr )
{
	StyleString sstr;
	sstr = fstr;
	sstr.addstring(bstr);
    return sstr;
}
*/

void StyleString::clear()
{
	conclete_tstr_.erase();
	sector.clear();
	colors.clear();
	sizes.clear();
	flags.clear();
}

vector<expandedString> StyleString::exportString()
{
	vector<expandedString> estrv;
	expandedString str;
	int i,length = 0;
	for(i=0; i < sector.size() ;i++)
	{
		str.assign(conclete_tstr(),length,sector[i]);
		str.size = sizes[i];
		str.color = colors[i];
		str.flag = flags[i];
		if(str.flag)
		{
			OnAssert(_T("test"),0,true);
		}

		estrv.push_back(str);

		length += sector[i];
	}

    return estrv;
}

bool StyleString::isFlagContent(unsigned int flagMask)
{
	bool result = false;
	int i, size = flags.size();
	for(i=0; i < size ;i++)
	{
		if(flags[i] & flagMask)
		{
			return true;
		}
	}
	return false;
}

StyleString StyleString::straightString()
{
	StyleString copyString(*this);
	int i,size = (int)copyString.conclete_tstr_.size();
	for(i=0;i<size;i++)
	{
		if(copyString.conclete_tstr_[i] == _T('\n'))
		{
			copyString.conclete_tstr_[i] = _T(' ');
		}
	}
	return copyString;
}

StyleString setStyle(const tstring & str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	StyleString sstr(str, color, sizeX, sizeY, flag);
	return sstr;
}
StyleString setStyle(const TCHAR* str, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	StyleString sstr((tstring)str, color, sizeX, sizeY, flag);
	return sstr;
}
StyleString setStyle(int figure, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	TCHAR t[32];
	_stprintf(t,_T("%d"),figure);
	StyleString sstr(t, color, sizeX, sizeY, flag);
	return sstr;
}

StyleString setStyle(double figure, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	TCHAR t[128];
	_stprintf_s(t,128,_T("%f"),figure);
	StyleString sstr(t, color, sizeX, sizeY, flag);
	return sstr;
}

//書式を指定する
StyleString setStyle(int figure,const TCHAR* form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	TCHAR t[32];
	_stprintf(t,form,figure);
	StyleString sstr(t, color, sizeX, sizeY, flag);
	return sstr;
}

//書式を指定する
StyleString setStyle(double figure,const TCHAR* form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	TCHAR t[128];
	_stprintf_s(t,128,form,figure);
	StyleString sstr(t, color, sizeX, sizeY, flag);
	return sstr;
}

//書式を指定する
StyleString setStyle(double figure,const tstring& form, unsigned long color, float sizeX, float sizeY, unsigned flag)
{
	TCHAR t[128];
	_stprintf_s(t,128,form.c_str(),figure);
	StyleString sstr(t, color, sizeX, sizeY, flag);
	return sstr;
}


//色がデフォルトか
bool StyleString::isColorDefault()
{
	int i;
	for(i=0;i<colors.size();i++)
	{
		if(colors[i] != DEFAULT_COLOR) return false;
	}
	return true;
}
//同色で一致させる
void StyleString::setSameColor(unsigned long color)
{
	int i;
	for(i=0;i<colors.size();i++)
	{
		colors[i] = color;
	}
}
//同サイズで一致させる
void StyleString::setSameSize(cRenderVector2 size)
{
	int i;
	for(i=0;i<sizes.size();i++)
	{
		sizes[i] = size;
	}
}
//同サイズで一致させる
void StyleString::setSameSize(double sizeX, double sizeY)
{
	int i;
	for(i=0;i<sizes.size();i++)
	{
		sizes[i] = cRenderVector2(sizeX, sizeY);
	}
}
//同フラグで一致させる
void StyleString::setSameFlag(unsigned flag)
{
	int i;
	for(i=0;i<flags.size();i++)
	{
		flags[i] = flag;
	}
}


//----------------------------------------
//expandedString
//----------------------------------------

expandedString::expandedString(void)
{
	color = DEFAULT_COLOR;
	size.x = 1;
	size.y = 1;
	flag = 0;
}

expandedString::~expandedString(void)
{
}

expandedString::expandedString(const expandedString& str)
{
	assign(str);
	color = str.color;
	size = str.size;
	flag = str.flag;
}

//文字列を分割します。
//２バイト文字に対する保障なし！
//空要素を飛ばしません！
void split(const tstring& str, const TCHAR* delim, vector<tstring>& output)
{

	output.clear();
    int cutAt = 0;//現在
	int bcutAt = 0;//一手前

    while( (cutAt = str.find_first_of(delim, bcutAt)) != str.npos )
    {
        //if(bcutAt != cutAt)
        //{
			//output.resize(output.size()+1);
			//output.back().assign(str, bcutAt, cutAt-bcutAt);
			output.push_back(str.substr(bcutAt, cutAt-bcutAt));
        //}
        bcutAt = cutAt+1;
    }

	if(bcutAt != str.length())
    {
		//output.resize(output.size()+1);
		//output.back().assign(str, bcutAt, str.length()-bcutAt);
		output.push_back(str.substr(bcutAt, str.length()-bcutAt));
    }

	return;
}

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<tstring>>& output)
{
	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		vector<tstring> strout;
		split(vstr[i], delim, strout);
		output.push_back(strout);
	}
}


void split(const tstring& str, const TCHAR* delim, vector<int>& output, const tstring& decodename)
{
	vector<tstring> vstr;
	split(str,delim,vstr);

	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		int res = 0;
		TCHAR* e;
		res = tstrtoi(vstr[i],decodename,i);

		output.push_back(res);
	}

}

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<int>>& output, const tstring& decodename)
{
	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		vector<int> iout;
		output.push_back(iout);
		split(vstr[i], delim, output.back(), decodename);
	}
}
void split_hex(const tstring& str, const TCHAR* delim, vector<int>& output, const tstring& decodename)
{
	vector<tstring> vstr;
	split(str,delim,vstr);

	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		int res = 0;
		TCHAR* e;
		res = tstrtoh(vstr[i],decodename,i);

		output.push_back(res);
	}

}

void split_hex(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<int>>& output, const tstring& decodename)
{
	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		vector<int> iout;
		output.push_back(iout);
		split_hex(vstr[i], delim, output.back(), decodename);
	}
}

void split(const tstring& str, const TCHAR* delim, vector<double>& output, const tstring& decodename)
{
	vector<tstring> vstr;
	split(str,delim,vstr);

	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		double res = 0;
		TCHAR* e;
		res = tstrtod(vstr[i], decodename, i);
		output.push_back(res);
	}

}

void split(const vector<tstring>& vstr, const TCHAR* delim, vector<vector<double>>& output, const tstring& decodename)
{
	output.clear();

	int i;
	int size = vstr.size();
	for(i=0;i<size;i++)
	{
		vector<double> dout;
		split(vstr[i], delim, dout, decodename);
		output.push_back(dout);
	}
}
int tstrtoh(const tstring& str,const tstring& decodename, const int line)
{
	int res = 0;
	TCHAR* e;
	res = _tcstol(str.c_str(),&e,16);
	OnAssert(decodename.c_str(), line, (str.length() != 0) ,_T(" 空欄が設定されています。<error> Blank space"));
	OnAssert(decodename.c_str(), line, (_tcslen(e) == 0) || ( !(res == 0) || (str == _T("0")) || (str == _T("00")))
		,_T(" 解釈不能な数値 が設定されています。空欄や余分な文字が入っていないかチェックしてください。<error> cannot understand and check extra space or letters: "), str.c_str() );

	return res;
}
int tstrtoi(const tstring& str,const tstring& decodename, const int line)
{
	int res = 0;
	TCHAR* e;
	res = _tcstol(str.c_str(),&e,10);
	OnAssert(decodename.c_str(), line, (str.length() != 0) ,_T(" 空欄が設定されています。<error> Blank space"));
	OnAssert(decodename.c_str(), line, (_tcslen(e) == 0) || ( !(res == 0) || (str == _T("0")) || (str == _T("00")))
		,_T(" 解釈不能な数値 が設定されています。空欄や余分な文字が入っていないかチェックしてください。<error> cannot understand and check extra space or letters: "), str.c_str() );

	return res;
}
double tstrtod(const tstring& str,const tstring& decodename, const int line)
{
	double res = 0;
	TCHAR* e;
	res = _tcstod(str.c_str(),&e);
	OnAssert(decodename.c_str(), line, (str.length() != 0) ,_T(" 空欄が設定されています。<error> Blank space"));
	OnAssert(decodename.c_str(), line, (_tcslen(e) == 0) || ( !(res == 0) || str == _T("0") || str == _T("0.0"))
		,_T(" 解釈不能な数値 が設定されています。空欄や余分な文字が入っていないかチェックしてください。<error> cannot understand and check extra space or letters: "), str.c_str() );

	return res;
}

tstring trimstring(const tstring& s)
{
	tstring output;
	trimstring(s, output);
	return output;
}
void trimstring(const tstring& s, tstring & output)
{
	if(s.length() == 0)
	{
		output = s;
		return;
	}
	int b = s.find_first_not_of(_T(" \t\r\n\""));
	int e = s.find_last_not_of(_T(" \t\r\n\""));
	if(b == -1)
	{// 左右両端に、スペース、タブ、改行がない。
		output.assign(_T(""));
		return;
	}
	output.assign(s, b, e - b + 1);
}