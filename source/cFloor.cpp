
#include "cDungeonSystem.h"

#include "cFloor.h"

#include "DungeonMake.h"

#include "cSaveStore.h"

#include "utility/debug.h"

#include "utility/StyleString.h"

#include <boost/make_shared.hpp>
#include "../GensouGage.h"


#define ArraySizeOf(array)     (sizeof(array) / sizeof(array[0]))


//----------------------------------------------------------------
//cFloor
//----------------------------------------------------------------
cFloor::cFloor(void)
{
	appearenemycount_ = 0;
	floorindex = 0;
	floorrandomSeed = 0;
}

int cFloor::decodeFloor(pcFloor pmodelfloor, pcScriptRLayer floordata)
{

	decodetstring(mapchipsetname_,pmodelfloor->mapchipsetname(),floordata,tstring(_T("チップセット")),0);
	{
		const tstring enemystr(_T("出現敵"));
		decodeProbabilityChara(enemyDP_, pmodelfloor->enemyDP(), floordata, enemystr);

		decodeInt(Maxenemynum_, pmodelfloor->Maxenemynum(), floordata, enemystr, tstring(_T("出現数最大")), 0);

		const tstring hindostr(_T("出現頻度"));
		decodeInt(Minappearenemy_, pmodelfloor->Minappearenemy(), floordata, enemystr, hindostr, 0);
		decodeInt(Maxappearenemy_, pmodelfloor->Maxappearenemy(), floordata, enemystr, hindostr, 1);
		nextappearenemy_ = (Minappearenemy_+Maxappearenemy_/2);
	
		int i;
		const tstring firstsetnumstr(_T("初期出現数"));
		for(i=0;i<8;i++)
		{
			decodeDouble(enemyfirstsetnums_[i], pmodelfloor->enemyfirstsetnums(i), floordata, enemystr, firstsetnumstr, i);
		}

		const tstring firstsetstr(_T("初期設置"));
		decodeIntMatrix(charafirstsets_, pmodelfloor->charafirstsets(), floordata, enemystr, firstsetstr);
		
		decodeDouble(overdriveEnemyPercent_, pmodelfloor->overdriveEnemyPercent(), floordata, enemystr, tstring(_T("オーバードライブ出現率％")), 0);
		
		decodeInt(overdriveMaxAppearEnemyNum_, pmodelfloor->overdriveMaxAppearEnemyNum(), floordata, enemystr, tstring(_T("オーバードライブ出現敵最大数")), 0);
	}
	{
		const tstring dropstr(_T("ドロップアイテム基礎"));
		decodeProbabilityGroup(dropitemDP_, pmodelfloor->dropitemDP(), floordata, dropstr);
	
		int i;
		const tstring firstsetnumstr(_T("初期設置数"));
		for(i=0;i<8;i++)
		{
			decodeDouble(itemfirstsetnums_[i], pmodelfloor->itemfirstsetnums(i), floordata, dropstr, firstsetnumstr, i);
		}

		const tstring firstsetstr(_T("初期設置"));
		decodeIntMatrix(itemfirstsets_, pmodelfloor->itemfirstsets(), floordata, dropstr, firstsetstr);
	}
	{
		const tstring shopstr(_T("店アイテム"));
		decodeProbabilityGroup(shopitemDP_, pmodelfloor->shopitemDP(), floordata, shopstr);
	}
	{
		const tstring trapstr(_T("設置罠"));
		decodeProbabilityItem(trapDP_, pmodelfloor->trapDP(), floordata, trapstr);
	
		int i;
		const tstring firstsetstr(_T("初期設置数"));
		for(i=0;i<8;i++)
		{
			decodeDouble(trapfirstsetnums_[i], pmodelfloor->trapfirstsetnums(i), floordata, trapstr, firstsetstr, i);
		}
	}
	{
		const tstring friendstr(_T("主人公初期位置"));
		vector<vector<int>> nomodel;
		decodeIntMatrix(friendfirstsets_, nomodel, floordata, friendstr);
	}
	{
		const tstring shopfundstr(_T("店資金"));
		int i;
		for(i=0;i<3;i++)
		{
			decodeDouble(shopfunddefines_[i], pmodelfloor->shopfunddefines(i), floordata, shopfundstr, i);
		}
	}
	//ダンジョンデコード
	decodeDungeon(pmodelfloor, floordata);

	//宴会デコード
	pcScriptRLayer decodedata = floordata->pmember(_T("宴会"));
	if(decodedata == NULL)
	{
		if(floorindex == 1)
		{
			OnAssert(_T(__FILE__), __LINE__ ,false,_T("1F 宴会が定義されていません。<error> 1F [宴会] Not Found"));
		}
		vpParty_ = pmodelfloor->vpParty();
		cPartyDP_ = pmodelfloor->cPartyDP();
	}
	else
	{
		map<tstring,pcScriptRLayer>::iterator itr;
		itr = decodedata->membermap().begin();
		for(;itr!=decodedata->membermap().end();itr++)
		{
			pcParty pparty = pcParty(new cParty);
			decodeParty(pparty,itr->second);
			vpParty_.push_back(pparty);
		}
		
		int i;
		for(i=0;i<vpParty_.size();i++)
		{
			cPartyDP_.set(vpParty_[i]->pos(),i);
		}

	}

	//追加設定スクリプト
	pcScriptRLayer decodeLuaScript = floordata->pmember(_T("追加設定スクリプト"));
	if(decodeLuaScript != NULL)
	{
		extraLuaScript_ = decodeLuaScript;
		decodeLuaScript->getLuaScriptKeys(extraLuaScriptKeys_);
	}


	return true;
}

