#pragma once


class CRect
{
private:
	CRect(const int _coordi_x, const int _coordi_y, const int _x, const int _y, const int _width, const int _height);
	CRect();
	~CRect();
public:
	static CRect*	Create(const int _coordi_x, const int _coordi_y, const int _x, const int _y, const int _width, const int _height);
	int				Render(HDC _hdc);
	bool			IsInRect(const TYPE _type,const int _x, const int _y);
	int				SetTypeNormal() { m_eType = TYPE::TYPE_NORMAL; return 0; }
	bool			FindPivotRect(const int _x, const int _y);
	int				SetParent(CRect* _p);

	// MustDeleteFunction
	int				TestRender(HDC _hdc);

public:

	int SetF(const int _f) { m_iF = _f; return 0; }
	int SetG(const int _g) { m_iG = _g; return 0; }
	int SetH(const int _h) { m_iH = _h; return 0; }

	int SetScanState(const bool _b) { m_bVisited = _b; return 0; }

	std::list<CRect*> GetNeighbor() { return m_lstNeigbor; }

	bool IsScaned() { return m_bVisited; }
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }

	int GetG() { return m_iG; }
	int GetH() { return m_iH; }
	int GetF() { return m_iF; }


private:
	std::list<CRect*> m_lstNeigbor;

	TYPE	m_eType;

	CRect*	m_pParent;

	bool	m_bVisited;

	int		m_iPivot_x;
	int		m_iPivot_y;

	int		m_iHeight;
	int		m_iWidth;

	int		m_iX;
	int		m_iY;

	int		m_iH;
	int		m_iF;
	int		m_iG;
};

