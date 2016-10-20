#include "cScriptReader.h"
#include "cFileManager.h"
#include "cScriptRLayer.h"
#include "cDataConverter.h"
#include <LuaUtility/LuaScript.h>
#include <common.h>
using namespace std;

const unsigned int FLAG_ONLY_MACRODEF = 1;
const unsigned int FLAG_MATRIXDECODE = 2;

cScriptReader::cScriptReader(void)
{
	scriptname_ = _T("");

	constsub();
}
cScriptReader::cScriptReader(tstring scriptname)
{
	scriptname_ = scriptname;

	constsub();
}
void cScriptReader::constsub()
{
	pcfm_ = pcFileManager(new cFileManager);
	pcsl_ = pcScriptRLayer(new cScriptRLayer);
	pcsl_->messageOutputTo() = &errormassage_;
	//pcdc_ = pcDataConverter(new cDataConverter);
	makedirpass(scriptname_);
}

cScriptReader::~cScriptReader(void)
{

}

int cScriptReader::save(void)
{
	if(scriptname_ == _T("")) return ERROR_FM_FILENAME_NULL;
	int error = 0;

	error |= pcfm_->saveFile(scriptname_,codes_);
	if(error) return error;

	return SUCCESS;
}
int cScriptReader::load(void)
{
	int error = simpleload();


	error |= decode();
	if(error) return error;

	return SUCCESS;
}
int cScriptReader::loadAndpacked(void)
{
	int error = 0;
	if((error = load()) != SUCCESS)
	{//ロード失敗
		error = 0;
		if((error = loadpacked()) != SUCCESS)
		{//パックロード失敗
			return error;
		}
	}
	else
	{
		savepacked();
		if((error = loadpacked()) != SUCCESS)
		{//パックロード失敗
			return error;
		}
	}

	return SUCCESS;
}

int cScriptReader::savepacked(void)
{
	vector<SByte> data;
	vector<vector<SByte>> vvdata;
	vvdata.resize(5);

	cDataConverter::ConvertVT2VecVecC2VecC(codes_,vvdata[0]);
	cDataConverter::ConvertT2VecC(scriptname_,vvdata[1]);
	cDataConverter::ConvertT2VecC(scriptdir_,vvdata[2]);
	ConvertT2VecC(pcsl_,vvdata[3]);
	cDataConverter::ConvertT2VecC(scriptfilename_,vvdata[4]);

	cDataConverter::VecVecC2VecC(vvdata,data);

	return pcfm_->saveEncryptFile(scriptdir_ + _T("Packed_") + scriptfilename_, data);
}
int cScriptReader::loadpacked(void)
{
	vector<SByte> data;
	vector<vector<SByte>> vvdata;
	int error = 0;
	if(error |= pcfm_->loadEncryptFile(scriptdir_ + _T("Packed_") + scriptfilename_, data))
	{
		return error;
	}

	cDataConverter::VecC2VecVecC(data,vvdata);

	if(vvdata.size() < 5) return ERROR_FM_LOAD_DECODE_ERROR;


	cDataConverter::BackDecodeVecC2VecVecC2VT(codes_,vvdata[0]);
	cDataConverter::BackDecodeVecC2T(scriptname_,vvdata[1]);
	cDataConverter::BackDecodeVecC2T(scriptdir_,vvdata[2]);
	cDataConverter::BackDecodeVecC2T(pcsl_,vvdata[3]);
	cDataConverter::BackDecodeVecC2T(scriptfilename_,vvdata[4]);


	pcsl_->AllsetmessageOutputTo(&errormassage_);

	return SUCCESS;
}
int cScriptReader::simpleload(void)//デコードしないロード
{
	if(scriptname_ == _T("")) return ERROR_FM_FILENAME_NULL;
	int error = 0;

	error |= pcfm_->loadFile(scriptname_,codes_);
	if(error) return error;

	return SUCCESS;
}


