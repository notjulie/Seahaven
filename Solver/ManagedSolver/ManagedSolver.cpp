#include <string>
#include "SeahavenProblem.h"
#include "Solver.h"
#include "SolverException.h"
#include "ManagedSolver.h"

namespace ManagedSolver {

   System::String ^ManagedSolver::Solve(System::String ^_jsonCardLocations)
   {
      try
      {
         // ugly, but my favorite way to convert managed strings to char arrays
         std::string jsonCardLocations;
         for (int i = 0; i < _jsonCardLocations->Length; ++i)
            jsonCardLocations += (char)_jsonCardLocations[i];

         // create our problem object from the JSON
         solver::SeahavenProblem problem = solver::SeahavenProblem::CreateFromJSON(jsonCardLocations);

         // solve
         solver::Solver solver;
         solver::Solution solution = solver.Solve(problem);

         // return JSON version of the solution 
         std::string jsonString = solution.ToJSONString();
         return gcnew System::String(jsonString.c_str());
      }
      catch (std::exception exception) 
      {
         throw gcnew System::Exception(gcnew System::String(exception.what()));
      }
   }

}
