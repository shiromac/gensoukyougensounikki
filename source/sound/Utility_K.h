#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <math.h>
#include "UtilityStruct.h"

typedef std::basic_string<TCHAR> tstring;

using namespace std;

#define NEW new

namespace MyDebug {
	void outputDebugString(const TCHAR* str, ...);
	void CheckMemoryLeak();
	void CheckDebugTime(tstring str, tstring fname = TEXT(__FILE__), int line = __LINE__);
	int GetTotalMemorySize(bool initflag = false);
	int GetHeapMemorySize(bool initflag = false);
}

namespace MyGame {
	void gameframe2time(DWORD i_frame, DWORD i_fps, int* o_hour, int* o_minute, int* o_second, int fps = 60);
	bool isdatFile(tstring& errorstr, tstring datname, int filenum, ...);
	DWORD Vec4f2DWORD(Vec4f vec);
	Vec4f DWORD2Vec4f(DWORD dw);
	///////////////////////////////////////////////////////
	/// 複数タスク起動が可能になる
	///////////////////////////////////////////////////////
	class TaskManager {
	public:
		TaskManager() { Init(); }
		~TaskManager() { ClearTask(); }
	public:
		enum ENUM_FUNCRETURN{
			POPTASK = 1, // タスク消去
			NEXTTASK = 2, // 次のタスクへ移動
			NEXTLOOPTASK = 3, // 次のループタスクへ移動
		};
		struct Timer{
			DWORD startframe;	// 開始フレーム数
			DWORD frame;		// 実行フレーム数
		};
		struct WorkInfo {
			bool timeflag;						// 時間制限があるかどうか
			Timer timer;						// フレーム管理構造体
			DWORD esframe;						// 実行開始フレーム数
			void* p;							// 作業スペース
			int (*pfunc)(void*, DWORD, DWORD);	// 作業内容
			bool deleteflag;					// 作業スペースを削除するかどうか
			bool loopflag;						// 作業がループするかどうか
			bool taskendflag;					// 作業が一時終了したフラグ
		};
	private:
		vector<WorkInfo> m_workqueue;
		vector<TaskManager*> m_retasks; // 関連する同期タスク
		DWORD m_framecounter;
		bool m_startexeflag; // タスクを実行開始する直前か
		bool m_beforeseflag; // startexeflagの前状態
	private:
		void EraseTask(unsigned int idx);
		bool CheckSync();
	public:
		void Init();
		void ClearTask();
		void PushTask(void* p, int (*pfunc)(void*, DWORD, DWORD), bool loopflag = false, bool deleteflag = false);
		void PushTask(Timer timer, void* p, int (*pfunc)(void*, DWORD, DWORD), bool loopflag = false, bool deleteflag = false);
		void ResetLoopTask();
		void Loop();
		bool GetNextTask(WorkInfo* ptask);
		bool IsNextFrameStartTask();
		bool IsEmpty();
		static void SetSyncTaskManager(vector<TaskManager*> vtm);
	};
}

namespace MyMath {
	static const int MATRIX_MAX = 128;
	void NormalizedCoordinateF(int dimension, ...);
	void MultiplyMatrix2Matrix(float n, float* a, int a_len, float* b, int b_len, float* s, int s_len);
	void MultiplyMatrix2Vector(float n, float* a, int a_len, float b[], float s[]);
	void MultiplyVector2Matrix(int n, float a[], float* b, int b_len, float s[]);
	float MultiplyVector2Vector(int n, float a[], float b[], float* s);
	void Beju2D(int n, float x[], float y[], float t, float* out_x, float* out_y);
	bool JudgeRectandRect(RECT& rect1, RECT& rect2);
	float FixValue(float val, float max, float min = 0);
	int getIndexinmax(int index, int max);
	double getVelocity2DFunc(double length, int frame, int maxframe, bool positiveflag);
	float GetDistance(Vec3f s, Vec3f e);
	float GetDistance(Vec2f s, Vec2f e);
	bool JudgeBox(Box b1, Box b2, Box& hitbox);
}

namespace MyFile { /// @todo: 使えるかチェック
	tstring GetFilename(tstring filename);
	tstring GetExtension(tstring filename);
	tstring GetDirectory(tstring filename);
	void GetTimeStamp(tstring filename, DWORD& ohigh, DWORD& olow);
	namespace FileEnum {
		void EnumDirectorys(tstring rootdir, vector<tstring>& odirs, tstring dirname = TEXT(""), bool recursionflag = false);
		void EnumFiles(tstring rootdir, vector<tstring>& ofiles, tstring filename = TEXT(""), bool recursionflag = false);
	}
	namespace FilePack {
		void Pack(const tstring dirname);
		void Pack(const tstring packname, const vector<tstring>& dirs);
		bool GetHeader(const tstring datname, const tstring filename, int* filesize);
		bool GetData(const tstring datname, const tstring filename, char* obuf, DWORD offset, DWORD bufsize, DWORD* obufsize);
		bool GetHandle(const tstring datname, const tstring filename, HANDLE* outh);
		bool GetFPointer(const tstring datname, const tstring filename, FILE** outfp, const tstring fmode = TEXT("rb"));
	}
}

namespace MyString {
	//! 変数値タグ
	struct tagVariable {
		tstring name;
		tstring svalue;
		int ivalue;
	};
	void tokenize(const tstring& input, vector<tstring>& result, const tstring& delim);
	void tokenizeInv(const tstring& input, vector<tstring>& result, const tstring& delim);
	void tokenizeMath(const tstring& input, vector<tstring>& numbers, vector<tstring>& symbols);
	tstring eliminate(const tstring& input, const tstring& delim);
	void GetArgument(const tstring& funcstr, vector<tagVariable>& vreturn);
	double CalcFormulaStr(const tstring& value);
	double CalcFormulaStr(const tstring& value, const vector<tagVariable>& vargs);
	void TransString(tstring& value, const vector<tagVariable>& vars);
	bool IsFunctionName(const tstring& str, const tstring& funcname, bool bracketflag = true);
	tstring TransNum2RankStr(int number);
	template <typename TYPE1>
	tstring SetArgument(const tstring funcname, TYPE1 arg1);
	template <typename TYPE1, typename TYPE2>
	tstring SetArgument(const tstring funcname, TYPE1 arg1, TYPE2 arg2);
}

namespace MyRand {
	void seed();
	double gendouble(double value);
	double gendoubleplus(double value);
	int genint(int value);
	int genintplus(int value);
	bool genbool();
}

namespace MyWindows {
	DWORD CreateThread(HANDLE* out_hThread, unsigned (__stdcall * _StartAddress) (void *), void* arg, int threadpriority = THREAD_PRIORITY_NORMAL);
	void DestroyThread(HANDLE& hThread);
}

//=========================================================
// テンプレート実装
//=========================================================

namespace MyString {
	template <typename TYPE1>
	tstring SetArgument(const tstring funcname, TYPE1 arg1)
	{
		tstring rstr = funcname + tstring(TEXT("("));
		rstr += lexical_cast<tstring>(arg1) + tstring(TEXT(")"));
		return rstr;
	}
	template <typename TYPE1, typename TYPE2>
	tstring SetArgument(const tstring funcname, TYPE1 arg1, TYPE2 arg2)
	{
		tstring rstr = funcname + tstring(TEXT("("));
		rstr += lexical_cast<tstring>(arg1) + tstring(TEXT(","));
		rstr += lexical_cast<tstring>(arg2) + tstring(TEXT(")"));
		return rstr;
	}
}