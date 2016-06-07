#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
IceDefenderMonster::IceDefenderMonster()
{
	m_fLastUpdateSkill = 0.0f;
}

void IceDefenderMonster::Render()
{
	if(GetStatus() == PST_EXT1)
	{
		HGE* hge = AfxGetHge();
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		if(!IsVisible())
		{
			return;
		}

		if(m_texs)
		{
			GameTextureManager* res = m_texs;
			//	画怪物
			int nTextIndex = 1530 + GetDirection() * 10 + m_bCurFrame;
			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				if(GameScene::sThis->GetTarget() == this)
				{
					HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextIndex);
					if(texSelected != NULL)
					{
						tex = texSelected;
					}
				}

				if(!m_pRender)
				{
					//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
					m_pRender = new hgeSprite(tex, 0, 0, res->GetTextureWidth(nTextIndex),
						res->GetTextureHeight(nTextIndex));
				}
				else
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, res->GetTextureWidth(nTextIndex), 
						res->GetTextureHeight(nTextIndex));
				}
				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				m_pRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
				//	记录当前纹理
				m_nCurrentTextureIndex = nTextIndex;
			}
		}
	}
	else
	{
		__super::Render();
	}

	if(GetRenderMode() == ORM_TRANSPARENT &&
		IsVisible())
	{
		if(GetStatus() == PST_ATTACKNOWEAPON)
		{
			//	额外攻击效果
			GameTextureManager* res = m_texs;
			//	画怪物
			int nTextIndex = 1610 + 10 * GetDirection() + GetCurFrame();
			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0,
					0,
					m_texs->GetTextureWidth(nTextIndex),
					m_texs->GetTextureHeight(nTextIndex));
				MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
		else if(GetStatus() == PST_EXT1)
		{
			//	额外攻击效果
			GameTextureManager* res = m_texs;
			//	画怪物
			int nTextIndex = 1690 + 10 * GetDirection() + GetCurFrame();
			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0,
					0,
					m_texs->GetTextureWidth(nTextIndex),
					m_texs->GetTextureHeight(nTextIndex));
				MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
	}
}

void IceDefenderMonster::Update(float _dt)
{
	if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSkill = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSkill = 0.0f;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

bool IceDefenderMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (GetCurFrame() == (6 - 1));
	}
	else
	{
		return __super::IsLastFrame();
	}
}

/*void IceDefenderMonster::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		END_HANDLE_PACKET(_pPkt)

		__super::OnPacket(_pPkt);
}

void IceDefenderMonster::DoPacket(const PkgObjectActionNot& _not)
{
	if(_not.uAction == ACTION_EXT1)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}
	else if(_not.uAction == ACTION_DEAD)
	{
		//	Magic effect
		MagicElement* pAni = new MagicMonsPosAni;
		if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 1770 + 10 * GetDirection(), 10, 80))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
		}
		else
		{
			SAFE_DELETE(pAni);
		}
	}

	__super::DoPacket(_not);
}*/
void IceDefenderMonster::OnObjectAction(const PkgObjectActionNot& _not)
{
	if(_not.uAction == ACTION_EXT1)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}
	else if(_not.uAction == ACTION_DEAD)
	{
		//	Magic effect
		MagicElement* pAni = new MagicMonsPosAni;
		if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 1770 + 10 * GetDirection(), 10, 80))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
		}
		else
		{
			SAFE_DELETE(pAni);
		}
	}

	__super::OnObjectAction(_not);
}