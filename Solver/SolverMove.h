
#ifndef SOLVERMOVE_H
#define SOLVERMOVE_H

struct SolverMove {
   enum {
      MoveNothing,
      MoveFromColumn
   } type = MoveNothing;
   union {
      uint8_t column;
   };
};

#endif
