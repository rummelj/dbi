/* 
 * File:   DBIException.cpp
 * Author: alexx
 * 
 * Created on 5. Mai 2013, 22:03
 */

#include "DBIException.hpp"

DBIException::DBIException(const char* message) {
    _message = message;
}

const char* DBIException::what() const throw()
{
  return _message;
}