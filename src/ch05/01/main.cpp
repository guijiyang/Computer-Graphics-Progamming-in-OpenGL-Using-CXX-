/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 20:41:28
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-28 09:56:47
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Drawer.h"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  START_EASYLOGGINGPP(argc, argv);

  el::Configurations conf("opengltest.conf");
  el::Loggers::setDefaultConfigurations(conf, true);

  LOG(INFO) << "start 5_1.";
  try {
    opengltest::Drawer app;
    app.run();
  } catch (std::exception &e) {
    LOG(ERROR) << "Error : " << e.what();
  }
  LOG(INFO) << "done.";
}