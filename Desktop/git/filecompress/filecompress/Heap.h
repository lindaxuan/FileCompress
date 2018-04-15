#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
using namespace std;


//仿函数
template<class T>
struct Greater
{
	bool operator()(const T&left, const T& right)
	{
		return left > right;
	}
};

template<class T>
struct Less
{
	bool operator()(const T&left, const T& right)
	{
		return left < right;
	}
};


//建一个堆类
template<class T, class Compare>
class Heap
{
public:
	//向下调整算法
	void AdjustDown(size_t root)
	{
		Compare com;
		int parent = root;
		int child = root * 2 + 1;

		while (child<_a.size())
		{
			//选左右中较大的一个(大堆)
			//if (child + 1 < _a.size() && _a[child + 1] > _a[child])
			if (child + 1 < _a.size() && com(_a[child + 1], _a[child]))
			{
				++child;
			}
			//孩子大于父亲，则交换，且继续向下找，否则break
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;//向下找
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//向上调整算法
	void AdjustUp(size_t child)
	{
		Compare com;
		int parent = (child - 1) / 2;

		while (child>0)
		{
			//孩子比父母大，进行交换
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;//向上走
				parent = (parent - 1) / 2;
			}
			else
			{
				break;
			}
		}

	}

	void Push(const T&x)
	{
		_a.push_back(x);
		AdjustUp(_a.size() - 1);
	}

	void Pop()
	{
		swap(_a[0], _a[_a.size() - 1]);//将要删除的堆顶换到数组尾
		_a.pop_back();//删除要删的那个数
		AdjustDown(0);//将换上去的数重排
	}

	const T& Top()
	{
		return _a[0];
	}

	size_t Size()
	{
		return _a.size();
	}

	bool Empty()
	{
		return _a.empty();
	}

	Heap()
	{}

	Heap(T *a, size_t n)
	{
		_a.reserve(n);//开空间
		for (size_t i = 0; i < n; ++i)
		{
			_a.push_back(a[i]);
		}
		//建堆
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}
protected:
	//底层是动态增长的数组
	vector<T> _a;
};

