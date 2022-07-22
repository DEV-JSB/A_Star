#include "pch.h"
#include "CMap.h"
#include "CRect.h"

#define RECT_POS_X 50
#define RECT_POS_Y 50

#define WEIGHTING_NORMAL 10
#define WEIGHTING_CORSS 14

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1


CRect* CMap::FindInRect(TYPE _type, const int _x, const int _y)
{
	for (int i = 0; i < m_vecRect.size(); ++i)
	{
		if (m_vecRect[i]->IsInRect(_type, _x, _y))
			return m_vecRect[i];
	}
	return nullptr;
}
CRect* CMap::FindInRect(const int _x, const int _y)
{
	for (int i = 0; i < m_vecRect.size(); ++i)
	{
		if (m_vecRect[i]->FindPivotRect(_x, _y))
			return m_vecRect[i];
	}
	return nullptr;
}
CMap::CMap()
{
}

CMap::~CMap()
{
}


int CMap::SetEndRect(const int _x, const int _y)
{
	if (nullptr != m_pEndRect)
		m_pEndRect->SetTypeNormal();
	m_pEndRect = FindInRect(TYPE::TYPE_POSEND, _x, _y);


	m_stEndPos.y = m_pEndRect->GetY();
	m_stEndPos.x = m_pEndRect->GetX();

	return 0;
}

int CMap::A_star()
{
	return 0;
}

int CMap::DeleteNode(CRect* _p)
{
	for (auto iter = m_vecRect.begin(); iter != m_vecRect.end(); ++iter)
	{
		if ((*iter) == _p)
		{
			m_vecRect.erase(iter);
			return 0;
		}
	}
	return 0;
}



int CMap::FuncSetValue(CRect* _owner, CRect* _pt)
{
	if (nullptr == _pt)
		return 0;
	// 나를 불렀던 아이를 기준으로의 가중치를 정해야 하니 우선 나를 부른 친구의 가중치 값을 가져온다.
	int Set = _owner->GetG();

	// X 그리고 Y 가 하나라도 같지 않다면 결국 대각선에 위치한 것 이니 대각선 가중치를 더한다
	if (_owner->GetX() != _pt->GetX() && _owner->GetY() != _pt->GetY())
		Set += WEIGHTING_CORSS;
	else
		Set += WEIGHTING_NORMAL;
	_pt->SetG(Set);


	// H 값도 구해주자 동시에 F 값도 구해서 넣어주자
	_pt->SetH(sqrt(pow(_pt->GetPivotX() - m_pEndRect->GetPivotX(), 2) + pow(_pt->GetPivotY() - m_pEndRect->GetPivotY(), 2)));


	return 0;
}



int CMap::DeleteNode(std::list<CRect*>& _lst, CRect* _p)
{
	for (auto iter = _lst.begin(); iter != _lst.end(); ++iter)
	{
		if ((*iter) == _p)
		{
			_lst.erase(iter);
			return 1;
		}
	}
	return 0;
}

bool CMap::IsInCloseList(CRect* _p)
{
	auto iter = std::find(m_lstCloseList.begin(), m_lstCloseList.end(), _p);
	if (iter == m_lstCloseList.end())
		return false;
	else
		return true;
}

bool CMap::IsInOpenList(CRect* _p)
{
	auto iter = std::find(m_lstOpenList.begin(), m_lstOpenList.end(), _p);
	if (iter == m_lstOpenList.end())
		return false;
	else
		return true;
}