void cScriptReader::makedirpass(tstring& filename)
{
	tstring::iterator itr = filename.end();
	

	for(;itr != filename.begin();)
	{
		itr--;

		if(*itr == _T('/') || *itr == _T('\\')) break;
		//if(itr == filename.begin()) break;
	}

	if(itr == filename.begin())
	{
		scriptdir_ = _T("");
		
		return;
	}
	scriptdir_.assign(filename.begin(), ++itr);

	scriptfilename_.assign(itr, filename.end());
}


int cScriptReader::decode(void)
{

	//tstring filename = scriptname();

	makedirpass(scriptname_);

	vector<pcScriptRLayer> vlayer;
	vlayer.push_back(pcsl_);
	pcsl_->name() = scriptname();
	pcsl_->messageOutputTo() = &errormassage_;


	return decode_roop(codes_, vlayer, scriptname_, 0);
}

int cScriptReader::decode_roop(vector<tstring>& codes, vector<pcScriptRLayer>& vlayer, tstring& codename, unsigned int flag)
{
	vector<tstring>::iterator itrstr = codes.begin();
	tstring::iterator itrchar;
	int line = 0;
	int deside_only_macro = 0;

	for(;itrstr!=codes.end();itrstr++)
	{
		line++;

		if(itrstr->empty()) continue;//空行

		itrchar = itrstr->begin();
		for(; itrchar != itrstr->end(); itrchar++)
		{//文字列の最初から
			if(*itrchar == _T(' ')
				|| *itrchar == _T('\t')
				)
			{//ホワイトスペース類
				continue;
			}
			else
			{
				break;
			}
		}

		//先頭ホワイトスペース類を排除した


		bool command = false;

		if(*itrchar == _T('\\') || *itrchar == _T('$'))
		{//コマンド
			command = true;
			itrchar++;
			for(; itrchar != itrstr->end(); itrchar++)
			{
				if(*itrchar == _T('/'))
				{//コメント

					++itrchar;

					if(itrchar != itrstr->end())
					{
						if(*itrchar == _T('>'))
						{
							if(decode_Comment(vlayer, codes, itrstr, codename, line))
							{//エラー
								if(itrstr!=codes.end())
								{
									addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_Comment コメントアウトでエラーが発生しました。<error> error in comment out")));
								}
								else
								{
									addError(vlayer.back(), codename, line, (tstring)_T("[EOF]"), tstring(_T("error cScriptReader::decode_Comment コメントアウトでエラーが発生しました。<error> error in comment out")));
								}
								
							}
							
							break;
						}
						else if(*itrchar == _T('<'))
						{//問答無用構文警告
							addError(vlayer.back(), codename, line, *itrstr, tstring(_T("warning コメントアウト終了コマンドが余分に存在します。<error> extra comment_out_end command")));
							break;
						}
					}

					break;
				}
				else if(*itrchar == _T('\\') || *itrchar == _T('$'))
				{//エスケープシーケンス
					command = false;
					break;
				}
				else if(*itrchar == _T('>'))
				{//新規レイヤー&レイヤーダウン
					if(!(flag & (FLAG_ONLY_MACRODEF | FLAG_MATRIXDECODE)) )
					{//マクロ強制状態でない
						tstring::iterator itr = itrstr->end();
						tstring name(++itrchar, --itr);
						if(decode_newAndDownLayer(vlayer,name))
						{//エラー
							addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_newAndDownLayer この名前のメンバーは作成できませんでした。<error> cannot make this name member")));
						}
						break;
					}
					else
					{
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー ここでは新規レイヤーを作成できません。")));
						break;
					}
				}
				else if(*itrchar == _T('<'))
				{//レイヤーアップ
					if(!(flag & (FLAG_ONLY_MACRODEF | FLAG_MATRIXDECODE)))
					{//マクロ強制状態でない
						if(decode_UpLayer(vlayer))
						{
							addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_UpLayer 上位の階層構造が無いため階層構造をさかのぼれません。<error> Cannot layer up. Not exist upper layer")));
						}

						break;
					}
					else
					{
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー ここではレイヤーをさかのぼれません。<error> Cannot layer up in this area.")));
						break;
					}
				}
				else if(*itrchar == _T('{'))
				{//マクロ定義
					if(!(flag & FLAG_MATRIXDECODE))
					{//まだ定義されてない
						++itrchar;

						if(itrchar != itrstr->end())
						{
							if(*itrchar == _T('>'))
							{
								if(!(flag & FLAG_ONLY_MACRODEF))
								{//まだ定義されてない
									//マクロ定義強制
									flag |= FLAG_ONLY_MACRODEF;
									deside_only_macro = 1;
									break;
								}
								else
								{//もう定義されてる
									addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー マクロ定義強制状態の行にマクロ定義強制コマンドがあります。<error> extra macro_area_begin command.")));
									break;
								}
							}
						}
						--itrchar;

						tstring::iterator itr = itrstr->end();
						tstring name(++itrchar, --itr);
						if(decode_newDefineMacro(vlayer, codes, itrstr, name, line))
						{//エラー
							if(itrstr!=codes.end())
							{
								addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_newDefineMacro マクロ定義でエラーが発生しています。<error> There are errors in defining macro.")));
							}
							else
							{
								addError(vlayer.back(), codename, line, (tstring)_T("[EOF]"), tstring(_T("error cScriptReader::decode_newDefineMacro マクロ定義でエラーが発生しています。<error> There are errors in defining macro.")));
							}
						}
						break;
					}
					else
					{
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー このレイヤーではマクロ強制状態を解除できません。<error> Cannot macro_area_end in this area.")));
						break;
					}
				}
				else if(*itrchar == _T(':'))
				{//ファイル参照
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_FileOpen(vlayer, scriptdir() + name, flag))
					{//エラー
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_FileOpen ファイルが見つからないか、ファイルが壊れています。<error> File Not Found. or Broken File")));
					}
					break;
				}
				else if(*itrchar == _T('!'))
				{//スクリプト読み込み
					tstring::iterator secondOperator = itrchar;
					secondOperator++;
					if(secondOperator != itrstr->end() && *secondOperator == _T(':'))
					{
						tstring::iterator endOfLineitr = itrstr->end();
						tstring name(++secondOperator, --endOfLineitr);
						if(decode_LuaDependScript(vlayer, scriptdir() + name, name))
						{//エラー
							addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_LuaDependScript ファイルが見つからないか、ファイルが壊れています。<error> File Not Found. or Broken File")));
						}
					}
					else {

						tstring::iterator itr = itrstr->end();
						tstring name(++itrchar, --itr);
						vlayer.back()->script().push_back(_T("#!"));
						/*vlayer.back()->script().back().insert(vlayer.back()->script().back().end(),
																scriptdir().begin(),
																scriptdir().end());*///実際のロケーション
						vlayer.back()->script().back().insert(vlayer.back()->script().back().end(),
																name.begin(),
																name.end());
						if(decode_LuaScript(vlayer, scriptdir() + name, name))
						{//エラー
							addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_LuaScript ファイルが見つからないか、ファイルが壊れています。<error> File Not Found. or Broken File")));
						}
					}
					break;
				}
				else if(*itrchar == _T('*'))
				{//マクロ展開
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_Macro(vlayer, name, flag))
					{//エラー
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_Macro 未定義のマクロを呼び出そうとしました。<error> Macro Not Found")));
					}
					break;
				}
				else if(*itrchar == _T('?'))
				{//データ参照
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_Reference(vlayer, name))
					{//エラー
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_Reference 未定義のデータを参照しようとしました。<error> Data Not Found")));
					}
					break;
				}
				else if(*itrchar == _T('}'))
				{
					if(!(flag & FLAG_MATRIXDECODE))
					{//まだ定義されてない
						++itrchar;

						if(itrchar != itrstr->end())
						{
							if(*itrchar == _T('<'))
							{
								if(!(flag & FLAG_ONLY_MACRODEF))
								{//まだ定義されてない
									addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー マクロ定義強制解除状態の行にマクロ定義強制解除コマンドがあります。<error> Extra macro_area_end command")));
									break;
								}
								else
								{//
									if(deside_only_macro)
									{//マクロ強制を決定したレイヤーでしか解除できない。
										//マクロ定義強制解除
										deside_only_macro = 0;
										flag &= (~(unsigned int)0) ^ FLAG_ONLY_MACRODEF;
										break;
									}
									else
									{
										addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー このレイヤーではマクロ強制状態を解除できません。<error> Cannot macro_area_end in this area.")));
										break;
									}
								}
							}
						}
						--itrchar;
					}
					else
					{
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー このレイヤーではマクロ強制状態を解除できません。<error> Cannot macro_area_end in this area.")));
						break;
					}

					//問答無用構文エラー
					addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー マクロ終了コマンドが重複しているか、マクロ定義の中でマクロ定義をしています。<error> Extra macro_end command. or difining macro in difining macro.")));
				}
				else if(*itrchar == _T('#'))
				{//表デコード
					if(!(flag & FLAG_ONLY_MACRODEF))
					{//マクロ強制状態でない
						++itrchar;

						if(itrchar != itrstr->end())
						{

							if(*itrchar == _T('>'))
							{
								if(!(flag & FLAG_MATRIXDECODE))
								{//まだ定義されてない
									//マクロ定義強制
									flag |= FLAG_MATRIXDECODE;
									tstring::iterator itr = itrstr->end();
									matrix_sepcode_.assign(++itrchar, --itr);
									tstring matdec = _T("__MATRIX_DECODE_LAYER__");
									if(decode_newAndDownLayer(vlayer,matdec))
									{
										addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_newAndDownLayer この名前のメンバーは作成できませんでした。<error> cannot make this name member")));
									}
									break;
								}
								else
								{//もう定義されてる
									addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー 表デコード内で表デコード開始コードは使用できません。<error> Extra matrix_decode_in code")));
									break;
								}
							}
							else if(*itrchar == _T('<'))
							{
								if(!(flag & FLAG_MATRIXDECODE))
								{//まだ定義されてない
									//マクロ定義強制
									addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー 余分な表デコード終了コードが見つかりました。<error> Extra matrix_decode_out code")));
									break;
								}
								else
								{//もう定義されてる
									decode_Matrix(vlayer,matrix_sepcode_);
									if(decode_UpLayer(vlayer))
									{
										addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error cScriptReader::decode_UpLayer 上位の階層構造が無いため階層構造をさかのぼれません。<error> Cannot layer up. Not exist upper layer")));
									}
									tstring matdec = _T("__MATRIX_DECODE_LAYER__");
									vlayer.back()->eraseMember(matdec);
									flag &= (~(unsigned int)0) ^ FLAG_MATRIXDECODE;
									break;
								}
							}

						}
						
						--itrchar;
					}
					else
					{
						addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー ここでは表デコードを実行できません。")));
						break;
					}
					//問答無用構文エラー
					addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 構文エラー 表デコードコマンド「#」の使い方が間違っています。<error> wrong using [#].")));
				}
				else
				{//エラー
					addError(vlayer.back(), codename, line, *itrstr, tstring(_T("error 解釈不能なコマンド文字「$」が発見されました。行頭に「$」を使用する際は「$$」を利用してください。<error> Unknown command. If you want use [$] top of line. use [$$].")));
					break;
				}
			}
		}

		if(!command)
		{//コマンド以外
			vlayer.back()->script().push_back(_T(""));
			tstring::iterator itr = itrstr->end();
			vlayer.back()->script().back().assign(itrchar,--itr);
		}
		
		if(itrstr==codes.end()) break;
	}
	

	return SUCCESS;
}

