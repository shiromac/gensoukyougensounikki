#pragma once

#include "cDropingDistribution.h"

#include <fileManage_ph.h>

//デコードできたらtrueを返す。
//失敗した場合dropingDistributionの内部状態は初期化される。
bool DropingDistributionDecode(pcScriptRLayer decodeData, cDropingDistribution& dropingDistribution);