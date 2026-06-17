#pragma once
#include "gameMainSystem/cRenderBackend.h"



typedef cRenderVector4 TextureRangeRect;
//-------------------------------------------------
//汎用エフェクト
//-------------------------------------------------
namespace EffectFunctions
{

	void 上吹き飛ばし風エフェクト(const double placeX,const double placeY, int shortflag = 0);


	//もわっ
	void 煙エフェクト1(const double placeX,const double placeY);


	//ドッカン
	void 爆発エフェクト_中(const double placeX,const double placeY, double color_H);



	void 夢想封印(const double fromplaceX,const double fromplaceY, const double placeX,const double placeY, double color_H);


	void 地震(int time_frame, int amplitude);

	void 三歩必殺(const double placeX,const double placeY);


	void 特殊能力発揮エフェクト(const double placeX,const double placeY,const double placeZ);

	void ボスエフェクト集中(const double placeX,const double placeY,const double placeZ);
	void ボスエフェクトため(const double placeX,const double placeY, cColor imagecolor1, cColor imagecolor2);
	void ボスエフェクト発散(const double placeX,const double placeY, cColor imagecolor1, cColor imagecolor2);

	void ボスエフェクト撃破(const double placeX,const double placeY,const double placeZ);

	void 落雷エフェクト(const double placeX,const double placeY);

	void ダメージ火エフェクト(const double placeX,const double placeY);
	void ダメージ水エフェクト(const double placeX,const double placeY);
	void ダメージ電気エフェクト(const double placeX,const double placeY);
	void ダメージ冷気エフェクト(const double placeX,const double placeY);
	void ダメージ爆発エフェクト(const double placeX,const double placeY);
	void ダメージ気エフェクト(const double placeX,const double placeY);

	void つるべ落としエフェクト(const double placeX,const double placeY);

	void 氷付けエフェクト(const double placeX,const double placeY);
	void 氷付けエフェクト_短(const double placeX,const double placeY);

	void パワーアップエフェクト(const double placeX,const double placeY, double color_H);
	void パワーダウンエフェクト(const double placeX,const double placeY, double color_H);

	void Mob29エフェクト(const double placeX,const double placeY);

	void Mob31エフェクト(const double placeX,const double placeY);


	void Mob31エフェクト(const double placeX,const double placeY);

	void 弾幕体当たり(const double placeX,const double placeY, pcCharacter pchara, const int aspect);

	
	void スタンプ(const double placeX, const double placeY, tstring fileName, const TextureRangeRect texRenge, cColor burnColor, const double size, const int duration, const int delay);

	void 霧(const double placeX, const double placeY, tstring fileName, const TextureRangeRect texRenge, cColor burnColor, const double size, const double generateRadius, const double generateRatio, const int duration, const int delay);

	class SpellEffectPart_class {
	public:
		enum SpellEffectPart {
			SpellEffectPart_Main = 0,
			SpellEffectPart_Effected,
			SpellEffectPart_NotEffected,
		};
	};
	void スペルエフェクト(const double placeX, const double placeY, const int ID, const SpellEffectPart_class::SpellEffectPart part);
};