void cScriptReader::addError(pcScriptRLayer layer, tstring& em)
{
	layer->messageOutputTo()->push_back(em+_T("\n"));
}
void cScriptReader::addError(pcScriptRLayer layer, tstring& errorfile, int line, tstring& errorcode, tstring& em)
{
	tstring str;
	TCHAR c[256];
	_stprintf(c,_T("%s: line %d: %s: %s \n"), errorfile.c_str(), line, errorcode.c_str(), em.c_str());
	str.assign(c);
	//str += em;
	layer->messageOutputTo()->push_back(str);
}


int cScriptReader::decode_newAndDownLayer(std::vector<pcScriptRLayer>& vlayer, tstring& name)
{
	pcScriptRLayer pcsl = vlayer.back()->addMember(name, vlayer.back());
	if(pcsl == NULLOFcScriptRLayer)
	{
		/*
		//addError(tstring(_T("error cScriptReader::decode_newAndDownLayer 同じ名前のメンバーが存在します。")));
		
		//仮のメンバーを作成
		pcsl = vlayer.back()->addMember(_T("DoubleErrorOf_")+name, vlayer.back());
		if(pcsl == NULLOFcScriptRLayer)
		{
			pcsl = vlayer.back()->pmember(_T("DoubleErrorOf_")+name);
		}
		vlayer.push_back(pcsl);
		return VERROR;
		*/

		//後のを優先するように変更
		/*
		addError(vlayer.back(),tstring(_T(__FILE__)),__LINE__ ,tstring(_T("error cScriptReader::decode_newAndDownLayer 同じ名前のメンバーが存在します。<error> There is the same name member. ")),name);
		vlayer.back()->eraseMember(name);
		pcsl = vlayer.back()->addMember(name, vlayer.back());
		if(pcsl == NULLOFcScriptRLayer)
		{
			return VERROR;
		}
		*/

		//追加するように変更
		pcsl = vlayer.back()->pmember(name);
		if(pcsl == NULLOFcScriptRLayer)
		{
			return VERROR;
		}

	}
	vlayer.push_back(pcsl);
	return SUCCESS;
}
int cScriptReader::decode_UpLayer(std::vector<pcScriptRLayer>& vlayer)
{
	if(vlayer.size() <= 1) return VERROR;
	vlayer.pop_back();
	return SUCCESS;
}

