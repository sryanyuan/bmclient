#ifndef _INC_RENDERITEMCONTROL_
#define _INC_RENDERITEMCONTROL_
//////////////////////////////////////////////////////////////////////////
#include "GameRenderObject.h"
#include <string>
//////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////
class BaseRenderItem
{
public:
	BaseRenderItem();
	virtual ~BaseRenderItem();

public:

protected:
	HTEXTURE m_tex;
	string m_xText;
	RECT m_rcClient;
	DWORD m_dwColor;
};
//////////////////////////////////////////////////////////////////////////
#endif