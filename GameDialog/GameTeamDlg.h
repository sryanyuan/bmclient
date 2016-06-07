#ifndef _INC_GAMETEAMDLG_
#define _INC_GAMETEAMDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameTeamDlg : public GameNewDlg
{
public:
	GameTeamDlg();
	virtual ~GameTeamDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _ptMouse);
};
//////////////////////////////////////////////////////////////////////////
#endif