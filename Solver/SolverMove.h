
#ifndef SOLVERMOVE_H
#define SOLVERMOVE_H

/// <summary>
/// Types of moves
/// </summary>
enum class SolverMoveType {
   /// <summary>
   /// Null value
   /// </summary>
   Nothing,

   /// <summary>
   /// Move the bottom card on an indicated column to the next higher card or
   /// to an empty column if the card is a King
   /// </summary>
   FromColumnToHigherCard,

   /// <summary>
   /// Move the bottom card on an indicated column to a tower
   /// </summary>
   FromColumnToTower,

   /// <summary>
   /// Move a king from an empty column to a tower
   /// </summary>
   FromThroneToTower,

   /// <summary>
   /// Moves a king from the tower to the corresponding empty throne
   /// </summary>
   FromTowerToEmptyThrone
};

struct SolverMove {
   SolverMoveType type = SolverMoveType::Nothing;
   union {
      uint8_t column;
      Suit suit;
   };
};

#endif
