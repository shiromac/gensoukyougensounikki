#pragma once
#include "cgamewindow.h"
#include "cCommand.h"

#include <vector>
#include <map>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

//文字入力用インターフェイス
const int STRINGWINDOW_STRNUM = 8;

const int STRINGWINDOW_SETNUM = 3;



class cMemoString
{
public:
	cMemoString(void);
	virtual ~cMemoString(void);
	cMemoString(cMemoString &rother);

protected:
	//初期のデータ参照先
	UByte escapedata_[STRINGWINDOW_STRNUM];
	UByte escapeDatasize_;

	//これを使う
	UByte* pdata_[STRINGWINDOW_STRNUM];
	UByte* pDatasize_;

	tstring str_;
	int strMadefrag_;
	int dataSizeCulfrag_;
public:

	//出力更新
	virtual void refresh();

	//データ出力
	virtual bool getDataEnable();
	virtual UByte getDataSize();
	virtual UByte getdata(int index);

	//保存先設定
	virtual void setpdata(UByte *pointer,int index);
	virtual void setpDataSize(UByte *pointer);

	//文字列変更
	virtual void setdata(UByte data,int index);
	virtual void setDataSize(UByte size);//普段は自動なので不要。

	//出力
	virtual const tstring& str();


	virtual cMemoString& operator=(cMemoString& rother);

	//比較関数
	inline bool operator<(cMemoString& rother)
	{
		return (str() < rother.str());
	};


	//セーブ
	virtual void save(std::vector<SByte>& data);
	virtual void load(const std::vector<SByte>& data);
};
	
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cMemoString> pcMemoString;




class cStringWindow :
	public cSelectWindow
{
public:
	cStringWindow(void);

	//cStringWindowは縦横の指定が不可能
	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);
	virtual void Init(cRenderDevice *pDev);

	pcMemoString targetStr;
	tstring* targetStr2;
protected:
	virtual int TextDraw(cRenderDevice *pDev);
	virtual int CursorDraw(cRenderDevice *pDev);
	//virtual int pageDraw(cRenderDevice *pDev);

	virtual int RerenderDraw(cRenderDevice *pDev);


	cRenderTexture* m_pTexture_Multiselect;


	//上のウィンドウのテキストに干渉できないので外部から描画する。
	SpriteText m_SpriteText_SumpleText;


	int cursolIndexXMax;
	int cursolIndexX;
	int cursolIndexYMax;
	int cursolIndexY;


	int StrSet;
	int cursolforcus;

	virtual StyleString shortExplanationString(cRenderDevice *pDev);


	//vector<tstring> strSample_;
	map<int,int> ShortCutPair_;
	vector<tstring> strSetList_;
	//int NullSampleIndex_;


	cMemoString str_;
	//SByte Strsize_;
	//SByte Str_[STRINGWINDOW_STRNUM];

	pcSelectWindow pSetselectW_;
	pcGameWindow pStrW_;

	int modeCover_;
	pcSelectWindow pcsCoverw_;

	virtual void commandWrite(int strindex);
	virtual void commandStrset(int index);
	virtual void commandErase();//消す
	virtual void commandCover();//補う
	virtual void commandChange();//変換
	virtual void commandBack();//戻る
	virtual void commandForward();//進む
	virtual void commandEnd();//終わる
	virtual void commandCancel();//変えずに終わる

	virtual void CoverAdd(cMemoString & memostr);//候補加える
	virtual void CoverErase(cMemoString & memostr);//候補消す
	//コマンドインデックス
	
	virtual int commandIndexErase(){return strSetList_.size();};//消す
	virtual int commandIndexCover(){return strSetList_.size()+1;};//補う
	virtual int commandIndexChange(){return strSetList_.size()+2;};//変換
	virtual int commandIndexBack(){return strSetList_.size()+3;};//戻る
	virtual int commandIndexForward(){return strSetList_.size()+4;};//進む
	virtual int commandIndexEnd(){return strSetList_.size()+5;};//終わる


	pcScriptRLayer StringWindowMemoData_;
public:
	virtual ~cStringWindow(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int pageIndexMaxF(){return pageIndexMax;};
};


typedef boost::shared_ptr<cStringWindow> pcStringWindow;
