#pragma once
#include "cgamewindow.h"
#include "cCommand.h"


#include <vector>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

#define CURSORSIZE 32


class cNumSetWindow :
	public cGameWindow
{
public:
	cNumSetWindow(void);

	virtual void Init(cRenderDevice *pDev,
		unsigned long int *valuepointer,
		unsigned long int min,
		unsigned long int max,
		int basenum);

public:

	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);

	virtual int pageDraw(cRenderDevice *pDev);
	virtual int TextDraw(cRenderDevice *pDev);

	static const int MAXKETA_BINARY = 32;
	int getMaxketa();

	void setCommand(pcCommand pcommand){pcommand_ = pcommand;};
	void setKetaCaption(const tstring& cap, int keta);
protected:
	vector<int> keta_;
	int basenum_;//基数
	vector<tstring> ketaCaption_;
	int cursolX_;
	unsigned long int first_;
	unsigned long int* output_;
	unsigned long int min_;
	unsigned long int max_;

	pcCommand pcommand_;//決定ボタンで実行するコマンド

	void setKeta(const unsigned long int val, vector<int> output);
	unsigned long int getKeta(const vector<int> keta);
public:
	virtual ~cNumSetWindow(void);

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cNumSetWindow> pcNumSetWindow;
