/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global CardID */

function GameState() {
   this.getCardLocation = function(cardID) {
      var cardInfo = CardID.all[cardID];
      var index = (cardInfo.rankIndex - 1) + cardInfo.suitIndex*13;
      var columnIndex = Math.floor(index / 5);
      var rowIndex = index % 5;
      if (columnIndex < 10)
         return 'C' + columnIndex + '-' + rowIndex;
      else
         return 'T' + rowIndex;
   };
}
