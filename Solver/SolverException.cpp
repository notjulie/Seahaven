/* 
 * File:   SolverException.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 3:35 PM
 */

#include "SolverIncludes.h"
#include "SolverException.h"


/// <summary>
/// Initializes a new instance of SolverException
/// </summary>
SolverException::SolverException(const std::string &_message)
   :
      std::runtime_error(_message.c_str())
{
}

