/**
 * @file    main.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-02-19 16:27:43
 *
 * Created on Sun Feb 19 16:25:24 2017.
 */

#include <iostream>
#include <string>
#include "ArgParser.h"
#include "Logger.h"

using namespace std;

int main (void)
{
    my_log::LoggerCtrl::set_max_verbosity(my_log::Verbosity::debug);
    my_log::LoggerCtrl::set_header("KKK: ");

    ofstream ofs("Test.txt");

    my_log::LoggerCtrl::add_stream(ofs);

    LOG << "HAHAHA" << endl;
    LOGE << "HAHAHA" << endl;
    LOGW << "HAHAHA" << endl;
    LOGI << "HAHAHA" << endl;

    my_log::LoggerCtrl::set_max_verbosity(my_log::Verbosity::warning);

    LOGD << "HAHAHA" << endl;
    LOGD << "HAHAHA";
    LOGD << "HAHAHA";
    LOGD << "HAHAHA" << endl;
}
