#pragma once
#include "utility/VariationNumber.h"
#include "cAnimation.h"
#include "utility/cNumField.h"
#include "gameMainSystem/cRenderBackend.h"
#include <tstring_ph.h>
#include <LuaUtility/LuaStringUtility.h>

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
class AnimeChip;
typedef boost::shared_ptr<AnimeChip> pAnimeChip;
typedef boost::weak_ptr<AnimeChip> wpAnimeChip;

typedef cRenderVector3 PositionPerGrid;
typedef cRenderVector4 TextureRangeRect;
typedef cRenderVector2 BeltRange;
typedef cRenderVector2 PerticleSize;

//-----------------------------------------------------
template <class T> T& pointer_to_ref(boost::shared_ptr<T> this_pointer)
{
	return *this_pointer;
};
template <class T> pAnimeChip cast_to_pAnimeChip(boost::shared_ptr<T> this_pointer)
{
	return boost::dynamic_pointer_cast<AnimeChip>(this_pointer);
};
//-----------------------------------------------------
class AnimeChip;
typedef boost::shared_ptr<AnimeChip> pAnimeChip;
class AnimeChip :
	public cAnimation
{
public:
	AnimeChip(void);
public:
	virtual ~AnimeChip(void);


	virtual int Process() = 0;
	virtual int process(cRenderDevice *pDev){return Process();};


	//これを呼び出すと後は全て描画される
	virtual int Draw(cRenderDevice *pDev) = 0;
	virtual int Init(pcAnimation me) = 0;

protected:
	int count_;
protected:
	wpAnimeChip wparent_;
public:
	pAnimeChip parent(){return wparent_.lock();};
	void setParent(pAnimeChip parent){wparent_ = wpAnimeChip(parent);};
public:
	pAnimeChip me_pAnimeChip()
	{return boost::dynamic_pointer_cast<AnimeChip>(me());};
	
	//Endフラグは以下全てのselfEndがtrueのときに立つ
	virtual bool selfEnd() = 0;

	virtual PositionPerGrid worldPosition();
	virtual PositionPerGrid localPosition() = 0;

	virtual bool isOnScreenJudgeFunction() = 0;
	virtual bool isOnScreenNowJudgeFunction() = 0;
	virtual bool isOnScreen() = 0;
	virtual bool isOnScreenNow() = 0;

	//セットすると常にOnScreenとなる
	virtual void setAlwaysOnScreen(bool flag) = 0;

	static bool isOnScreenStatic(const PositionPerGrid& position, double marginInGrid);
};

//-------------------------------------------------
//リスト
//-------------------------------------------------
class AnimeChip_Chain;
typedef boost::shared_ptr<AnimeChip_Chain> pAnimeChip_Chain;
class AnimeChip_Chain :
	public AnimeChip
{
public:
	AnimeChip_Chain(void);
public:
	virtual ~AnimeChip_Chain(void);

	virtual int Draw(cRenderDevice *pDev);
	virtual int Process();
	virtual int Init(pcAnimation me);

protected:
	pAnimeChip nextchip_;
	bool alwaysOnScreen_;
public:
	void addNextChip(pAnimeChip animechip);

protected:
	int delay_;
	int delayCount_;
	int life_;
	//xy座標平面状で回転
	double worldRotation_;
public:
	void setLife(const int life){life_ = life;};
	void setDelay(const int delay){delay_ = delay;};
	void setWorldRotation(double rotation){worldRotation_ = rotation;};
public:
	virtual void selfDraw(cRenderDevice *pDev) = 0;
	virtual void selfProcessInitialize(){return;};
	virtual void selfProcess() = 0;
	virtual void selfProcessFinalize(){return;};
	//Endフラグは以下全てのselfEndがtrueのときに立つ
	virtual bool selfEnd();

	virtual bool isOnScreen();
	virtual bool isOnScreenNow();
	//virtual void copy(const caEchip_Basic & chip);
	
	virtual void setAlwaysOnScreen(bool flag);
};

//-------------------------------------------------
//葉（リスト）
//-------------------------------------------------
class AnimeChip_DrawableObject;
typedef boost::shared_ptr<AnimeChip_DrawableObject> pAnimeChip_DrawableObject;
class AnimeChip_DrawableObject :
	public AnimeChip_Chain
{
public:
	AnimeChip_DrawableObject(void);
public:
	virtual ~AnimeChip_DrawableObject(void);
protected:

public:

	virtual PositionPerGrid localPosition();

	VariationValue<double> scaleX;
	VariationValue<double> scaleY;

	VariationValue<cColor> color;

	VariationValue<PositionPerGrid> position;

	VariationValue<int> colorBlendMode;
	VariationValue<int> drawMode;

	VariationValue<TextureRangeRect> textureRange;
	
	virtual bool isOnScreenJudgeFunction();
	virtual bool isOnScreenNowJudgeFunction();

	virtual void setTexture(const tstring& textureFileName) = 0;
	virtual void setTexture(const pLuaString textureFileName);

	//virtual void copy(const caEchip_Basic & chip);
};

