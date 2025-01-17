/*****************************************************************************************
//	界面--聊天窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-27
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiSkills.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/SkillDef.h"
#include "../../../core/src/coreshell.h"
#include "../../../engine/src/kdebug.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;

#define 	SCHEME_INI_SHEET			"UiSkillsSheet.ini"
#define 	SCHEME_INI_FIGHT			"UiSkillsLive.ini"
#define 	SCHEME_INI_FIGHT_SUB_PAGE	"UiSkillsFightSub.ini"
#define		SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE	9
#define		AUTO_SET_IMMED_SKILL_MSG_ID					"24"

KUiFightSkillSubPage::KUiFightSkillSubPage()
{
	m_nRemainSkillPoint = 0;
//	m_nSubPagIndex = 0;
}

//初始化
void KUiFightSkillSubPage::Initialize(/*int nSubPageIndex*/)
{
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i ++)
	{
		AddChild(&m_FightSkills[i]);
		m_FightSkills[i].Celar();
		m_FightSkills[i].SetContainerId((int)UOC_SKILL_LIST);
	}
//	m_nSubPagIndex = nSubPageIndex;
}

//载入界面方案
void KUiFightSkillSubPage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT_SUB_PAGE, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
		{
			sprintf(Buff, "Skill_%d", i);
			m_FightSkills[i].Init(&Ini, Buff);
			m_FightSkills[i].EnablePickPut(false);
		}
		Ini.GetInteger("SkillText", "Font", 12, &m_SkillTextParam.nFont);
		Ini.GetInteger2("SkillText", "Offset",
			(int*)&m_SkillTextParam.Offset.cx, (int*)&m_SkillTextParam.Offset.cy);
		Ini.GetString("SkillText", "Color", "", Buff, 16);
		m_SkillTextParam.Color = GetColor(Buff);
	}
}

//更新升级点数
void KUiFightSkillSubPage::UpdateRemainPoint(int nPoint)
{
	m_nRemainSkillPoint = nPoint;
}

//更新升级点数
void KUiFightSkillSubPage::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill && nIndex >= 0 && nIndex < FIGHT_SKILL_COUNT_PER_PAGE);
	m_FightSkills[nIndex].HoldObject(pSkill->uGenre, pSkill->uId, 0, 0);
}

//更新数据
void KUiFightSkillSubPage::UpdateData(KUiSkillData* pSkills)
{
	_ASSERT(pSkills);
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
		m_FightSkills[i].HoldObject(pSkills[i].uGenre, pSkills[i].uId, pSkills[i].nLevel, 0);
}

//窗口函数
int	KUiFightSkillSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_LEFT_CLICK_ITEM:
	{
		if (uParam)
		{
			KUiDraggedObject* pObj = (KUiDraggedObject*)uParam;
			if (pObj->uGenre != CGOG_NOTHING)
			{
				if (m_nRemainSkillPoint > 0)
				{
					if (pObj->uGenre != CGOG_NOTHING)
					{
						m_nRemainSkillPoint --;	// 使用技能点数
						g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_FIGHT, pObj->uId);
					}
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_NONE;
					Msg.byParamSize = 0;
					Msg.byPriority = 0;
					Msg.eType = SMT_NORMAL;
					Msg.uReservedForUi = 0;
					strcpy(Msg.szMessage, "觤 k� n╪g hi謓 t筰 kh玭g !");
					KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
				}
			}
		}
		break;
	}
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}

//绘制窗口
void KUiFightSkillSubPage::PaintWindow()
{
	KWndPage::PaintWindow();
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
	{
		KUiDraggedObject	Obj;
		m_FightSkills[i].GetObject(Obj);
		if (Obj.uGenre != CGOG_NOTHING)
		{				
			int nLeft, nTop, nWidth, nHeight;
			m_FightSkills[i].GetAbsolutePos(&nLeft, &nTop);
			m_FightSkills[i].GetSize(&nWidth, &nHeight);
			g_pCoreShell->DrawGameObj(CGOG_SKILL_FIGHT, Obj.uId, nLeft, nTop, nWidth, nHeight, 0);

			unsigned int dwColor = 0; 
			int nLevel = 0;
			if (g_pCoreShell->GetSkillData(Obj.uId, &nLevel) == TRUE)
				dwColor = 0xff3130ff;
			else
				dwColor = m_SkillTextParam.Color;	
			if (nLevel)
			{
				char	szLevel[8];
				int		nLen;
				itoa(nLevel, szLevel, 10);
				nLen = strlen(szLevel);
				nLeft += m_SkillTextParam.Offset.cx;
				nTop += m_SkillTextParam.Offset.cy;
				nLeft += (nWidth - nLen * m_SkillTextParam.nFont / 2) / 2;

				g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
					dwColor, 0);
			}
		}
	}
}

//初始化
void KUiFightSkillSub::Initialize()
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
	{
		m_SubPages[i].Initialize();
		AddPage(&m_SubPages[i], &m_SubPageBtn[i]);
	}
}

