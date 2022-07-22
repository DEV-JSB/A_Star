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
	// ���� �ҷ��� ���̸� ���������� ����ġ�� ���ؾ� �ϴ� �켱 ���� �θ� ģ���� ����ġ ���� �����´�.
	int Set = _owner->GetG();

	// X �׸��� Y �� �ϳ��� ���� �ʴٸ� �ᱹ �밢���� ��ġ�� �� �̴� �밢�� ����ġ�� ���Ѵ�
	if (_owner->GetX() != _pt->GetX() && _owner->GetY() != _pt->GetY())
		Set += WEIGHTING_CORSS;
	else
		Set += WEIGHTING_NORMAL;
	_pt->SetG(Set);


	// H ���� �������� ���ÿ� F ���� ���ؼ� �־�����
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
	// Ž���� ���� ������ ������ ���� ����Ʈ�� �־��ش�.
	CRect* tmp[8];
	tmp[0] = FindInRect(_owner->GetX() + 1, _owner->GetY());
	tmp[2] = FindInRect(_owner->GetX() - 1, _owner->GetY());
	tmp[1] = FindInRect(_owner->GetX(), _owner->GetY() + 1);
	tmp[3] = FindInRect(_owner->GetX(), _owner->GetY() - 1);
	tmp[4] = FindInRect(_owner->GetX() + 1, _owner->GetY() + 1);
	tmp[5] = FindInRect(_owner->GetX() + 1, _owner->GetY() - 1);
	tmp[6] = FindInRect(_owner->GetX() - 1, _owner->GetY() + 1);
	tmp[7] = FindInRect(_owner->GetX() - 1, _owner->GetY() - 1);

	// ���� ���� ���¸���Ʈ���� �����Ѵ�.
	DeleteNode(m_lstOpenList, _owner);
	// ���� ����Ʈ�� ���� ������ �Ѵ�. ( �ٽ� �� �ʿ䰡 ����. )
	m_lstCloseList.push_back(_owner);
	for (int i = 0; i < 8; ++i)
	{
		// nullptr �׸��� ���� close ����Ʈ�� ������ �߰����� ���ƾ��Ѵ�.
		// + ���� open ����Ʈ�� ���� ������
		if (nullptr != tmp[i] && !IsInCloseList(tmp[i])&& !IsInOpenList(tmp[i]))
		{
			// �θ� ��带 ������ ���ְ� ���� ����Ʈ�� �߰��� �Ѵ�.
			tmp[i]->SetParent(_owner);
			m_lstOpenList.push_back(tmp[i]);
		}
	}

	auto iter = m_lstOpenList.begin();
	// ���� ����Ʈ���� ������ ��ȸ�Ѵ�
	for (; iter != m_lstOpenList.end(); ++iter)
	{
		FuncSetValue(_owner, (*iter));
		(*iter)->Render(GetDC(m_hWnd), CreateSolidBrush(RGB(0, 255, 0)));
	}
	// ���� ���� F ���� ã�´�
	
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

	// ������ Ž���� �ߴٸ� ���ȣ���� �Ѵ�
	DFS(_owner);
}

int CMap::Update()
{
	if (nullptr != m_pEndRect && nullptr != m_pStartRect)
	{
		// �������� ���� ��Ͽ� �־��ش�
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
