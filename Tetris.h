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
	Tetris(int rows,int cols,int left,int top,int blocksize); //���캯��
	void init();  //��ʼ��
	void play();	//��ʼ��Ϸ
private:
	void keyEvent();
	void updateWindow();
	int getDelay(); //���ؾ�����һ�ε��øú�������˶���ʱ�䣨ms��
	void drop();
	void claerLine();
	void moveLeftRight(int offset);
	void rotate(); 
	void drawScore();  //���Ƶ�ǰ�ķ���
	void checkOver();  //�����Ϸ�Ƿ����
	void saveScore();  //������߷���
	void displayOver(); //������Ϸ��������


private:
	int delay;
	bool update;  //�Ƿ����

	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock; //��ǰ����
	Block* nextBlock; //Ԥ�淽��
	Block bakBlock;  //���÷���,���ݽ������һ������ĺϷ�λ��


	int score;  //��ǰ�ķ���
	int highestScore;  //��ߵ÷�
	int level; //��ǰ�ؿ�
	int lineCount; //��ǰ�����˶�����
	bool gameOver;//�ж���Ϸ����

	IMAGE imgOver;
	IMAGE imgWin;
};

