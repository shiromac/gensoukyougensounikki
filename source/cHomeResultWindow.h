#pragma once
#include "cGameWindow.h"
#include "cParameterGage.h"

typedef int(*ExperienceTabelFunction)(int);

class cHomeResultWindow : public cGameWindow
{
public:
	cHomeResultWindow();
	virtual ~cHomeResultWindow();

	virtual void Init(cRenderDevice *pDev,
						const int levelBefore,
						const int experienceBefore,
						const int experienceAddition,
						const ExperienceTabelFunction experienceTableFunction,
						const double yakuBefore,
						const double yakuAddition
		);

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
protected:
	enum State
	{
		kInited,
		kIncreasePoint,
		kFinished,
	};
	State state_;

	int levelBefore_;
	int experienceBefore_;
	int experienceAddition_;

	ExperienceTabelFunction experienceTableFunction_;

	double yakuBefore_;
	double yakuAddition_;

	double counter_;

protected:
	int retain_experience_;
	double retain_yaku_;

	int now_experience_;
	double now_yaku_;

	cParameterGage expGage;

protected:
	int first_level_;
	int now_level_;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cHomeResultWindow> pcHomeResultWindow;
