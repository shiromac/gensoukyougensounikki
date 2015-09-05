#pragma once

/*
void cAnimationManager::GetNormalAttackAttribute(int Input_ID,
		tstring &effectFileName, int &vi, double &speed, tstring &seFileName)
{
	switch(Input_ID)
	{
	case ATTACKATTRI_PUNCH1:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_punch1.wav");
		break;
	case ATTACKATTRI_PUNCH1_DOUBLESPEED:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 2;
		seFileName = _T("attack_punch1.wav");
		break;
	case ATTACKATTRI_SWING1:
		effectFileName = _T("effect\\attack_swing.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_swing1.wav");
		break;
	default:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_punch1.wav");
	}
}
*/

#define ATTACKATTRI_PUNCH1 \
		(tstring)_T("effect\\attack_hit.png"),/*effectFileName*/\
		0,/*vi*/\
		1,/*speed*/\
		(tstring)_T("attack_punch1.wav")/*seFileName*/

#define ATTACKATTRI_PUNCH1_DOUBLESPEED \
		(tstring)_T("effect\\attack_hit.png"),/*effectFileName*/\
		0,/*vi*/\
		2,/*speed*/\
		(tstring)_T("attack_punch1.wav")/*seFileName*/

#define ATTACKATTRI_SWING1 \
		(tstring)_T("effect\\attack_swing.png"),/*effectFileName*/\
		0,/*vi*/\
		1,/*speed*/\
		(tstring)_T("attack_swing1.wav")/*seFileName*/

#define DEFENSEATTRI_SWING1 \
		(tstring)_T("effect\\defense_swing.png"),/*effectFileName*/\
		0,/*vi*/\
		1,/*speed*/\
		(tstring)_T("defense_swing1.wav")/*seFileName*/


