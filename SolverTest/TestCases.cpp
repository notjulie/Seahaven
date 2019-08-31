
#include "SolverIncludes.h"

#include "SeahavenProblem.h"
#include "Solution.h"

#include "TestCases.h"

/// <summary>
/// simple test case that verifies that cards get moved to the aces automatically
/// </summary>
const SeahavenProblem zeroMoveProblem(
        ProblemCards("KS", "QS", "JS", "TS", "9S"),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards()
        );
const Solution zeroMoveSolution;


/// <summary>
/// Scott's original problem that he suspects is not solvable
/// </summary>
const SeahavenProblem scottsProblem(
    ProblemCards("JS", "6H", "QH", "JH", "7H"),
    ProblemCards("4C", "JD", "8S", "2C"),
    ProblemCards("5S", "QC"),
    ProblemCards("AC", "7S", "9C"),
    ProblemCards("4S", "3D", "3C", "8C", "QD"),
    ProblemCards("JC", "KS", "5C", "KC", "6S"),
    ProblemCards("TC", "5H", "9S", "TS", "8D"),
    ProblemCards("KH", "6C", "TD", "5D", "6D"),
    ProblemCards("2S", "4D", "9H", "QS", "7D"),
    ProblemCards("7C", "3S", "AS", "8H", "TH"),
    ProblemCards("9D", "KD")
    );


/// <summary>
/// simple test case that requires only one column to column move
/// </summary>
const SeahavenProblem singleColumnToColumnProblem(
        ProblemCards("KS", "QS", "9S", "TS"),
        ProblemCards("JS"),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards()
        );
const Solution singleColumnToColumnSolution(
      "TS"
      );


/// <summary>
/// simple test case that requires only one column to column move
/// </summary>
const SeahavenProblem singleColumnToTowerProblem(
        ProblemCards("KS", "JS", "9S", "TS"),
        ProblemCards("QS"),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards(),
        ProblemCards()
        );
const Solution singleColumnToTowerSolution(
      "TS"
      );


/// <summary>
/// simple cache test to verify that the cache is eliminating iterations
/// </summary>
const SeahavenProblem cacheTestProblem(
        ProblemCards("4C", "QC", "3C"),
        ProblemCards("4D", "QD", "3D"),
        ProblemCards("4H", "QH", "3H"),
        ProblemCards("4S", "QS", "3S"),
        ProblemCards(),
        ProblemCards(),
        ProblemCards("KC", "5C", "2C", "JC", "TC", "9C", "8C", "7C", "6C"),
        ProblemCards("KD", "5D", "2D", "JD", "TD", "9D", "8D", "7D", "6D"),
        ProblemCards("KH", "5H", "2H", "JH", "TH", "9H", "8H", "7H", "6H"),
        ProblemCards("KS", "5S", "2S", "JS", "TS", "9S", "8S", "7S", "6S"),
        ProblemCards()
        );

/// <summary>
/// a test case that takes longer than some
/// </summary>
const SeahavenProblem slowTestProblem(
   ProblemCards("2S", "TD", "JH", "5D", "9H"),
   ProblemCards("AS", "8S", "7C", "2D", "7S"),
   ProblemCards("TC", "3C", "QH", "AD", "6S"),
   ProblemCards("QS", "JC", "5C", "KH", "QD"),
   ProblemCards("9C", "4H", "3H", "KC", "2C"),
   ProblemCards("AC", "2H", "3S", "JD", "7D"),
   ProblemCards("4D", "3D", "TS", "5S", "KD"),
   ProblemCards("8C", "AH", "6C", "TH", "8H"),
   ProblemCards("KS", "5H", "JS", "8D", "9D"),
   ProblemCards("7H", "4C", "9S", "QC", "6H"),

   ProblemCards("6D", "4S")
);


