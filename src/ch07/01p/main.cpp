/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 21:06:22
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-01 18:17:53
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#ifdef _MSVC_LANG
#include <Shlwapi.h>
// #include <Windows.h>
#endif
#include "Drawer.h"
#include <array>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  START_EASYLOGGINGPP(argc, argv);

  el::Configurations conf("opengltest.conf");
  el::Loggers::setDefaultConfigurations(conf, true);

  // std::array<char, 10> name{""};
  // std::string name;
  // #ifdef _MSVC_LANG
  // GetModuleFileName(nullptr, name.data(),10);
  // #endif
  
  auto name=PathFindFileName(argv[0]);
  LOG(INFO) << "start "<<name;
  try {
    opengltest::Drawer app;
    app.run(name);
  } catch (std::exception &e) {
    LOG(ERROR) << "Error : " << e.what();
  }
  LOG(INFO) << "done.";
  return 0;
}