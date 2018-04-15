#pragma once

#include"Heap.h"

using namespace std;


template<class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W> *_left;
	HuffmanTreeNode<W> *_right;
	W _w;//权值

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
		//内部类
		struct NodeCompare
		{
			//重载operator()，实现取节点里的权值
			bool operator()(const Node* left, const Node* right)
			{
				return left->_w < right->_w;
			}
		};


		//传进来的数组a，用Heap来存放
		Heap<Node*, NodeCompare> minHeap;
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)
				minHeap.Push(new Node(a[i]));
		}

		while (minHeap.Size()>1)
		{
			//取出当前最小的两个值，做左右节点
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			//new一个父节点，将左右节点链上
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			minHeap.Push(parent);//push里重新调整了排序
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

