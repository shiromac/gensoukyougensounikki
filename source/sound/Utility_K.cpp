#include "stdafx.h"
#include "Utility_K.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <process.h>
#include <fstream>
#include <windows.h>
#include <psapi.h>
//#include <Common.h>
#include <tchar.h>
#include <boost/lexical_cast.hpp>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "winmm.lib")
using boost::lexical_cast;

namespace MyDebug {
	void outputDebugString(const TCHAR* str, ...)
	{
		static std::ofstream s_ofs(TEXT("debug.log"));
		TCHAR buf[512] = {0};
		va_list args;

		va_start(args, str);
		_vstprintf_s(buf, 512, str, args);
		OutputDebugString(buf);
		s_ofs << buf << endl;
		va_end(args);
	}
	void CheckMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	}
	void CheckDebugTime(tstring str, tstring fname, int line)
	{
		static DWORD stime = timeGetTime();
		static DWORD btime = timeGetTime();
		static std::basic_ofstream<TCHAR, char_traits<TCHAR> > s_ofs(TEXT("debugtime.log"));
		s_ofs << MyFile::GetFilename(fname) << TEXT("(") << line << TEXT(") [") << str << TEXT("] : ");
		s_ofs << timeGetTime() - stime << TEXT("(") << timeGetTime() - btime << TEXT(")") << endl;
		btime = timeGetTime();
	}
	///////////////////////////////////////////////////////
	/// 総メモリ使用量を返す関数
	/// 最初は初期メモリ使用量を取得し、
	/// その次からは初期メモリ使用量からどれだけメモリ使用量が増えたかを返す関数
	///////////////////////////////////////////////////////
	int GetTotalMemorySize(bool initflag)
	{
		static bool sInitflag = true;
		static int initTotalCount;
		PROCESS_MEMORY_COUNTERS info;
		GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
		if(sInitflag || initflag)
		{
			initTotalCount = info.WorkingSetSize;
			sInitflag = false;
		}
		return info.WorkingSetSize - initTotalCount;
	}
	///////////////////////////////////////////////////////
	/// ヒープメモリ使用量を返す関数（デバックのみ
	/// 最初は初期メモリ使用量を取得し、
	/// その次からは初期メモリ使用量からどれだけメモリ使用量が増えたかを返す関数
	///////////////////////////////////////////////////////
	int GetHeapMemorySize(bool initflag)
	{
		static bool sInitflag = true;
		static int initTotalCount;
		_CrtMemState state;
		_CrtMemCheckpoint(&state);
		if(sInitflag || initflag)
		{
			initTotalCount = state.lSizes[_NORMAL_BLOCK];
			sInitflag = false;
		}
		return state.lSizes[_NORMAL_BLOCK] - initTotalCount;
	}
}

namespace MyGame {
	void gameframe2time(DWORD i_frame, DWORD i_fps, int* o_hour, int* o_minute, int* o_second, int fps)
	{
		int second = (i_frame / i_fps) % fps;
		int minute = (i_frame / i_fps / fps) % fps;
		int hour = i_frame / i_fps / (fps * fps);
		if(o_hour)
			*o_hour = hour;
		else
			minute += hour * fps;
		if(o_minute)
			*o_minute = minute;
		else
			second += minute * fps;
		if(o_second)
			*o_second = second;
	}

	bool isdatFile(tstring& errorstr, tstring datname, int filenum, ...)
	{
		va_list args;

		va_start(args, filenum);
		for(int i = 0; i < filenum; i++)
		{
			tstring filename = va_arg(args, TCHAR*);
			int filesize;
			if(!MyFile::FilePack::GetHeader(datname, filename, &filesize))
			{
				errorstr = datname + TEXT("のデータが不正です");
				va_end(args);
				return false;
			}
		}
		va_end(args);
		return true;
	}

	//-----------------------------------------------------
	// Vec4f(a,b,c,d)構造体から0xAABBCCDDというデータへ変換
	// 注意 : (a,b,c,d)は0～1まで
	//-----------------------------------------------------
	DWORD Vec4f2DWORD(Vec4f vec)
	{
		float fa = MyMath::FixValue(vec.x, 1);
		float fb = MyMath::FixValue(vec.y, 1);
		float fc = MyMath::FixValue(vec.z, 1);
		float fd = MyMath::FixValue(vec.w, 1);
		DWORD a = (DWORD)(fa * 0xFF) << 24;
		DWORD b = (DWORD)(fb * 0xFF) << 16;
		DWORD c = (DWORD)(fc * 0xFF) << 8;
		DWORD d = (DWORD)(fd * 0xFF);
		return a + b + c + d;
	}
	//-----------------------------------------------------
	// 0xAABBCCDDというデータからVec4f(a,b,c,d)構造体へ変換
	// 注意 : (a,b,c,d)は0～1まで
	//-----------------------------------------------------
	Vec4f DWORD2Vec4f(DWORD dw)
	{
		DWORD a = (dw & 0xFF000000) >> 24;
		DWORD b = (dw & 0x00FF0000) >> 16;
		DWORD c = (dw & 0x0000FF00) >> 8;
		DWORD d = (dw & 0x000000FF);
		return Vec4f(a / 256.0f, b / 256.0f, c / 256.0f, d / 256.0f);
	}
	//-----------------------------------------------------
	// 初期化関数
	// deleteflag : 作業スペースをdeleteするかどうか
	//-----------------------------------------------------
	void TaskManager::Init()
	{
		m_framecounter = 0;
		ClearTask();
	}

	//-----------------------------------------------------
	// タスクを空っぽにする
	//-----------------------------------------------------
	void TaskManager::ClearTask()
	{
		for(UINT i = 0; i < m_workqueue.size(); i++)
		{
			bool deleteflag = m_workqueue[i].deleteflag;
			if(deleteflag)
				delete m_workqueue[i].p;
		}
		m_workqueue.clear();
		m_startexeflag = true;
		m_beforeseflag = true;
	}

	///////////////////////////////////////////////////////
	/// ループタスクを普通のタスクへ戻す
	///////////////////////////////////////////////////////
	void TaskManager::ResetLoopTask()
	{
		for(UINT i = 0; i < m_workqueue.size(); i++)
		{
			if(m_workqueue[i].taskendflag) // 実行終了している場合
				EraseTask(i--);
			else
				m_workqueue[i].loopflag = false;
		}
	}

