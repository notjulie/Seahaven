#include "pch.h"
#include <string>
#include "SeahavenProblem.h"
#include "SolverException.h"
#include "ManagedSolver.h"

namespace Solver::NET {

   System::String ^ManagedSolver::Solve(System::String ^_jsonCardLocations)
   {
      try
      {
         // ugly, but my favorite way to convert managed strings to char arrays
         std::string jsonCardLocations;
         for (int i = 0; i < _jsonCardLocations->Length; ++i)
            jsonCardLocations += (char)_jsonCardLocations[i];

         SeahavenProblem problem = SeahavenProblem::CreateFromJSON(jsonCardLocations);

         return gcnew System::String(jsonCardLocations.c_str());
      }
      catch (SolverException solverException) 
      {
         throw gcnew System::Exception(gcnew System::String(solverException.GetMessage().c_str()));
      }
   }

}
