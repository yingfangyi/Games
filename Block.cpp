#include "Block.h"
#include <iostream>
#include <vector>
#include "Tetris.h"
using namespace std;


IMAGE* Block::imgs[7] = { NULL,0 };
int Block::size = 36;

Block::Block()
{
	//初始化一次
	if (imgs[0] == NULL)
	{
		IMAGE imgTmp;
		loadimage(&imgTmp, "res/tiles.jpg");
		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++)
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();
		srand(time(NULL));
	}
	int blocks[7][4] = {
		1,3,5,7, // I
		2,4,5,7, // Z 1型
		3,5,4,6, // Z 2型
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // 田
	};
	//随机生成一种俄罗斯方块
	blockType=1+rand() % 7;
	//初始化smallBlocks
	for (int i = 0; i < 4; i++)
	{
		smallBlocks[i].row = blocks[blockType - 1][i] / 2;
		smallBlocks[i].col = blocks[blockType - 1][i] % 2;
	}
	img = imgs[blockType - 1];
}

void Block::drop()
{
	for (int i=0;i<4;i++)
	{
		smallBlocks[i].row++;
	}
}

void Block::moveLeltRiight(int offset)
{
	for (int i = 0; i < 4; i++) {
		smallBlocks[i].col += offset;
	}
}

void Block::Rotate()
{
	Point p = smallBlocks[1];
	for (int i = 0; i < 4; i++)
	{
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}

void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0; i < 4; i++)
	{
		int x = smallBlocks[i].col * size + leftMargin;
		int y = smallBlocks[i].row * size + topMargin;
		putimage(x, y, img);
	}
}

IMAGE** Block::getImages()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	if (this == &other) return *this;
	this->blockType = other.blockType;
	for (int i = 0; i < 4; i++) {
		this->smallBlocks[i] = other.smallBlocks[i];
	}
	return *this;
}

bool Block::blockInMap(const vector<vector<int>>& mp)
{
	int rows = mp.size();
	int cols = mp[0].size();
	for (int i = 0; i < 4; i++)
	{
		if (smallBlocks[i].col < 0||smallBlocks[i].col>=cols|| smallBlocks[i].row<0|| smallBlocks[i].row >= rows||mp[smallBlocks[i].row][smallBlocks[i].col])
		{
			return false;
		}
	}
	return true;
}

void Block::solidify(vector<vector<int>>& mp)
{
	for (int i = 0; i < 4; i++)
	{
		// 设置标记，“固化”对应位置
		mp[smallBlocks[i].row][smallBlocks[i].col] = blockType; 
	}
}

int Block::getBlockType()
{
	return blockType;
}