	//-----------------------------------------------------
	// タスクを入れる
	// pfunc : (void* p, DWORD frame, DWORD endframe) 
	// pは自分の作業領域、frameは現在フレーム数、endframeは実行終了フレーム数
	//-----------------------------------------------------
	void TaskManager::PushTask(Timer timer, void* p, int (*pfunc)(void*, DWORD, DWORD), bool loopflag, bool deleteflag)
	{
		WorkInfo wi;
		wi.timeflag = true;
		wi.timer = timer;
		wi.p = p;
		wi.pfunc = pfunc;
		wi.esframe = m_framecounter;
		wi.deleteflag = deleteflag;
		wi.loopflag = loopflag;
		wi.taskendflag = false;
		m_workqueue.push_back(wi);
	}

	//-----------------------------------------------------
	// 時間制限なしタスクを入れる
	// pfunc : (void* p, DWORD frame, DWORD reserve) 
	// pは自分の作業領域、frameは現在フレーム数、reserveは使用してない
	//-----------------------------------------------------
	void TaskManager::PushTask(void* p, int (*pfunc)(void*, DWORD, DWORD), bool loopflag, bool deleteflag)
	{
		WorkInfo wi;
		wi.timeflag = false;
		wi.p = p;
		wi.pfunc = pfunc;
		wi.esframe = m_framecounter;
		wi.deleteflag = deleteflag;
		wi.loopflag = loopflag;
		wi.taskendflag = false;
		m_workqueue.push_back(wi);
	}

	//-----------------------------------------------------
	// タスク処理を行う
	//-----------------------------------------------------
	void TaskManager::Loop() 
	{
		m_beforeseflag = m_startexeflag;
		if(m_workqueue.empty() || !CheckSync()) // タスク処理ができない
			return;
		unsigned int idx = 0;
		while(1)
		{
			WorkInfo wi = m_workqueue[idx];
			if(wi.timeflag) // まだ、旧バージョン。
			{
				DWORD sframe = wi.timer.startframe + wi.esframe;
				if(sframe <= m_framecounter)
				{
					DWORD nowframe = m_framecounter - sframe;
					DWORD endframe = wi.timer.frame;
					m_startexeflag = false; // 実行中
					if(endframe >= nowframe)
					{
						EraseTask(0);
						if(!m_workqueue.empty())
							m_workqueue.front().esframe = m_framecounter + 1;
					}
					wi.pfunc(wi.p, nowframe, endframe);
				}
				break; // タスク処理は一つのみ
			}
			else if(!wi.taskendflag) // タスクが稼働中の場合
			{
				int rvalue = wi.pfunc(wi.p, m_framecounter - wi.esframe, 0);
				m_startexeflag = false; // 実行中
				if(rvalue == POPTASK || (rvalue == NEXTLOOPTASK && !wi.loopflag)) // このタスクはもう使われない場合
				{
					bool isnext = m_workqueue.size() > idx + 1;
					EraseTask(idx);
					if(isnext)
						m_workqueue[idx].esframe = m_framecounter + 1;
					break; // タスク処理は終了
				}
				else if(rvalue == NEXTTASK) // 次のタスクへ移動、現在タスクも稼動
				{
					bool isnext = m_workqueue.size() > idx + 1;
					if(isnext && m_workqueue[++idx].esframe == 0) // 実行開始フレームを書き込む
						m_workqueue[idx].esframe = m_framecounter + 1;
					else if(!isnext)// 次のタスクがないので終了
						break;
				}
				else if(rvalue == NEXTLOOPTASK) // 次のループタスクへ移動、現在タスクは休止
				{
					// 実行終了した処理
					m_workqueue[idx].taskendflag = true;
					m_workqueue[idx].esframe = 0; 
					// 次のタスクを呼ぶ処理
					bool isnext = m_workqueue.size() > idx + 1;
					if(isnext && m_workqueue[idx + 1].loopflag) // 実行開始フレームを書き込む
						m_workqueue[idx + 1].esframe = m_framecounter + 1;
					else // 次がループタスクではない → 戻る
					{
						for(unsigned int i = 0; i <= idx; i++)
						{
							m_workqueue[i].taskendflag = false;
							m_workqueue[i].esframe = m_framecounter + 1;
						}
						break;
					}
				}
				else // タスク処理は終了
					break;
			}
			else if(++idx >= m_workqueue.size()) // タスク処理が休止中の場合、次のタスクへ移動。次がない場合は中止
				break;
		}
		m_framecounter++;
	}

	//-----------------------------------------------------
	// 次のタスクを取得
	//-----------------------------------------------------
	bool TaskManager::GetNextTask(MyGame::TaskManager::WorkInfo *ptask)
	{
		if(!m_framecounter && !m_workqueue.empty()) // 最初
		{
			*ptask = m_workqueue[0];
			return true;
		}
		else if(m_workqueue.size() > 1)
		{
			*ptask = m_workqueue[1];
			return true;
		}
		return false;
	}

	//--------------------------------------------------------
	// 次のフレームで次のタスクを行う場合、trueを返す
	//--------------------------------------------------------
	bool TaskManager::IsNextFrameStartTask()
	{
		if(!m_framecounter && !m_workqueue.empty()) // 最初
			return true;
		else if(m_workqueue.size() > 1)
		{
			WorkInfo& nowtask = m_workqueue[0];
			DWORD endframe = (nowtask.timeflag) ? nowtask.timer.startframe + nowtask.timer.frame : 0;
			return (m_framecounter - nowtask.esframe >= endframe);
		}
		return false;
	}
	//--------------------------------------------------------
	// 空かどうか
	//--------------------------------------------------------
	bool TaskManager::IsEmpty()
	{
		return m_workqueue.empty();
	}

	void TaskManager::EraseTask(unsigned int idx)
	{
		vector<WorkInfo>::iterator itr = m_workqueue.begin();
		bool deleteflag = m_workqueue[idx].deleteflag;
		if(deleteflag)
			delete m_workqueue[idx].p;
		for(unsigned int i = 0; i < idx; ++i)
			++itr;
		m_workqueue.erase(itr);
		m_startexeflag = true;
	}

