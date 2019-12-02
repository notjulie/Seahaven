
#include "SolverIncludes.h"

#include "SeahavenProblem.h"
#include "Solution.h"

#include "TestCases.h"

/// <summary>
/// simple test case that verifies that cards get moved to the aces automatically
/// </summary>
const ProblemDefinition zeroMoveProblem =
{
   // columns
   {
      {"KS", "QS", "JS", "TS", "9S"},
   },
   // towers
   {
   }
};
const Solution zeroMoveSolution;

/// <summary>
/// simple test case that tests combining towers
/// </summary>
const ProblemDefinition combineTowersProblem = 
{
   // columns
   {
      {"QS", "6S", "KS", "7S", "8S"},
   },
   // towers
   {
      "JS", "TS", "9S"
   }
};


/// <summary>
/// Scott's original problem that he suspects is not solvable
/// </summary>
const ProblemDefinition scottsProblem = {
   // columns
   {
      {"JS", "6H", "QH", "JH", "7H"},
      {"4C", "JD", "8S", "2C"},
      {"5S", "QC"},
      {"AC", "7S", "9C"},
      {"4S", "3D", "3C", "8C", "QD"},
      {"JC", "KS", "5C", "KC", "6S"},
      {"TC", "5H", "9S", "TS", "8D"},
      {"KH", "6C", "TD", "5D", "6D"},
      {"2S", "4D", "9H", "QS", "7D"},
      {"7C", "3S", "AS", "8H", "TH"}
   },
   //towers
   { "9D", "KD" }
};

ProblemDefinition scott2 =
{
   // columns
   {
      {"9S", "2D", "4S", "KS", "9H"},
      {"KD", "9C", "5S", "TC"},
      {"5H", "TH", "6H", "3S", "KC"},
      {"8S", "7D", "4H", "2S", "9D"},
      {"5C", "7H", "7S"},
      {"3D", "6S", "KH", "5D", "7C"},
      {"4C", "8C", "JH", "6C", "TD"},
      {"QH", "2H", "JS", "AH", "8H"},
      {"TS", "3H", "QC", "3C", "QD"},
      {"JD", "8D", "6D", "JC", "QS"},
   },
   // towers
   {
      "4D"
   }
};

/// <summary>
/// simple test case that requires only one column to column move
/// </summary>
const ProblemDefinition singleColumnToColumnProblem =
{
   // columns
   {
      {"KS", "QS", "9S", "TS"},
      {"JS"}
   },
   // towers
   {
   }
};
const Solution singleColumnToColumnSolution(
      "TS"
      );


/// <summary>
/// simple test case that requires only one column to column move
/// </summary>
const ProblemDefinition singleColumnToTowerProblem =
{
   // columns
   {
      {"KS", "QS", "JS", "9S", "TS"}
   },
   // towers
   {
   }
};
const Solution singleColumnToTowerSolution(
      "TS"
      );


/// <summary>
/// simple cache test to verify that the cache is eliminating iterations
/// </summary>
const ProblemDefinition cacheTestProblem =
{
   // columns
   {
      {"4C", "QC", "3C"},
      {"4D", "QD", "3D"},
      {"4H", "QH", "3H"},
      {"4S", "QS", "3S"},
      {},
      {},
      {"KC", "5C", "2C", "JC", "TC", "9C", "8C", "7C", "6C"},
      {"KD", "5D", "2D", "JD", "TD", "9D", "8D", "7D", "6D"},
      {"KH", "5H", "2H", "JH", "TH", "9H", "8H", "7H", "6H"},
      {"KS", "5S", "2S", "JS", "TS", "9S", "8S", "7S", "6S"},
   },
   // towers
   {
   }
};

/// <summary>
/// A test case that used to take longer than some; it sped up just fine
/// when I put a cap on how long I will search for a better solution after
/// having found one.
/// </summary>
const ProblemDefinition slowTestProblem =
{
   // columns
   {
      {"2S", "TD", "JH", "5D", "9H"},
      {"AS", "8S", "7C", "2D", "7S"},
      {"TC", "3C", "QH", "AD", "6S"},
      {"QS", "JC", "5C", "KH", "QD"},
      {"9C", "4H", "3H", "KC", "2C"},
      {"AC", "2H", "3S", "JD", "7D"},
      {"4D", "3D", "TS", "5S", "KD"},
      {"8C", "AH", "6C", "TH", "8H"},
      {"KS", "5H", "JS", "8D", "9D"},
      {"7H", "4C", "9S", "QC", "6H"},
   },
   // towers
   {
      "6D", "4S"
   }
};


/// <summary>
/// a test case that takes longer than some (initially 18s on release build)
/// </summary>
const ProblemDefinition slowTestProblem2 =
{
   // columns
   {
      {"AD", "2D", "5D", "7D", "KS"},
      {"3S", "3H", "9C", "5C", "8S"},
      {"TC", "9H", "4S", "5S", "7H"},
      {"KD", "AH", "QH", "7S", "4D"},
      {"2H", "KC", "8D", "8C", "6H"},
      {"3D", "5H", "QD", "JD", "AC"},
      {"2S", "9S", "QC", "6S", "2C"},
      {"JH", "7C", "TD", "9D", "6D"},
      {"JS", "QS", "4C", "6C", "AS"},
      {"4H", "3C", "JC", "8H", "TS"}
   },
   // towers
   {
      "KH", "TH"
   }
};