//-------------------------------------------------
//ハブ（支点）
//-------------------------------------------------
class AnimeChip_Hab;
typedef boost::shared_ptr<AnimeChip_Hab> pAnimeChip_Hab;
class AnimeChip_Hab :
	public AnimeChip
{
public:
	AnimeChip_Hab();
	AnimeChip_Hab(const PositionPerGrid& pos);
public:
	virtual ~AnimeChip_Hab(void);

	static pAnimeChip_Hab new_shared_ptr()
	{
		return pAnimeChip_Hab(new AnimeChip_Hab);
	};

	virtual void selfProcess(){return;};
	virtual void selfDraw(cRenderDevice *pDev){return;};
	virtual int Process();
	virtual int Draw(cRenderDevice *pDev);
	virtual bool selfEnd(){return true;};
	virtual int Init(pcAnimation me);
	virtual PositionPerGrid localPosition();

protected:
	std::vector<pAnimeChip> chipVector_;
	bool alwaysOnScreen_;
public:
	virtual void addAnimeChip(pAnimeChip animechip);
	virtual void clearAnimechip();

	virtual bool childEmpty();

	virtual bool isOnScreenJudgeFunction();
	virtual bool isOnScreenNowJudgeFunction();
	virtual bool isOnScreen();
	virtual bool isOnScreenNow();

	virtual void setAlwaysOnScreen(bool flag);

	VariationValue<PositionPerGrid> position;
};

//-------------------------------------------------
//基礎
//-------------------------------------------------
class AnimeChip_Perticle;
typedef boost::shared_ptr<AnimeChip_Perticle> pAnimeChip_Perticle;
class AnimeChip_Perticle :
	public AnimeChip_DrawableObject
{
public:
	AnimeChip_Perticle(const int life);
	AnimeChip_Perticle(const pLuaString filename, const PerticleSize& perticleSize, const int life);
public:
	virtual ~AnimeChip_Perticle(void);

	static pAnimeChip_Perticle new_shared_ptr(const int life)
	{
		return pAnimeChip_Perticle(new AnimeChip_Perticle(life));
	};

	virtual void selfProcess();

	virtual void selfDraw(cRenderDevice *pDev);

	cRenderVector2 perticleSize_;
public:

	VariationValue<double> rotationBefore;
	VariationValue<double> rotationAfter;

	virtual void setTexture(const tstring& textureFileName);

	virtual void setPerticleSize(const PerticleSize& perticleSize);

protected:

	cDrawingObject DO;
};

//-------------------------------------------------
//数字
//-------------------------------------------------
class AnimeChip_Num;
typedef boost::shared_ptr<AnimeChip_Num> pAnimeChip_Num;
class AnimeChip_Num :
	public AnimeChip_DrawableObject
{
public:
	AnimeChip_Num(const int life);
public:
	virtual ~AnimeChip_Num(void);

	virtual void selfProcess();

	virtual void selfDraw(cRenderDevice *pDev);

	cNumField::PositionArrangeX arrangeX_;
	cNumField::PositionArrangeY arrangeY_;
public:
	VariationValue<double> fontSize;
	VariationValue<double> captionValue;
	VariationValue<int> fontIndex;
	VariationValue<int> fontSeparateDistance;

	void setArrangeX(cNumField::PositionArrangeX arrangeX);
	void setArrangeY(cNumField::PositionArrangeY arrangeY);
	//void setTexture(const tstring& textureFileName)
private:
	cNumField num_;

};
typedef boost::shared_ptr<AnimeChip_Num> pAnimeChip_Num;

//-------------------------------------------------
//ベルト
//-------------------------------------------------
class AnimeChip_Belt;
typedef boost::shared_ptr<AnimeChip_Belt> pAnimeChip_Belt;
class AnimeChip_Belt :
	public AnimeChip_DrawableObject
{
public:
	AnimeChip_Belt(const pLuaString filename, const PositionPerGrid& leftStartingPoint, const PositionPerGrid& rightStartingPoint, const int life);
	AnimeChip_Belt(const int life);
public:
	virtual ~AnimeChip_Belt(void);
	static pAnimeChip_Belt new_shared_ptr(const int life)
	{
		return pAnimeChip_Belt(new AnimeChip_Belt(life));
	};

	virtual void selfProcess();

	virtual void selfDraw(cRenderDevice *pDev);

	//精度（ポリゴン数/2）
	int Accuracy_;
	PositionPerGrid leftEdgeStartingPositionalPoint_;
	PositionPerGrid rightEdgeStartingPositionalPoint_;

	bool loop_;
public:

	std::vector<VariationValue<PositionPerGrid>> leftEdge;
	std::vector<VariationValue<PositionPerGrid>> rightEdge;
	VariationValue<BeltRange> drawRange;

	virtual bool isOnScreenJudgeFunction();
	virtual bool isOnScreenNowJudgeFunction();

	void setAccuracy(int accuracy);
	void setLoop(bool loop);
	void setLeftEdgeStartingPositionalPoint(const PositionPerGrid& position);
	void setRightEdgeStartingPositionalPoint(const PositionPerGrid& position);
	virtual void setTexture(const tstring& textureFileName);
private:
	cDrawingObject_belt DOb;


};
typedef boost::shared_ptr<AnimeChip_Belt> pAnimeChip_Belt;

