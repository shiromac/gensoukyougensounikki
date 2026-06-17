#include "AnimationScript.h"
#include "../cDungeonSystem.h"
#include "../AnimeChips.h"
#include "../cDroping.h"

#include "../utility/debug.h"

#include <cstdlib>

#include "cScriptRLayer.h"
#include "LuaBindModuleLoading.h"
	
#include "../gameMainSystem/cRenderBackend.h"
#include <math.h>

#include <LuaUtility/LuaEnvironment.h>
#include <LuaUtility/LuaScript.h>
#include <LuaUtility/LuaStringURL.h>
/*
#define SET_LUABIND_setAnimation(CHIP) \
	void setAnimation_##CHIP##_wait(p##CHIP pchip){\
		sg_pDungeonSystem->AnimationManager().AddAnime(boost::dynamic_pointer_cast<AnimeChip>(pchip));};\
	void setAnimation_##CHIP##_parallel(p##CHIP pchip){\
		sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::dynamic_pointer_cast<AnimeChip>(pchip), 0);};

*/
void setAnimation_wait(pAnimeChip pchip)
{
	sg_pDungeonSystem->AnimationManager().AddAnime(pchip);
}
void setAnimation_parallel(pAnimeChip pchip)
{
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(pchip);
}
double random_for_effect()
{
	return ((double)rand()) / ((double)RAND_MAX+1);
}
double sqrt_for_effect(double value)
{
	return sqrt(value);
}
class AnimationScript_LuaEnvironment;


