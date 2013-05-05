/* 
 * File:   DBIException.hpp
 * Author: alexx
 *
 * Created on 5. Mai 2013, 22:03
 */

#ifndef DBIEXCEPTION_HPP
#define	DBIEXCEPTION_HPP

#include <exception>

class DBIException : public std::exception {
public:
    DBIException(const char* message = 0);
    
      virtual const char* what() const throw();
private:
    const char* _message;

};

#endif	/* DBIEXCEPTION_HPP */