pcParty cFloor::getParty()
{
	int index = cPartyDP_.get(random());
	if(index >= vpParty_.size()) return pcParty((cParty*)NULL);
	return vpParty_[index];
}

void cFloor::decodeDungeon(pcFloor pmodelfloor, pcScriptRLayer floordata)
{
	tstring name(_T("ダンジョン生成ルール"));
	pcScriptRLayer decodedata = floordata->pmember(name);
	if(decodedata == NULL)
	{
		dungeonDP_ = pmodelfloor->dungeonDP();
		dungeonFormVal_ = pmodelfloor->dungeonFormVal();
		dungeonRandomVal_ = pmodelfloor->dungeonRandomVal();
		floorMapGenForm_ = pmodelfloor->floorMapGenForm();
	}
	else
	{

		if(decodedata->topscript() == _T("ランダム"))
		{
			//ランダム生成！
			floorMapGenForm_ = FLOORMAPGENFORM_RANDOM;
			decodeDungeonRandom(pmodelfloor,decodedata);
		
		}
		else if(decodedata->topscript() == _T("固定"))
		{
			//固定！
			floorMapGenForm_ = FLOORMAPGENFORM_CONST;
			decodeDungeonConst(pmodelfloor,decodedata);
		
		}
		else if(decodedata->topscript() == _T("シャッフル"))
		{
			//シャッフル！
			floorMapGenForm_ = FLOORMAPGENFORM_SHUFFLE;
			decodeDungeonShuffle(pmodelfloor,decodedata);
		
		}
		else
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T(" ダンジョン生成ルール 生成方式が分かりません。「ランダム」または「固定」または「シャッフル」を設定してください。<error> in [ダンジョン生成ルール] Input「ランダム」or「固定」or 「シャッフル」"));
		}

		decodeDungeonForm(pmodelfloor, decodedata);

	}


}
int cFloor::enemyAppear()
{
	appearenemycount_++;
	if(appearenemycount_ >= nextappearenemy_)
	{
		appearenemycount_ = 0;
		//nextappearenemy_ = random_range(Minappearenemy_,Maxappearenemy_+1);
		//return enemyDP_.get(random());
		return 1;
	}

	return 0;
}
int cFloor::enemyfirstsetnum(const int roomnum,const int landnum)
{
	return random_range(MinenemyfirstsetnumPerFloor(),MaxenemyfirstsetnumPerFloor())
		+ roomnum*random_range(MinenemyfirstsetnumPerRoom(),MaxenemyfirstsetnumPerRoom())
		+ landnum*random_range(MinenemyfirstsetnumPerLand(),MaxenemyfirstsetnumPerLand())
		+ floorindex*random_range(MinenemyfirstsetnumPerFloorIndex(),MaxenemyfirstsetnumPerFloorIndex());
}
int cFloor::trapfirstsetnum(const int roomnum, const int landnum)
{
	return random_range(MintrapfirstsetnumPerFloor(),MaxtrapfirstsetnumPerFloor())
		+ roomnum*random_range(MintrapfirstsetnumPerRoom(),MaxtrapfirstsetnumPerRoom())
		+ landnum*random_range(MintrapfirstsetnumPerLand(),MaxtrapfirstsetnumPerLand())
		+ floorindex*random_range(MintrapfirstsetnumPerFloorIndex(),MaxtrapfirstsetnumPerFloorIndex());
}
int cFloor::itemfirstsetnum(const int roomnum, const int landnum)
{
	return random_range(MinitemfirstsetnumPerFloor(),MaxitemfirstsetnumPerFloor())
		+ roomnum*random_range(MinitemfirstsetnumPerRoom(),MaxitemfirstsetnumPerRoom())
		+ landnum*random_range(MinitemfirstsetnumPerLand(),MaxitemfirstsetnumPerLand())
		+ floorindex*random_range(MinitemfirstsetnumPerFloorIndex(),MaxitemfirstsetnumPerFloorIndex());
}

