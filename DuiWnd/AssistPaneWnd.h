#ifndef _INC_ASSISTPANEWND_
#define _INC_ASSISTPANEWND_
//////////////////////////////////////////////////////////////////////////
#include <UIlib.h>
#include <set>
#include <string>
#include <map>
#include <list>
//////////////////////////////////////////////////////////////////////////
typedef std::set<string> StringSet;
typedef std::map<int, int> KintVintMap;
//////////////////////////////////////////////////////////////////////////
#define TIMER_REFRESHUI		0x0001
//////////////////////////////////////////////////////////////////////////
class AssistPaneWnd : public DuiLib::WindowImplBase
{
public:
	AssistPaneWnd();
	virtual ~AssistPaneWnd();

protected:
	virtual DuiLib::CDuiString GetSkinFolder();
	virtual DuiLib::CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage( HWND hWnd );

	virtual void Notify(DuiLib::TNotifyUI& msg);

public:
	void OnNotifyTextChanged(DuiLib::TNotifyUI& msg);
	void OnNotifySelectChanged(DuiLib::TNotifyUI& msg);
	void OnNotifyWindowInit(DuiLib::TNotifyUI& msg);
	void OnNotifyClick(DuiLib::TNotifyUI& msg);
	void OnNotifyTimer(DuiLib::TNotifyUI& msg);
	void OnNotifyReturn(DuiLib::TNotifyUI& msg);
	void OnNotifySetFocus(DuiLib::TNotifyUI& msg);
	void OnNotifyItemClick(DuiLib::TNotifyUI& msg);

public:
	inline void SetParentHWND(HWND _hParent)
	{
		m_hParentHWND = _hParent;
	}

public:
	void AdjustWindowPos();
	void TranslateDispatchMessage(const MSG* pMsg);

public:
	//	Game interface
	bool CheckItemVisible(const char* _pszItemName);
	bool CheckItemAlert(const char* _pszItemName);
	bool CheckMonsterAlert(const char* _pszMonsName);

	int CheckMappedKey(int _nKey);

	//	internal implement
	void LoadItemVisibleFromLocal(){}
	void LoadItemAlertFromLoad(){}
	void LoadMonsterAlertFromLocal(){}

	void WriteItemVisibleToLocal(){}
	void WriteItemAlertToLocal(){}
	void WriteMonsterAlertToLocal(){}

	void LoadConfigFromLocal();
	void WriteConfigToLocal();

private:
	void ProcessTabChange(DuiLib::TNotifyUI& msg);

	void ProcessPageOK(DuiLib::TNotifyUI& msg);
	void ApplyItemVisible();
	void ApplyKeyMap();

	void CopyKeySettingToUI(int _nKey, int _nMappedKey);
	void InsertItemVisible(const char* _pszItem);

	void ApplyItemNamesSearch(const char* _pszPattern);
	void ApplyMonsNamesSearch(const char* _pszPattern);

protected:
	HWND m_hParentHWND;

	//	for item visible using
	StringSet m_xItemVisibleSet;
	//	for item alert using
	StringSet m_xItemAlertSet;
	//	for monster alert using
	StringSet m_xMonsterAlertSet;
	// for quick search item id and monster id
	std::map<std::string, int> m_xItemNamesSearchMap;
	std::map<std::string, int> m_xMonsNamesSearchMap;

	//	for key map
	KintVintMap m_xKeyMap;

	//	controls
	DuiLib::CTabLayoutUI* m_pTabLayout;

	// Last focus edit
	DuiLib::CEditUI* m_pLastFocusEdit;
};

//////////////////////////////////////////////////////////////////////////
template<typename StringArray>
void	StringSplit(const std::string& src,	const std::string tok, StringArray&	arr, bool bIgnoreEmptyItem	= true)
{
	if(	src.empty()	)
	{
		return;
	}
	if(	tok.empty()	)
	{
		arr.push_back(src);
		return;
	}

	std::string::size_type pre_index = 0, index	= 0, len = 0;
	while( (index =	src.find_first_of(tok, pre_index)) != std::string::npos	)
	{
		len = index - pre_index;
		if( !bIgnoreEmptyItem || len != 0 )
		{
			arr.push_back(src.substr(pre_index,	len));
		}
		pre_index =	index +	1;
	}

	arr.push_back(src.substr(pre_index));
}

int TranslateKey(int _nKey);
//////////////////////////////////////////////////////////////////////////
#endif