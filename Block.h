#pragma once
#include <graphics.h>
#include <vector>
using namespace std;

struct Point
{
	int row;
	int col;
};

class Block
{
public:
	Block(); //构造函数
	void drop();
	void moveLeltRiight(int offset);
	void Rotate();  //旋转
	void draw(int leftMargin,int topMargin);
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& mp);
	//Point* getSmallBlocks();
	void solidify(vector<vector<int>>& mp);
	int getBlockType();
private:
	int blockType; //方块类型
	Point smallBlocks[4]; 
	IMAGE *img;

	static IMAGE* imgs[7];
	static int size;

	
};

