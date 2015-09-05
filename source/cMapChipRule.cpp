#include "stdafx.h"


#include "cMapChipRule.h"



#include "utility/debug.h"



cMapChipRule::cMapChipRule(void)
{

}

cMapChipRule::~cMapChipRule(void)
{

}

int cMapChipRule::Init(pcScriptRLayer player)
{

	if(player == NULL) return false;

	ruleName_ = player->name();

	{
		//size
		tstring Music_in, Music_roop;
		pcScriptRLayer pSize = player->pmember(_T("サイズ"));
		if(pSize != NULL && pSize->script().size() >= 2)
		{
			tstring namestr = ruleName_+pSize->name();
			sizeX_ = tstrtoi(pSize->script()[0],namestr,0);
			sizeY_ = tstrtoi(pSize->script()[1],namestr,1);
		}
		else
		{
			sizeX_ = sizeY_ = 1;
		}
	}
	
	{
		//maxnum
		pcScriptRLayer pMaxnum = player->pmember(_T("最大数"));
		if(pMaxnum != NULL && pMaxnum->script().size() >= 1)
		{
			tstring namestr = ruleName_+pMaxnum->name();
			maxNum_ = tstrtoi(pMaxnum->script()[0],namestr,0);
		}
		else
		{
			maxNum_ = 100000000;
		}
	}

	{
		//判定割合
		pcScriptRLayer pCheckPer = player->pmember(_T("判定割合"));
		if(pCheckPer != NULL && pCheckPer->script().size() >= 1)
		{
			tstring namestr = ruleName_+pCheckPer->name();
			checkPer_ = tstrtod(pCheckPer->script()[0],namestr,0);
		}
		else
		{
			checkPer_ = 1.0;
		}
	}

	{
		//出現確率
		pcScriptRLayer pAppearPos = player->pmember(_T("出現確率"));
		if(pAppearPos != NULL && pAppearPos->script().size() >= 1)
		{
			tstring namestr = ruleName_+pAppearPos->name();
			appearPos_ = tstrtod(pAppearPos->script()[0],namestr,0);
		}
		else
		{
			appearPos_ = 1.0;
		}
	}

	{
		//出現パターン
		pcScriptRLayer pPattern = player->pmember(_T("出現パターン"));
		if(pPattern != NULL && pPattern->script().size() >= 4)
		{
			tstring namestr = ruleName_+pPattern->name();
			paternModX_ = tstrtoi(pPattern->script()[0],namestr,0);
			paternModY_ = tstrtoi(pPattern->script()[1],namestr,1);
			paternModAnswerX_ = tstrtoi(pPattern->script()[2],namestr,2);
			paternModAnswerY_ = tstrtoi(pPattern->script()[3],namestr,3);
		}
		else
		{
			paternModX_ = 1;
			paternModY_ = 1;
			paternModAnswerX_ = 0;
			paternModAnswerY_ = 0;
		}
	}

	{
		//マッチング
		pcScriptRLayer pMatching = player->pmember(_T("マッチング"));
		if(pMatching != NULL)
		{
			tstring namestr = ruleName_+pMatching->name();
			split_hex(pMatching->script(),_T("|"),matching_,namestr);
		}
		if(matching_.size() != sizeY_)
		{
			OnAssert(ruleName_.c_str(),0,false,_T("error マッチングの大きさがサイズと異なります。 error \"マッチング\" form"));
			return false;
		}
		int i;
		for(i=0;i<sizeY_;i++)
		{
			if(matching_[i].size() != sizeX_)
			{
				OnAssert(ruleName_.c_str(),0,false,_T("error マッチングの大きさがサイズと異なります。 error \"マッチング\" form"));
				return false;
			}
		}
	}

	{
		//占有
		pcScriptRLayer pLocation = player->pmember(_T("占有"));
		if(pLocation != NULL)
		{
			tstring namestr = ruleName_+pLocation->name();
			split_hex(pLocation->script(),_T("|"),location_,namestr);
		}
		if(location_.size() != sizeY_)
		{
			OnAssert(ruleName_.c_str(),0,false,_T("error 占有の大きさがサイズと異なります。 error \"占有\" form"));
			return false;
		}
		int i;
		for(i=0;i<sizeY_;i++)
		{
			if(location_[i].size() != sizeX_)
			{
				OnAssert(ruleName_.c_str(),0,false,_T("error 占有の大きさがサイズと異なります。 error \"占有\" form"));
				return false;
			}
		}
	}

	{
		//アニメフレーム
		pcScriptRLayer pMnime = player->pmember(_T("アニメフレーム"));
		if(pMnime != NULL)
		{
			tstring namestr = ruleName_+pMnime->name();
			split(pMnime->script(),_T("|"),animeframe_,namestr);
		}
		int i;
		for(i=0;i<animeframe_.size();i++)
		{
			animeframe_[i].resize(2,-1);
		}
	}

	{
		//チップ
		pcScriptRLayer pChip = player->pmember(_T("チップ"));
		if(pChip != NULL)
		{
			tstring namestr = ruleName_+pChip->name();
			split_hex(pChip->script(),_T("|"),chip_,namestr);
		}
		int Ysize = sizeY_*max(animeframe_.size(),1);
		if(chip_.size() != Ysize)
		{
			OnAssert(ruleName_.c_str(),0,false,_T("error Y方向のチップの大きさが規定と異なります。 error \"チップ\" form"));
			return false;
		}
		int i;
		for(i=0;i<sizeY_;i++)
		{
			if(chip_[i].size() % sizeX_ != 0)
			{
				OnAssert(ruleName_.c_str(),0,false,_T("error X方向のチップの大きさが規定と異なります。 error \"チップ\" form"));
				return false;
			}
			if(chip_[i].size() != sizeX_*LANDFORM_DECO_LAYERNUM)
			{
				chip_[i].resize(sizeX_*LANDFORM_DECO_LAYERNUM, 0);
			}
		}
	}

	{
		//描画モード
		pcScriptRLayer pDrawmode = player->pmember(_T("描画モード"));
		if(pDrawmode != NULL)
		{
			tstring namestr = ruleName_+pDrawmode->name();
			split_hex(pDrawmode->script(),_T("|"),drawmode_,namestr);
		}
		if(drawmode_.size() != sizeY_)
		{
			OnAssert(ruleName_.c_str(),0,false,_T("error 描画モードの大きさが規定と異なります。 error \"描画モード\" form"));
			return false;
		}
		int i;
		for(i=0;i<sizeY_;i++)
		{
			if(drawmode_[i].size() % sizeX_ != 0)
			{
				OnAssert(ruleName_.c_str(),0,false,_T("error 描画モードの大きさが規定と異なります。 error \"描画モード\" form"));
				return false;
			}
			if(drawmode_[i].size() != sizeX_*LANDFORM_DECO_LAYERNUM)
			{
				drawmode_[i].resize(sizeX_*LANDFORM_DECO_LAYERNUM, 0);
			}
		}
	}

	return true;
}
int cMapChipRule::Matching(int placeX, int placeY, int LocationCheck)
{
	if(placeX % paternModX_ != paternModAnswerX_ ||
		placeY % paternModY_ != paternModAnswerY_ ) return false;//パターン
	cMap& Map = sg_pDungeonSystem->Map();

	int x,y;
	
	for(x=0;x<sizeX_;x++)
	{
		for(y=0;y<sizeY_;y++)
		{
			pcLandform pland = Map.Land(placeX+x,placeY+y);
			if(LocationCheck && location_[y][x] & pland->getDecorationLocation()) return false;//すでに占有されてる
			if(!(matching_[y][x] & pland->getDecorationMatchingFlag())) return false;//マッチング失敗
		}
	}
	return true;
}

