/*
 * @Author: jiyang Gui
 * @Date: 2023-03-24 19:10:20
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-24 19:11:56
 * @Description: 
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved. 
 */

#include "Cube.h"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
  START_EASYLOGGINGPP(argc, argv);
  el::Configurations conf("opengltest.conf");

  el::Loggers::setDefaultConfigurations(conf, true);

  LOG(INFO)<<"start 4_2.";
  try {
    opengltest::Cube app;
    app.run();
  } catch (std::exception &e) {
    // logger->error("Error : ",e.what());
    LOG(ERROR)<<"Error : "<<e.what();
  }
  // logger->info("end 4_1.");
  LOG(INFO)<<"end 4_2.";
}