#define SET_LUABIND_SHARED_PTR(class_name) \
	//luabind::class_<p##class_name>("p"#class_name)\
		.def("cast2pAnimeChip", (pAnimeChip(*)(p##class_name))&cast_to_pAnimeChip),\
	luabind::def("pointer2ref", (class_name&(*)(p##class_name))&pointer_to_ref)
	

#define SET_LUABIND_VariationValue(class_name) \
	luabind::class_<VariationValue<class_name>>("VariationValue_"#class_name)\
		.def(luabind::constructor<>())\
		.def(luabind::constructor<const class_name&>())\
		.def("empty", &VariationValue<class_name>::empty)\
		.def("value", &VariationValue<class_name>::value)\
		.def("sumLength", &VariationValue<class_name>::sumLength)\
		.def("setOutRangeMode", &VariationValue<class_name>::setOutRangeMode)\
		.def("setDefault", &VariationValue<class_name>::setDefault)\
		.def("addCascade", (void(VariationValue<class_name>::*)(const class_name&, double))&VariationValue<class_name>::addCascade)\
		.def("addCascade", (void(VariationValue<class_name>::*)(const class_name&, const class_name&, double))&VariationValue<class_name>::addCascade)\
		.def("addCascade", (void(VariationValue<class_name>::*)(const class_name&, const class_name&, const class_name&, double))&VariationValue<class_name>::addCascade)\
		.def("addCascade", (void(VariationValue<class_name>::*)(const class_name&, const class_name&, const class_name&, const class_name&, double))&VariationValue<class_name>::addCascade)\
		.def("clear",  &VariationValue<class_name>::clear)\
/*
#define SET_LUABIND_setAnimationFunction(CHIP) \
	luabind::def("setAnimation_wait", (void(*)(p##CHIP))&setAnimation_##CHIP##_wait),\
	luabind::def("setAnimation_parallel", (void(*)(p##CHIP))&setAnimation_##CHIP##_parallel)
*/
#define SET_LUABIND_ALL_ANIMACHI_PMACRO(CHIP) \
SET_LUABIND_SHARED_PTR(CHIP)
//SET_LUABIND_setAnimationFunction(CHIP)


#define D3DWRAPED_FUNCTION4(function_name, return_value, first_type, second_type, third_type, fourth_type) \
	return_value function_name##_forLua(first_type x, second_type y, third_type z, fourth_type w){\
	return function_name(x,y,z,w);\
}
#define D3DWRAPED_FUNCTION3(function_name, return_value, first_type, second_type, third_type) \
	return_value function_name##_forLua(first_type x, second_type y, third_type z){\
	return function_name(x,y,z);\
}
#define D3DWRAPED_FUNCTION2(function_name, return_value, first_type, second_type) \
	return_value function_name##_forLua(first_type x, second_type y){\
	return function_name(x,y);\
}

D3DWRAPED_FUNCTION2(cRenderMatrixRotationX, cRenderMatrix*, cRenderMatrix*, float)
D3DWRAPED_FUNCTION2(cRenderMatrixRotationY, cRenderMatrix*, cRenderMatrix*, float)
D3DWRAPED_FUNCTION2(cRenderMatrixRotationZ, cRenderMatrix*, cRenderMatrix*, float)
D3DWRAPED_FUNCTION3(cRenderVec3TransformCoord, cRenderVector3*, cRenderVector3*, const cRenderVector3*, const cRenderMatrix*)
D3DWRAPED_FUNCTION4(cRenderMatrixLookAtLH, cRenderMatrix*, cRenderMatrix*, const cRenderVector3*, const cRenderVector3*, const cRenderVector3*)
D3DWRAPED_FUNCTION4(cRenderMatrixLookAtRH, cRenderMatrix*, cRenderMatrix*, const cRenderVector3*, const cRenderVector3*, const cRenderVector3*)
D3DWRAPED_FUNCTION4(cRenderMatrixRotationYawPitchRoll, cRenderMatrix*, cRenderMatrix*, float, float, float)
D3DWRAPED_FUNCTION3(cRenderQuaternionRotationAxis, cRenderQuaternion*, cRenderQuaternion*, const cRenderVector3*, float)
D3DWRAPED_FUNCTION2(cRenderMatrixRotationQuaternion, cRenderMatrix*, cRenderMatrix*, const cRenderQuaternion*)
D3DWRAPED_FUNCTION2(cRenderVec3Normalize, cRenderVector3*, cRenderVector3*, const cRenderVector3*)

cRenderVector3 *cRenderVec3RotateForLua(
   cRenderVector3 *pOut,
   const cRenderVector3 *pAxis,      // ‰ñ“]Ž²
   float Angle,                             // ‰ñ“]Šp“x
   const cRenderVector3 *pV          // ‰ñ“]‚³‚¹‚½‚¢ˆÊ’uƒxƒNƒgƒ‹
)
{
	// ‰ñ“]Ž²‚ÆŠp“x‚©‚çƒNƒH[ƒ^ƒjƒIƒ“‚ðì¬
	cRenderQuaternion Q, R, Qv, Out;
	cRenderQuaternionRotationAxis(&Q, pAxis, Angle);
	cRenderQuaternionConjugate(&R, &Q);

	// ƒxƒNƒgƒ‹‰ñ“]
	cRenderVector3 V;
	float fLength = cRenderVec3Length(pV);   // ’·‚³
	cRenderVec3Normalize(&V, pV);   // ‰ñ“]‚³‚¹‚éƒxƒNƒgƒ‹‚Ì•W€‰»
	Qv.x = V.x;
	Qv.y = V.y;
	Qv.z = V.z;
	Qv.w = 0.0f;
	cRenderQuaternionMultiply(&Qv, &R, &Qv);
	cRenderQuaternionMultiply(&Qv, &Qv, &Q);

	pOut->x = Qv.x * fLength;
	pOut->y = Qv.y * fLength;
	pOut->z = Qv.z * fLength;
	
	return pOut;
}
cRenderVector3 *cRenderVec3XYPlaneRotateForLua(
   cRenderVector3 *pV,          // ‰ñ“]‚³‚¹‚½‚¢ˆÊ’uƒxƒNƒgƒ‹
   float Angle                             // ‰ñ“]Šp“x
)
{
	cRenderVector3 axis(0,0,1);
	return cRenderVec3RotateForLua(
	pV,
	&axis,      // ‰ñ“]Ž²
	Angle,                             // ‰ñ“]Šp“x
	pV          // ‰ñ“]‚³‚¹‚½‚¢ˆÊ’uƒxƒNƒgƒ‹
		);
}
double radianForDegree( 
   float degree                             // ‰ñ“]Šp“x
)
{
	return degree*3.14159265358679/180;
}
class AnimationScript_LuaEnvironment :public LuaEnvironment
{
protected:
	//std::map<tstring, pluaString> 
public:
	AnimationScript_LuaEnvironment(::std::basic_string<LuaString> name);
	virtual ~AnimationScript_LuaEnvironment(void);
public:

	virtual void luabind_module();

	void setValiable(cValiableField& valiable);

};
AnimationScript_LuaEnvironment::AnimationScript_LuaEnvironment(::std::basic_string<LuaString> name):LuaEnvironment(name)
{};
AnimationScript_LuaEnvironment::~AnimationScript_LuaEnvironment(void)
{};
void AnimationScript_LuaEnvironment::luabind_module()
{
	LuaBindModule::StyleString_Loading(*this);
	LuaBindModule::Character_private_Loading(*this);
	LuaBindModule::AnimationScript_Loading(*this);
	LuaBindModule::EffectScript_Loading(*this);
}

void LuaBindModule::AnimationScript_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		SET_LUABIND_VariationValue(double)
		,
		luabind::class_<cColor>("Color")
			.def(luabind::constructor<>())
			.def(luabind::constructor<unsigned long>())
			.def(luabind::constructor<int,int,int,int>())
			.def(luabind::constructor<cColor&>())
			.def_readwrite("red", &cColor::red)
			.def_readwrite("green", &cColor::green)
			.def_readwrite("blue", &cColor::blue)
			.def_readwrite("alpha", &cColor::alpha)
			.def("formalize", &cColor::formalize)
			.def("HSV", &cColor::HSV)
			.def("ARGB", &cColor::ARGB)
			.def("D3Dcolor", &cColor::D3Dcolor)
			.def("inputD3Dcolor", &cColor::inputD3Dcolor)
			.def(luabind::self * cColor())
			.def(luabind::self + cColor())
			.def(luabind::self == cColor())
		,
		SET_LUABIND_VariationValue(cColor)
		
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<PositionPerGrid>("PositionPerGrid")
			.def(luabind::constructor<>())
			.def(luabind::constructor<float,float,float>())
			.def(luabind::constructor<PositionPerGrid>())
			.def_readwrite("x", &PositionPerGrid::x)
			.def_readwrite("y", &PositionPerGrid::y)
			.def_readwrite("z", &PositionPerGrid::z)
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self + PositionPerGrid())
			.def(luabind::self - PositionPerGrid())
			.def(luabind::self == PositionPerGrid())
		,
		SET_LUABIND_VariationValue(PositionPerGrid)
		,
		SET_LUABIND_VariationValue(int)
	];

	luabind::module(luaEnvironment.luaState())
	[
		/*
		SET_LUABIND_VariationValue(BeltRange)
		,
		luabind::class_<BeltRange>("BeltRange")
			.def(luabind::constructor<>())
			.def(luabind::constructor<float,float>())
			.def(luabind::constructor<BeltRange>())
			.def_readwrite("first", &BeltRange::x)
			.def_readwrite("last", &BeltRange::y)
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self + BeltRange())
			.def(luabind::self - BeltRange())
			.def(luabind::self == BeltRange())
		,
		luabind::class_<PerticleSize>("PerticleSize")
			.def(luabind::constructor<>())
			.def(luabind::constructor<float,float>())
			.def(luabind::constructor<PerticleSize>())
			.def_readwrite("x", &PerticleSize::x)
			.def_readwrite("y", &PerticleSize::y)
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self + PerticleSize())
			.def(luabind::self - PerticleSize())
			.def(luabind::self == PerticleSize())
			*/
		

		luabind::class_<cRenderVector2>("D3DXVECTOR2")
			.def(luabind::constructor<>())
			.def(luabind::constructor<float,float>())
			.def(luabind::constructor<cRenderVector2>())
			.def_readwrite("x", &cRenderVector2::x)
			.def_readwrite("y", &cRenderVector2::y)
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self + cRenderVector2())
			.def(luabind::self - cRenderVector2())
			.def(luabind::self == cRenderVector2())
		,
		SET_LUABIND_VariationValue(cRenderVector2)
		
		
	];
	luabind::module(luaEnvironment.luaState())
	[
		
		luabind::class_<TextureRangeRect>("TextureRangeRect")
			.def(luabind::constructor<>())
			.def(luabind::constructor<TextureRangeRect&>())
			.def(luabind::constructor<float,float,float,float>())
			.def_readwrite("x1", &TextureRangeRect::x)
			.def_readwrite("y1", &TextureRangeRect::y)
			.def_readwrite("x2", &TextureRangeRect::z)
			.def_readwrite("y2", &TextureRangeRect::w)
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self + TextureRangeRect())
			.def(luabind::self - TextureRangeRect())
			.def(luabind::self == TextureRangeRect())
		,
		SET_LUABIND_VariationValue(TextureRangeRect)
		
	];
	/*
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<std::vector<VariationValue<PositionPerGrid>>>("vector_VariationValue_PositionPerGrid")\
			.def("empty", &std::vector<VariationValue<PositionPerGrid>>::empty)\
			.def("clear", &std::vector<VariationValue<PositionPerGrid>>::clear)\
			.def("size", &std::vector<VariationValue<PositionPerGrid>>::size)\
			.def("push_back", &std::vector<VariationValue<PositionPerGrid>>::push_back)\
	];
	*/
	vector_Loading<VariationValue<PositionPerGrid> >(luaEnvironment,"VariationValue_PositionPerGrid");
	luabind::module(luaEnvironment.luaState())
	[
		//D3DXƒ‰ƒCƒuƒ‰ƒŠŒn
		luabind::class_<cRenderMatrix>("D3DXMATRIX")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cRenderMatrix&>())
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self * cRenderMatrix())
			.def(luabind::self + cRenderMatrix())
			.def(luabind::self - cRenderMatrix())
			.def(luabind::self == cRenderMatrix())
		,
		luabind::class_<cRenderQuaternion>("D3DXQUATERNION")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cRenderQuaternion&>())
			.def(luabind::self * float())
			.def(luabind::self / float())
			.def(luabind::self * cRenderQuaternion())
			.def(luabind::self + cRenderQuaternion())
			.def(luabind::self - cRenderQuaternion())
			.def(luabind::self == cRenderQuaternion())
		,
		luabind::def("D3DXMatrixRotationX", (cRenderMatrix*(*)(cRenderMatrix*, float))&cRenderMatrixRotationX_forLua)
		,
		luabind::def("D3DXMatrixRotationY", (cRenderMatrix*(*)(cRenderMatrix*, float))&cRenderMatrixRotationY_forLua)
		,
		luabind::def("D3DXMatrixRotationZ", (cRenderMatrix*(*)(cRenderMatrix*, float))&cRenderMatrixRotationZ_forLua)
		,
		luabind::def("D3DXVec3TransformCoord", (cRenderVector3 *(*)(cRenderVector3*, const cRenderVector3*, const cRenderMatrix*))&cRenderVec3TransformCoord_forLua)
		,
		luabind::def("D3DXMatrixLookAtLH", (cRenderMatrix*(*)(cRenderMatrix*, const cRenderVector3*, const cRenderVector3*, const cRenderVector3*))&cRenderMatrixLookAtLH_forLua)
		,
		luabind::def("D3DXMatrixLookAtRH", (cRenderMatrix*(*)(cRenderMatrix*, const cRenderVector3*, const cRenderVector3*, const cRenderVector3*))&cRenderMatrixLookAtRH_forLua)
		,
		luabind::def("D3DXMatrixRotationYawPitchRoll", (cRenderMatrix*(*)(cRenderMatrix*, float, float, float))&cRenderMatrixRotationYawPitchRoll_forLua)
		,
		luabind::def("D3DXQuaternionRotationAxis", (cRenderQuaternion*(*)(cRenderQuaternion*, const cRenderVector3*, float))&cRenderQuaternionRotationAxis_forLua)
		,
		luabind::def("D3DXMatrixRotationQuaternion", (cRenderMatrix*(*)(cRenderMatrix*, const cRenderQuaternion*))&cRenderMatrixRotationQuaternion_forLua)
		,
		luabind::def("D3DXVec3Rotate", &cRenderVec3RotateForLua)
		,
		luabind::def("D3DXVec3Normalize", &cRenderVec3Normalize_forLua)
		,
		luabind::def("D3DXVec3XYPlaneRotate", &cRenderVec3XYPlaneRotateForLua)
		,
		luabind::def("radianForDegree", &radianForDegree)
		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<AnimeChip,pAnimeChip>("AnimeChip")
		//,
		//luabind::class_<AnimeChip>("AnimeChip")
		,
		luabind::def("setAnimation_wait", (void(*)(pAnimeChip))&setAnimation_wait)
		,
		luabind::def("setAnimation_parallel", (void(*)(pAnimeChip))&setAnimation_parallel)
		,
		luabind::def("random", &random_for_effect)
		,
		luabind::def("sqrt", &sqrt_for_effect)
		,
		luabind::def("sin", (double(*)(double))&sin)
		,
		luabind::def("cos", (double(*)(double))&cos)
		,
		luabind::def("tan", (double(*)(double))&tan)
		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		//SET_LUABIND_ALL_ANIMACHI_PMACRO(AnimeChip_Hab)
		//,
		luabind::class_<AnimeChip_Hab, AnimeChip, pAnimeChip>("AnimeChip_Hab")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const PositionPerGrid&>())
	        .def("addAnimeChip", (void(AnimeChip_Hab::*)(pAnimeChip))&AnimeChip_Hab::addAnimeChip)
            .def("clearAnimechip", (void(AnimeChip_Hab::*)(void))&AnimeChip_Hab::clearAnimechip)
            .def("childEmpty", (bool(AnimeChip_Hab::*)(void))&AnimeChip_Hab::childEmpty)
		,
		luabind::def("pAnimeChip_Hab_new", (pAnimeChip_Hab(*)())&AnimeChip_Hab::new_shared_ptr)
		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<AnimeChip_Chain, AnimeChip, pAnimeChip>("AnimeChip_Chain")
			//.def(luabind::constructor<void>())
            .def("addNextChip", &AnimeChip_Chain::addNextChip)
            .def("setLife", &AnimeChip_Chain::setLife)
            .def("setDelay", &AnimeChip_Chain::setDelay)
			.def("setWorldRotation", &AnimeChip_Chain::setWorldRotation)
		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<AnimeChip_DrawableObject, AnimeChip_Chain, pAnimeChip>("AnimeChip_DrawableObject")
			//.def(luabind::constructor<void>())
            .def_readwrite("scaleX", &AnimeChip_DrawableObject::scaleX)
			.def_readwrite("scaleY", &AnimeChip_DrawableObject::scaleY)
			.def_readwrite("color", &AnimeChip_DrawableObject::color)
			.def_readwrite("position", &AnimeChip_DrawableObject::position)
			.def_readwrite("colorBlendMode", &AnimeChip_DrawableObject::colorBlendMode)
			.def_readwrite("drawMode", &AnimeChip_DrawableObject::drawMode)
			.def_readwrite("textureRange", &AnimeChip_Belt::textureRange)
			.def("setTexture", (void(AnimeChip_DrawableObject::*)(const tstring&))&AnimeChip_DrawableObject::setTexture)
			.def("setTexture", (void(AnimeChip_DrawableObject::*)(const pLuaString))&AnimeChip_DrawableObject::setTexture)
			//,
	];
	luaEnvironment.setGlobalConstValiable("ColorBlendMode_multiple",cDrawableObject::COLOR_BLEND_MULTIPLE);
	luaEnvironment.setGlobalConstValiable("ColorBlendMode_addition",cDrawableObject::COLOR_BLEND_ADDITION);
	luaEnvironment.setGlobalConstValiable("ColorBlendMode_fill",cDrawableObject::COLOR_BLEND_FILL);
	luaEnvironment.setGlobalConstValiable("DrawMode_addition",cDrawableObject::DRAW_MODE_ADDITION);
	luaEnvironment.setGlobalConstValiable("DrawMode_normal",cDrawableObject::DRAW_MODE_NORMAL);
	luaEnvironment.setGlobalConstValiable("DrawMode_substraction",cDrawableObject::DRAW_MODE_SUBSTRACTION);

	luabind::module(luaEnvironment.luaState())
	[
		//SET_LUABIND_ALL_ANIMACHI_PMACRO(AnimeChip_Perticle)
		//,
		luabind::class_<AnimeChip_Perticle, AnimeChip_DrawableObject, AnimeChip_Chain, pAnimeChip>("AnimeChip_Perticle")
			.def(luabind::constructor<AnimeChip_Perticle>())
			.def(luabind::constructor<const pLuaString, const PerticleSize&, const int>())
            .def_readwrite("rotationBefore", &AnimeChip_Perticle::rotationBefore)
			.def_readwrite("rotationAfter", &AnimeChip_Perticle::rotationAfter)
			.def("setPerticleSize", &AnimeChip_Perticle::setPerticleSize)
			//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		//SET_LUABIND_ALL_ANIMACHI_PMACRO(AnimeChip_Perticle)
		//,
		luabind::class_<AnimeChip_Bound, AnimeChip_DrawableObject, AnimeChip_Chain, pAnimeChip>("AnimeChip_Bound")
			.def(luabind::constructor<AnimeChip_Bound>())
			.def(luabind::constructor<const pLuaString, const PerticleSize&, const PositionPerGrid&, const int>())
            .def_readwrite("rotationBefore", &AnimeChip_Bound::rotationBefore)
			.def_readwrite("rotationAfter", &AnimeChip_Bound::rotationAfter)
			.def_readwrite("gravity", &AnimeChip_Bound::gravity)
			.def_readwrite("floorHeight", &AnimeChip_Bound::floorHeight)
			.def_readwrite("restitution", &AnimeChip_Bound::restitution)
			.def_readwrite("airResistance", &AnimeChip_Bound::airResistance)
			.def("setPerticleSize", &AnimeChip_Bound::setPerticleSize)
			.def("setStartVector", &AnimeChip_Bound::setStartVector)
			.def("setBoundLimit", &AnimeChip_Bound::setBoundLimit)
			//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<AnimeChip_Sound, AnimeChip_Chain, pAnimeChip>("AnimeChip_Sound")
			.def(luabind::constructor<const pLuaString, const PositionPerGrid&, const double>())
            .def(luabind::constructor<AnimeChip_Sound>())
			.def("setSoundFile", (void(AnimeChip_Sound::*)(const tstring&))&AnimeChip_Sound::setSoundFile)
			.def("setSoundFile", (void(AnimeChip_Sound::*)(const pLuaString))&AnimeChip_Sound::setSoundFile)
			.def("setPosition", &AnimeChip_Sound::setPosition)
			.def("setVolume", &AnimeChip_Sound::setVolume)
		
		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<AnimeChip_Belt, AnimeChip_DrawableObject, AnimeChip_Chain, pAnimeChip>("AnimeChip_Belt")
			.def(luabind::constructor<const pLuaString, const PositionPerGrid&, const PositionPerGrid&, const int>())
			.def(luabind::constructor<AnimeChip_Belt>())
			.def_readwrite("leftEdge", &AnimeChip_Belt::leftEdge)
			.def_readwrite("rightEdge", &AnimeChip_Belt::rightEdge)
			.def_readwrite("drawRange", &AnimeChip_Belt::drawRange)
			.def("setAccuracy", &AnimeChip_Belt::setAccuracy)
			.def("setLoop", &AnimeChip_Belt::setLoop)
			.def("setLeftEdgeStartingPositionalPoint", &AnimeChip_Belt::setLeftEdgeStartingPositionalPoint)
			.def("setRightEdgeStartingPositionalPoint", &AnimeChip_Belt::setRightEdgeStartingPositionalPoint)

		//,
	];
	luabind::module(luaEnvironment.luaState())
	[
		//SET_LUABIND_ALL_ANIMACHI_PMACRO(AnimeChip_Chara)
		//,
		luabind::class_<AnimeChip_Chara, AnimeChip_Chain, pAnimeChip>("AnimeChip_Chara")
			.def(luabind::constructor<AnimeChip_Chara>())
			.def(luabind::constructor<pcCharacter, const int>())
            .def_readwrite("scaleX", &AnimeChip_Chara::scaleX)
			.def_readwrite("scaleY", &AnimeChip_Chara::scaleY)
			.def_readwrite("pitching", &AnimeChip_Chara::pitching)
			.def_readwrite("yawing", &AnimeChip_Chara::yawing)
			.def_readwrite("position", &AnimeChip_Chara::position)
			//.def_readwrite("opaque", &AnimeChip_Chara::opaque)
			//.def("setWorldRotation", (void(AnimeChip_Chara::*)(int))&AnimeChip_Chara::setWorldRotation)
		
	];
	luabind::module(luaEnvironment.luaState())
	[
		//SET_LUABIND_ALL_ANIMACHI_PMACRO(AnimeChip_Quake)
		//,
		luabind::class_<AnimeChip_Quake, AnimeChip_Chain, pAnimeChip>("AnimeChip_Quake")
			.def(luabind::constructor<AnimeChip_Quake>())
			.def(luabind::constructor<const int>())
            .def_readwrite("amplitude", &AnimeChip_Quake::amplitude)
			.def_readwrite("periodicity", &AnimeChip_Quake::periodicity)
			.def_readwrite("direction", &AnimeChip_Quake::direction)
			.def_readwrite("random", &AnimeChip_Quake::random)

			//.def("setWorldRotation", (void(AnimeChip_Chara::*)(int))&AnimeChip_Chara::setWorldRotation)
		
	];
	luaEnvironment.setGlobalConstValiable("OutRangeMode_Cramp",VariationValue_Enum::Mode_Cramp);
	luaEnvironment.setGlobalConstValiable("OutRangeMode_Wrap",VariationValue_Enum::Mode_Wrap);

	luaEnvironment.setGlobalConstValiable("PixelPerGrid",MAPTEXBOXSIZE*MAPTEXPOWER);
	luaEnvironment.setGlobalConstValiable("MATH_PI",3.14159265358679);
	luaEnvironment.setGlobalConstValiable("EFFECT_TRANS_GRID",EDGELANDWIDTH);
	
}

