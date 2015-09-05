#pragma once

#include <cstdlib>
#include <new>
#include <memory>


#include <crtdbg.h>
// crtdbg.h をインクルードしたあとに _CRTDBG_MAP_ALLOC を定義してやる
// 前でも問題ないけど、それじゃつまらんので（ぉ
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
