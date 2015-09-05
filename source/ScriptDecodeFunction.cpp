#include "ScriptDecodeFunction.h"
#include <cScriptRLayer.h>
#include <cScriptReader.h>
#include <debug.h>
#include <StyleString.h>

bool DropingDistributionDecode(pcScriptRLayer decodeData, cDropingDistribution& dropingDistribution)
{
	if(decodeData == NULL)
	{
		dropingDistribution.clear();
		return false;
	}
	else
	{
		
		tstring groupstr(_T("グループ分布"));
		pcScriptRLayer groupdata = decodeData->pmember(groupstr);
		if(groupdata == NULL)
		{
			dropingDistribution.setItemGroupPos(1,1,1,1,1,1,1,1,0);
		}
		else
		{
			if(groupdata->script().size() != 9)
			{
				OnAssert((groupstr).c_str(),-1,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				dropingDistribution.setItemGroupPos(1,1,1,1,1,1,1,1,0);
			}
			else
			{
				vector<vector<double>> vvdouble;
				split(groupdata->script(),_T("|"),vvdouble,(decodeData->name()+groupstr).c_str());
				dropingDistribution.setItemGroupPos(vvdouble[0][0],vvdouble[1][0],vvdouble[2][0],vvdouble[3][0],vvdouble[4][0],vvdouble[5][0],vvdouble[6][0],vvdouble[7][0],vvdouble[8][0]);
			}
		}


		int size = decodeData->script().size();

		int i;
		for(i=0;i<size;i++)
		{
			vector<int> vint;
			split(decodeData->script()[i],_T("|"),vint,decodeData->name());
			if(vint.size() != 4)
			{
				OnAssert(decodeData->name().c_str(),i,false,_T(" 書式にエラーが発生しました。<error> There is an error of form."));
				continue;
			}
			//vint[1] = (int)((UByte)(vint[1]));
			//vint[2] = (int)((UByte)(vint[2]));

			dropingDistribution.set(vint[0], vint[3]);
		}

	}

	return true;
}