void cFloor::decodeDungeonForm(pcFloor pmodelfloor, pcScriptRLayer floordata)
{
	pcScriptRLayer formrule = floordata->pmember(_T("各種規定"));
	if(formrule == NULL)
	{
		dungeonFormVal_ = pmodelfloor->dungeonFormVal();
	}
	else
	{
		
		map<tstring,pcScriptRLayer>::iterator itr = formrule->membermap().begin();
		for(;itr != formrule->membermap().end();itr++)
		{
			vector<vector<double>> vvdouble;
			split(itr->second->script(),_T("|"),vvdouble,itr->first);
			int k;
			vector<double> val;
			for(k=0;k<vvdouble.size();k++)
			{
				if(!vvdouble[k].empty()) val.push_back(vvdouble[k][0]);
			}
			dungeonFormVal_.insert(pair<tstring,vector<double>>(itr->first,val));
		}
	}
}


void cFloor::decodeDungeonRandom(pcFloor pmodelfloor, pcScriptRLayer floordata)
{
	pcScriptRLayer randomrule = floordata->pmember(_T("ランダム生成ルール"));
	if(randomrule == NULL)
	{
		dungeonDP_ = pmodelfloor->dungeonDP();
		dungeonRandomVal_ = pmodelfloor->dungeonRandomVal();
	}
	else
	{
		vector<vector<tstring>> vvstr;
		split(randomrule->script(),_T("|"),vvstr);


		int i;
		int size = vvstr.size();
		for(i=0;i<size;i++)
		{
			if(vvstr[i].size() != 2)
			{
				OnAssert(_T("ランダム生成ルール"),i,false,_T(" 書式にエラーが発生しました。スキップします。There is an error of form. Skip it."));
				continue;
			}
			pcScriptRLayer casevalue = randomrule->pmember(vvstr[i][0]);
			
			//可能性
			double pos = tstrtod(vvstr[i][1],vvstr[i][0]);
			
			vector<vector<double>> vvdouble;
			split(casevalue->script(), _T("|"), vvdouble, vvstr[i][0]);
			
			int k;
			//変数
			vector<double> val;
			for(k=0;k<vvdouble.size();k++)
			{
				if(!vvdouble[k].empty()) val.push_back(vvdouble[k][0]);
			}

			//インデックス
			int index;
			for(k=0;k< ArraySizeOf(DungeonCaseStr) ;k++)
			{
				if(DungeonCaseStr[k] == vvstr[i][0])
				{
					index = k;
				}
			}
			//挿入
			dungeonRandomVal_.insert( pair<int,vector<double>>(index,val) );

			dungeonDP_.set(pos,index);
		}
		
	}
}
void cFloor::decodeDungeonConst(pcFloor pmodelfloor, pcScriptRLayer floordata)
{

	pcScriptRLayer floormapdata = floordata->pmember(_T("マップ"));
	if(floormapdata == NULL)
	{
		floormap_ = pmodelfloor->floormap();
	}
	else
	{
		vector<vector<tstring>> vvstr;
		split(floormapdata->script(),_T("|"),floormap_);
		
	}
}
void cFloor::decodeDungeonShuffle(pcFloor pmodelfloor, pcScriptRLayer floordata)
{
	vector<vector<tstring>> vvstr;
	std::map<tstring,pcScriptRLayer>& membermap = floordata->membermap();
	std::map<tstring,pcScriptRLayer>::iterator itr = membermap.begin();
	
	if(membermap.empty())
	{
		sufflefloormap_ = pmodelfloor->sufflefloormap();
	}
	else
	{
		for(; itr != membermap.end(); ++itr)
		{
			pcScriptRLayer map = itr->second->pmember(_T("マップ"));

			if(map != NULL)
			{
				sufflefloormap_.insert(pair<tstring,vector<vector<tstring>>>(itr->first,vvstr));
				split(map->script(),_T("|"),sufflefloormap_[itr->first]);
			}
			else
			{
				OnAssert(itr->second->name().c_str(),__LINE__,false,_T(" シャッフルの個別設定に「マップ」がありません<error> member「マップ」not found."));
				continue;
			}

			pcScriptRLayer pos = itr->second->pmember(_T("出現率係数"));
			if(pos != NULL)
			{
				sufflefloorpossiblity_.insert(pair<tstring,double>(itr->first,tstrtod(pos->topscript())));
			}
			else
			{
				sufflefloorpossiblity_.insert(pair<tstring,double>(itr->first,1));
			}
		}
	}
}

