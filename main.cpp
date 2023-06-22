#include <iostream>
#include "Tetris.h"
#include "Block.h"
using namespace std; 
int main()
{
	Tetris game(20, 10, 263, 133, 36);
	game.play();
	return 0;
}