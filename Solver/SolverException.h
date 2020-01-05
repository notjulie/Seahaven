/* 
 * File:   SolverException.h
 * Author: Randy
 *
 * Created on April 2, 2014, 3:35 PM
 */

#ifndef SOLVEREXCEPTION_H
#define	SOLVEREXCEPTION_H

#include <stdexcept>

/// <summary>
/// Solver base exception class
/// </summary>
class SolverException : public std::runtime_error {
public:
    SolverException(const std::string &message);
};

#endif	/* SOLVEREXCEPTION_H */

