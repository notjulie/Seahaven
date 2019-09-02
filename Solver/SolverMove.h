
#ifndef SOLVERMOVE_H
#define SOLVERMOVE_H

struct SolverMove {
   enum {
      MoveNothing,
      MoveFromColumnToHigherCard,
      MoveFromColumnToTower
   } type = MoveNothing;
   union {
      uint8_t column;
   };
};

#endif
