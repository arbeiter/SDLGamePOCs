#pragma once

class Player
{
  int x, y;
  float mvelX, mVely;
  int width;
  int height;

  public:
    Player(int posx, int posy): x(posx), y(posy) {};
    void draw(string texName, int width, int height);
};
