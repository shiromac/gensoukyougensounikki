#pragma once

class cColor
{
public:
	cColor(int a, int r, int g, int b);
	cColor();
	cColor(unsigned long d3dcolor);
public:
	~cColor(void);


	int red;
	int green;
	int blue;
	int alpha;

	//rgb‚ğ0~255‚É‹­§‚·‚é
	void formalize();

	cColor operator *(const cColor& rother);//*‰‰Zq(Š|‚¯Z)
	void operator *=(const cColor& rother);//*=‰‰Zq(Š|‚¯Z)
	cColor operator +(const cColor& rother);//+‰‰Zq()
	void operator +=(const cColor& rother);//+=‰‰Zq()
	bool operator ==(const cColor& rother)const;//+=‰‰Zq()

	//h=0~360
	void HSV(double h, double s, double v);

	void ARGB(int a, int r, int g, int b);

	//D3D‚Åo—Í‚·‚é
	unsigned long D3Dcolor();

	//D3D‚Å“ü—Í‚·‚é
	void inputD3Dcolor(unsigned long d3dcolor);
};
cColor operator *(double power, const cColor& rother);//*‰‰Zq(Š|‚¯Z)
