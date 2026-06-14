#include "cLandform.h"
#include "utility/4DVector.h"
#include "cDungeonSystem.h"

cLandform::cLandform(void)
{
	place.Set(-1,-1);
	placeX = -1;
	placeY = -1;
	Constractsub();
}
cLandform::cLandform(int x,int y)
{
	place.Set(x,y);
	placeX = x;
	placeY = y;
	Constractsub();
	Dangerflag_ = Darkflag_ = 0;
}

void cLandform::Constractsub(void)
{

	kind = MAPKIND_FLOOR;
	mapped = false;
	//minimapDrawed = false;
	RoomIndex = -1;
	pOnDrop = NULLDROP;
	pOnChar = NULLCHARA;

}

cLandform::~cLandform(void)
{
	
}


const int cLandform::kind_surface_visible()
{
	if(MAPKIND_WALLEGDE == visiblekind
		|| MAPKIND_FRAGILEWALL == visiblekind)
	{
		return MAPKIND_WALL;
	}
	else
	{
		return visiblekind;
	}
}

const int cLandform::kind_surface()
{
	if(MAPKIND_WALLEGDE == kind
		|| MAPKIND_FRAGILEWALL == kind)
	{
		return MAPKIND_WALL;
	}
	else
	{
		return kind;
	}
}

//通り抜け可、不可
int cLandform::through(int waterwalk,int wallwalk, int airwalk)
{
	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return false | wallwalk;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return true;

	case MAPKIND_FLOOR2://通路の床
		return true;

	case MAPKIND_WATER://水路
		return false | waterwalk;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return false;

	case MAPKIND_ABYSS://奈落
		return false | airwalk;
	}
	return false;
}

//アイテム配置可、不可
int cLandform::canputdrop()
{




	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return false;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return true;

	case MAPKIND_FLOOR2://通路の床
		return true;

	case MAPKIND_WATER://水路
		return true;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return false;

	case MAPKIND_ABYSS://奈落
		return false;

	}
	return false;
}

int cLandform::diagonthrough(int waterwalk,int wallwalk, int airwalk)
{
	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return false | wallwalk;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return true;

	case MAPKIND_FLOOR2://通路の床
		return true;

	case MAPKIND_WATER://水路
		return true;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return false;

	case MAPKIND_ABYSS://奈落
		return true;
	}
	return false;
}

int cLandform::iswall()
{
	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return true;

	case MAPKIND_WALL2://固い壁
		return true;

	case MAPKIND_WALLEGDE://マップ端の壁
		return true;

	case MAPKIND_FLOOR://部屋の床
		return false;

	case MAPKIND_FLOOR2://通路の床
		return false;

	case MAPKIND_WATER://水路
		return false;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return true;

	case MAPKIND_ABYSS://奈落
		return false;
	}
	return false;
}
int cLandform::iswater()
{
	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return false;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return false;

	case MAPKIND_FLOOR2://通路の床
		return false;

	case MAPKIND_WATER://水路
		return true;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return false;

	case MAPKIND_ABYSS://奈落
		return false;
	}
	return false;
}


int cLandform::caninto(int waterwalk,int wallwalk, int airwalk)
{
	if(pOnChar != NULL)
	{//誰かいる
		return false;
	}

	return through(waterwalk,wallwalk,airwalk);
}
//隣接マスの種別
int cLandform::visibleneighbor(int aspect)
{
	cCoordinate coo;
	coo.SetAspect(aspect);
	coo += place;
	return sg_pDungeonSystem->Map().Land(coo)->kind_surface_visible();
}
int cLandform::neighbor(int aspect)
{
	cCoordinate coo;
	coo.SetAspect(aspect);
	coo += place;
	return sg_pDungeonSystem->Map().Land(coo)->kind_surface();
}
void cLandform::CutIn(タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(kind == MAPKIND_FRAGILEWALL)
		{
			sg_pDungeonSystem->穴掘り(sg_pDungeonSystem->Map().Land(placeX,placeY),false);
			sg_pDungeonSystem->地形整理();
		}
	}
}

int cLandform::breakable()
{
	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return true;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return false;

	case MAPKIND_FLOOR2://通路の床
		return false;

	case MAPKIND_WATER://水路
		return false;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return true;

	case MAPKIND_ABYSS://奈落
		return false;
	}
	return false;
}

//乾き可
int cLandform::driable()
{

	switch(kind)
	{
	case MAPKIND_WALL://普通の壁
		return false;

	case MAPKIND_WALL2://固い壁
		return false;

	case MAPKIND_WALLEGDE://マップ端の壁
		return false;

	case MAPKIND_FLOOR://部屋の床
		return false;

	case MAPKIND_FLOOR2://通路の床
		return false;

	case MAPKIND_WATER://水路
		return true;

	case MAPKIND_FRAGILEWALL://壊れやすい壁
		return false;

	case MAPKIND_ABYSS://奈落
		return false;
	}
	return false;
}