int cScriptReader::outputerrormassage(tstring& filename)
{
	if(errormassage().empty()) return SUCCESS;
	oftstream ofs((_LOGFOLDER + filename).c_str());
	
	if(ofs)
	{
		vector<tstring>::iterator itr = errormassage().begin();
		for(;itr!=errormassage().end();itr++)
		{
			ofs << *itr;
		}

		ofs.close();
	}
	else
	{
		return VERROR;
	}

	return SUCCESS;
}

int cScriptReader::decode_FileOpen(std::vector<pcScriptRLayer>& vlayer, tstring& name, unsigned int flag)
{

	pcScriptReader pcsrFO = pcScriptReader(new cScriptReader(name));

	if(pcsrFO->simpleload()) return VERROR;

	return pcsrFO->decode_roop( pcsrFO->codes(), vlayer, name, flag);

}

int cScriptReader::decode_Comment(std::vector<pcScriptRLayer>& vlayer, std::vector<tstring>& codes, std::vector<tstring>::iterator& itrstr, tstring& name, int &line)
{

	itrstr++;
	tstring::iterator itrchar;

	for(;itrstr!=codes.end();itrstr++)
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


		bool command = false;

		if(*itrchar == _T('\\') || *itrchar == _T('$'))
		{//コマンド
			command = true;
			itrchar++;
			for(; itrchar != itrstr->end(); itrchar++)
			{
				
				
				if(*itrchar == _T('/'))
				{//コメント
					++itrchar;

					if((itrchar != itrstr->end()) && *itrchar == _T('<'))
					{//コメントアウト終わり
						return SUCCESS;
					}
					break;
				}
				/*
				else if(*itrchar == _T('\\') || *itrchar == _T('$'))
				{//エスケープシーケンス
					command = false;
					break;
				}
				
				else if(*itrchar == _T('>'))
				{//新規レイヤー&レイヤーダウン
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_newAndDownLayer(vlayer,name))
					{//エラー
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_newAndDownLayer 同じ名前のメンバーが存在します。")));
					}
					break;
				}
				else if(*itrchar == _T('<'))
				{//レイヤーアップ
					if(decode_UpLayer(vlayer))
					{
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_UpLayer 上位の階層構造が無いため階層構造をさかのぼれません。")));
					}

					break;
				}
				else if(*itrchar == _T(':'))
				{//ファイル参照
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_FileOpen(vlayer, scriptdir() + name))
					{//エラー
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_FileOpen ファイルが見つからないか、ファイルが壊れています。")));
					}
					break;
				}
				
				else if(*itrchar == _T('}'))
				{//マクロ定義終わり
					return SUCCESS;
				}
				*/
			}
		}

		if(!command)
		{//コマンド以外
			
			//何もしなくていい
		}
		
	}
	
	if(itrstr!=codes.end())
	{
		addError(vlayer.back(), scriptname() + _T(" 内コメントアウト ") + name, line, *itrstr, tstring(_T("error cScriptReader::decode_Comment コメントアウト終了コードが見つかりません。<error> No comment_out_end command.")));
	}
	else
	{
		addError(vlayer.back(), scriptname() + _T(" 内コメントアウト ") + name, line, (tstring)_T("[EOF]"), tstring(_T("error cScriptReader::decode_Comment コメントアウト終了コードが見つかりません。<error> No comment_out_end command.")));
	}
	return VERROR;//コメントアウト終了コードが見つからない
}
int cScriptReader::decode_Matrix(std::vector<pcScriptRLayer>& vlayer, tstring& sepcode)
{
	if(sepcode.empty())
	{
		sepcode = _T(" ");
	}
	afterdecode_table(vlayer.back()->script(),sepcode.c_str(),vlayer.back()->pparent());

	return SUCCESS;//コメントアウト終了コードが見つからない
}

