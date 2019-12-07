/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global CardID, LocationID, deckOfCards, gameState, world */


function CardLocations() {
   const cardLocations = {};
   
   
   /// <summary>
   /// shuffles the deck and deals
   /// </summary>
   this.newGame = function() {
      // make an array of all the CardIDs
      var cardIDs = new Array();
      for (var cardID in CardID.all)
         cardIDs.push(cardID);
      
      // shuffle
      for (var i=cardIDs.length-1; i>=0; --i) {
         var n = Math.floor((Math.random() * i));
         var tempCardID = cardIDs[n];
         cardIDs[n] = cardIDs[i];
         cardIDs[i] = tempCardID;
      }
      
      // deal
      for (var column=0; column<10; ++column)
         for (var row=0; row<5; ++row)
            cardLocations[cardIDs[column*5 + row]] = LocationID.columns[column][row];
      cardLocations[cardIDs[50]] = LocationID.towers[1];
      cardLocations[cardIDs[51]] = LocationID.towers[2];
   };

   this.getCardLocation = function(cardID) {
      return cardLocations[cardID];
   };
   
   this.cardsOnAce = function(suit) {
      var result = 0;
      for (var cardID in cardLocations) {
         var locationInfo = LocationID.all[cardLocations[cardID]];
         if (locationInfo.isAce)
            if (locationInfo.suit === suit)
               ++result;
      }
      return result;
   };
   
   this.getCardAtLocation = function(locationID) {
      for (var cardID in cardLocations)
         if (cardLocations[cardID] === locationID)
            return cardID;
   };
   
   this.getTowerCardID = function(towerIndex) {
      return this.getCardAtLocation(LocationID.towers[towerIndex]);
   };
   
   this.getBottomColumnCardID = function(column) {
      var result;
      var maxRow = -1;
      
      for (var cardID in cardLocations) {
         var locationInfo = LocationID.all[cardLocations[cardID]];
         if (locationInfo.column===column && locationInfo.row>maxRow) {
            result = cardID;
            maxRow = locationInfo.row;
         }
      }
      
      return result;
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
   
   /// <summary>
   /// Moves all Card3D objects to the current positions and shows them
   /// </summary>
   this.repositionAll = function() {
      for (var cardID in CardID.all) {
         var card3D = deckOfCards.getCard3D(cardID);
         var locationID = this.getCardLocation(cardID);
         card3D.position.copy(world.getCardLocation(locationID));
         card3D.visible = true;
      }
   };
   
   // start by shuffling
   this.newGame();
}