//---------------------------------------------------------
//cMapChipObject
//---------------------------------------------------------

cMapChipObject::cMapChipObject(void)
{

}

cMapChipObject::~cMapChipObject(void)
{

}

int cMapChipObject::Init(pcMapChipRule rule, int placeX, int placeY, int ID)
{
	rule_ = rule;
	placeX_ = placeX;
	placeY_ = placeY;
	ID_ = ID;
	return true;
}

pcMapChipRule cMapChipObject::rule()
{
	return rule_;
}

int cMapChipObject::SetDecoration()
{
	int x,y,i,layer;
	cMap& Map = sg_pDungeonSystem->Map();
	
	int animesize = rule_->animeframe().size();

	for(x=0;x<rule_->sizeX();x++)
	{
		for(y=0;y<rule_->sizeY();y++)
		{
			pcLandform pland = Map.Land(placeX_+x,placeY_+y);
				
			for(layer=0;layer<LANDFORM_DECO_LAYERNUM;layer++)
			{
				if(rule_->location(x,y) & (0x01<<layer))
				{
					for(i=0;i<animesize;i++)
					{
						pland->SetDecoration(rule_->chip(x,y,layer,i),
							ID_,
							layer,
							rule_->drawmode(x,y,layer),
							rule_->animeframe(i)[0],
							rule_->animeframe(i)[1]);
					}
					if(i == 0)//animeなし
					{
						pland->SetDecoration(rule_->chip(x,y,layer,i),
							ID_,
							layer,
							rule_->drawmode(x,y,layer));
					}
				}
			}

			pland->SetDecoration(-1,ID_,cMap::DECORATIONLAYER_NULL,0);//conditionにのみ追加
		}
	}
	return true;
}
int cMapChipObject::CheckDecoration()
{
	return rule_->Matching(placeX_,placeY_,false);
}
int cMapChipObject::DeleteDecoration()
{
	int x,y,i,layer;
	cMap& Map = sg_pDungeonSystem->Map();
	
	int animesize = rule_->animeframe().size();

	for(x=0;x<rule_->sizeX();x++)
	{
		for(y=0;y<rule_->sizeY();y++)
		{
			pcLandform pland = Map.Land(placeX_+x,placeY_+y);

			pland->DeleteDecorationConditionID(ID_);
		}
	}
	return true;
}