pLuaScript cScriptReader_decode_LuaScript(std::vector<pcScriptRLayer>& vlayer, tstring& filepass, tstring& filename)
{
	pLuaScript pluas = pLuaScript(new LuaScript);
#ifdef _UNRELEASE
	pluas->setFlexible(true);
#endif
	if(!pluas->loadFile(filepass))
	{//失敗
		return NULLOFLuaScript;
	}
	
	return boost::static_pointer_cast<LuaScript>(pluas);
}

int cScriptReader::decode_LuaScript(std::vector<pcScriptRLayer>& vlayer, tstring& filepass, tstring& filename)
{
	pLuaScript pluas_casted = cScriptReader_decode_LuaScript(vlayer, filepass, filename);
	if(pluas_casted && !vlayer.back()->addContent(filename, pluas_casted))
	{
		return VERROR;
	}

	return SUCCESS;//コメントアウト終了コードが見つからない
}

int cScriptReader::decode_LuaDependScript(std::vector<pcScriptRLayer>& vlayer, tstring& filepass, tstring& filename)
{
	pLuaScript pluas_casted = cScriptReader_decode_LuaScript(vlayer, filepass, filename);
	if(pluas_casted && !vlayer.back()->addDependContent(filename, pluas_casted))
	{
		return VERROR;
	}

	return SUCCESS;//コメントアウト終了コードが見つからない
}



