#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <graphics.h>
#include "Block.h"
using namespace std;

class Tetris
{
public:
	Tetris(int rows,int cols,int left,int top,int blocksize); //构造函数
	void init();  //初始化
	void play();	//开始游戏
private:
	void keyEvent();
	void updateWindow();
	int getDelay(); //返回距离上一次调用该函数间隔了多少时间（ms）
	void drop();
	void claerLine();
	void moveLeftRight(int offset);
	void rotate(); 
	void drawScore();  //绘制当前的分数
	void checkOver();  //检查游戏是否结束
	void saveScore();  //保存最高分数
	void displayOver(); //更新游戏结束画面


private:
	int delay;
	bool update;  //是否更新

	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock; //当前方块
	Block* nextBlock; //预告方块
	Block bakBlock;  //备用方块,备份降落的上一个方块的合法位置


	int score;  //当前的分数
	int highestScore;  //最高得分
	int level; //当前关卡
	int lineCount; //当前消除了多少行
	bool gameOver;//判断游戏结束

	IMAGE imgOver;
	IMAGE imgWin;
};

