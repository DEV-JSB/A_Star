#include "pch.h"
#include "CRect.h"


CRect::CRect(const int _coordi_x,const int _coordi_y,const int _x, const int _y, const int _width, const int _height)
	: m_iHeight(_height)
	, m_iWidth(_width)
	, m_iPivot_x(_x)
	, m_iPivot_y(_y)
	, m_iH(0)
	, m_iF(0)
	, m_iG(0)
	, m_bVisited(false)
	, m_pParent(nullptr)
	, m_eType(TYPE::TYPE_NORMAL)
	,m_iX(_coordi_x)
	,m_iY(_coordi_y)
{
}

CRect::CRect()
{
}

CRect::~CRect()
{
}

CRect* CRect::Create(const int _coordi_x, const int _coordi_y, const int _x, const int _y, const int _width, const int _height)
{
	return new CRect(_coordi_x, _coordi_y,_x,_y,_width,_height);
}

int CRect::Render(HDC _hdc)
{
	RECT rt{ m_iPivot_x - (int)(m_iWidth * 0.5) ,m_iPivot_y - (int)(m_iHeight * 0.5) ,m_iPivot_x + (int)(m_iWidth * 0.5) , m_iPivot_y + (int)(m_iHeight * 0.5) };
	HBRUSH brush;
	HBRUSH oldbrush;
	char* buffer;

	switch (m_eType)
	{
	case TYPE::TYPE_NORMAL:
		brush = CreateSolidBrush(RGB(255, 255, 255));
		oldbrush = (HBRUSH)SelectObject(_hdc, brush);
		Rectangle(_hdc, m_iPivot_x - m_iWidth * 0.5, m_iPivot_y - m_iHeight * 0.5, m_iPivot_x + m_iWidth * 0.5, m_iPivot_y + m_iHeight * 0.5);
		SelectObject(_hdc, oldbrush);
		DeleteObject(brush);
		break;
	case TYPE::TYPE_POSSTART:
		brush = CreateSolidBrush(RGB(5, 5, 5));
		oldbrush = (HBRUSH)SelectObject(_hdc, brush);
		Rectangle(_hdc, m_iPivot_x - m_iWidth * 0.5, m_iPivot_y - m_iHeight * 0.5, m_iPivot_x + m_iWidth * 0.5, m_iPivot_y + m_iHeight * 0.5);
		SelectObject(_hdc, oldbrush);
		DeleteObject(brush);
		break;
	case TYPE::TYPE_POSEND:
		brush = CreateSolidBrush(RGB(200, 0, 0));
		oldbrush = (HBRUSH)SelectObject(_hdc, brush);
		Rectangle(_hdc, m_iPivot_x - m_iWidth * 0.5, m_iPivot_y - m_iHeight * 0.5, m_iPivot_x + m_iWidth * 0.5, m_iPivot_y + m_iHeight * 0.5);
		SelectObject(_hdc, oldbrush);
		DeleteObject(brush);
		break;
	case TYPE::TYPE_WALL:
		brush = CreateSolidBrush(RGB(20, 20, 20));
		oldbrush = (HBRUSH)SelectObject(_hdc, brush);
		Rectangle(_hdc, m_iPivot_x - m_iWidth * 0.5, m_iPivot_y - m_iHeight * 0.5, m_iPivot_x + m_iWidth * 0.5, m_iPivot_y + m_iHeight * 0.5);
		SelectObject(_hdc, oldbrush);
		DeleteObject(brush);
		break;
	}

	TCHAR str[256] = {};
	wsprintf(str, TEXT("%d  %d"), m_iF, m_iG);
	DrawText(_hdc, str, _tcslen(str), &rt, DT_CENTER | DT_WORDBREAK);	
	return 0;
}

bool CRect::IsInRect(const TYPE _type, const int _x, const int _y)
{
	if (m_iPivot_x - m_iWidth * 0.5 <= _x && _x <= m_iPivot_x + m_iWidth * 0.5
		&& m_iPivot_y - m_iHeight * 0.5 <= _y && _y <= m_iPivot_y + m_iHeight * 0.5)
	{
		m_eType = _type;
		return true;
	}
	else
		return false;
}

bool CRect::FindPivotRect(const int _x, const int _y)
{
	if (_x == m_iX && _y == m_iY)
		return true;
	else
		return false;
}

int CRect::SetParent(CRect* _p)
{
	m_pParent = _p;
	return 0;
}

int CRect::TestRender(HDC _hdc)
{
	RECT rt{ m_iPivot_x - (int)(m_iWidth * 0.5) ,m_iPivot_y - (int)(m_iHeight * 0.5) ,m_iPivot_x + (int)(m_iWidth * 0.5) , m_iPivot_y + (int)(m_iHeight * 0.5) };
	HBRUSH brush;
	HBRUSH oldbrush;
	char* buffer;
	brush = CreateSolidBrush(RGB(0, 255, 0));
	oldbrush = (HBRUSH)SelectObject(_hdc, brush);
	Rectangle(_hdc, m_iPivot_x - m_iWidth * 0.5, m_iPivot_y - m_iHeight * 0.5, m_iPivot_x + m_iWidth * 0.5, m_iPivot_y + m_iHeight * 0.5);
	SelectObject(_hdc, oldbrush);
	DeleteObject(brush);

	return 0;
}
