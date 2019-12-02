/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global CardID, LocationID, deckOfCards, gameState, world */

function GameState() {
   const cardLocations = {};
   
   
   function getDefaultCardLocation(cardID) {
      var cardInfo = CardID.all[cardID];
      var index = (cardInfo.rankIndex - 1) + cardInfo.suitIndex*13;
      var columnIndex = Math.floor(index / 5);
      var rowIndex = index % 5;
      if (columnIndex < 10)
         return LocationID.columns[columnIndex][rowIndex];
      else
         return LocationID.towers[rowIndex];
   };

   for (var cardID in CardID.all)
      cardLocations[cardID] = getDefaultCardLocation(cardID);

   this.getCardLocation = function(cardID) {
      return cardLocations[cardID];
   };
   
   this.getNumberOfCardsOnColumn = function(column) {
      var result = 0;
      for (var cardID in cardLocations) {
         var locationID = cardLocations[cardID];
         if (LocationID.all[locationID].column === column)
            ++result;
      }
      return result;
   };
   
   this.moveToBottomOfColumn = function(cardID, column) {
      cardLocations[cardID] = LocationID.columns[column][this.getNumberOfCardsOnColumn(column)];
   };
   
   this.repositionAll = function() {
      for (var cardID in CardID.all) {
         var card = deckOfCards.getCard3D(cardID);
         var locationID = this.getCardLocation(cardID);
         card.position.copy(world.getCardLocation(locationID));
      }
   };
}
