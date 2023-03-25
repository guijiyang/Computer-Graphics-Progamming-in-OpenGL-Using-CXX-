/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 18:43:39
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-26 14:57:00
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Cube.h"
#include <easylogging++.h>


INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  // set flag to easy logging from command line.
  START_EASYLOGGINGPP(argc, argv);
  // read configure file to configure object.
  el::Configurations conf("opengltest.conf");
  // reconfigure the logger default with conf object.
  // el::Loggers::reconfigureLogger("default", conf);
  // reconfigure all loggers with speific configures.
  // el::Loggers::reconfigureAllLoggers(conf);
  // set configuration to existing and future loggers
  el::Loggers::setDefaultConfigurations(conf,true);
  // user default logger
  // auto logger = el::Loggers::getLogger("default");
  // logger->info("start 4_1.");
  LOG(INFO)<<"start 4_1.";
  try {
    opengltest::Cube app;
    app.run();
  } catch (std::exception &e) {
    // logger->error("Error : ",e.what());
    LOG(ERROR)<<"Error : "<<e.what();
  }
  // logger->info("end 4_1.");
  LOG(INFO)<<"end 4_1.";
}