void cFloor::decodeProbabilityItem(cDiscreteProbability& dp, const cDiscreteProbability& modeldp, pcScriptRLayer pfloordata, const tstring& name)
{

	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		dp = modeldp;
	}
	else
	{
		int size = decodedata->script().size();
		int i;
		for(i=0;i<size;i++)
		{
			vector<int> vint;
			split(decodedata->script()[i],_T("|"),vint,name.c_str());
			if(vint.size() != 4)
			{
				OnAssert(name.c_str(),i,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				continue;
			}
			if(sg_pDungeonSystem->DataBase.GetSampleDroping(vint[0]) == NULLDROP)
			{
				OnAssert(name.c_str(),i,false,setStyle(vint[0]).c_str(),_T(" そのIDのアイテムは存在しません。<error> Item of this ID does not exist."));
				continue;
			}

			vint[1] = (int)((UByte)(vint[1]));
			vint[2] = (int)((UByte)(vint[2]));

			dp.set(vint[3]*sg_pDungeonSystem->DataBase.GetSampleDroping(vint[0])->レア度() , (vint[0] & 0x7FFF)<<16 | (vint[1] & 0xFF)<<8 | (vint[2] & 0xFF));
		}
	}
}
void cFloor::decodeProbabilityChara(cDiscreteProbability& dp, const cDiscreteProbability& modeldp, pcScriptRLayer pfloordata, const tstring& name)
{

	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		dp = modeldp;
	}
	else
	{
		int size = decodedata->script().size();
		int i;
		for(i=0;i<size;i++)
		{
			vector<int> vint;
			split(decodedata->script()[i],_T("|"),vint,name.c_str());
			if(vint.size() != 4)
			{
				OnAssert(name.c_str(),i,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				continue;
			}
			if(sg_pDungeonSystem->DataBase.GetSampleCharacter(vint[0]) == NULLCHARA)
			{
				OnAssert(name.c_str(),i,false,setStyle(vint[0]).c_str(),_T(" そのIDのキャラクターは存在しません。<error> Character of this ID does not exist."));
				continue;
			}

			vint[1] = (int)((UByte)(vint[1]));
			vint[2] = (int)((UByte)(vint[2]));

			dp.set(vint[3] , (vint[0] & 0x7FFF)<<16 | (vint[1] & 0xFF)<<8 | (vint[2] & 0xFF));
		}
	}
}
void cFloor::decodeProbabilityGroup(cDropingDistribution& dp, const cDropingDistribution& modeldp, pcScriptRLayer pfloordata, const tstring& name)
{
	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		dp = modeldp;
	}
	else
	{

		
		tstring groupstr(_T("グループ分布"));
		pcScriptRLayer groupdata = decodedata->pmember(groupstr);
		if(groupdata == NULL)
		{
			dp.setItemGroupPos(1,1,1,1,1,1,1,1,0);
		}
		else
		{
			if(groupdata->script().size() != 9)
			{
				OnAssert((name+groupstr).c_str(),-1,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				dp.setItemGroupPos(1,1,1,1,1,1,1,1,0);
			}
			else
			{
				vector<vector<double>> vvdouble;
				split(groupdata->script(),_T("|"),vvdouble,(name+groupstr).c_str());
				dp.setItemGroupPos(vvdouble[0][0],vvdouble[1][0],vvdouble[2][0],vvdouble[3][0],vvdouble[4][0],vvdouble[5][0],vvdouble[6][0],vvdouble[7][0],vvdouble[8][0]);
			}
		}


		int size = decodedata->script().size();

		int i;
		for(i=0;i<size;i++)
		{
			vector<int> vint;
			split(decodedata->script()[i],_T("|"),vint,name.c_str());
			if(vint.size() != 4)
			{
				OnAssert(name.c_str(),i,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				continue;
			}
			//vint[1] = (int)((UByte)(vint[1]));
			//vint[2] = (int)((UByte)(vint[2]));

			dp.set(vint[0], vint[3]);
		}



	}
}

void cFloor::decodetstring(tstring& tv, const tstring& modelv, pcScriptRLayer pfloordata, const tstring& name, const int index)
{
	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		tv = modelv;
		return;
	}
	else
	{
		if(decodedata->script().size() > index)
		{
			tv = decodedata->script()[index];
		}
		else
		{
			tv = modelv;
			return;
		}
	}
}

