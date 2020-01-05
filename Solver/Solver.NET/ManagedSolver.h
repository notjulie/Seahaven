#pragma once

namespace Solver::NET {
   public ref class ManagedSolver
   {
   public:
      System::String ^Solve(System::String^ jsonCardLocations);
   };
}

