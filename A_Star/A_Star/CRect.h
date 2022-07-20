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


public:

	int SetF(const int _f) { m_iF = _f; return 0; }
	int SetG(const int _g) { m_iG = _g; return 0; }
	int SetH(const int _h) { m_iH = _h; return 0; }
	int SetLeft(CRect* _p) { m_pLeft = _p; return 0; };
	int SetRight(CRect* _p) { m_pRight = _p; return 0;	};
	int SetTop(CRect* _p) { m_pTop= _p; return 0;	};
	int SetBottom(CRect* _p) { m_pBottom= _p; return 0;	};
	int SetLB(CRect* _p) { m_pLB = _p; return 0;	};
	int SetLT(CRect* _p) { m_pLT = _p; return 0;	};
	int SetRB(CRect* _p) { m_pRB = _p; return 0;	};
	int SetRT(CRect* _p) { m_pRT = _p; return 0;	};
	int SetScanState(const bool _b) { m_bVisited = _b; return 0; }

	CRect* GetLeft() { return m_pLeft; };
	CRect* GetRight() { return m_pRight; };
	CRect* GetTop() { return m_pTop; };
	CRect* GetBottom() { return m_pBottom; };
	CRect* GetLB() { return m_pLB; };
	CRect* GetLT() { return m_pLT; };
	CRect* GetRB() { return m_pRB; };
	CRect* GetRT() { return m_pRT; };



	bool IsScaned() { return m_bVisited; }
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }

private:
	TYPE	m_eType;

	CRect*	m_pParent;
	
	CRect* m_pLT;
	CRect* m_pRT;
	CRect* m_pLB;
	CRect* m_pRB;
	CRect* m_pLeft;
	CRect* m_pRight;
	CRect* m_pTop;
	CRect* m_pBottom;


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