void cFloor::decodeInt(int& tv, const int modelv, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name, const int index)
{
	pcScriptRLayer decodedata = pfloordata->pmember(parentname);
	if(decodedata == NULL)
	{
		tv = modelv;
		return;
	}
	else
	{
		decodeInt(tv, modelv, decodedata, name, index);
	}
}
void cFloor::decodeInt(int& tv, const int modelv, pcScriptRLayer pfloordata, const tstring& name, const int index)
{
	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		tv = modelv;
		return;
	}
	else
	{
		if(decodedata->script().size() > index)
		{
			tv = tstrtoi(decodedata->script()[index],name);
		}
		else
		{
			tv = modelv;
			return;
		}
	
	}
}
void cFloor::decodeDouble(double& tv, const double modelv, pcScriptRLayer pfloordata, const tstring& name, const int index)
{
	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		tv = modelv;
		return;
	}
	else
	{
		if(decodedata->script().size() > index)
		{
			tv = tstrtod(decodedata->script()[index],name);
		}
		else
		{
			tv = modelv;
			return;
		}
	
	}
}
void cFloor::decodeDouble(double& tv, const double modelv, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name, const int index)
{
	pcScriptRLayer decodedata = pfloordata->pmember(parentname);
	if(decodedata == NULL)
	{
		tv = modelv;
		return;
	}
	else
	{
		decodeDouble(tv, modelv, decodedata, name, index);
	}
}

void cFloor::decodeIntMatrix(vector<vector<int>>& tvvi, const vector<vector<int>>& modelvvi, pcScriptRLayer pfloordata, const tstring& name)
{
	pcScriptRLayer decodedata = pfloordata->pmember(name);
	if(decodedata == NULL)
	{
		tvvi = modelvvi;
		return;
	}
	else
	{
		split(decodedata->script(),_T("|"),tvvi,name);
	}
}
void cFloor::decodeIntMatrix(vector<vector<int>>& tvvi, const vector<vector<int>>& modelvvi, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name)
{
	pcScriptRLayer decodedata = pfloordata->pmember(parentname);
	if(decodedata == NULL)
	{
		tvvi = modelvvi;
		return;
	}
	else
	{
		decodeIntMatrix(tvvi,modelvvi,decodedata,name);
	}
}

void cFloor::decodeParty(pcParty& tv, pcScriptRLayer floordata)
{
	int i;
	tv->enemyDPexist = 0;
	tv->dropitemDPexist = 0;
	tv->trapDPexist = 0;

	decodeInt(tv->pos(), 0, floordata, (tstring)_T("確率係数"), 0);


	if(floordata->pmember((tstring)_T("出現敵")) != NULL)
	{
		decodeProbabilityChara(tv->enemyDP_, enemyDP(), floordata, (tstring)_T("出現敵"));
		tv->enemyDPexist = 1;
	}

	if(floordata->pmember((tstring)_T("ドロップアイテム")) != NULL)
	{
		decodeProbabilityGroup(tv->dropitemDP_, dropitemDP(), floordata, (tstring)_T("ドロップアイテム"));
		tv->dropitemDPexist = 1;
	}

	if(floordata->pmember((tstring)_T("設置罠")) != NULL)
	{
		decodeProbabilityItem(tv->trapDP_, trapDP(), floordata, (tstring)_T("設置罠"));
		tv->trapDPexist = 1;
	}


	for(i=0;i<8;i++)
	{
		decodeDouble(tv->enemyfirstsetnums(i), 0, floordata, (tstring)_T("敵出現数"), i);
		decodeDouble(tv->trapfirstsetnums(i), 0, floordata, (tstring)_T("罠出現数"), i);
		decodeDouble(tv->itemfirstsetnums(i), 0, floordata, (tstring)_T("アイテム出現数"), i);
	}

}
	
//----------------------------------------------------------------
//cDungeon
//----------------------------------------------------------------

