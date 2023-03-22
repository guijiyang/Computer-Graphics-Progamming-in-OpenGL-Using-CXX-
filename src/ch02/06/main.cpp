/*
 * @Author: jiyang Gui
 * @Date: 2023-03-22 14:21:39
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-22 14:21:39
 * @Description: 
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved. 
 */


#include "AnimationTriangle.hpp"

int main(int argc, char** argv)
{
  Triangle t;
  try {
  t.run();
  } catch (std::exception& e) {
    cout << "Error: " << e.what() <<endl;
  }
}