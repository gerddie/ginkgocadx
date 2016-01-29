// Created:20030427
// By Jeff Connelly

#ifndef __EP_EXCEPTION_H
#define __EP_EXCEPTION_H
#include <string>

class EndpointException
{
public:
    EndpointException(std::string str):m_str(str){}

    std::string m_str;
};

#endif
