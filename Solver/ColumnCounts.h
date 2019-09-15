
#ifndef COLUMNCOUNTS_H
#define COLUMNCOUNTS_H


class ColumnCounts {
public:
   ColumnCounts(void);

   void Clear(void);
   void Decrement(uint8_t column);
   uint8_t Get(uint8_t column) const;
   int GetEmptyCount(void) const;
   const uint8_t* GetPointer(void) const { return columnCounts; }
   void Increment(uint8_t column);

private:
   uint8_t        columnCounts[10];
};



#endif