//---------------------------------------------------------
//cMapChipAnime
//---------------------------------------------------------

cMapChipAnimeAll::cMapChipAnimeAll(void)
{
	animeframeAll_.resize(4);
}

cMapChipAnimeAll::~cMapChipAnimeAll(void)
{

}
int cMapChipAnimeAll::Init(pcScriptRLayer player)
{
	{
		tstring Music_in, Music_roop;
		pcScriptRLayer pwall = player->pmember(_T("壁"));
		if(pwall != NULL)
		{
			vector<vector<int>> vvint;
			split(pwall->script(),_T("|"),vvint);
			int i;
			for(i=0;i<vvint.size();i++)
			{
				if(vvint[i].size() >= 2)
				{
					animeframe(MAPCHIP_WALL).push_back(pair<int,int>(vvint[i][0],vvint[i][1]));
				}
			}
		}
		else
		{
			animeframe(MAPCHIP_WALL).push_back(pair<int,int>(0,0));
		}
	}
	
	{
		tstring Music_in, Music_roop;
		pcScriptRLayer pwall = player->pmember(_T("部屋床"));
		if(pwall != NULL)
		{
			vector<vector<int>> vvint;
			split(pwall->script(),_T("|"),vvint);
			int i;
			for(i=0;i<vvint.size();i++)
			{
				if(vvint[i].size() >= 2)
				{
					animeframe(MAPCHIP_ROOM).push_back(pair<int,int>(vvint[i][0],vvint[i][1]));
				}
			}
		}
		else
		{
			animeframe(MAPCHIP_ROOM).push_back(pair<int,int>(0,0));
		}
	}
	{
		tstring Music_in, Music_roop;
		pcScriptRLayer pwall = player->pmember(_T("通路床"));
		if(pwall != NULL)
		{
			vector<vector<int>> vvint;
			split(pwall->script(),_T("|"),vvint);
			int i;
			for(i=0;i<vvint.size();i++)
			{
				if(vvint[i].size() >= 2)
				{
					animeframe(MAPCHIP_AISLE).push_back(pair<int,int>(vvint[i][0],vvint[i][1]));
				}
			}
		}
		else
		{
			animeframe(MAPCHIP_AISLE).push_back(pair<int,int>(0,0));
		}
	}
	{
		tstring Music_in, Music_roop;
		pcScriptRLayer pwall = player->pmember(_T("水路"));
		if(pwall != NULL)
		{
			vector<vector<int>> vvint;
			split(pwall->script(),_T("|"),vvint);
			int i;
			for(i=0;i<vvint.size();i++)
			{
				if(vvint[i].size() >= 2)
				{
					animeframe(MAPCHIP_WATER).push_back(pair<int,int>(vvint[i][0],vvint[i][1]));
				}
			}
		}
		else
		{
			animeframe(MAPCHIP_WATER).push_back(pair<int,int>(0,0));
		}
	}
	return Init();
}
int cMapChipAnimeAll::Init(void)
{
	animeframeAll_[MAPCHIP_WALL].Init();
	animeframeAll_[MAPCHIP_ROOM].Init();
	animeframeAll_[MAPCHIP_AISLE].Init();
	animeframeAll_[MAPCHIP_WATER].Init();
	return true;
}

int cMapChipAnimeAll::process(void)
{
	animeframeAll_[MAPCHIP_WALL].process();
	animeframeAll_[MAPCHIP_ROOM].process();
	animeframeAll_[MAPCHIP_AISLE].process();
	animeframeAll_[MAPCHIP_WATER].process();
	return true;
}
int cMapChipAnimeAll::getFrame(int mapchip)
{
	return animeframeAll_[mapchip].getFrame();
}

vector<pair<int,int>>& cMapChipAnimeAll::animeframe(int mapchip)
{
	return animeframeAll_[mapchip].animeframe_;
}

cMapChipAnimeAll::cMapChipAnime::cMapChipAnime(void)
{
	animefase_ = 0;
	animecount_ = 0;
}

cMapChipAnimeAll::cMapChipAnime::~cMapChipAnime(void)
{
}

int cMapChipAnimeAll::cMapChipAnime::process(void)
{
	if(animeframe_.size() < 2) return true;
	animecount_++;
	if(animecount_ >= animeframe_[animefase_].second)
	{
		animefase_++;
		if(animefase_ >= animeframe_.size()) animefase_ = 0;
		animecount_ = 0;
	}
	 return true;
}

int cMapChipAnimeAll::cMapChipAnime::getFrame(void)
{
	if(animeframe_.size() < 2) return 0;
	return animeframe_[animefase_].first;
}

int cMapChipAnimeAll::cMapChipAnime::Init(void)
{
	animefase_ = 0;
	animecount_ = 0;
	return true;
}