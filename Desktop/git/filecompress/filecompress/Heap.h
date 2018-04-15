#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
using namespace std;


//�º���
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


//��һ������
template<class T, class Compare>
class Heap
{
public:
	//���µ����㷨
	void AdjustDown(size_t root)
	{
		Compare com;
		int parent = root;
		int child = root * 2 + 1;

		while (child<_a.size())
		{
			//ѡ�����нϴ��һ��(���)
			//if (child + 1 < _a.size() && _a[child + 1] > _a[child])
			if (child + 1 < _a.size() && com(_a[child + 1], _a[child]))
			{
				++child;
			}
			//���Ӵ��ڸ��ף��򽻻����Ҽ��������ң�����break
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;//������
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//���ϵ����㷨
	void AdjustUp(size_t child)
	{
		Compare com;
		int parent = (child - 1) / 2;

		while (child>0)
		{
			//���ӱȸ�ĸ�󣬽��н���
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;//������
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
		swap(_a[0], _a[_a.size() - 1]);//��Ҫɾ���ĶѶ���������β
		_a.pop_back();//ɾ��Ҫɾ���Ǹ���
		AdjustDown(0);//������ȥ��������
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
		_a.reserve(n);//���ռ�
		for (size_t i = 0; i < n; ++i)
		{
			_a.push_back(a[i]);
		}
		//����
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}
protected:
	//�ײ��Ƕ�̬����������
	vector<T> _a;
};

