/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 21:06:22
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-28 09:55:54
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

// #include "Sphere.h"
#include "Drawer.h"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  START_EASYLOGGINGPP(argc, argv);

  el::Configurations conf("opengltest.conf");
  el::Loggers::setDefaultConfigurations(conf, true);

  LOG(INFO) << "start 6_1.";
  try {
    opengltest::Drawer app;
    app.run();
  } catch (std::exception &e) {
    LOG(ERROR) << "Error : " << e.what();
  }
  LOG(INFO) << "done.";
  return 0;
}