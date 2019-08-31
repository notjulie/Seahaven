/* 
 * File:   SolverException.h
 * Author: Randy
 *
 * Created on April 2, 2014, 3:35 PM
 */

#ifndef SOLVEREXCEPTION_H
#define	SOLVEREXCEPTION_H

class SolverException {
public:
    SolverException(const std::string &message);
    SolverException(const SolverException& orig);
    virtual ~SolverException();

    const std::string &GetMessage(void) const { return message; }
    
private:
    std::string message;
};

#endif	/* SOLVEREXCEPTION_H */