	//--------------------------------------------------------
	// 同期させたいタスクを入れる
	//--------------------------------------------------------
	void TaskManager::SetSyncTaskManager(vector<TaskManager*> vtm)
	{
		for(UINT i = 0; i < vtm.size(); i++)
		{
			for(UINT j = i + 1; j < vtm.size(); j++)
			{
				vtm[i]->m_retasks.push_back(vtm[j]);
				vtm[j]->m_retasks.push_back(vtm[i]);
			}
		}
	}

	//--------------------------------------------------------
	// 実行していいかを返す
	//--------------------------------------------------------
	bool TaskManager::CheckSync()
	{
		bool exeflag = false; // 同期しているタスクが実行中かどうか
		for(UINT i = 0; i < m_retasks.size(); i++)
		{
			exeflag |= !m_retasks[i]->m_beforeseflag;
		}
		return (!exeflag || !m_startexeflag);
	}
}	

namespace MyMath {
	//----------------------------------------------------------------------------
	// 座標を正規化する
	// @variable
	// dimension <int>	 : 次元
	// x1...     <float*> : 座標たち
	//----------------------------------------------------------------------------
	void NormalizedCoordinateF(int dimension, ...)
	{
		va_list args;
		float*	p_point[128];
		float	length;
		if(dimension >= 128)
		{
			OutputDebugString(TEXT("Error NormalizedCoordinateF"));
			return;
		}
		va_start(args, dimension);
		for(int i = 0; i < dimension; i++)
			p_point[i] = va_arg(args, float*);
		// 距離を計算
		length = 0;
		for(int i = 0; i < dimension; i++)
			length += *p_point[i] * *p_point[i];
		length = (float)sqrt((long double)length);
		for(int i = 0; i < dimension; i++)
			*p_point[i] /= length;
		va_end(args);
	}
	void MultiplyMatrix2Matrix(float n, float* a, int a_len, float* b, int b_len, float* s, int s_len)
	{
		float t[MATRIX_MAX][MATRIX_MAX] = {0};
		if(n > MATRIX_MAX)
			return;
		// 行列計算
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				for(int k = 0; k < n; k++)
				{
					t[i][j] += a[i * a_len + k] * b[k * b_len + j];
				}
			}
		}
		// 答えに代入
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				s[i * s_len + j] = t[i][j];
			}
		}
	}

	void MultiplyMatrix2Vector(float n, float* a, int a_len, float b[], float s[])
	{
		float t[MATRIX_MAX] = {0};
		if(n > MATRIX_MAX)
			return;
		// 行列計算
		for(int i = 0; i < n; i++)
		{
			for(int k = 0; k < n; k++)
			{
				t[i] += a[i * a_len + k] * b[k];
			}
		}
		// 答えに代入
		for(int i = 0; i < n; i++)
		{
			s[i] = t[i];
		}
	}

	void MultiplyVector2Matrix(int n, float a[], float* b, int b_len, float s[])
	{
		float t[MATRIX_MAX] = {0};
		if(n > MATRIX_MAX)
			return;
		// 行列計算
		for(int i = 0; i < n; i++)
		{
			for(int k = 0; k < n; k++)
			{
				t[i] += a[k] * b[k * b_len + i];
			}
		}
		// 答えに代入
		for(int i = 0; i < n; i++)
		{
			s[i] = t[i];
		}
	}

	float MultiplyVector2Vector(int n, float a[], float b[], float* s)
	{
		float t = 0;
		if(n > MATRIX_MAX)
			return -1;
		// 計算
		for(int i = 0; i < n; i++)
		{
			t += a[i] * b[i];
		}
		// 答えに代入
		if(s)
			*s = t;
		return t;
	}

	//----------------------------------------------------------------------------
	// ベジュ曲線を行う関数(2次元
	//----------------------------------------------------------------------------
	void Beju2D(int n, float x[], float y[], float t, float* out_x, float* out_y)
	{
		float b[MATRIX_MAX + 1][MATRIX_MAX + 1] = {0};
		float vt[MATRIX_MAX];
		float vtemp[MATRIX_MAX];
		// 限界なのでやめておく
		if(n > 15)
			return;
		// 行列作成
		b[n - 1][n - 1] = 1;
		for(int i = n - 2; i >= 0; i--)
		{
			for(int j = 0; j < n; j++)
				b[i][j] = b[i + 1][j + 1] - b[i + 1][j];
		}
		float temp = 1;
		float temp2 = 1;
		// n! / (n - i)! * i! -> temp / temp2
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
				b[i][j] = (temp / temp2) * b[i][j];
			temp *= n - i - 1;
			temp2 *= i + 1;
		}
		// tを用いたベクトル作成
		vt[0] = 1;
		for(int i = 1; i < n; i++)
		{
			vt[i] = vt[i - 1] * t;
		}
		MultiplyVector2Matrix(n, x, (float*)b, MATRIX_MAX + 1, vtemp);
		*out_x = MultiplyVector2Vector(n, vtemp, vt, NULL);
		MultiplyVector2Matrix(n, y, (float*)b, MATRIX_MAX + 1, vtemp);
		*out_y = MultiplyVector2Vector(n, vtemp, vt, NULL);
	}

	bool JudgeRectandRect(RECT& rect1, RECT& rect2)
	{
		bool flag1 = ((rect1.top > rect2.bottom) || (rect1.bottom < rect2.top));
		bool flag2 = ((rect1.left > rect2.right) || (rect1.right < rect2.left));
		return !(flag1 || flag2);
	}

	bool JudgeBox(Box b1, Box b2, Box& hitbox)
	{
		float b1minx = (b1.size.x < 0) ? b1.pos.x + b1.size.x : b1.pos.x;
		float b1miny = (b1.size.y < 0) ? b1.pos.y + b1.size.y : b1.pos.y;
		float b1minz = (b1.size.z < 0) ? b1.pos.z + b1.size.z : b1.pos.z;
		float b2minx = (b2.size.x < 0) ? b2.pos.x + b2.size.x : b2.pos.x;
		float b2miny = (b2.size.y < 0) ? b2.pos.y + b2.size.y : b2.pos.y;
		float b2minz = (b2.size.z < 0) ? b2.pos.z + b2.size.z : b2.pos.z;
		float b1maxx = (b1.size.x > 0) ? b1.pos.x + b1.size.x : b1.pos.x;
		float b1maxy = (b1.size.y > 0) ? b1.pos.y + b1.size.y : b1.pos.y;
		float b1maxz = (b1.size.z > 0) ? b1.pos.z + b1.size.z : b1.pos.z;
		float b2maxx = (b2.size.x > 0) ? b2.pos.x + b2.size.x : b2.pos.x;
		float b2maxy = (b2.size.y > 0) ? b2.pos.y + b2.size.y : b2.pos.y;
		float b2maxz = (b2.size.z > 0) ? b2.pos.z + b2.size.z : b2.pos.z;
		bool inxflag = !((b1maxx < b2minx) || (b2maxx < b1minx));
		bool inyflag = !((b1maxy < b2miny) || (b2maxy < b1miny));
		bool inzflag = !((b1maxz < b2minz) || (b2maxz < b1minz));
		if(inxflag && inyflag && inzflag) // 当たっている場合
		{
			float startx = (b1minx < b2minx) ? b2minx : b1minx;
			float starty = (b1miny < b2miny) ? b2miny : b1miny;
			float startz = (b1minz < b2minz) ? b2minz : b1minz;
			float endx = (b1maxx < b2maxx) ? b1maxx : b2maxx;
			float endy = (b1maxy < b2maxy) ? b1maxy : b2maxy;
			float endz = (b1maxz < b2maxz) ? b1maxz : b2maxz;
			hitbox.pos = Vec3f(startx, starty, startz);
			hitbox.size = Vec3f(endx - startx, endy - starty, endz - startz);
			return true;
		}
		return false;
	}

	//-----------------------------------------------------
	// 値をmax, minの内に収める
	// ex. 1 <= val <= 12の場合、valが15ならば12になる
	//-----------------------------------------------------
	float FixValue(float val, float max, float min)
	{
		if(max > min)
		{
			if(val >= max)
				val = max;
			if(val < min)
				val = min;
		}
		return val;
	}

	//-----------------------------------------------------
	// Indexをmax内に収める
	// ex. 0 <= index < 12の場合、indexが15ならば3になる
	//-----------------------------------------------------
	int getIndexinmax(int index, int max)
	{
		if(max > 0)
		{
			while(index >= max)
				index -= max;
			while(index < 0)
				index += max;
		}
		return index;
	}


	//----------------------------------------------------------------------------
	// 距離を入れて、２次関数移動の速度を求める
	//----------------------------------------------------------------------------
	double getVelocity2DFunc(double length, int frame, int maxframe, bool positiveflag)
	{
		double a = (positiveflag) ? 0.5f * length : -0.5f * length;
		double rate = (double)(frame % maxframe) / maxframe;
		double nextrate = (double)((frame + 1) % maxframe) / maxframe;
		double x = rate - 0.5;
		double nx = nextrate - 0.5;
		double y = a * x * x;
		double ny = a * nx * nx;
		// 奇数と偶数の場合
		return ((frame / maxframe) & 1) ? y - ny : ny - y;
	}

	//--------------------------------------------------------
	// ユークリッド距離を取得(3D)
	//--------------------------------------------------------
	float GetDistance(Vec3f s, Vec3f e)
	{
		float x = e.x - s.x;
		float y = e.y - s.y;
		float z = e.z - s.z;
		return sqrt((x * x) + (y * y) + (z * z));
	}

	//--------------------------------------------------------
	// ユークリッド距離を取得(2D)
	//--------------------------------------------------------
	float GetDistance(Vec2f s, Vec2f e)
	{
		float x = e.x - s.x;
		float y = e.y - s.y;
		return sqrt((x * x) + (y * y));
	}
}

