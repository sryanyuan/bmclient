#include "GameBigStoreDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	(275+70+10)
#define DLG_HEIGHT	(370-30+160)

const int s_nLeftOft = 14;
const int s_nTopOft = 76;
//////////////////////////////////////////////////////////////////////////
GameBigStoreDlg::GameBigStoreDlg()
{
	m_rcClient.left = WINDOW_WIDTH - DLG_WIDTH;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.top = 20;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT;
	m_nPageIndex = 0;
	ClearItem();

	SetBkStype(1);
	CalcCloseButtonPosition();
	m_pRender = NEW_EMPTY_SPRITE;
	SetWindowTitle("������");

	CenterWindow();
	Move(0, -20);
	SetVisible(false);
	/*for(int i = 0; i < MAX_BIGSTORE_NUMBER; ++i)
	{
		AddItem(i + 1);
	}*/
}

GameBigStoreDlg::~GameBigStoreDlg()
{
	SAFE_DELETE(m_pRender);
}


void GameBigStoreDlg::Render()
{
	__super::Render();

	HTEXTURE tex = 0;
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(40);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 328, 410);
		m_pRender->Render(RELATIVE_X(s_nLeftOft), RELATIVE_Y(s_nTopOft));
	}

	int nItemIndex = 0;
	ItemAttrib* pItem = NULL;
	int nRenderX = 0;
	int nRenderY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);

	for(int i = 0; i < MAX_BIGSTORE_NUMBER; ++i)
	{
		pItem = &m_stItems[i];
		if(pItem->type != ITEM_NO)
		{
			nRenderX = i % 8 * 41 + 4 + s_nLeftOft;
			nRenderY = i / 8 * 41 + 4 + s_nTopOft;
			tex = pTexMgr->GetTexture(pItem->tex);
			if(tex)
			{
				nTexWidth = pTexMgr->GetTextureWidth(pItem->tex);
				nTexHeight = pTexMgr->GetTextureHeight(pItem->tex);

				//nRenderX += (34 / 2 - nTexWidth / 2);
				//nRenderY += (34 / 2 - nTexHeight / 2);

				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0,
					nTexWidth, nTexHeight);
				m_pRender->Render(RELATIVE_X(nRenderX + 34 / 2 - nTexWidth / 2),
					RELATIVE_Y(nRenderY + 34 / 2 - nTexHeight / 2));

				if(pItem->type == ITEM_COST)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX),
						RELATIVE_Y(nRenderY + 20),
						"%d",
						pItem->atkSpeed);
				}
			}
		}
	}
}

bool GameBigStoreDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	POINT ptTest = _mp;
	ptTest.x -= m_rcClient.left;
	ptTest.y -= m_rcClient.top;
	ptTest.x -= s_nLeftOft;
	ptTest.y -= s_nTopOft;
	bool bInItem = false;
	GameBagDlg2* pBagDlg2 = GameMainOptUI::GetInstance()->GetBagDlg2();
	ItemAttrib* pItem = NULL;

	if(ptTest.x >= 0 &&
		ptTest.x <= 328 &&
		ptTest.y >= 0 &&
		ptTest.y <= 410)
	{
		int nCol = -1;
		int nRow = -1;

		nCol = ptTest.x / 41;
		nRow = ptTest.y / 41;

		if(nCol >= 0 &&
			nCol < 8 &&
			nRow >= 0 &&
			nRow < 10)
		{
			if(pBagDlg2->IsItemSel())
			{
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					pItem = pBagDlg2->GetSelItem();

					if(pItem->type != ITEM_NO)
					{
						PkgPlayerShopOpReq req;
						req.bOp = SHOP_BIGSTORE_STORE;
						req.dwData = pItem->tag;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
						pBagDlg2->CancelSel();
					}
				}
			}

			int nColLeft = ptTest.x % 41;
			int nRowLeft = ptTest.y % 41;

			if(nColLeft >= 4 &&
				nColLeft <= 38 &&
				nRowLeft >= 4 &&
				nRowLeft <= 38)
			{
				OnItem(nCol + nRow * 8);
				bInItem = true;
			}
		}
	}

	if(!bInItem)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
	}

	return true;
}

void GameBigStoreDlg::OnItem(int _idx)
{
	if(_idx < 0 ||
		_idx >= MAX_BIGSTORE_NUMBER)
	{
		return;
	}

	GameBagDlg2* pBagDlg2 = GameMainOptUI::GetInstance()->GetBagDlg2();
	GameIntroDlg* pIntroDlg = GameMainOptUI::GetInstance()->GetIntroDlg();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	ItemAttrib* pItem = NULL;
	int nSelIndex = _idx;

	{
		bool bHaveItem = false;

		if(m_stItems[nSelIndex].type != ITEM_NO)
		{
			pIntroDlg->ShowItemAttrib(&m_stItems[nSelIndex]);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&m_stItems[nSelIndex]);
			bHaveItem = true;
		}
		else
		{
			pIntroDlg->ShowNothing();
		}

		if(bHaveItem)
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				PkgPlayerShopOpReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.bOp = SHOP_BIGSTORE_FETCH;
				req.dwData = m_stItems[nSelIndex].tag;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
			}
		}
	}
}

void GameBigStoreDlg::ClearItem()
{
	ZeroMemory(m_stItems, sizeof(m_stItems));
	m_nItemCounter = 0;
}

void GameBigStoreDlg::AddItem(int _id)
{
	if(m_nItemCounter >= MAX_BIGSTORE_NUMBER)
	{
		return;
	}

	if(GameInfoManager::GetInstance()->GetItemAttrib(_id, &m_stItems[m_nItemCounter]))
	{
		++m_nItemCounter;
	}
	else
	{
		m_stItems[m_nItemCounter].type = ITEM_NO;
	}
}

void GameBigStoreDlg::AddItem(const ItemAttrib* _pItem)
{
	int nIndex = -1;
	for(int i = 0; i < MAX_BIGSTORE_NUMBER; ++i)
	{
		if(m_stItems[i].type == ITEM_NO)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex != -1)
	{
		memcpy(&m_stItems[nIndex], _pItem, sizeof(ItemAttrib));
	}
}

ItemAttrib* GameBigStoreDlg::GetItem(DWORD _dwTag)
{
	for(int i = 0; i < MAX_BIGSTORE_NUMBER; ++i)
	{
		if(m_stItems[i].type != ITEM_NO)
		{
			if(m_stItems[i].tag == _dwTag)
			{
				return &m_stItems[i];
			}
		}
	}
	return NULL;
}

bool GameBigStoreDlg::OnShowDialog()
{
	MoveTo(0, 0);
	return true;
}

bool GameBigStoreDlg::OnCloseDialog()
{
	GameBagDlg2* pBagDlg = GET_DLG(BagDlg2);
	if(pBagDlg->IsVisible())
	{
		pBagDlg->MoveTo(0, 0);
	}
	return true;
}

void GameBigStoreDlg::BindAllItem()
{
	for(int i = 0; i < sizeof(m_stItems) / sizeof(m_stItems[0]); ++i)
	{
		if(m_stItems[i].type != ITEM_NO)
		{
			SET_FLAG(m_stItems[i].atkPois, POIS_MASK_BIND);
		}
	}
}