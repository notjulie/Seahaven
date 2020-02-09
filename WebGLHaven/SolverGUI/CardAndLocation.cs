using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SolverGUI
{
   /// <summary>
   /// Card and location tuple
   /// </summary>
   public class CardAndLocation
   {
      /// <summary>
      /// card name
      /// </summary>
      public string card;

      /// <summary>
      /// location ID
      /// </summary>
      public string location;

      /// <summary>
      /// Creates a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return "\"" + card + "\":\"" + location + "\"";
      }
   }
}