namespace MyFile {
	///////////////////////////////////////////////////////
	/// ディレクトリなしファイル名を取得（拡張子はあり
	///////////////////////////////////////////////////////
	tstring GetFilename(tstring filename)
	{
		tstring::size_type at = filename.rfind('\\');
		if(at != tstring::npos)
			return filename.substr(at + 1, tstring::npos);
		return filename;
	}
	//-----------------------------------------------------
	// ファイルの拡張子を取得	
	//-----------------------------------------------------
	tstring GetExtension(tstring filename)
	{
		tstring::size_type at = filename.rfind('.');
		if(at != tstring::npos)
			return filename.substr(at + 1);
		return tstring(TEXT(""));
	}
	//-----------------------------------------------------
	// ファイルのディレクトリを取得	
	// ex. ../public/xfile.txt → 出力 : ../public
	//-----------------------------------------------------
	tstring GetDirectory(tstring filename)
	{
		tstring::size_type at = filename.rfind('/');
		if(at != tstring::npos)
			return filename.substr(0, at);
		return tstring(TEXT(""));
	}
	///////////////////////////////////////////////////////
	/// ファイルスタンプを取得
	///////////////////////////////////////////////////////
	void GetTimeStamp(tstring filename, DWORD& ohigh, DWORD& olow)
	{
		FILETIME file_time;
		HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			return;
		GetFileTime(hFile, NULL, NULL, &file_time);
		ohigh = file_time.dwHighDateTime;
		olow = file_time.dwLowDateTime;
		CloseHandle(hFile);
	}
	namespace FileEnum {
		// プロトタイプ宣言
		int NextFindFile(HANDLE handle, WIN32_FIND_DATA wfd, vector<tstring>& onames, bool dirflag);
		//-------------------------------------------------
		// 指定したディレクトリからディレクトリの名前を見つけ出す
		//-------------------------------------------------
		void EnumDirectorys(tstring rootdir, vector<tstring>& odirs, tstring dirname, bool recursionflag)
		{
			HANDLE hDir;
			tstring name;
			WIN32_FIND_DATA	wfd;
			if(dirname == TEXT("")) // dirnameに指定がない場合は、ワイルドカードにしとく
				dirname = TEXT("*");
			name = rootdir + TEXT("/") + dirname;
			hDir = FindFirstFile(name.c_str(), &wfd);
			while(NextFindFile(hDir, wfd, odirs, true));
			FindClose(hDir);
		}
		//-------------------------------------------------
		// 指定したディレクトリからファイルの名前を見つけ出す
		//-------------------------------------------------
		void EnumFiles(tstring rootdir, vector<tstring>& ofiles, tstring filename, bool recursionflag)
		{
			HANDLE hFile;
			tstring name;
			WIN32_FIND_DATA	wfd;
			if(filename == TEXT("")) // filenameに指定がない場合は、ワイルドカードにしとく
				filename = TEXT("*");
			name = rootdir + TEXT("/") + filename;
			hFile = FindFirstFile(name.c_str(), &wfd);
			while(NextFindFile(hFile, wfd, ofiles, false));
			FindClose(hFile);
		}
		//---------------------------------------------------------
		//_	<function>	nextFindFile
		//_	<return>		(int)	: 非０は失敗、０は成功
		//---------------------------------------------------------
		int NextFindFile(HANDLE handle, WIN32_FIND_DATA wfd, vector<tstring>& onames, bool dirflag)
		{
			if(!handle)
				return -1;
			// TEXT(".")とTEXT("..")は除く
			while(_tcscmp(wfd.cFileName, TEXT(".")) == 0 || _tcscmp(wfd.cFileName, TEXT("..")) == 0)
				FindNextFile(handle, &wfd);
			// ディレクトリ、ファイルの列挙
			do
			{
				if(!!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == dirflag)
				{
					onames.push_back(tstring(wfd.cFileName));
					FindNextFile(handle, &wfd);
					return 0;
				}
			}
			while(FindNextFile(handle, &wfd));

			return -1;
		}
	}
	namespace FilePack {
		// 構造体
		struct FileInfoHeader
		{
			TCHAR filename[256];
			DWORD filesize;
		};
		struct FileInfo 
		{
			FileInfoHeader header;
			char* srcdata;
		};
		// クラス
		class ResourceManager { // 廃棄担当
		public:
			~ResourceManager()
			{
				map<tstring, HANDLE>::iterator itr = m_handles.begin();
				while(itr != m_handles.end())
				{
					CloseHandle(itr->second);
					++itr;
				}
				map<tstring, FILE*>::iterator itr2 = m_fps.begin();
				while(itr2 != m_fps.end())
				{
					fclose(itr2->second);
					++itr;
				}
			}
		private:
			map<tstring, HANDLE> m_handles;
			map<tstring, FILE*> m_fps;
		public:
			HANDLE GetHandle(tstring datname, tstring filename) {
				map<tstring, HANDLE>::iterator itr = m_handles.find(datname + filename);
				if(itr != m_handles.end()) // 要素みっけた
					return itr->second;
				return NULL;
			}
			FILE* GetFile(tstring datname, tstring filename) {
				map<tstring, FILE*>::iterator itr = m_fps.find(datname + filename);
				if(itr != m_fps.end()) // 要素あり
					return itr->second;
				return NULL;
			}
			void InsertHandle(tstring datname, tstring filename, HANDLE handle) { 
				m_handles.insert(pair<tstring, HANDLE>(datname + filename, handle)); 
			}
			void InsertFP(tstring datname, tstring filename, FILE* fp) {
				m_fps.insert(pair<tstring, FILE*>(datname + filename, fp));
			}
		};
		// 静的グローバル変数
		static ResourceManager s_rmanager;
		// プロトタイプ宣言
		void CreateFileInfo(const tstring dirname, vector<FileInfo>& v_finfo);
		void CreateFilePack(const tstring packname, const vector<FileInfo>& v_finfo);
		bool ReadHeader(const tstring datname, vector<FileInfoHeader> &fhinfos);
		int GetIndexFileInfo(const tstring filename, vector<FileInfoHeader> &fhinfos);

