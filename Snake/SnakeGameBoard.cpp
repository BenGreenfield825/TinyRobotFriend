#include "SnakeGameBoard.h"

using namespace std;

SnakeGameBoard::SnakeGameBoard() {
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    u8g2.sendBuffer();
}