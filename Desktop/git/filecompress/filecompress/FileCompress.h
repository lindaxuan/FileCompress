#pragma once

#include <iostream>
#include <assert.h>
#include <string>
#include "HuffmanTree.h"
#include <algorithm>


typedef long long LongType;
struct CharInfo  //�ַ��������Ϣ
{
	char _ch;//�ַ�
	LongType _count;//���ִ���
	string _code;//Huffman code

	bool operator< (const CharInfo& info)const
	{
		return _count < info._count;
	}

	CharInfo operator+(const CharInfo info)
	{
		CharInfo ret;
		ret._count = _count + info._count;
		return ret;
	}

	bool operator != (const CharInfo& info)const
	{
		return _count != info._count;
	}
};


class FileCompress
{

	typedef HuffmanTreeNode<CharInfo> Node;
protected:
	CharInfo _infos[256];

public:

	FileCompress()
	{
		//��ʼ��
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
			_infos[i]._code = "";
		}
	}

	//���ڼ�¼������Ϣ����
	struct _HuffmanInfo
	{
		char _ch;	//�ַ�
		LongType _count;	//��Ӧ�ַ����ָ���
	};

	//����huffman code
	void GenerateHaffmanCode(Node* root, string code)
	{
		if (root == NULL)
			return;

		//���Ҷ�Ϊ�գ����ǵ�Ҷ���ˣ���code���浽��Ӧ��λ��
		if (root->_left == NULL&&root->_right == NULL)
		{
			_infos[(unsigned char)root->_w._ch]._code = code;
			return;
		}
		GenerateHaffmanCode(root->_left, code + "0");
		GenerateHaffmanCode(root->_right, code + "1");
	}


	//ѹ��
	void Compress(const char* filename)

	{
		assert(filename); //ȷ���ļ�����Ϊ��
		FILE* fout = fopen(filename, "rb");
		assert(fout);

		//1��ͳ���ַ���������
		char ch = fgetc(fout);
		while (!feof(fout))
		{
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}

		//2������huffman��
		CharInfo invalid;
		invalid._count = 0;

		HuffmanTree<CharInfo> tree(_infos, 256, invalid);


		//3�����ɱ���
		string code;
		GenerateHaffmanCode(tree.GetRoot(), code);

		//4��ѹ���ļ�(����һ��ѹ������ļ�����д�룩
		string compressFile = filename;
		compressFile += ".huffman";
		FILE* fIn = fopen(compressFile.c_str(), "wb");
		assert(fIn);


		_HuffmanInfo info;
		size_t size = 0;
    	//д������Ϣ����¼��Ӧ�ַ����ִ���
		for (size_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count > 0)
			{
				info._ch = _infos[i]._ch;
				info._count = _infos[i]._count;
				size = fwrite(&info, sizeof(_HuffmanInfo), 1, fIn);
				//fwrite����д�����ĸ�����ȷ��д��ɹ�
				assert(size == 1);
			}
		}

		//�����ǣ������ѹ��
		info._count = 0;
		size = fwrite(&info, sizeof(_HuffmanInfo), 1, fIn);
		assert(size == 1);
		char value = 0;
		int count = 0;

		//���¶��ļ������ļ�ָ��������Ϊ��ʼλ��
		fseek(fout, 0, SEEK_SET);

		//��ѹ���ļ��п�ʼд���Ӧ�ַ���huffman����
		ch = fgetc(fout);
		while (!feof(fout))
		{
			string& code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')
					value |= 1;
				else
					value |= 0;
				++count;

				//��һ���ֽھ�д��
				if (count == 8)
				{
					fputc(value, fIn);
					value = 0;
					count = 0;
				}
			}
			ch = fgetc(fout);
		}

		if (count != 0)
		{
			value <<= (8 - count);
			fputc(value, fIn);
		}
		fclose(fIn);
		fclose(fout);
	}


	//��ѹ��
	void UnCopmpress(const char* filename)
	{
		assert(filename);

		//�޸Ľ�ѹ������ļ���׺
		string uncompress = filename;
		size_t pos = uncompress.rfind('.');
		assert(pos != string::npos);
		uncompress = uncompress.substr(0, pos);
		uncompress += ".unhuffman";
		
    	FILE* fin = fopen(uncompress.c_str(), "wb");
		assert(fin);

    	FILE* fout = fopen(filename, "rb");
		assert(fout);


		while (1)
		{
			_HuffmanInfo info;
			size_t size = fread(&info, sizeof(_HuffmanInfo), 1, fout);
			assert(size == 1);

			if (info._count)
			{
				_infos[(unsigned char)info._ch]._ch = info._ch;
				_infos[(unsigned char)info._ch]._count = info._count;
			}
			else
				break;
		}

		//�ؽ�huffman
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		Node* root = tree.GetRoot();
		//���ڵ��count���Ƕ�ȡ�������ַ�����
		LongType charCount = root->_w._count;


	   //��ѹ��
        char value = fgetc(fout);
        Node* cur = root;
        while (!feof(fout))
		{
			//��ȡ����һ���ֽڷֱ�̽��
			for (int pos = 7; pos >= 0; --pos)
			{
				if (value & (1 << pos)) //1
		    		cur = cur->_right;
				else
					cur = cur->_left;

				//�ж�Ҷ�ӽڵ�
				if (cur->_left == NULL && cur->_right == NULL)
				{
					fputc(cur->_w._ch, fin);
					cur = root;
   			        --charCount;
					if (charCount == 0)
						break;
				}
			}
			//���ַ�������ɣ�������һ���ַ�����
			value = fgetc(fout);
		}
		fclose(fin);
		fclose(fout);
	}
};



void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("2.jpg");
	fc.UnCopmpress("2.jpg.huffman");
}