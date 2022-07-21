#include "pch.h"
#include "CMap.h"
#include "CRect.h"

#define RECT_POS_X 50
#define RECT_POS_Y 50

#define WEIGHTING_NORMAL 5
#define WEIGHTING_CORSS 7

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

int CMap::NodeConnect()
{
	for (int i = 0; i < m_vecRect.size(); ++i)
	{
		CRect* tmp;
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY());
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX(), m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY() - 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY());
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() + 1, m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX(), m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}
		tmp = FindInRect(m_vecRect[i]->GetX() - 1, m_vecRect[i]->GetY() + 1);
		if (nullptr != tmp && tmp != m_pStartRect)
		{
			tmp->SetParent(m_vecRect[i]);
			m_vecRect[i]->PushNeigbor(tmp);
		}


	}
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

	printf("%d , %d 에 위치한 도형에서의 목표점 까지 거리는 %d 입니다.\n", _pt->GetX(), _pt->GetY(), _pt->GetH());
	return 0;
}


CRect* CMap::DFS(CRect* _owner, std::list<CRect*> _lst)
{
	// 내 이웃 노드들을 탐색하면서 Value 값들을 구해준다
	for (auto iter = _lst.begin(); iter != _lst.end(); ++iter)
	{
		// 일단 찾았다!!
		if ((*iter) == m_pEndRect)
			return nullptr;
		// 혹시 모르니 테스트로 렌더링을 진행한다
		Sleep(20);
		// Valu 값을 구하나 우선은 F값만 탐색을 한다.
		// 지나간 노드는 세팅을 하지 마라
		if (!(*iter)->IsVisited())
			FuncSetValue(_owner, (*iter));
		//(*iter)->TestRender(GetDC(m_hWnd));
	}

	// 탐색을 다 해줬다면 이제 F 값을 탐색해서 다음 탐색을 할 리스트들을 넘겨준다
	// 우선은 임시로 10000의 값을 부여하고 최솟값을 찾아보자
	int F_Value = 10000;
	for (auto iter = _lst.begin(); iter != _lst.end(); ++iter)
	{
		if (!(*iter)->IsVisited())
		{
			if (F_Value > (*iter)->GetF())
			{
				F_Value = (*iter)->GetF();
				_owner = (*iter);
			}
		}
	}
	// 얘가 최종적으로 로드이지 않은가 이게 왜 if 문 안에 있지??
	_owner->SetTypeRoad();
	_owner->Render(GetDC(m_hWnd));

	// 끝까지 탐색을 했다면 재귀호출을 한다
	DFS(_owner, _owner->GetNeighbor());


}

int CMap::Update()
{
	if (nullptr != m_pEndRect && nullptr != m_pStartRect)
	{
		NodeConnect();
		DFS(m_pStartRect, m_pStartRect->GetNeighbor());
		return 0;
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
