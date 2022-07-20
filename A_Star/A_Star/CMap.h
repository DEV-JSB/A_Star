#pragma once

class CRect;

#define RECT_WIDTH 50
#define RECT_HEIHGT 50

struct Pos
{
	int x;
	int y;
};

class CMap
{
private:
	int A_star();
	int NodeConnect();
	CRect* FindInRect(TYPE _type, const int _x, const int _y);
	CRect* FindInRect(const int _x, const int _y);
	CRect* DFS(std::vector<CRect*> _lstRect);
private:
	CMap();
	~CMap();
public:
	static CMap* GetInstance()
	{
		static CMap map;
		return &map;
	}
public:
	int FuncSetH(CRect* _pt);
	int FuncSetG(CRect* _pt);
	int SetEndRect(const int _x,const int _y);
	int Update();
	int SetStartRect(const int _x,const int _y);
	int Render(HDC _hdc);
	int Init(HWND _hwnd,const int _row, const int _col, const int _rect_width = RECT_WIDTH, const int _rect_height = RECT_HEIHGT);
private:
	HWND m_hWnd;
	
	Pos m_stStartPos;
	Pos m_stEndPos;

	CRect* m_pStartRect;
	CRect* m_pEndRect;

	std::vector<CRect*> m_vecRect;
};

