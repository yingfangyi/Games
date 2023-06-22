#include "Tetris.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include "Block.h"
#include <mmsystem.h>
#include <fstream>
#pragma comment(lib,"winmm.lib")
#define MAX_Level 5
#define RECORDER_FILE "recorder.txt"

using namespace std; 

const int SPEED_NORMAL[5] = { 500,300,150,100,80 }; //��ͨ�ٶ� 
const int SPEED_QUICK = 50;   //���ٽ����ٶ�

Tetris::Tetris(int rows, int cols, int left, int top, int blocksize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	//this->blockSize = blockSize;
	this->blockSize = blocksize;
	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	mciSendString("play res/jntm.mp3 repeat", 0, 0, 0);

	delay = SPEED_NORMAL[0];

	//�����������
	srand(time(NULL));
	//������Ϸ����
	initgraph(938, 896);
	//���ر���ͼƬ

	loadimage(&imgBg, "res/bg2.jpg");

	loadimage(&imgWin, "res/win.jpg");
	loadimage(&imgOver, "res/over.jpg");
	
	//��ʼ����Ϸ����
	char data[20][10];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	score = 0;
	level = 1;
	lineCount = 0;

	//��ʼ����ߵ÷�
	ifstream file(RECORDER_FILE);
	if (!file.is_open())
	{
		cout << RECORDER_FILE << "��ʧ��\n";
		highestScore = 0;
	}
	else
	{
		file >> highestScore;
	}
	file.close();  //�ر��ļ� 
	gameOver = false;
}

void Tetris::play()
{
	init();

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;

	int timer = 0;
	while (1)
	{
		//�����û�����
		keyEvent();

		timer += getDelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			//��Ⱦ��Ϸ����
			update = true;
		}
		if (update)
		{
			update = false;
			//������Ϸ����
			updateWindow();
			//������Ϸ����
			claerLine();

		}
		if (gameOver)
		{
			//������߷���
			saveScore();

			//������Ϸ��������
			displayOver();

			system("pause");
			init(); //���¿�ʼ��Ϸ
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool rotateflag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch=_getch();
		if (ch == 224)
		{
			ch =_getch();
			switch (ch)
			{
				case 72:
					rotateflag = true;
					break;
				case 80:
					delay = SPEED_QUICK;
					break;
				case 75:
					dx = -1;
					break;
				case 77:
					dx = 1;
					break;
				default:
					break;
			}
		}
	}
	if (rotateflag)
	{
		rotate();
		update = true;
	}
	if (dx != 0)
	{
		moveLeftRight(dx);
		update = true;
	}
}

void Tetris::updateWindow()
{
	BeginBatchDraw();
	putimage(0, 0, &imgBg); //���Ʊ���ͼƬ
	IMAGE** imgs = Block::getImages();
	

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0) continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j]- 1]);
		}
	}
	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore();  //����

	EndBatchDraw();
}

int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0)
	{
		lastTime = currentTime;
		return 0;
	}
	else
	{
		int ret=currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();
	if (!curBlock->blockInMap(map))
	{
		//����̻�
		bakBlock.solidify(map);
		delete curBlock; 
		curBlock = nextBlock;
		nextBlock = new Block;

		//�����Ϸ�Ƿ����
		checkOver();
	}
	delay = SPEED_NORMAL[level-1];
}

void Tetris::claerLine()
{

	int lines = 0;
	int k = rows-1; //�洢��������
	for (int i = rows - 1; i > 0; i--)
	{
		int cnt = 0;
		//����i���Ƿ�����
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				cnt++;
			}
			map[k][j] = map[i][j]; //ɨ��ʱ�洢
		}
		if (cnt < cols)  //��������
		{
			k--;
		}
		else
		{
			lines++;
		}
	}
	if (lines > 0)
	{
		//����÷�
		int addScore[4] = { 10,30,60,80 };
		score+=addScore[lines - 1];

		//��Ч
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;

		//ÿ100��һ������
		 level=(score + 99) / 100;

		 if (level > MAX_Level)
		 {
			 gameOver = true;
		 }
		 lineCount += lines;

		 
	}
}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeltRiight(offset);
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType()==7) //�����β���Ҫ��ת
	{
		return;
	}
	bakBlock = *curBlock;
	curBlock->Rotate();
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180));
	LOGFONT f;
	gettextstyle(&f); //��ȡ��ǰ����
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;  //��������
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);

	outtextxy(670, 727, scoreText);

	//���������˶�����
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);  
	gettextstyle(&f);
	int xpos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xpos, 817, scoreText);

	//���ƹؿ�
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);  
	outtextxy(224 - 30, 727, scoreText);

	//������߷���
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	gameOver = (curBlock->blockInMap(map) == false);

}

void Tetris::saveScore()
{
	if (score > highestScore)
	{
		highestScore = score;
		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/jntm.mp3", 0, 0, 0);
	//ʤ������ or ʧ�ܽ���
	if (level <= MAX_Level)
	{
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else
	{
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