		//-------------------------------------------------
		// ディレクトリの中にあるファイルをパックする
		//-------------------------------------------------
		void Pack(const tstring dirname)
		{
			vector<FileInfo> v_finfo;
			CreateFileInfo(dirname, v_finfo);
			// ファイル作成
			CreateFilePack(dirname, v_finfo);
		}
		//-------------------------------------------------
		// 複数のディレクトリの中にあるファイルをパックする
		//-------------------------------------------------
		void Pack(const tstring packname, const vector<tstring>& dirs)
		{
			vector<FileInfo> v_finfo;
			for(UINT dirindex = 0; dirindex < dirs.size(); dirindex++)
				CreateFileInfo(dirs[dirindex].c_str(), v_finfo);
			CreateFilePack(packname, v_finfo);
		}
		//-------------------------------------------------
		// パックしたデータから指定ファイルヘッダ情報(ファイルサイズ)を取得
		//-------------------------------------------------
		bool GetHeader(const tstring datname, const tstring filename, int* filesize)
		{
			int fileindex = -1;
			vector<FileInfoHeader> fhinfos;
			if(!ReadHeader(datname, fhinfos))
				return false;
			fileindex = GetIndexFileInfo(filename, fhinfos);
			// 探しているファイルがなかった
			if(fileindex == -1)
				return false;
			*filesize = fhinfos[fileindex].filesize;
			return true;
		}
		//-------------------------------------------------
		// パックしたデータから指定ファイルデータを取得
		//-------------------------------------------------
		bool GetData(const tstring datname, const tstring filename, char* obuf, DWORD offset, DWORD bufsize, DWORD* obufsize)
		{
			int fileindex = -1;
			vector<FileInfoHeader> fhinfos;
		
			if(!ReadHeader(datname, fhinfos))
				return false;
			fileindex = GetIndexFileInfo(filename, fhinfos);
			// 探しているファイルがなかった
			if(fileindex == -1)
				return false;
			// データを移す
			long lDistance = offset + sizeof(int) + sizeof(FileInfoHeader) * fhinfos.size();
			for(int i = 0; i < fileindex; i++)
				lDistance += fhinfos[i].filesize;
			HANDLE hFile = CreateFile(datname.c_str(), GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			SetFilePointer(hFile, lDistance, NULL, FILE_CURRENT);
			ReadFile(hFile, obuf, bufsize, obufsize, NULL);
			// 次のファイルに読み込みを行っていないか、チェック
			if(offset + bufsize > fhinfos[fileindex].filesize)
			{
				MyDebug::outputDebugString(TEXT("次のファイルまで読み込みを行っている\n"));
				*obufsize = fhinfos[fileindex].filesize - offset;
				CloseHandle(hFile);
				return false;
			}
			// EOFチェック
			if(bufsize != *obufsize)
			{
				MyDebug::outputDebugString(TEXT("多分、EOFにたどり着いた\n"));
				CloseHandle(hFile);
				return false;
			}
			CloseHandle(hFile);
			return true;
		}
		//-------------------------------------------------
		// パックしたデータから指定ファイルハンドルを取得
		//-------------------------------------------------
		bool GetHandle(const tstring datname, const tstring filename, HANDLE* outh)
		{
			int fileindex = -1;
			vector<FileInfoHeader> fhinfos;
			HANDLE hFile = s_rmanager.GetHandle(datname, filename);
			if(hFile) // 既に取得してた	
			{
				*outh = hFile;
				return true;
			}
			if(!ReadHeader(datname, fhinfos))
				return false;
			fileindex = GetIndexFileInfo(filename, fhinfos);
			// 探しているファイルがなかった
			if(fileindex == -1)
				return false;
		
			hFile = CreateFile(datname.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			long lDistance = (long)(sizeof(int) + sizeof(FileInfoHeader) * fhinfos.size());
			for(int i = 0; i < fileindex; i++)
				lDistance += fhinfos[i].filesize;
			SetFilePointer(hFile, lDistance, NULL, FILE_CURRENT);
			s_rmanager.InsertHandle(datname, filename, hFile);
			*outh = hFile;
			return true;
		}
		//-------------------------------------------------
		// パックしたデータから指定ファイルポインタを取得
		//-------------------------------------------------
		bool GetFPointer(const tstring datname, const tstring filename, FILE** outfp, const tstring fmode)
		{
			int fileindex = -1;
			vector<FileInfoHeader> fhinfos;
			FILE* fp = s_rmanager.GetFile(datname, filename);
			if(fp) // 既に取得してた
			{
				*outfp = fp;
				return true;
			}
			if(!ReadHeader(datname, fhinfos))
				return false;
			fileindex = GetIndexFileInfo(filename, fhinfos);
			// 探しているファイルがなかった
			if(fileindex == -1)
				return false;
		
			_tfopen_s(&fp, datname.c_str(), fmode.c_str());
			long lDistance = (long)(sizeof(int) + sizeof(FileInfoHeader) * fhinfos.size());
			for(int i = 0; i < fileindex; i++)
				lDistance += fhinfos[i].filesize;
			fseek(fp, lDistance, SEEK_CUR);
			s_rmanager.InsertFP(datname, filename, fp);
			*outfp = fp;
			return true;
		}
		void CreateFileInfo(const tstring dirname, vector<FileInfo>& v_finfo)
		{
			vector<tstring> dirs;
			FileEnum::EnumFiles(dirname, dirs);
			for(int i = 0; dirs.size(); i++)
			{
				const tstring& name = dirs[i];
				FileInfo finfo;
				HANDLE hFile;
				DWORD dwfilesize;
				// ファイル名
				tstring str = TEXT("./") + dirname + TEXT("/") + name;
				_tcscpy_s(finfo.header.filename, str.size(), str.c_str());
				// ファイル容量
				hFile = CreateFile(finfo.header.filename, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				dwfilesize = GetFileSize(hFile ,NULL);
				// ファイルデータ写し
				finfo.srcdata = NEW char[dwfilesize];
				ReadFile(hFile, finfo.srcdata, dwfilesize, &finfo.header.filesize, NULL);
				CloseHandle(hFile);
				v_finfo.push_back(finfo);
			}
		}
		void CreateFilePack(const tstring packname, const vector<FileInfo>& v_finfo)
		{
			int filenum = (int)v_finfo.size();
			DWORD wSize, wSizetmp;
			tstring filename = TEXT("./") + packname + TEXT(".dat");
			HANDLE hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			// ヘッダ情報 [FileNum] [FileInfo0] [FileInfo1]...
			WriteFile(hFile, &filenum, sizeof(int), &wSize, NULL);
			for(int i = 0; i < filenum; i++)
			{
				WriteFile(hFile, &v_finfo[i].header, sizeof(FileInfoHeader), &wSizetmp, NULL);
				wSize += wSizetmp;
			}
			if(wSize != sizeof(int) + sizeof(FileInfoHeader) * filenum)
				MyDebug::outputDebugString(TEXT("ヘッダがうまく書き出していない可能性があります。\n"));
			// データ情報
			for(int i = 0; i < filenum; i++)
			{		
				WriteFile(hFile, v_finfo[i].srcdata, v_finfo[i].header.filesize, &wSize, NULL);
				if(wSize != v_finfo[i].header.filesize)
					MyDebug::outputDebugString(TEXT("ファイルがうまく書き出していない可能性があります。\n"));
			}
			CloseHandle(hFile);
		}
		bool ReadHeader(const tstring datname, vector<FileInfoHeader> &fhinfos)
		{
			DWORD rSize, rSizetmp;
			DWORD dwfilesize;
			DWORD filenum;
			HANDLE hFile = CreateFile(datname.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(hFile == 0 || hFile == INVALID_HANDLE_VALUE) // ファイルを読み込めなかった
			{
				MyDebug::outputDebugString(TEXT("探しているDATファイルは存在しませんでした。\n"));
				return false;
			}
			// ヘッダ情報を読み取る
			ReadFile(hFile, &filenum, sizeof(int), &rSize, NULL);
			dwfilesize = GetFileSize(hFile ,NULL);
			if(dwfilesize < filenum) // ファイルが不正っぽいとき
			{
				CloseHandle(hFile);
				return false;
			}
			for(DWORD i = 0; i < filenum; i++)
			{
				FileInfoHeader fhinfo;
				ReadFile(hFile, &fhinfo, sizeof(FileInfoHeader), &rSizetmp, NULL);
				fhinfos.push_back(fhinfo);
				rSize += rSizetmp;
			}
			if(rSize != sizeof(int) + sizeof(FileInfoHeader) * filenum)
				MyDebug::outputDebugString(TEXT("ヘッダがうまく読み込めてない可能性があります。\n"));
			CloseHandle(hFile);
			return true;
		}
		int GetIndexFileInfo(const tstring filename, vector<FileInfoHeader> &fhinfos)
		{
			int fileindex = -1;
			// 取るべきデータはどこにあるか捜索
			for(unsigned int i = 0; i < fhinfos.size(); i++)
			{
				if(fhinfos[i].filename == filename)
				{
					fileindex = i;
					break;
				}
			}
			if(fileindex == -1)
				MyDebug::outputDebugString(TEXT("filename : %sは存在しませんでした。\n"), filename.c_str());
			return fileindex;
		}
	}
}

namespace MyString {
	void tokenize(const tstring& input, vector<tstring>& result, const tstring& delim)
	{
		tstring::size_type first = 0;
		tstring::size_type last = 0;
		while ( (first = input.find_first_not_of(delim, last)) != tstring::npos ) {
			last = input.find_first_of(delim, first);
			result.push_back(input.substr(first, last == tstring::npos ? last : last-first));
		}
	}
	void tokenizeInv(const tstring& input, vector<tstring>& result, const tstring& delim)
	{
		tstring::size_type first = 0;
		tstring::size_type last = 0;
		while ( (first = input.find_first_of(delim, last)) != tstring::npos ) {
			last = input.find_first_not_of(delim, first);
			result.push_back(input.substr(first, last == tstring::npos ? last : last-first));
		}
	}
	enum MATHSYMBOL{ // 優先順で並んでいる
		BRACKET, // ()
		POW, // ^
		MULDIV, // */
		SURPLUS, // %
		PLUSMINUS, // +-
		NONE,
		SYMBOLNUM
	};
	MATHSYMBOL GetSymbolPriority(const tstring symbol)
	{
		tstring s = symbol;
		if(s[0] == ')' && s.size() >= 2) // TEXT(")+")というケースを考慮
			s = s.substr(1);
		if(s == TEXT("("))
			return NONE;
		else if(s == TEXT("^"))
			return POW;
		else if(s == TEXT("*") || s == TEXT("/"))
			return MULDIV;
		else if(s == TEXT("%"))
			return SURPLUS;
		else if(s == TEXT("+") || s == TEXT("-"))
			return PLUSMINUS;
		return NONE;
	}
	void tokenizeMath(const tstring& input, vector<tstring>& numbers, vector<tstring>& symbols)
	{
		// TODO : 括弧対応
		//  b fl n
		// 1*5+20*4 -> 1*5=a 2*4=b a+b=c ->
		// * * + , "1" "5" "2" "4" "" ""
		// 1*5+2 ->
		// * + , "1" "5" "" "2"
		// 1*(5*(2+4)) ->
		// + * * , "2" "4" "5" "" "1" "" 
		vector<int> idxnumbers;
		tstring delim = TEXT("()^*/%+-");
		tstring::size_type first = 0;
		tstring::size_type last = 0;
		int aindex[SYMBOLNUM] = {0};
		int loopnum = 0;
		while ( (first = input.find_first_of(delim, last)) != tstring::npos ) 
		{
			last = input.find_first_not_of(delim, first);
			tstring symbol = input.substr(first, last == tstring::npos ? last : last-first);
			// どこに挿入するべきか調べる
			MATHSYMBOL insidx = GetSymbolPriority(symbol);
			if(insidx != NONE) // 挿入先がある
			{
				tstring arg1 = TEXT("");
				tstring arg2 = TEXT("");
				tstring::size_type before = input.find_last_of(delim, first-1);
				tstring::size_type next = input.find_first_of(delim, last);
				if(before == tstring::npos)
					before = 0;
				else
					++before;
				arg1 = input.substr(before, first-before);
				arg2 = input.substr(last, next == tstring::npos ? next : next-last);
				// 名前を挿入
				{
					vector<tstring>::iterator itr;
					itr = numbers.begin();
					for(int i = 0; i < aindex[insidx] * 2; i++)
						++itr;
					itr = numbers.insert(itr, arg2);
					numbers.insert(itr, arg1);
				}
				// 記号を挿入
				{
					vector<tstring>::iterator itr;
					itr = symbols.begin();
					for(int i = 0; i < aindex[insidx]; i++)
						++itr;
					symbols.insert(itr, symbol);
				}
				// 名前インデックスを挿入
				{
					vector<int>::iterator itr;
					itr = idxnumbers.begin();
					for(int i = 0; i < aindex[insidx] * 2; i++)
						++itr;
					itr = idxnumbers.insert(itr, loopnum + 1);
					idxnumbers.insert(itr, loopnum);
				}
				// 挿入位置を更新
				for(int i = insidx; i < SYMBOLNUM; i++)
					++aindex[i];
				loopnum++;
			}
		}
		// 記号がない場合、数字のみがありえる
		if(symbols.empty())
			numbers.push_back(input);
		else // 式がある場合
		{
			// 名前の重複を避ける
			vector<int> usenametable(loopnum + 1, 0);
			for(unsigned int i = 0; i < numbers.size(); i++)
			{
				if(!usenametable[idxnumbers[i]]) 
					usenametable[idxnumbers[i]] = 1;
				else
					numbers[i] = TEXT("");
			}
		}
	}
	tstring eliminate(const tstring& input, const tstring& delim)
	{
		tstring rvalue = TEXT("");
		vector<tstring> token;
		tokenize(input, token, delim);
		for(unsigned int i = 0; i < token.size(); i++)
			rvalue += token[i];
		return rvalue;
	}
	void GetArgument(const tstring& funcstr, vector<tagVariable>& vreturn)
	{
		vector<tstring> args;
		tstring strvarvalue = funcstr;
		// ( の前まで移動
		if(funcstr.find(TEXT("(")) != tstring::npos)
			strvarvalue = funcstr.substr(funcstr.find(TEXT("(")));
		// strvarvalueから()を排除
		tstring value = eliminate(strvarvalue, TEXT("()"));
		tokenize(value, args, TEXT(","));
		for(unsigned int i = 0; i < args.size(); i++)
		{
			tagVariable tag;
			tag.name = SetArgument(TEXT(""), i);
			tag.ivalue = _ttoi(args[i].c_str());
			tag.svalue = args[i];
			vreturn.push_back(tag);
		}
	}
	double CalcFormulaStr(const tstring& value)
	{
		vector<tagVariable> vargs;
		return CalcFormulaStr(value, vargs);
	}
	double CalcFormulaStr(const tstring& value, const vector<tagVariable>& vargs)
	{
		double rvalue;
		double temp1, temp2;
		vector<tstring> numbers;
		vector<tstring> symbols;
		bool usetemp1flag = false;
		tokenizeMath(value, numbers, symbols);
		if(numbers.empty()) // 式は存在しない
			return 0;
		else if(symbols.empty()) // 数字のみ存在する
		{
			for(unsigned int x = 0; x < vargs.size(); x++)
			{
				if(vargs[x].name == numbers[0])
					return vargs[x].ivalue;
			}
			return 0; // 見つからなかった
		}
		for(unsigned int i = 0; i < symbols.size(); i++) 
		{
			tstring symbol = symbols[i];
			tstring arg1str = numbers[i * 2];
			tstring arg2str = numbers[i * 2 + 1];
			double arg1, arg2;
			unsigned int x;
			bool arg1numberflag = true;
			bool arg2numberflag = true;
			for(x = 0; x < vargs.size(); x++)
			{
				if(vargs[x].name == arg1str)
				{
					arg1 = vargs[x].ivalue;
					arg1numberflag = false;
				}
				if(vargs[x].name == arg2str)
				{
					arg2 = vargs[x].ivalue;
					arg2numberflag = false;
				}
			}
			if(arg1numberflag)
			{
				if(arg1str != TEXT("")) // 変数ではなく、数値
					arg1 = lexical_cast<double>(arg1str);
				else // 前回の値を使用
				{
					arg1 = temp1;
					usetemp1flag = false;
				}
			}
			if(arg2numberflag)
			{
				if(arg2str != TEXT(""))
					arg2 = lexical_cast<double>(arg2str);
				else if(arg1str != TEXT(""))
				{
					arg2 = temp1;
					usetemp1flag = false;
				}
				else
					arg2 = temp2;
			}
			// 計算開始
			if(symbol == TEXT("+")) // 加算
				rvalue = arg1 + arg2;
			else if(symbol == TEXT("-")) // 減算
				rvalue = arg1 - arg2;
			else if(symbol == TEXT("*")) // 乗算
				rvalue = arg1 * arg2;
			else if(symbol == TEXT("/")) // 除算
				rvalue = arg1 / arg2;
			else if(symbol == TEXT("%")) // 除余
				rvalue = (int)arg1 % (int)arg2;
			else if(symbol == TEXT("^")) // 階乗
				rvalue = pow(arg1, arg2);
			// tempに一時保管
			if(usetemp1flag)
				temp2 = rvalue;
			else
			{
				temp1 = rvalue;
				usetemp1flag = true;
			}
		}
		return rvalue;
	}
	void TransString(tstring& value, const vector<tagVariable>& vargs)
	{
		for(unsigned int i = 0; i < vargs.size(); i++)
		{
			if(value.find(vargs[i].name) != tstring::npos) // 該当した場合
			{
				// TODO : valueが文字列か、数字かを判断しなくてはいけない
				// 簡単な見分け方
				// valueが文字列の場合
				if(value[0] == '$') // 前にドルがついていた場合、文字列置換として扱う
				{
					tstring head = value.substr(1, value.find(vargs[i].name) - 1);
					tstring foot = value.substr(value.find(vargs[i].name) + vargs[i].name.size());
					value = head + vargs[i].svalue + foot;
				}
				else
				{
					// valueが数式の場合
					double v = CalcFormulaStr(value, vargs);
					value = lexical_cast<tstring>(v);
				}
			}
		}
	}
	bool IsFunctionName(const tstring& str, const tstring& funcname, bool bracketflag)
	{
		int startargindex = (int)str.find(TEXT("("));
		if(startargindex == tstring::npos)
		{
			if(!bracketflag && str == funcname)
				return true;
			return false;
		}
		return funcname == str.substr(0, startargindex);
	}
	//////////////////////////////////////////////////////////
	/// 1,000,000という風にString変換する
	//////////////////////////////////////////////////////////
	tstring TransNum2RankStr(int number)
	{
		tstring str = lexical_cast<tstring>(number);
		for(int i = str.size() - 3; i > 0; i -= 3)
			str.insert(i, TEXT(","));
		return str;
	}
}

namespace MyRand {
	void seed()
	{
	}

	// [-value,value)
	double gendouble(double value)
	{
		double rate = (((double)rand() / (RAND_MAX + 1)) - 0.5f) * 2;
		return value * rate;
	}

	// [0,value)
	double gendoubleplus(double value)
	{
		return gendouble(value) + value;
	}

	// [-value,value]
	int genint(int value)
	{
		int rvalue = (int)(gendouble(1) * (value + 1));
		return rvalue;
	}

	// [0,value]
	int genintplus(int value)
	{
		int rvalue = (int)(gendoubleplus(1) * (value + 1));
		return rvalue;
	}

	bool genbool()
	{
		return (genintplus(1)) ? true : false;
	}
}

namespace MyWindows {
	//////////////////////////////////////////////////////////
	/// スレッドを作る関数
	/// @param[out] out_hThread スレッドハンドル（失敗した場合はNULL
	/// @param[in] _StartAddress メイン関数ポインタ
	/// @param[in] arg メイン関数の引数ポインタ
	/// @param[in] threadpriority スレッドの優先順位
	/// @return スレッド識別子
	//////////////////////////////////////////////////////////
	DWORD CreateThread(HANDLE* out_hThread, unsigned (__stdcall * _StartAddress) (void *), void* arg, int threadpriority)
	{
		DWORD thID;
		// まず、サブスレッドを作成
		*out_hThread = (HANDLE)_beginthreadex(NULL, 0, _StartAddress, arg, CREATE_SUSPENDED, (unsigned int*)&thID);
		if (*out_hThread == 0) {
			MyDebug::outputDebugString(TEXT("スレッド作成失敗"));
			return 0;
		}
		// 作ったら、優先順位を決めよう。
		SetThreadPriority(*out_hThread, threadpriority);
		// 非シグナル状態にします
		ResumeThread(*out_hThread);
		return thID;
	}
	//////////////////////////////////////////////////////////
	/// スレッドを壊す関数
	/// @param[in,out] hThread スレッドハンドル
	//////////////////////////////////////////////////////////
	void DestroyThread(HANDLE& hThread)
	{
		if(hThread)
		{
			WaitForSingleObject(hThread, INFINITE);
			if(!CloseHandle(hThread))
				MyDebug::outputDebugString(TEXT("エラー\n"));
			hThread = NULL;
		}
	}
}