int cLandform::setAttribute(int kind_ID)
{
	kind = kind_ID;
	kindFlag_ = 0;
	switch(kind_ID)
	{
	case MAPKIND_WALL://普通の壁
		kindFlag_ |= 1<<0;
		break;
	case MAPKIND_WALL2://固い壁
		kindFlag_ |= 1<<0;
		break;
	case MAPKIND_WALLEGDE://マップ端の壁
		kindFlag_ |= 1<<0;
		break;
	case MAPKIND_FLOOR://部屋の床
		kindFlag_ |= 1<<1;
		break;
	case MAPKIND_FLOOR2://通路の床
		kindFlag_ |= 1<<2;
		break;
	case MAPKIND_WATER://水路
		kindFlag_ |= 1<<3;
		break;
	case MAPKIND_FRAGILEWALL://壊れやすい壁
		kindFlag_ |= 1<<0;
		break;
	case MAPKIND_ABYSS://奈落
		return false;
		kindFlag_ |= 1<<4;
		break;
	}
	return kind;
}
int cLandform::getAttribute()
{
	return kind;
}
//見た目のリセット
int cLandform::visibleReset()
{
	visiblekind = kind;
	return true;
}

void cLandform::process_decoration()
{
	getdecochip(cMap::DECORATIONLAYER_LAND1).process();
	getdecochip(cMap::DECORATIONLAYER_LAND2).process();
	getdecochip(cMap::DECORATIONLAYER_LAND3).process();
	getdecochip(cMap::DECORATIONLAYER_SKY).process();
	getdecochip(cMap::DECORATIONLAYER_SKY2).process();
}
cLandform::cDecochip::cDecochip()
{
	decorationanimecount_ = 0;
	decorationanimefase_ = 0;
	decorationID_ = -1;
}
cLandform::cDecochip::~cDecochip()
{

}
void cLandform::cDecochip::process()
{
	if(decorationchip_.size() < 2) return;
	decorationanimecount_++;
	if(decorationanimenext_ <= decorationanimecount_)
	{
		decorationanimefase_++;
		if(decorationanimefase_ >= decorationchip_.size()) decorationanimefase_ = 0;
		decorationanimenext_ = decorationanimeframe_[decorationanimefase_].first + rand()*(decorationanimeframe_[decorationanimefase_].second - decorationanimeframe_[decorationanimefase_].first +1)/(RAND_MAX+1);
		decorationanimecount_ = 0;
	}
}
dChipTexPos cLandform::cDecochip::getDecorationchip()
{
	if(decorationchip_.size() <= decorationanimefase_) return -1;
	return decorationchip_[decorationanimefase_];
}
const dChipTexPos cLandform::getDecorationchip(int decorationlayer)
{
	cDecochip& dc = getdecochip(decorationlayer);
	return dc.getDecorationchip();
}
const int cLandform::getDecorationID(int decorationlayer)
{
	return getdecochip(decorationlayer).decorationID_;
}
const int cLandform::getDecorationDrawMode(int decorationlayer)
{
	return getdecochip(decorationlayer).decorationdrawmode_;
}
int cLandform::getDecorationLocation()//占有状況を得る
{
	int Flag = (int)(Deco_[0].decorationID_ != -1)
		| ((int)(Deco_[1].decorationID_ != -1)<<1)
		| ((int)(Deco_[2].decorationID_ != -1)<<2)
		| ((int)(Deco_[3].decorationID_ != -1)<<3)
		| ((int)(Deco_[4].decorationID_ != -1)<<4);
	return Flag;
}
void cLandform::SetDecoration(dChipTexPos pos, int decorationID, int decorationlayer, int drawmode, int decorationanimeframe_min, int decorationanimeframe_max)
{
	cDecochip& dc = getdecochip(decorationlayer);
	if(decorationlayer != cMap::DECORATIONLAYER_NULL)
	{
		dc.decorationchip_.push_back(pos);
		dc.decorationID_ = decorationID;
		dc.decorationdrawmode_ = drawmode;
		if(decorationanimeframe_max == -1) decorationanimeframe_max = decorationanimeframe_min;
		if(dc.decorationanimeframe_.empty())
		{
			dc.decorationanimenext_ = decorationanimeframe_min + rand()*(decorationanimeframe_max - decorationanimeframe_min +1)/(RAND_MAX+1);
		}
		dc.decorationanimeframe_.push_back(pair<int,int>(decorationanimeframe_min,decorationanimeframe_max));
	}

	decorationConditionID_.insert(decorationID);
}
void cLandform::DeleteDecorationConditionID(int decorationID)
{
	set<int>::iterator itr = decorationConditionID_.find(decorationID);
	if(itr != decorationConditionID_.end())
	{
		decorationConditionID_.erase(itr);
	}
	int i;
	for(i=0;i<LANDFORM_DECO_LAYERNUM;i++)
	{
		if(Deco_[i].decorationID_ == decorationID)
		{
			Deco_[i].decorationID_ = -1;
			Deco_[i].decorationanimecount_ = 0;
			Deco_[i].decorationanimenext_ = 0;
			Deco_[i].decorationdrawmode_ = 0;
			Deco_[i].decorationanimefase_ = 0;
			Deco_[i].decorationchip_.clear();
			Deco_[i].decorationanimeframe_.clear();
		}
	}
}
const set<int>& cLandform::getDecorationConditionID()
{
	return decorationConditionID_;
}
	
