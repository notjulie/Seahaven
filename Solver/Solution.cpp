/* 
 * File:   Solution.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 2:19 PM
 */

#include "SolverIncludes.h"
#include "json.hpp"

#include "SolverException.h"
#include "Solution.h"

using namespace json;


namespace solver {

   Solution::Solution(
      const SolutionStep& step1
   )
   {
      success = true;
      if (!step1.IsNull())
         steps.push_back(step1);
   }


   void Solution::AddStep(const SolutionStep& step)
   {
      steps.push_back(step);
   }

   void Solution::Dump(FILE* f) const
   {
      fprintf(f, success ? "Solved!\r\n" : "Not solved.\r\n");
      for (int i = 0; i < steps.size(); ++i)
         steps[i].Dump(f);
   }


   /// <summary>
   /// Converts this object to a JSON string
   /// </summary>
   std::string Solution::ToJSONString(void) const
   {
      JSON result = JSON::Make(JSON::Class::Object);
      result["success"] = this->success;
      result["steps"] = JSON::Make(JSON::Class::Array);
      for (int i = 0; i < steps.size(); ++i)
         result["steps"][i] = steps[i].ToString();

      return result.dump();
   }


   bool Solution::operator==(const Solution& that) const
   {
      if (success != that.success)
         return false;
      if (steps.size() != that.steps.size())
         return false;
      for (int i = 0; i < steps.size(); ++i)
         if (steps[i] != that.steps[i])
            return false;
      return true;
   }


   Solution Solution::Fail(void)
   {
      Solution result;
      result.success = false;
      return result;
   }

}