bool CMap::DFS(CRect* _owner)
{
	if (_owner == m_pEndRect)
		return true;
	// 탐색할 노드와 인접한 노드들을 오픈 리스트에 넣어준다.
	CRect* tmp[8];
	tmp[0] = FindInRect(_owner->GetX() + 1, _owner->GetY());
	tmp[2] = FindInRect(_owner->GetX() - 1, _owner->GetY());
	tmp[1] = FindInRect(_owner->GetX(), _owner->GetY() + 1);
	tmp[3] = FindInRect(_owner->GetX(), _owner->GetY() - 1);
	tmp[4] = FindInRect(_owner->GetX() + 1, _owner->GetY() + 1);
	tmp[5] = FindInRect(_owner->GetX() + 1, _owner->GetY() - 1);
	tmp[6] = FindInRect(_owner->GetX() - 1, _owner->GetY() + 1);
	tmp[7] = FindInRect(_owner->GetX() - 1, _owner->GetY() - 1);

	// 나를 이제 오픈리스트에서 삭제한다.
	DeleteNode(m_lstOpenList, _owner);
	// 닫힌 리스트에 나를 저장을 한다. ( 다시 볼 필요가 없다. )
	m_lstCloseList.push_back(_owner);
	for (int i = 0; i < 8; ++i)
	{
		// nullptr 그리고 만약 close 리스트에 있을때 추가하지 말아야한다.
		// + 만약 open 리스트에 있지 않을때
		if (nullptr != tmp[i] && !IsInCloseList(tmp[i])&& !IsInOpenList(tmp[i]))
		{
			// 부모 노드를 설정을 해주고 오픈 리스트에 추가를 한다.
			tmp[i]->SetParent(_owner);
			m_lstOpenList.push_back(tmp[i]);
		}
	}

	auto iter = m_lstOpenList.begin();
	// 오픈 리스트들의 노드들을 순회한다
	for (; iter != m_lstOpenList.end(); ++iter)
	{
		FuncSetValue(_owner, (*iter));
		(*iter)->Render(GetDC(m_hWnd), CreateSolidBrush(RGB(0, 255, 0)));
	}
	// 가장 작은 F 값을 찾는다
	
	iter = m_lstOpenList.begin();
	_owner = (*iter);
	int F_value = (*iter++)->GetF();
	for (; iter != m_lstOpenList.end(); ++iter)
	{
		if (F_value > (*iter)->GetF())
		{
			F_value = (*iter)->GetF();
			_owner = (*iter);
		}
	}
	_owner->Render(GetDC(m_hWnd), CreateSolidBrush(RGB(0, 100, 0)));

	// 끝까지 탐색을 했다면 재귀호출을 한다
	DFS(_owner);
}

int CMap::Update()
{
	if (nullptr != m_pEndRect && nullptr != m_pStartRect)
	{
		// 시작점을 열린 목록에 넣어준다
		DFS(m_pStartRect);
	}
	return 0;
}

int CMap::SetWallRect(const int _x, const int _y)
{
	CRect* pRect = FindInRect(TYPE::TYPE_WALL, _x, _y);
	if (nullptr != pRect)
		DeleteNode(pRect);
	return 0;
}
int CMap::SetStartRect(const int _x, const int _y)
{
	if (nullptr != m_pStartRect)
		m_pStartRect->SetTypeNormal();

	m_pStartRect = FindInRect(TYPE::TYPE_POSSTART, _x, _y);
	if (nullptr != m_pStartRect)
	{
		m_stStartPos.y = m_pStartRect->GetY();
		m_stStartPos.x = m_pStartRect->GetX();
	}
	return 0;
}

int CMap::Render(HDC _hdc)
{
	for (std::vector<CRect*>::iterator iter = m_vecRect.begin(); iter != m_vecRect.end(); ++iter)
		(*iter)->Render(_hdc,nullptr);
	if (nullptr != m_pStartRect)
		m_pStartRect->Render(_hdc, CreateSolidBrush(RGB(255,0,0)));
	if (nullptr != m_pEndRect)
		m_pEndRect->Render(_hdc, CreateSolidBrush(RGB(0, 0, 255)));
	return 0;
}

int CMap::Init(HWND _hwnd, const int _row, const int _col, const int _rect_width, const int _rect_height)
{
	m_hWnd = _hwnd;

	int Pos_X = RECT_POS_X;
	int Pos_Y = RECT_POS_Y;
	for (int y = 0; y < _row; ++y)
	{
		for (int x = 0; x < _col; ++x)
		{
			m_vecRect.push_back(CRect::Create(x, y, Pos_X, Pos_Y, _rect_width, _rect_height));
			Pos_X += (_rect_width);
		}
		Pos_Y += (_rect_height);
		Pos_X = RECT_POS_X;
	}
	m_pStartRect = nullptr;
	m_pEndRect = nullptr;


	return 0;
}