cLandform::cDecochip& cLandform::getdecochip(int decorationlayer)
{
	if(decorationlayer >= 0 && decorationlayer < LANDFORM_DECO_LAYERNUM)
	{
		return Deco_[decorationlayer];
	}
	return Deco_[0];
}
	


//-----------------------------------------------------------------
//cCoordinate
//-----------------------------------------------------------------
cCoordinate::cCoordinate():x(0),y(0)
{
}
cCoordinate::cCoordinate(int ix, int iy):x(ix),y(iy)
{
}
cCoordinate::~cCoordinate()
{

}

void cCoordinate::Set(int x_int,int y_int)
{
	x = x_int;
	y = y_int;
}

//方向を得る。
int cCoordinate::GetAspect()
{
	int a;
	double d;
	c4DVector v;

	v.set(x,y,0,0);
	d = degree(v);
	if(d == -360)
	{
		a = ASPECT_DOWN;
	}
	else if(d > 360 * 1 / 16 && d <= 360 * 3 / 16 )
	{
		a = ASPECT_DOWNRIGHT;
	}
	else if(d > 360 * 3 / 16 && d <= 360 * 5 / 16 )
	{
		a = ASPECT_DOWN;
	}
	else if(d > 360 * 5 / 16 && d <= 360 * 7 / 16 )
	{
		a = ASPECT_DOWNLEFT;
	}
	else if(d > 360 * 7 / 16 && d <= 360 * 9 / 16 )
	{
		a = ASPECT_LEFT;
	}
	else if(d > 360 * 9 / 16 && d <= 360 * 11 / 16 )
	{
		a = ASPECT_UPLEFT;
	}
	else if(d > 360 * 11 / 16 && d <= 360 * 13 / 16 )
	{
		a = ASPECT_UP;
	}
	else if(d > 360 * 13 / 16 && d <= 360 * 15 / 16 )
	{
		a = ASPECT_UPRIGHT;
	}
	else
	{
		a = ASPECT_RIGHT;
	}


	return a;
}

//方向を与える。周囲８マスを得る
void cCoordinate::SetAspect(int aspect)
{
	aspect = safeAspect(aspect);
	switch(aspect)
	{
	case ASPECT_UP:
		Set(0,-1);
		break;
	case ASPECT_UPRIGHT:
		Set(1,-1);
		break;
	case ASPECT_RIGHT:
		Set(1,0);
		break;
	case ASPECT_DOWNRIGHT:
		Set(1,1);
		break;
	case ASPECT_DOWN:
		Set(0,1);
		break;
	case ASPECT_DOWNLEFT:
		Set(-1,1);
		break;
	case ASPECT_LEFT:
		Set(-1,0);
		break;
	case ASPECT_UPLEFT:
		Set(-1,-1);
		break;
	}
}

//距離を得る(到達するために必要なターン)
int cCoordinate::dif()
{
	return (abs(x) < abs(y))? abs(y):abs(x);
}

	//-180 ~ 180 右回り正
double cCoordinate::calcuDegree(const cCoordinate& rother)
{
	c4DVector a(x, y, 0, 0) ,b(rother.x, rother.y, 0, 0);
	double absDegree = degree(a,b);
	if(absDegree == -360) {
		return 0;
	}
	c4DVector crossProduct = exproduct(a,b);
	if(crossProduct.z < 0){
		absDegree *= -1;
	}
	return absDegree;
}
void cCoordinate::operator +=(const cCoordinate& rother)
{
	x += rother.x;
	y += rother.y;
}

void cCoordinate::operator -=(const cCoordinate& rother)
{
	x -= rother.x;
	y -= rother.y;
}

cCoordinate& cCoordinate::operator =(const cCoordinate& rother)
{
	x = rother.x;
	y = rother.y;
	return *this;
}
bool cCoordinate::operator ==(const cCoordinate& rother)
{
	return (x == rother.x && y == rother.y);
}
cCoordinate cCoordinate::operator +(const cCoordinate& rother)
{
	cCoordinate c;
	c.x = x + rother.x;
	c.y = y + rother.y;
	return c;
}

cCoordinate cCoordinate::operator -(const cCoordinate& rother)
{
	cCoordinate c;
	c.x = x - rother.x;
	c.y = y - rother.y;
	return c;
}

cCoordinate cCoordinate::operator *(const int rother)
{
	cCoordinate c;
	c.x = x * rother;
	c.y = y * rother;
	return c;
}
void cCoordinate::operator *=(const int rother)
{
	x *= rother;
	y *= rother;
}

cCoordinate cCoordinate::operator /(const int rother)
{
	cCoordinate c;
	c.x = x / rother;
	c.y = y / rother;
	return c;
}