//-------------------------------------------------
//バウンド
//-------------------------------------------------
class AnimeChip_Bound;
typedef boost::shared_ptr<AnimeChip_Bound> pAnimeChip_Bound;
class AnimeChip_Bound :
	public AnimeChip_Perticle
{
public:
	AnimeChip_Bound(const int life);
	AnimeChip_Bound(const pLuaString filename, const PerticleSize& perticleSize, const PositionPerGrid& startVector, const int life);
public:
	virtual ~AnimeChip_Bound(void);

	virtual void selfProcess();

	//virtual void selfDraw(cRenderDevice *pDev);

	virtual bool selfEnd();
public:

	VariationValue<double> gravity;
	VariationValue<double> floorHeight;
	VariationValue<double> restitution;
	VariationValue<double> airResistance;

	virtual PositionPerGrid localPosition();

	void setStartVector(const PositionPerGrid& startVector);
	void setBoundLimit(const int boundLimit);

private:
	int boundCount_;
	int boundLimit_;
	PositionPerGrid speedVector_;
	PositionPerGrid extraPosition_;
};

//-------------------------------------------------
//音声
//-------------------------------------------------
class AnimeChip_Sound;
typedef boost::shared_ptr<AnimeChip_Sound> pAnimeChip_Sound;
class AnimeChip_Sound :
	public AnimeChip_Chain
{
public:
	AnimeChip_Sound(void);
	AnimeChip_Sound(const tstring& soundFileName, const PositionPerGrid& position,const double volume);
	AnimeChip_Sound(const pLuaString soundFileName, const PositionPerGrid& position,const double volume);
public:
	virtual ~AnimeChip_Sound(void);
	static pAnimeChip_Sound new_shared_ptr(const tstring& soundFileName, const PositionPerGrid& position,const double volume)
	{
		pAnimeChip_Sound sound = pAnimeChip_Sound(new AnimeChip_Sound(soundFileName, position, volume));
		return sound;
	};

	virtual void selfDraw(cRenderDevice *pDev);
	virtual void selfProcess();
	virtual bool selfEnd();
public:

	virtual PositionPerGrid localPosition();

	virtual void setSoundFile(const tstring& soundFileName);
	virtual void setSoundFile(const pLuaString soundFileName);
	virtual void setPosition(const PositionPerGrid& position);
	virtual void setVolume(double volume);//0~1

	virtual bool isOnScreenJudgeFunction();
	virtual bool isOnScreenNowJudgeFunction();
private:
	tstring soundFileName_;
	int didSoundFrag_;
	double volume_;
	PositionPerGrid position_;
};

//-------------------------------------------------
//キャラいじり
//-------------------------------------------------
class cCharacter;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCharacter> pcCharacter;
class AnimeChip_Chara;
typedef boost::shared_ptr<AnimeChip_Chara> pAnimeChip_Chara;
class AnimeChip_Chara :
	public AnimeChip_Chain
{
public:
	AnimeChip_Chara(pcCharacter pchara, const int life);
public:
	virtual ~AnimeChip_Chara(void);
	static pAnimeChip_Chara new_shared_ptr(pcCharacter pchara, const int life);

	virtual void selfDraw(cRenderDevice *pDev);
	virtual void selfProcess();
	virtual void selfProcessFinalize();
protected:
public:
	virtual PositionPerGrid localPosition();

	VariationValue<double> scaleX;
	VariationValue<double> scaleY;
	VariationValue<double> pitching;
	VariationValue<double> yawing;

	//VariationValue<double> opaque;

	VariationValue<PositionPerGrid> position;

	virtual bool isOnScreenJudgeFunction();
	virtual bool isOnScreenNowJudgeFunction();
private:
	pcCharacter pchara_;
};
//-------------------------------------------------
//地震
//-------------------------------------------------
class AnimeChip_Quake;
typedef boost::shared_ptr<AnimeChip_Quake> pAnimeChip_Quake;
class AnimeChip_Quake :
	public AnimeChip_Chain
{
public:
	AnimeChip_Quake(const int life);
public:
	virtual ~AnimeChip_Quake(void);
	//static pAnimeChip_Quake new_shared_ptr(const int life);

	virtual void selfProcess();
	virtual void selfProcessFinalize();
	virtual void selfDraw(cRenderDevice *pDev);

	virtual PositionPerGrid localPosition();
protected:

		
public:
	VariationValue<double> amplitude;
	VariationValue<double> periodicity;

	VariationValue<double> direction;
	VariationValue<double> random;
private:
	double QuakedX_;
	double QuakedY_;

	double phase_;
	double quakeDirection_;
	double quakeDirection_phase_;

	virtual bool isOnScreenJudgeFunction(){return true;};
	virtual bool isOnScreenNowJudgeFunction(){return true;};
};