/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, stateMachine, cardLocations, CardID */

function MoveToAcesState() {
   // inherit State
   State.call(this);
   
   // private members
   var animations = [];
   
   /// <summary>
   /// Determines if a card can move to the aces
   /// </summary>
   function canMoveToAce(cardID) {
      var cardInfo = CardID.info[cardID];
      return cardInfo.rankIndex === cardLocations.cardsOnAce(cardInfo.suitIndex) + 1;
   }
   
   /// <summary>
   /// Gets the next card to move
   /// </summary>
   function getNextCardToMove() {
      // try towers
      for (var tower=0; tower<4; ++tower) {
         var cardID = cardLocations.getTowerCardID(tower);
         if (cardID)
            if (canMoveToAce(cardID))
               return cardID;
      }
      
      // try columns
      for (var column=0; column<10; ++column) {
         var cardID = cardLocations.getBottomColumnCardID(column);
         if (cardID)
            if (canMoveToAce(cardID))
               return cardID;
      }
      
      // else nothing
      return;
   }

   /// <summary>
   /// Starts animating the card
   /// </summary>
   function startAnimation(cardID) {
      // get the starting location
      var startLocation = cardLocations.getCardLocation(cardID);
      
      // move the card's static location
      cardLocations.moveToAce(cardID);
      var endLocation = cardLocations.getCardAtLocation(cardID);
      
      // start animating it to that position
      animations.push(new AnimateToAce(cardID, startLocation, endLocation));
   }
   
   this.enter = function() {
      // either we have something to do or we don't
      var cardToMove = getNextCardToMove();
      if (cardToMove)
         startAnimation(cardToMove);
      else
         stateMachine.setState(new GameIdleState());
   };
   
   this.service = function(time) {
      var cardToMove = getNextCardToMove();
      if (cardToMove)
         startAnimation(cardToMove);
      else
         stateMachine.setState(new GameIdleState());
   };
}


