#pragma once
#include "cdroping.h"
#include <tstring_ph.h>
//コマンドを解決する;
int FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList, pcDroping pdrop);

//Fireできるときtrue;
int isCanFireCommand(pcDroping subject, vector<pcDroping> &ObjectList, tstring verb);

//オブジェクト(目的語)リスト、案内を得る;
void GetmenuFuncObjectsList(pcDroping subject, vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb);


//共通
int 説明(cRenderDevice *pDev, pcDroping pdrop);
int 置く(cRenderDevice *pDev, pcDroping pdrop);
int 拾う(cRenderDevice *pDev);
int 投げる(cRenderDevice *pDev, pcDroping pdrop);
int 交換(cRenderDevice *pDev, pcDroping pdrop);
int メモ(cRenderDevice *pDev, pcDroping pdrop);
int 名前(cRenderDevice *pDev, pcDroping pdrop);

//準共通
int 装備(cRenderDevice *pDev, pcDroping pdrop);
int はずす(cRenderDevice *pDev, pcDroping pdrop);


//book
int 読む(cRenderDevice *pDev, pcDroping pdrop);
int 読み要請(pcCharacter pchara, pcDroping pdrop);

//Box
int 入れる(cRenderDevice *pDev, pcDroping pdrop, pcDroping pbox);
int 入れ要請(pcDroping pdrop, pcDroping pbox);
int 倉庫に入れる(cRenderDevice *pDev, pcDroping pdrop, pcDroping pstore);
int 倉庫にまとめて入れる(cRenderDevice *pDev, vector<pcDroping>& pdroplist, pcDroping pstore);

int 見る(cRenderDevice *pDev, pcDroping pbox);
int 見る要請(pcDroping pbox);

int のぞく(cRenderDevice *pDev, pcDroping pbox);
int のぞく要請(pcDroping pbox);

int 開ける(cRenderDevice *pDev, pcDroping pbox);
int 開け要請(pcDroping pbox);

int 振る(cRenderDevice *pDev, pcDroping pbox);
int 開け要請(pcDroping pbox);

int 倉庫から出す(cRenderDevice *pDev, pcDroping pdrop, pcDroping pstore);
int 倉庫からまとめて出す(cRenderDevice *pDev, vector<pcDroping>& pdroplist, pcDroping pstore);

int 出す(cRenderDevice *pDev, pcDroping pdrop, pcDroping pbox);
//まとめて
int まとめて出す(cRenderDevice *pDev, vector<pcDroping>& pdroplist);

int 出し要請(pcDroping pdrop, pcCharacter receiver, pcDroping pbox);

int 入れ替え(cRenderDevice *pDev, pcDroping pdrop, pcDroping pdrop2, pcDroping pbox);
int 入れ替え要請(pcDroping pdrop, pcDroping pdrop2, pcDroping pbox);

//Bullet
int Bullet装備(cRenderDevice *pDev, pcDroping pdrop);
int Bullet装備要請(pcCharacter pchara, pcDroping pdrop);

int Bulletはずす(cRenderDevice *pDev, pcDroping pdrop);
int Bullet装備はずし要請(pcCharacter pchara, pcDroping pdrop);

int 撃つ(cRenderDevice *pDev, pcDroping pdrop);
int 撃ち要請(pcCharacter pchara, pcDroping pdrop);

//Drink
int 飲む(cRenderDevice *pDev, pcDroping pdrop);
int 飲み要請(pcCharacter pchara, pcDroping pdrop);
int 汲む(cRenderDevice *pDev, pcDroping pdrop);
int 汲み要請(pcCharacter pchara, pcDroping pdrop);
int カクテル(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop);
int カクテル要請(pcCharacter pchara, vector<pcDroping> &ObjectList, pcDroping pdrop);

//Equipment
int Equipment装備(cRenderDevice *pDev, int part, pcDroping pdrop);
int Equipment装備選択(cRenderDevice *pDev, pcDroping pdrop);
int Equipment装備要請(pcCharacter pchara, int part, pcDroping pdrop);

int Equipmentはずす(cRenderDevice *pDev, pcDroping pdrop);
int Equipment装備はずし要請(pcCharacter pchara, pcDroping pdrop);

int 能力(cRenderDevice *pDev, pcDroping pdrop);

//Food
int 食べる(cRenderDevice *pDev, pcDroping pdrop);
int 食べ要請(pcCharacter pchara, pcDroping pdrop);

//Spell
int Spell装備(cRenderDevice *pDev, int part, pcDroping pdrop);
int Spell装備選択(cRenderDevice *pDev, pcDroping pdrop);
int Spell装備要請(pcCharacter pchara, int part, pcDroping pdrop);

int Spellはずす(cRenderDevice *pDev, pcDroping pdrop);
int Spell装備はずし要請(pcCharacter pchara, pcDroping pdrop);

int 破壊(cRenderDevice *pDev, pcDroping pdrop);
int 破壊要請(pcCharacter pchara, pcDroping pdrop);

int 宣言(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop);
int 宣言要請(pcCharacter pchara ,vector<pcDroping> &ObjectList, pcDroping pdrop);
int 宣言強制要請(pcCharacter pchara , pcDroping pdrop);

int スキル(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop);
int スキル要請(pcCharacter pchara ,vector<pcDroping> &ObjectList, pcDroping pdrop);

//trap
int 踏む(cRenderDevice *pDev, pcDroping pdrop);
int 踏み要請(pcDroping pdrop);



//stair
int 上る(cRenderDevice *pDev, pcDroping pdrop);
int 下りる(cRenderDevice *pDev, pcDroping pdrop);
int やめる(cRenderDevice *pDev, pcDroping pdrop);
int 一旦中断する(cRenderDevice *pDev);
int 昇降要請();

//stair
int 上る(cRenderDevice *pDev, pcDroping pdrop);
int 下りる(cRenderDevice *pDev, pcDroping pdrop);

//Shop
int キープする(cRenderDevice *pDev, pcDroping pdrop);
int キープをやめる(cRenderDevice *pDev, pcDroping pdrop);
int ポイント交換(cRenderDevice *pDev, pcDroping pdrop);
int 買う(cRenderDevice *pDev, pcDroping pdrop);
int 売る(cRenderDevice *pDev, pcDroping pdrop);
int 店買う(cRenderDevice *pDev, pcDroping pdrop);
int 店売る(cRenderDevice *pDev, pcDroping pdrop);
int まとめて買う(cRenderDevice *pDev, vector<pcDroping>& pdroplist);
int まとめて売る(cRenderDevice *pDev, vector<pcDroping>& pdroplist);
int まとめて店買う(cRenderDevice *pDev, vector<pcDroping>& pdroplist);
int まとめて店売る(cRenderDevice *pDev, vector<pcDroping>& pdroplist);

extern vector<pcDroping> 足元リスト;
extern vector<pcDroping> エスケープリスト;