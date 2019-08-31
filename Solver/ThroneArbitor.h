/* 
 * File:   ThroneArbitor.h
 * Author: Randy
 *
 * Created on April 13, 2014, 9:36 PM
 */

#ifndef THRONEARBITOR_H
#define	THRONEARBITOR_H

enum ThroneState {
   UNSPECIFIED,
   ON_TOWER,
   ON_COLUMN
};

class ThroneArbitor {
public:
   ThroneArbitor(void);

   void  Arbitrate(uint8_t _towersUsed, uint8_t _emptyColumns, const uint8_t *throneSizes);
   bool  CanMoveToThrone(uint8_t size) const;
   int   GetEmptyTowers(void) const { return 4 - towersUsed; }
   const ThroneState *GetThroneStates(void) const { return throneStates; }

private:
   void  HandleAllOnColumns(uint8_t _towersUsed, uint8_t _emptyColumns);
   void  HandleAllOnTowers(uint8_t _towersUsed, const uint8_t *throneSizes);
   
private:
   uint8_t  towersUsed;
   uint8_t  emptyColumns;
   ThroneState throneStates[4];
};

#endif	/* THRONEARBITOR_H */

