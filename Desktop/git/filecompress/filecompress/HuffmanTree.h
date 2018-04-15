#pragma once

#include"Heap.h"

using namespace std;


template<class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W> *_left;
	HuffmanTreeNode<W> *_right;
	W _w;//Ȩֵ

	HuffmanTreeNode(const W& w)
		:_left(NULL)
		, _right(NULL)
		, _w(w)
	{}
};


template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}


	HuffmanTree(W* a, size_t n, W& invalid)
	{
		//�ڲ���
		struct NodeCompare
		{
			//����operator()��ʵ��ȡ�ڵ����Ȩֵ
			bool operator()(const Node* left, const Node* right)
			{
				return left->_w < right->_w;
			}
		};


		//������������a����Heap�����
		Heap<Node*, NodeCompare> minHeap;
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)
				minHeap.Push(new Node(a[i]));
		}

		while (minHeap.Size()>1)
		{
			//ȡ����ǰ��С������ֵ�������ҽڵ�
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			//newһ�����ڵ㣬�����ҽڵ�����
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			minHeap.Push(parent);//push�����µ���������
		}
		_root = minHeap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}

protected:
	Node* _root;
};

