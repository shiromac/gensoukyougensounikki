#pragma once

#include "cMap.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "gameMainSystem/filemanage/cScriptReader.h"
#include <boost/shared_ptr.hpp>

class cMapChipRule
{
public:
	cMapChipRule(void);
public:
	virtual ~cMapChipRule(void);


	virtual int Init(pcScriptRLayer player);

	//ìKçáÇµÇΩÇÁtrueÇï‘Ç∑
	virtual int Matching(int placeX, int placeY, int LocationCheck = 1);

	virtual int sizeX(){return sizeX_;};
	virtual int sizeY(){return sizeY_;};
	virtual double checkPer(){return checkPer_;};
	virtual double appearPos(){return appearPos_;};
	virtual int maxNum(){return maxNum_;};
	virtual const vector<vector<int>>& animeframe(){return animeframe_;};
	virtual dChipTexPos chip(int x, int y, int layer, int anime = 0){return chip_[y+anime*sizeY_][x+layer*sizeX_];};
	virtual int drawmode(int x, int y, int layer){return drawmode_[y][x+layer*sizeX_];};
	virtual const vector<int>& animeframe(int i){return animeframe_[i];};
	virtual int location(int x, int y){return location_[y][x];};
	virtual tstring ruleName(){return ruleName_;};
protected:

	tstring ruleName_;

	int sizeX_;
	int sizeY_;
	int maxNum_;
	double appearPos_;
	double checkPer_;

	int paternModX_;
	int paternModY_;
	int paternModAnswerX_;
	int paternModAnswerY_;

	vector<vector<int>> drawmode_;

	vector<vector<int>> animeframe_;
	vector<vector<int>> matching_;

	vector<vector<int>> location_;
	vector<vector<dChipTexPos>> chip_;
};
typedef boost::shared_ptr<cMapChipRule> pcMapChipRule;

class cMapChipObject
{
public:
	cMapChipObject(void);
public:
	virtual ~cMapChipObject(void);


	virtual int Init(pcMapChipRule rule, int placeX, int placeY, int ID);
	
	virtual int SetDecoration();
	virtual int CheckDecoration();
	virtual int DeleteDecoration();

	virtual pcMapChipRule rule();

protected:

	pcMapChipRule rule_;

	int placeX_;
	int placeY_;
	int ID_;
};
typedef boost::shared_ptr<cMapChipObject> pcMapChipObject;


class cMapChipAnimeAll
{
public:
	cMapChipAnimeAll(void);
public:
	virtual ~cMapChipAnimeAll(void);

	static const int MAPCHIP_WALL = 0;
	static const int MAPCHIP_ROOM = 1;
	static const int MAPCHIP_AISLE = 2;
	static const int MAPCHIP_WATER = 3;

	virtual int Init(pcScriptRLayer player);
	virtual int Init(void);
	

	virtual int process(void);
	virtual int getFrame(int mapchip);

	virtual vector<pair<int,int>>& animeframe(int mapchip);
protected:
	class cMapChipAnime
	{
	public:
		cMapChipAnime(void);
	public:
		virtual ~cMapChipAnime(void);
	public:
		int animefase_;
		int animecount_;
		virtual int process(void);
		virtual int getFrame(void);
		virtual int Init(void);
		vector<pair<int,int>> animeframe_;
	};

	vector<cMapChipAnime> animeframeAll_;
};
typedef boost::shared_ptr<cMapChipAnimeAll> pcMapChipAnimeAll;