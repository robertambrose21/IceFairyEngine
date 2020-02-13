#include "icexception.h"

using namespace IceFairy;

ICException::ICException()
    : message(ICE_FAIRY_UNDEFINED_EXCEPTION)
{ }

ICException::ICException(std::string message)
    : message(message)
{ }

ICException::~ICException()
{ }

const char* ICException::what() const throw() {
    return message.c_str();
}