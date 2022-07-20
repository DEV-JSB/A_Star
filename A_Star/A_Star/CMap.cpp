#include "pch.h"
#include "CMap.h"
#include "CRect.h"

#define RECT_POS_X 50
#define RECT_POS_Y 50

#define NORMAL 3
#define CORSS 5

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1


CRect* CMap::FindInRect(TYPE _type, const int _x, const int _y)
{
	for (int i = 0; i < m_vecRect.size(); ++i)
	{
		if (m_vecRect[i]->IsInRect(_type,_x, _y))
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
	m_pEndRect = FindInRect(TYPE::TYPE_POSEND,_x, _y);


	m_stEndPos.y = m_pEndRect->GetY();
	m_stEndPos.x = m_pEndRect->GetX();

	return 0;
}

int CMap::A_star()
{
	return 0;
}

int CMap::NodeConnect()
{
	for (int i = 0; i < m_vecRect.size(); ++i)
	{
		CRect* tmp;
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY());
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY());
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX(), m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX(), m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}		
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->SetNeighbor(tmp);
		}
	}
	return 0;
}

int CMap::FuncSetH(CRect* _pt)
{
	if (nullptr == _pt)
		return 0;
	int Set = (abs(m_stEndPos.x - _pt->GetX()) * NORMAL) + (abs(m_stEndPos.y - _pt->GetY()) * NORMAL);
	_pt->SetF(Set);
	printf("%d , %d 좌표에 있는 Rect 는 H 값을 %d 로 조정했습니다 .\n", _pt->GetX(), _pt->GetY(), Set);
	return 0;
}

int CMap::FuncSetG(CRect* _pt)
{
	if (nullptr == _pt)
		return 0;
	int Set = (abs(m_stEndPos.x - _pt->GetX()) * NORMAL) + (abs(m_stEndPos.y - _pt->GetY()) * NORMAL);
	_pt->SetG(Set);
	printf("%d , %d 좌표에 있는 Rect 는 G 값을 %d 로 조정했습니다 .\n", _pt->GetX(), _pt->GetY(), Set);

	return 0;
}


CRect* CMap::DFS(std::vector<CRect*> _lstRect)
{
	// DFS 내부에 FOR문을 도는 것 일까?
	for (int i = 0; i < _lstRect.size(); ++i)
	{
		// 일단 찾았다!!
		if (_lstRect[i] == m_pEndRect)
			return nullptr;
		if (!_lstRect[i]->IsScaned())
		{
			printf("x : %d , y : %d 스캔중 \n", _lstRect[i]->GetX(), _lstRect[i]->GetY());
			_lstRect[i]->SetScanState(true);
			_lstRect[i]->TestRender(GetDC(m_hWnd));
			Sleep(300);
			//FuncSetG(_p);
		}
		DFS((_lstRect[0]->GetNeighborVector()));
		DFS((_lstRect[1]->GetNeighborVector()));
		DFS((_lstRect[2]->GetNeighborVector()));
		DFS((_lstRect[3]->GetNeighborVector()));
		DFS((_lstRect[4]->GetNeighborVector()));
		DFS((_lstRect[5]->GetNeighborVector()));
		DFS((_lstRect[6]->GetNeighborVector()));
		DFS((_lstRect[7]->GetNeighborVector()));

	}
}

int CMap::Update()
{
	int x, y;
	if (nullptr != m_pEndRect && nullptr != m_pStartRect)
	{
		DFS(m_pStartRect->GetNeighborVector());
	}
	return 0;
}


int CMap::SetStartRect(const int _x, const int _y)
{
	if (nullptr != m_pStartRect)
		m_pStartRect->SetTypeNormal();
	
	m_pStartRect = FindInRect(TYPE::TYPE_POSSTART,_x, _y);
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
		(*iter)->Render(_hdc);
	if (nullptr != m_pStartRect)
		m_pStartRect->Render(_hdc);
	if (nullptr != m_pEndRect)
		m_pEndRect->Render(_hdc);
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
			m_vecRect.push_back(CRect::Create(x,y,Pos_X, Pos_Y, _rect_width, _rect_height));
			Pos_X += (_rect_width);
		}
		Pos_Y += (_rect_height);
		Pos_X = RECT_POS_X;
	}
	m_pStartRect = nullptr;
	m_pEndRect = nullptr;	

	NodeConnect();

	return 0;
}
