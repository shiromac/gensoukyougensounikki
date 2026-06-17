
#include "DropsScript.h"
#include "../cDroping.h"

#include "../utility/debug.h"

#include <cstdlib>

#include "cScriptRLayer.h"

#include "LuaBindModuleLoading.h"
#include "DungeonScriptFunction.h"
#include <LuaUtility/LuaEnvironment.h>
#include <LuaUtility/LuaScript.h>
#include <LuaUtility/LuaStringURL.h>

#define SET_LUABIND_CONSTVALUE(val) setGlobalConstValiable(#val, val)

namespace LuaScript_DropScript{
//ここからLuaScript用
	StyleString* print_Target = NULL;
	std::map<tstring, StyleString >* valiable_Target = NULL;
	wpcScriptRLayer layer_Target;

	pcScriptRLayer urlLayer(const pLuaString url, pcScriptRLayer selfLayer);

	//通常のプリント
	int LuaPrintPath(const pLuaString url)
	{
		if(print_Target == NULL || valiable_Target == NULL || layer_Target.lock() == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		
		pcScriptRLayer pLayer = urlLayer(url,layer_Target.lock());
		pLayer = urlLayer(url,layer_Target.lock());
		if(pLayer == NULL)
		{
			return false;
		}
		Drop_StringAddStyle(pLayer, *print_Target, *valiable_Target);
		return true;
	}
	//生のプリント
	int LuaPrintRaw(const pLuaString str)
	{
		if(print_Target == NULL || valiable_Target == NULL || layer_Target.lock() == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		tstring tstr;
		setLuaString(&tstr, str, strlen(str)+1);

		if(print_Target->sector.size())
		{
			print_Target->addstring(tstr,
									print_Target->colors.back(),
									print_Target->sizes.back().x,
									print_Target->sizes.back().y,
									print_Target->flags.back());
		}
		else
		{
			print_Target->addstring(tstr);
		}
		return true;
	}
	//生のプリント
	int LuaPrintRaw(const pLuaString str, const unsigned int color, const float size_x, const float size_y)
	{
		if(print_Target == NULL || valiable_Target == NULL || layer_Target.lock() == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		tstring tstr;
		setLuaString(&tstr, str, strlen(str)+1);

		unsigned int flag = 0;

		if(print_Target->sector.size())
		{
			flag = print_Target->flags.back();
		}

		if(print_Target->sector.size())
		{
			flag = print_Target->flags.back();
		}

		print_Target->addstring(tstr,
								color,
								size_x,
								size_y,
								flag);

		return true;
	}
	//カラー
	unsigned int colorARGB(const int alpha, const int red, const int green, const int blue)
	{
		cColor c;
		c.ARGB(alpha,red,green,blue);
		return c.D3Dcolor();
	}
	//カラー
	unsigned int colorAHSB(const int alpha, const double hue, const double saturation, const double brightness)
	{
		cColor c;
		c.HSV(hue,saturation,brightness);
		c.alpha = alpha;
		return c.D3Dcolor();
	}

	//生のプリント
	int LuaPrintRaw(const pLuaString str, const int position_x, const int position_y)
	{
		if(print_Target == NULL || valiable_Target == NULL || layer_Target.lock() == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		tstring tstr;
		setLuaString(&tstr, str, strlen(str)+1);

		unsigned int flag = 0;
		unsigned int color = ITEM_NORMAL_COLOR;
		float size_x = 1.0;
		float size_y = 1.0;

		if(print_Target->sector.size())
		{
			flag = print_Target->flags.back();
			color = print_Target->colors.back();
			size_x = print_Target->sizes.back().x;
			size_y = print_Target->sizes.back().y;
		}

		if(position_x >= 0)
		{
			flag = (flag & ~(STYLES_POSX_MASK | STYLES_POSX_FLAG)) | (( position_x << STYLES_POSX_FLOOR ) & STYLES_POSX_MASK) | STYLES_POSX_FLAG;
		}

		if(position_y >= 0)
		{
			flag = (flag & ~(STYLES_POSY_MASK | STYLES_POSY_FLAG)) | (( position_y << STYLES_POSY_FLOOR ) & STYLES_POSY_MASK) | STYLES_POSY_FLAG;
		}

		print_Target->addstring(tstr,
								color,
								size_x,
								size_y,
								flag);

		return true;
	}

	
	int LuaPrint(const StyleString& sstr)
	{
		if(print_Target == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		
		print_Target->addstring(sstr);
		return true;
	}

	//通常のプリント
	int LuaSetPath(StyleString& target, const pLuaString url)
	{
		if( valiable_Target == NULL || layer_Target.lock() == NULL)
		{
			OnAssert((TCHAR*)__FILE__,__LINE__,false,(TCHAR*)_T("error"),(TCHAR*)_T("LuaPrintでprint_Targetが選択されていないエラー"));
			return false;
		}
		
		pcScriptRLayer pLayer = urlLayer(url,layer_Target.lock());
		pLayer = urlLayer(url,layer_Target.lock());
		if(pLayer == NULL)
		{
			return false;
		}
		Drop_StringAddStyle(pLayer, target, *valiable_Target);
		return true;
	}

	pcScriptRLayer urlLayer(const pLuaString url, pcScriptRLayer selfLayer)
	{
		tstring uniString;
		LuaStringURL urlstr(url);
		pcScriptRLayer nowLayer = selfLayer;
		pcScriptRLayer templayer;
		tTokenizer tokanizer = urlstr.getLocation(&uniString);
		tTokenizer::iterator token = tokanizer.begin();
		for(;token != tokanizer.end(); token++)
		{
			templayer = nowLayer->path2Layer(*token);
			if(templayer != NULL)
			{
				nowLayer = templayer;
			}
			else
			{
				return NULLOFcScriptRLayer;
			}
		}
		return nowLayer;
	}

	const int SETVALIABLE_STRLEN = 16;
	
	class DropScript_LuaEnvironment :public LuaEnvironment
	{
	protected:
		//std::map<tstring, pluaString> 
	public:
		DropScript_LuaEnvironment(	StyleString* print_target,
									std::map<tstring, StyleString >* valiable_target,
									wpcScriptRLayer layer_target,
									::std::basic_string<LuaString> name):LuaEnvironment(name)
		{
			print_Target = print_target;
			valiable_Target = valiable_target;
			layer_Target = layer_target;
		};
		virtual ~DropScript_LuaEnvironment(void){};
		virtual void luabind_module()
		{

			LuaBindModule::StyleString_Loading(*this);

			luabind::module(luaState_)
			[
				luabind::def("random", &forLua::random_independent),
				luabind::def("textPrintPath", &LuaPrintPath),
				luabind::def("textSetPath", &LuaSetPath),
				luabind::def("textPrint", &LuaPrint),
				luabind::def("textPrintRaw", (int(*)(const pLuaString))&LuaPrintRaw),
				luabind::def("textPrintRaw", (int(*)(const pLuaString, const unsigned int, const float, const float))&LuaPrintRaw),
				luabind::def("textPrintRaw", (int(*)(const pLuaString, const int, const int))&LuaPrintRaw),
				luabind::def("colorARGB", (unsigned int(*)(const int, const int, const int, const int))&colorARGB),
				luabind::def("colorAHSB", (unsigned int(*)(const int, const double, const double, const double))&colorAHSB)
			];
			SET_LUABIND_CONSTVALUE(ITEM_NORMAL_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_CAPTION_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_CURSE_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_GOOD_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_NONAPP_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_HALFAPP_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_QUARAPP_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_NOTICE_COLOR);
			SET_LUABIND_CONSTVALUE(ITEM_NOTICE_COLOR2);
			SET_LUABIND_CONSTVALUE(ITEM_NOTICE_COLOR3);
			SET_LUABIND_CONSTVALUE(ITEM_NOTICE_COLOR4);
			SET_LUABIND_CONSTVALUE(ITEM_MITAMA_COLOR);


			LuaBindModule::ItemExplanationScript_Loading(*this);
		};

		void setValiable(std::map<tstring, StyleString>& valiable)
		{
			std::map<tstring, StyleString>::iterator itr = valiable.begin();
	
			for(;itr != valiable.end();itr++)
			{
				LuaStringTransformer luastr_name(itr->first);
				LuaStringTransformer luastr_value(itr->second.conclete_tstr());
				
				setGlobalConstValiable(luastr_name.lua_str(), luastr_value.lua_str());
			}
		};
	};
}
int Drop_StringAddStyle(pcScriptRLayer player, StyleString & sstr, std::map<tstring, StyleString > & valiable )
{

	vector<tstring>& vstr = player->script();
	vector<tstring>::iterator itrstr = vstr.begin();
	tstring::iterator itrchar;

	unsigned long color, flag;
	cRenderVector2 size;
	size.x = 1;
	size.y = 1;
	color = 0xFFFFFFFF;
	flag = 0;

	if(sstr.sector.size())
	{
		flag = sstr.flags.back();
		color = sstr.colors.back();
		size.x = sstr.sizes.back().x;
		size.y = sstr.sizes.back().y;
	}

	int line = 0;

	for(;itrstr!=vstr.end();itrstr++)
	{
		line++;

		if(itrstr->empty()) continue;//空行

		itrchar = itrstr->begin();
		for(; itrchar != itrstr->end(); itrchar++)
		{//文字列の最初から
			if(*itrchar == _T(' ') || *itrchar == _T('\t'))
			{//ホワイトスペース類
				continue;
			}
			else
			{
				break;
			}
		}

		//先頭ホワイトスペース類を排除した




		for(; itrchar != itrstr->end();)
		{
			bool command = false;

			if(*itrchar == _T('#'))
			{//コマンド
				command = true;
				itrchar++;
				for(; itrchar != itrstr->end();)
				{
					if(*itrchar == _T('#'))
					{//エスケープシーケンス
						command = false;
						break;
					}

					//引数をとらないものは連続して命令できる
					else if(*itrchar == _T('n'))
					{//改行
						sstr.addstring(_T("\n"),color,size.x,size.y,flag);
						break;
					}
					else if(*itrchar == _T('i'))
					{//初期化
						size.x = 1;
						size.y = 1;
						color = 0xFFFFFFFF;
						flag = 0;
						break;
					}
					
					//引数を取る
					else if(*itrchar == _T('c'))
					{//色
						itrchar++; if(itrchar == itrstr->end()) break;
						if(*itrchar == _T('d'))
						{//定義
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							
							if(op == _T("NOTICE"))
							{
								color = ITEM_NOTICE_COLOR;
							}							
							else if(op == _T("NOTICE2"))
							{
								color = ITEM_NOTICE_COLOR2;
							}
							else if(op == _T("NOTICE3"))
							{
								color = ITEM_NOTICE_COLOR3;
							}	
							else if(op == _T("NOTICE4"))
							{
								color = ITEM_NOTICE_COLOR4;
							}	
							else if(op == _T("NORMAL"))
							{
								color = ITEM_NORMAL_COLOR;
							}

						}
						else if(*itrchar == _T('v'))
						{//即値
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							TCHAR** end = NULL;
							color = _tcstoul(op.c_str(), end, 16);
						}

						itrchar = itrstr->end(); break;
					}
					else if(*itrchar == _T('s'))
					{//サイズ
						itrchar++; if(itrchar == itrstr->end()) break;
						if(*itrchar == _T('x'))
						{
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							size.x = _tstof(op.c_str());
						}
						else if(*itrchar == _T('y'))
						{
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							size.y = _tstof(op.c_str());
						}
						itrchar = itrstr->end(); break;
					}

					else if(*itrchar == _T('p'))
					{//位置補正
						itrchar++; if(itrchar == itrstr->end()) break;
						if(*itrchar == _T('x'))
						{
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							int px = tstrtoi(op,_T(" #px "));
							flag = (flag & ~(STYLES_POSX_MASK | STYLES_POSX_FLAG)) | (( px << STYLES_POSX_FLOOR ) & STYLES_POSX_MASK) | STYLES_POSX_FLAG;
						}
						else if(*itrchar == _T('y'))
						{
							itrchar++; if(itrchar == itrstr->end()) break;
							tstring op(itrchar,itrstr->end());
							int py = tstrtoi(op,_T(" #py "));
							flag = (flag & ~(STYLES_POSY_MASK | STYLES_POSY_FLAG)) | (( py << STYLES_POSY_FLOOR ) & STYLES_POSY_MASK) | STYLES_POSY_FLAG;
						}
						itrchar = itrstr->end(); break;
					}
					
					//引数を取る
					else if(*itrchar == _T('f'))
					{//フラグ
						tstring op(itrchar,itrstr->end());
						TCHAR** end = NULL;
						flag = _tcstol(op.c_str(), end, 16);
						itrchar = itrstr->end(); break;
					}

					else if(*itrchar == _T('!'))
					{//スクリプト
						itrchar++; if(itrchar == itrstr->end()) break;
						tstring op(itrchar,itrstr->end());
						pLuaScript pcontent = player->pcontent(op);
						pLuaScript pscript = boost::dynamic_pointer_cast<LuaScript>(pcontent);
						if(pscript == NULL)
						{
							OnAssert(_T(""), line ,false, op.c_str(),_T(" : そのようなLuaスクリプトはありません。 <error> LuaScript Not Found"));
						}
						else
						{
							LuaScript_DropScript::DropScript_LuaEnvironment luaEnv(&sstr, &valiable, wpcScriptRLayer(player), tString2luastring(op));
							LuaStringTransformer oplua(op);
							
							luaEnv.init();
							luaEnv.setValiable(valiable);

							if(luaEnv.runLuaScriptWithBuffer(pscript->buffer(),pscript->buffersize(),oplua.lua_str()))
							{
								tstring filename = _T("Error_LuaScript_DropScript_");
								filename += player->name();
								filename += _T(".txt");

								luaEnv.outputErrorFile(filename);
							}

							if(sstr.sector.size())
							{//フィードバック
								flag = sstr.flags.back();
								color = sstr.colors.back();
								size.x = sstr.sizes.back().x;
								size.y = sstr.sizes.back().y;
							}
						}

						itrchar = itrstr->end();break;
						
					}
					//プログラム内変数を取る
					else if(*itrchar == _T('*'))
					{//フラグ
						itrchar++; if(itrchar == itrstr->end()) break;
						tstring op(itrchar,itrstr->end());
						std::map<tstring, StyleString >::iterator itr(valiable.find(op));
						if(itr == valiable.end())
						{
							//OnAssert(_T(""), line ,false, op.c_str(),_T(" : そのような名前の変数はありません。 <error> Valiable Not Found"));
							tstring tstr = _T("[#*")+op+_T("]");
							sstr.addstring(tstr);
						}
						else
						{
							if(!itr->second.colors.empty() && itr->second.isColorDefault())
							{//色がデフォルトなら色をつける
								StyleString tmp(itr->second);
								tmp.setSameColor(color);
								sstr.addstring(tmp);
							}
							else
							{
								int size = sstr.flags.size();
								if(itr->second.conclete_tstr().empty())
								{
									itr->second.addstring(_T(""));
								}
								sstr.addstring(itr->second);
								sstr.flags[size] = flag;
							}
						}

						//フラグの一部毎回リセット
						flag = (flag & ~(STYLES_POSX_MASK | STYLES_POSX_FLAG | STYLES_POSY_FLAG | STYLES_POSY_MASK));

						itrchar = itrstr->end();break;
						

					}
					else if(*itrchar == _T('@'))
					{//定義済みフラグ
						itrchar++; if(itrchar == itrstr->end()) break;
						tstring op(itrchar,itrstr->end());
						
						if(op == _T("CLEAR"))
						{
							flag = flag | STYLES_FLAG_MESSAGE_CLEAR;
						}							

						itrchar = itrstr->end(); break;
					}

					if(itrchar != itrstr->end()) itrchar++;
				}
			}

			if(!command)
			{//コマンド以外
				sstr.addstring(tstring(itrchar,itrstr->end()),color,size.x,size.y,flag);
				
				//フラグの一部毎回リセット
				flag = (flag & ~(STYLES_POSX_MASK | STYLES_POSX_FLAG | STYLES_POSY_FLAG | STYLES_POSY_MASK));

				itrchar = itrstr->end(); break;
			}

			if(itrchar != itrstr->end()) itrchar++;
		}
		
	}
	

	return true;

}
int Drop_StringAddStyle(pcScriptRLayer player, StyleString & sstr)
{

	std::map<tstring, StyleString > valiable;
	return Drop_StringAddStyle(player, sstr, valiable);
}