void AnimationScript_LuaEnvironment::setValiable(cValiableField& valiable)
{
	{
		std::map<tstring, double>& doublemap = valiable.doubles.ValiableStringMap();
		std::map<tstring, double>::iterator double_map_itr = doublemap.begin();
		for(;double_map_itr != doublemap.end();++double_map_itr)
		{
			LuaStringTransformer luastr_name(double_map_itr->first);
			setGlobalConstValiable<double>(luastr_name.lua_str(), double_map_itr->second);
		}
	}
	{
		std::map<tstring, pcCharacter>& charamap = valiable.charas.ValiableStringMap();
		std::map<tstring, pcCharacter>::iterator chara_map_itr = charamap.begin();
		for(;chara_map_itr != charamap.end();++chara_map_itr)
		{
			LuaStringTransformer luastr_name(chara_map_itr->first);
			setGlobalConstValiable<pcCharacter>(luastr_name.lua_str(), chara_map_itr->second);
		}
	}
	{
		std::map<tstring, tstring>& tstringmap = valiable.tstrings.ValiableStringMap();
		std::map<tstring, tstring>::iterator tstring_map_itr = tstringmap.begin();
		for(;tstring_map_itr != tstringmap.end();++tstring_map_itr)
		{
			LuaStringTransformer luastr_name(tstring_map_itr->first);
			setGlobalConstValiable<tstring>(luastr_name.lua_str(), tstring_map_itr->second);
		}
	}
}

int AnimationScript_AddAnimation(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable)
{
	pLuaScript psrc = player->pcontent(contentName);
	pLuaScript pluascript = boost::dynamic_pointer_cast<LuaScript>(psrc);
	
	if(pluascript == NULL)
	{
		return false;
	}

	tstring name(_T("AnimationScript_"));
	name += contentName;
	static AnimationScript_LuaEnvironment luaEnv(tString2luastring(name));
	luaEnv.init();
	luaEnv.setValiable(valiable);

	LuaStringTransformer luastr_contentName(contentName);

	if(luaEnv.runLuaScriptWithBuffer(pluascript->buffer(), pluascript->buffersize(), luastr_contentName.lua_str()))
	{
		tstring filename = _T("Error_LuaScript_AnimationScript_");
		filename += contentName;
		filename += _T(".txt");

		luaEnv.outputErrorFile(filename);
	}

	return true;
}
