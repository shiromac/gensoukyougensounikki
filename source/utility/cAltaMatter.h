#pragma once

class cAltaMatter
{
public:
	cAltaMatter(void);
public:
	virtual ~cAltaMatter(void);

	void process(bool Input);//各値更新//ここに値を入れる


	bool on;
	bool eve;//直前
	bool justOn;
	bool justOff;

};