//载入界面方案
void KUiFightSkillSub::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPageSet::Init(&Ini, "Main");
		m_oFixPos.x = 0;
		m_oFixPos.y = 0;
		SetPosition(0, 0);
		for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		{
			m_SubPages[i].LoadScheme(pScheme);
			sprintf(Buff, "SubPageBtn_%d", i);
			m_SubPageBtn[i].Init(&Ini, Buff);
		}
		Show();
	}
}

//更新升级点数
void KUiFightSkillSub::UpdateRemainPoint(int nPoint)
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateRemainPoint(nPoint);
}

//更新技能
void KUiFightSkillSub::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill);
	int nPage = nIndex / FIGHT_SKILL_COUNT_PER_PAGE;
	nIndex = nIndex % FIGHT_SKILL_COUNT_PER_PAGE;
	_ASSERT(nPage >= 0 && nPage < FIGHT_SKILL_SUB_PAGE_COUNT);
	m_SubPages[nPage].UpdateSkill(pSkill, nIndex);
}

//更新数据
void KUiFightSkillSub::UpdateData()
{
	KUiSkillData	Skills[FIGHT_SKILL_COUNT];
	g_pCoreShell->GetGameData(GDI_FIGHT_SKILLS, (unsigned int)Skills, 0);
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateData(&Skills[i * FIGHT_SKILL_COUNT_PER_PAGE]);
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiFightSkill::Initialize()
{
	m_InternalPad.Initialize();
	AddChild(&m_InternalPad);
	AddChild(&m_RemainSkillPoint);
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiFightSkill::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_InternalPad.LoadScheme(pScheme);
		m_RemainSkillPoint.Init(&Ini, "RemainPoint");
	}
}

//--------------------------------------------------------------------------
//	功能：更新数据
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateData()
{
	int nRemainSkillPoint = g_pCoreShell->GetGameData(GDI_FIGHT_SKILL_POINT, 0, 0);
	m_InternalPad.UpdateRemainPoint(nRemainSkillPoint);
	m_RemainSkillPoint.SetIntText(nRemainSkillPoint);
	m_InternalPad.UpdateData();
}

//--------------------------------------------------------------------------
//	功能：更新技能
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	m_InternalPad.UpdateSkill(pSkill, nIndex);
}

//--------------------------------------------------------------------------
//	功能：更新升级点数
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateRemainPoint(int nPoint)
{
	m_RemainSkillPoint.SetIntText(nPoint);
	m_InternalPad.UpdateRemainPoint(nPoint);

}


KUiSkills* KUiSkills::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiSkills* KUiSkills::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiSkills;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_FightSkillPad.UpdateData();

		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：如果窗口正被显示，则返回实例指针
//--------------------------------------------------------------------------
KUiSkills* KUiSkills::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口，同时可以选则是否删除对象实例
//--------------------------------------------------------------------------
void KUiSkills::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiSkills::Initialize()
{
	m_FightSkillPad.Initialize();
	AddPage(&m_FightSkillPad, &m_FightSkillPadBtn);

	AddChild(&m_Close);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiSkills::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHEET);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_FightSkillPadBtn.Init(&Ini, "FightBtn");
		m_pSelf->m_Close           .Init(&Ini, "CloseBtn");

		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
	}
}

//--------------------------------------------------------------------------
//	功能：更新技能
//--------------------------------------------------------------------------
void KUiSkills::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill)
	{
		if (m_pSelf)
		{
			if (pSkill->uGenre == CGOG_SKILL_FIGHT)
				m_pSelf->m_FightSkillPad.UpdateSkill(pSkill, nIndex);
		}
		if (g_pCoreShell)
		{
			KUiPlayerAttribute	Info;
			memset(&Info, 0, sizeof(KUiPlayerAttribute));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
			if (Info.nLevel <= SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE)
			{
				g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, (unsigned int)pSkill, 1);
				KSystemMessage	Msg;
				Msg.byConfirmType = SMCT_NONE;
				Msg.byParamSize = 0;
				Msg.byPriority = 0;
				Msg.eType = SMT_NORMAL;
				Msg.uReservedForUi = 0;

				KIniFile*	pIni = g_UiBase.GetCommConfigFile();
				if (pIni)
				{
					if (pIni->GetString("InfoString", AUTO_SET_IMMED_SKILL_MSG_ID,
						"", Msg.szMessage, sizeof(Msg.szMessage)))
					{
						KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
					}
					g_UiBase.CloseCommConfigFile();
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
//	功能：更新战斗技能升级点数
//--------------------------------------------------------------------------
void KUiSkills::UpdateFightRemainPoint(int nPoint)
{
	m_FightSkillPad.UpdateRemainPoint(nPoint);
}

//窗口函数
int KUiSkills::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_Close)
	{
		Hide();	// 关闭装备框
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}