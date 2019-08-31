/* 
 * File:   SolverException.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 3:35 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"

SolverException::SolverException(const std::string &_message)
        :
        message(_message)
{
}

SolverException::SolverException(const SolverException& orig) {
}

SolverException::~SolverException() {
}