cDungeon::cDungeon(void){
	appreciationpos_ = 1;
	gensouGage_ = boost::make_shared<GensouGage>();
};
void cDungeon::decode(pcScriptRLayer data)
{
	if(data == NULL) return;
	data_ = data;

	randomgen().init_genrand(sg_pDungeonSystem->RandomInitGen().genrand_int32());

	int i;
	int floormax = 100;

	//-----------------------------------------------------
	//フロア数
	tstring strfmax(_T("最大階層"));
	pcScriptRLayer floornumdata = data->pmember(strfmax);
	if(floornumdata == NULL)
	{//データ記述なし
		OnAssert(_T(__FILE__), __LINE__ ,false,_T("最大階層が定義されていません。<error> [最大階層] Not Found"));
	}
	else
	{
		floormax = tstrtoi(floornumdata->topscript(),strfmax);
	}

	//-----------------------------------------------------
	//アイテム識別率
	tstring striapp(_T("アイテム識別率"));
	pcScriptRLayer appreciationposdata = data->pmember(striapp);
	if(appreciationposdata == NULL)
	{//データ記述なし
		OnAssert(_T(__FILE__), __LINE__ ,false,_T("アイテム識別率が定義されていません。<error> [アイテム識別率] Not Found"));
		appreciationpos_ = 0;
	}
	else
	{
		appreciationpos_ = tstrtod(appreciationposdata->topscript(),striapp);

		int i;
		const tstring str(_T("アイテム別"));
		pcScriptRLayer decodedata = appreciationposdata->pmember(str);
		if(decodedata != NULL)
		{
			for(i=0;i<9;i++)
			{
				if(decodedata->script().size() <= i) break;
				appreciationposkind_.push_back(tstrtod(decodedata->script()[i],str));
			}
		}
	}

	//-----------------------------------------------------
	//初期所持アイテム
	tstring strfitem(_T("初期所持アイテム"));
	pcScriptRLayer firstItemsdata = data->pmember(strfitem);
	if(firstItemsdata == NULL)
	{//データ記述なし
		OnAssert(_T(__FILE__), __LINE__ ,false,_T("初期所持アイテムが定義されていません。<error> [初期所持アイテム] Not Found"));
	}
	else
	{
		split(firstItemsdata->script(),_T("|"),firstItems_,strfitem);
	}

	//pcFloor pmodelfloor = pcFloor((cFloor*)NULL);
	pcFloor pmodelfloor = pcFloor(new cFloor);

	//-----------------------------------------------------
	//拠点フラグ
	tstring strhomef(_T("拠点フラグ"));
	pcScriptRLayer floorhomef = data->pmember(strhomef);
	if(floorhomef == NULL)
	{//データ記述なし
		homeFlag_ = 0;
	}
	else
	{
		homeFlag_ = tstrtoi(floorhomef->topscript(),strhomef);
	}

	//-----------------------------------------------------
	//プレイヤー経験値係数
	tstring strplayerexp(_T("プレイヤー経験値係数"));
	pcScriptRLayer floorplayerexp = data->pmember(strplayerexp);
	if(floorplayerexp == NULL)
	{//データ記述なし
		playerExpPower_ = 1.0;
	}
	else
	{
		playerExpPower_ = tstrtod(floorplayerexp->topscript(),strplayerexp);
	}

	//-----------------------------------------------------
	//クリア演出省略フラグ
	tstring strclearskip(_T("クリア演出省略フラグ"));
	pcScriptRLayer floorclearskip = data->pmember(strclearskip);
	if(floorclearskip == NULL)
	{//データ記述なし
		clearResultSkipFlag_ = 0;
	}
	else
	{
		clearResultSkipFlag_ = tstrtoi(floorclearskip->topscript(),strclearskip);
	}


	//-----------------------------------------------------
	//ストーリースクリプト
	tstring strstory(_T("ストーリースクリプト"));
	pcScriptRLayer dungeonstory = data->pmember(strstory);
	if(dungeonstory != NULL)
	{
		storyScript_ = dungeonstory;
		dungeonstory->getLuaScriptKeys(storyScriptKeys_);
		cValiableField valiable;

		int i, size = storyScriptKeys_.size();
		//storyScriptEnvironments_.resize(size);//resize不可
		for(i=0;i<size;i++)
		{
			storyScriptEnvironments_.push_back(DungeonStoryScript_LuaEnvironment(tString2luastring(storyScriptKeys_[i])));
		}
		for(i=0;i<size;i++)
		{
			if(!storyScriptEnvironments_[i].loadDungeonStoryScript(dungeonstory,storyScriptKeys_[i],valiable))
			{
				OnAssert(_T(__FILE__),__LINE__,false,_T("ストーリースクリプトのロードに失敗しました　error. failed to load \"ストーリースクリプト\"："),storyScriptKeys_[i].c_str());
			}
		}
		
	}

	//-----------------------------------------------------
	//ダンジョン進行方向
	tstring strdirection(_T("ダンジョン進行方向"));
	pcScriptRLayer floordirection = data->pmember(strdirection);
	if(floordirection == NULL)
	{//データ記述なし
		dungeonDirection_ = DungeonDirection_Up;
	}
	else
	{
		int direction = tstrtoi(floordirection->topscript(),strdirection);
		if(direction == 0){
			dungeonDirection_ = DungeonDirection_Up;
		}
		else if(direction == 1){
			dungeonDirection_ = DungeonDirection_Down;
		}
		else
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("ダンジョン進行方向の指定が定義外です　error. failed to load \"ダンジョン進行方向\"："),floornumdata->topscript().c_str());
		}
	}

	//-----------------------------------------------------
	//フロアのデータ
	for(i=1;i<=floormax;i++)
	{
		StyleString floorstr;
		floorstr.addstring(i);
		//_itot_s(i,t,8*sizeof(TCHAR),10);
		pcScriptRLayer floordata = data->pmember(floorstr.c_str());

		if(floordata == NULL)
		{//データ記述なし

			vpfloor_.push_back(pcFloor(new cFloor(*pmodelfloor)));

		}
		else
		{
			if(pmodelfloor == NULL)
			{
				pmodelfloor = pcFloor(new cFloor);
			}
			
			vpfloor_.push_back(pcFloor(new cFloor));
			vpfloor_.back()->decodeFloor(pmodelfloor, floordata);
			vpfloor_.back()->floorindex = i;

			pmodelfloor = vpfloor_.back();
		}
		
		vpfloor_.back()->floorrandomSeed = randomgen().genrand_int32();
	}

	
}
int cDungeon::MaxFloor()
{
	return vpfloor_.size();
}
pcFloor cDungeon::pfloor(int floorindex)
{
	if(floorindex < 1) floorindex = 1;//一階から
	if(floorindex > MaxFloor()) floorindex = MaxFloor();
	return vpfloor_.at(floorindex-1);
}
int cDungeon::culcuscore(const pcCharacter pPlayer,
						 const int floorlevel,
						 const int sumTurnCount,
						 const int sumEnemyDefeatNum,
						 const int FirstConditionOfMoney,
						 const pcCharacter pFirstConditionOfPlayer,
						 const bool appreciationSupport)
{
	int score = 0;

	score += pPlayer->EXP*0.01;
	score += (floorlevel -1)*1000;
	score += sumEnemyDefeatNum*50;

	int itemnum = 0;
	int money = FirstConditionOfMoney;
	pcCharacter pchara = pFirstConditionOfPlayer;
	if(pchara != NULL)
	{
		itemnum = sg_pDungeonSystem->ItemSize(pchara->holdItem);
	}
	double fewitembonus = max(0,20 - itemnum)/20.0;
	double nomoneybonus = max(0,1000 - money)/1000.0;
	double nomoneynoitembonus = (int)(itemnum == 0 && money == 0);

	score *= (1.0 + fewitembonus * 0.2 + nomoneybonus * 0.1 + nomoneynoitembonus * 0.2);

	double aveturn = (sumTurnCount+500)/(double)(floorlevel);
	double turnbonus = (aveturn - 500)/1000;
	turnbonus = 1-turnbonus*turnbonus;
	score *= turnbonus*500/(aveturn+100);

	if(!appreciationSupport)
	{
		score *= 1.2;
	}
	return score;
}
const bool cDungeon::allAppreciationOfBullet()
{
	int index = 1;
	if(appreciationpos() >= 1.0)
	{
		return true;
	}
	if(appreciationposkind().size() > index && appreciationposkind()[index] >= 1.0)
	{
		return true;
	}
	return false;
}
const bool cDungeon::allAppreciationOfSpell()
{
	int index = 2;
	if(appreciationpos() >= 1.0)
	{
		return true;
	}
	if(appreciationposkind().size() > index && appreciationposkind()[index] >= 1.0)
	{
		return true;
	}
	return false;
}
const bool cDungeon::allAppreciationOfDrink()
{
	int index = 4;
	if(appreciationpos() >= 1.0)
	{
		return true;
	}
	if(appreciationposkind().size() > index && appreciationposkind()[index] >= 1.0)
	{
		return true;
	}
	return false;
}
const bool cDungeon::allAppreciationOfBook()
{
	int index = 5;
	if(appreciationpos() >= 1.0)
	{
		return true;
	}
	if(appreciationposkind().size() > index && appreciationposkind()[index] >= 1.0)
	{
		return true;
	}
	return false;
}
const bool cDungeon::allAppreciationOfBox()
{
	int index = 6;
	if(appreciationpos() >= 1.0)
	{
		return true;
	}
	if(appreciationposkind().size() > index && appreciationposkind()[index] >= 1.0)
	{
		return true;
	}
	return false;
}
int cDungeon::culcuscore(pcSaveQuest pQuest)
{
	return culcuscore(pQuest->pPlayer,
				pQuest->floor,
				pQuest->Sumturn,
				pQuest->SumdefeatNum,
				pQuest->FirstConditionOfMoney,
				pQuest->pFirstConditionOfPlayer,
				pQuest->privateFlags[cSaveQuest::privateFlagKey_AppreciationSupportKey()]
				);
}
int cDungeon::culcuscore()
{
	return culcuscore(sg_pDungeonSystem->pPlayerChara(),
				sg_pDungeonSystem->FloorLevel(),
				sg_pDungeonSystem->SumTurnCount(),
				sg_pDungeonSystem->SumEnemyDefeatNum(),
				sg_pDungeonSystem->pSaveQuest->FirstConditionOfMoney,
				sg_pDungeonSystem->pSaveQuest->pFirstConditionOfPlayer,
				sg_pDungeonSystem->pSaveQuest->privateFlags[cSaveQuest::privateFlagKey_AppreciationSupportKey()]
				);
/*
	int score = 0;

	score += sg_pDungeonSystem->pPlayerChara()->EXP*0.01;
	score += (sg_pDungeonSystem->FloorLevel() -1)*1000;
	score += sg_pDungeonSystem->SumEnemyDefeatNum()*50;

	int itemnum = 0;
	int money = sg_pDungeonSystem->pSaveQuest->FirstConditionOfMoney;
	pcCharacter pchara = sg_pDungeonSystem->pSaveQuest->pFirstConditionOfPlayer;
	if(pchara != NULL)
	{
		itemnum = sg_pDungeonSystem->ItemSize(pchara->holdItem);
	}
	double fewitembonus = max(0,20 - itemnum)/20.0;
	double nomoneybonus = max(0,1000 - money)/1000.0;
	double nomoneynoitembonus = (int)(itemnum == 0 && money == 0);

	score *= (1.0 + fewitembonus * 0.2 + nomoneybonus * 0.1 + nomoneynoitembonus * 0.2);

	double aveturn = (sg_pDungeonSystem->SumTurnCount()+500)/(double)(sg_pDungeonSystem->FloorLevel());
	double turnbonus = (aveturn - 500)/1000;
	turnbonus = 1-turnbonus*turnbonus;
	score *= turnbonus*500/(aveturn+100);

	return score;
	*/
}

