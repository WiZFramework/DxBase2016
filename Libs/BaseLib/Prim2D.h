#pragma once

#include "stdafx.h"

namespace basedx11{

template<typename T>
struct Point2D{
	T x;
	T y;
	Point2D() throw():
		x{},
		y{}
	{}
	Point2D(T initX, T initY) throw():
		x{ initX },
		y{ initY }
	{
	}
	bool operator==(Point2D<T> point) const throw(){
		return (x == point.x && y == point.y);
	}
	bool operator!=(Point2D<T> point) const throw(){
		return (x != point.x || y != point.y);
	}
	void operator+=(Point2D<T> point) throw(){
		x += point.x; y += point.y;
	}
	void operator-=(Point2D<T> point) throw(){
		x -= point.x; y -= point.y;
	}
	Point2D operator+(Point2D<T> point) const throw(){
		return Point2D(x + point.x, y + point.y);
	}
	Point2D operator-(Point2D<T> point) const throw(){
		return Point2DF(x - point.x, y - point.y);
	}
	Point2D operator-() const throw(){
		return Point2DF(-x, -y);
	}
};

template<typename T>
struct Rect2D{
	T left;
	T top;
	T right;
	T bottom;
	Rect2D() throw():
		left{}, top{}, right{}, bottom{}
	{}
	Rect2D(T l, T t, T r, T b)throw():
		left{l}, top{t}, right{r}, bottom{b}
	{
	}
	//空かどうか
	bool IsRectEmpty()const throw(){
		if (left >= right || top >= bottom){
			return true;
		}
		return false;
	}
	//幅
	T Width()const throw(){
		return abs(right - left);
	}
	//高さ
	T Height()const throw(){
		return abs(bottom - top);
	}
	//移動
	void operator+=(Point2D<T> point) throw(){
		left += point.x;
		right += point.x;
		top += point.y;
		bottom += point.y;
	}
	void operator-=(Point2D<T> point) throw(){
		left -= point.x;
		right -= point.x;
		top -= point.y;
		bottom -= point.y;
	}
	Rect2D<T> operator+(Point2D<T> point) const throw(){
		Rect2D<T> rect(*this);
		rect += point;
		return rect;
	}
	Rect2D<T> operator-(Point2D<T> point) const throw(){
		Rect2D<T> rect(*this);
		rect -= point;
		return rect;
	}
	//ポイントが含まれるかどうか
	bool PtInRect(Point2D<T> point) const throw(){
		if (point.x >= left && point.x < right
			&& point.y >= top && point.y < bottom){
			return true;
		}
		return false;
	}
	//中心点
	Point2D<T> CenterPoint() const throw(){
		return Point2D<T>((left + right) / 2.0f, (top + bottom) / 2.0f);
	}
	//中心点を中心にスケーリング
	void InflateRect(T l,T t,T r,T b){
		left -= l;			top -= t;
		right += r;			bottom += b;
	}
};



}

//EndOfNameSpace basedx11.



