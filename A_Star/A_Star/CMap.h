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
	int DeleteNode(CRect* _p);
	CRect* FindInRect(TYPE _type, const int _x, const int _y);
	CRect* FindInRect(const int _x, const int _y);
	CRect* DFS(CRect* _owner, std::list<CRect*> _lst);
	int FuncSetValue(CRect* _owner, CRect* _pt);
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
	int SetWallRect(const int _x, const int _y);
	int Update();
	int SetEndRect(const int _x,const int _y);
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

