/*
 * @Author: jiyang Gui
 * @Date: 2023-03-21 20:30:17
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-21 21:53:10
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Point.hpp"

int main() {
  Point p;
  try {
    p.run();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}