int cParty::enemyfirstsetnum(const int landnum, int floornum)
{
	int sqrtland = sqrt((double)landnum);
	return random_range(enemyfirstsetnums(0),enemyfirstsetnums(1))
		+ sqrtland * random_range(enemyfirstsetnums(2),enemyfirstsetnums(3))
		+ landnum * random_range(enemyfirstsetnums(4),enemyfirstsetnums(5))
		+ floornum * random_range(enemyfirstsetnums(4),enemyfirstsetnums(5));
}
int cParty::trapfirstsetnum(const int landnum, int floornum)
{
	int sqrtland = sqrt((double)landnum);
	return random_range(trapfirstsetnums(0),trapfirstsetnums(1))
		+ sqrtland * random_range(trapfirstsetnums(2),trapfirstsetnums(3))
		+ landnum * random_range(trapfirstsetnums(4),trapfirstsetnums(5))
		+ floornum * random_range(trapfirstsetnums(4),trapfirstsetnums(5));
}
int cParty::itemfirstsetnum(const int landnum, int floornum)
{
	int sqrtland = sqrt((double)landnum);
	return random_range(itemfirstsetnums(0),itemfirstsetnums(1))
		+ sqrtland * random_range(itemfirstsetnums(2),itemfirstsetnums(3))
		+ landnum * random_range(itemfirstsetnums(4),itemfirstsetnums(5))
		+ floornum * random_range(itemfirstsetnums(4),itemfirstsetnums(5));
}

cDiscreteProbability cParty::enemyDP()
{
	if(enemyDPexist)
		return enemyDP_;
	else
		return sg_pDungeonSystem->pDungeon()->pfloor(sg_pDungeonSystem->FloorLevel())->enemyDP();
}
cDropingDistribution cParty::dropitemDP()
{
	if(dropitemDPexist)
		return dropitemDP_;
	else
		return sg_pDungeonSystem->pDungeon()->pfloor(sg_pDungeonSystem->FloorLevel())->dropitemDP();
}
cDiscreteProbability cParty::trapDP()
{
	if(trapDPexist)
		return trapDP_;
	else
		return sg_pDungeonSystem->pDungeon()->pfloor(sg_pDungeonSystem->FloorLevel())->trapDP();
}