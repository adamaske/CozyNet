#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Utils/Constants.h"

#include <string>
#include <functional>


namespace CozyNet{

    int Initialize();

    void Shutdown();


    //Logging
    void Info(std::string message);
    void Error(std::string message);
    //Callbacks
    typedef std::function<void(const std::string&)>  LogCallbackParameter;
    void SetInfoCallback(LogCallbackParameter callback);
    void SetErrorCallback(LogCallbackParameter callback);

    static LogCallbackParameter InfoCallback = nullptr;
    static LogCallbackParameter ErrorCallback = nullptr;
    
}