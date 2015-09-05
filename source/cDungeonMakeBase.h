#pragma once

#include "cDrawingObject.h"

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cRectObj> pcRectObj;

#include <boost/shared_ptr.hpp>
struct s_rect {
  boolean done_split_v;
  boolean done_split_h;
  cRectObj rect;
  pcRectObj room;
};
typedef boost::shared_ptr<s_rect> ps_rect;

struct s_couple {
  int v_or_h;
  ps_rect rect0, rect1; 
};
typedef boost::shared_ptr<s_couple> ps_couple;


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class cDungeonMakeBase 
{
public:
	cDungeonMakeBase(void){};
public:
	virtual ~cDungeonMakeBase(void){};


protected:
	//区画
	vector<ps_rect> MD_rectList;
	//部屋
	vector<pcRectObj> MD_roomList;
	//通路
	vector<pcRectObj> MD_lineList;
	//ペア(奇数と偶数のペア)
	vector<ps_couple> MD_pairList;

	//水路
	vector<pcRectObj> MD_waterlineList;

};