int cScriptReader::decode_newDefineMacro(std::vector<pcScriptRLayer>& vlayer, std::vector<tstring>& codes, std::vector<tstring>::iterator& itrstr, tstring& name, int &line)
{
	
	vector<tstring>* pmacro = vlayer.back()->addMacro(name);
	
	if(pmacro == NULL)
	{
		addError(vlayer.back(), scriptname() + _T(" 内マクロ ") + name, line, *itrstr, tstring(_T("error cScriptReader::decode_newDefineMacro マクロ名が重複しています。<error> The same name macro exist.")));
		vlayer.back()->eraseMacro(name);
		pmacro = vlayer.back()->addMacro(name);
		if(pmacro == NULL)
		{
			return VERROR;
		}
	}

	itrstr++;
	tstring::iterator itrchar;

	for(;itrstr!=codes.end();itrstr++)
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


		bool command = false;

		if(*itrchar == _T('\\') || *itrchar == _T('$'))
		{//コマンド
			command = true;
			itrchar++;
			for(; itrchar != itrstr->end(); itrchar++)
			{
				if(*itrchar == _T('/'))
				{//コメント
					break;
				}
				else if(*itrchar == _T('\\') || *itrchar == _T('$'))
				{//エスケープシーケンス
					command = false;
					break;
				}
				/*
				else if(*itrchar == _T('>'))
				{//新規レイヤー&レイヤーダウン
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_newAndDownLayer(vlayer,name))
					{//エラー
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_newAndDownLayer 同じ名前のメンバーが存在します。")));
					}
					break;
				}
				else if(*itrchar == _T('<'))
				{//レイヤーアップ
					if(decode_UpLayer(vlayer))
					{
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_UpLayer 上位の階層構造が無いため階層構造をさかのぼれません。")));
					}

					break;
				}
				else if(*itrchar == _T(':'))
				{//ファイル参照
					tstring::iterator itr = itrstr->end();
					tstring name(++itrchar, --itr);
					if(decode_FileOpen(vlayer, scriptdir() + name))
					{//エラー
						addError(codename, line, *itrstr, tstring(_T("error cScriptReader::decode_FileOpen ファイルが見つからないか、ファイルが壊れています。")));
					}
					break;
				}
				*/
				else if(*itrchar == _T('}'))
				{//マクロ定義終わり
					return SUCCESS;
				}

				command = false;
				itrchar--;
				break;
			}
		}

		if(!command)
		{//コマンド以外
			pmacro->push_back(_T(""));
			tstring::iterator itr = itrstr->end();
			pmacro->back().assign(itrchar,itr);
		}
		
	}
	
	if(itrstr!=codes.end())
	{
		addError(vlayer.back(), scriptname() + _T(" 内マクロ ") + name, line, *itrstr, tstring(_T("error cScriptReader::decode_newDefineMacro マクロ終了コードが見つかりません。<error> No macro_end command.")));
	}
	else
	{
		addError(vlayer.back(), scriptname() + _T(" 内マクロ ") + name, line, (tstring)_T("[EOF]"), tstring(_T("error cScriptReader::decode_newDefineMacro マクロ終了コードが見つかりません。<error> No macro_end command.")));
	}
	return VERROR;//マクロ終了コードが見つからない
}
int cScriptReader::decode_Macro(std::vector<pcScriptRLayer>& vlayer, tstring& name, unsigned int flag)
{
	/*
	vector<pcScriptRLayer>::iterator itr;
	vector<tstring>* pmacro = NULL;
	for(itr = vlayer.end();;)
	{
		itr--;
		pmacro = (*itr)->pmacro(name);
		if(pmacro != NULL)
		{
			break;
		}
	
		if(itr == vlayer.begin())
		{
			break;
		}
		
	}
	*/

	vector<tstring>* pmacro = vlayer.back()->findpmacro(name);

	if(pmacro == NULL)
	{
		return VERROR;
	}

	return decode_roop( *pmacro, vlayer, scriptname()+name, flag);
}

int cScriptReader::decode_Reference(std::vector<pcScriptRLayer>& vlayer, tstring& name)
{
	/*
	vector<pcScriptRLayer>::iterator itr;
	pcScriptRLayer pmember = NULLOFcScriptRLayer;
	for(itr = vlayer.end();;)
	{
		itr--;
		pmember = (*itr)->pmember(name);
		if(pmember != NULL)
		{
			break;
		}
	
		if(itr == vlayer.begin())
		{
			break;
		}
		
	}
	*/

	
	pcScriptRLayer pmember = vlayer.back()->findpmember(name);

	if(pmember == NULL)
	{
		return VERROR;
	}

	vlayer.back()->script().insert(
		vlayer.back()->script().end(),
		pmember->script().begin(),
		pmember->script().end());


	return SUCCESS;
}
