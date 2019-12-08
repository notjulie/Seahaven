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
   
   function canMoveToAce(cardID) {
      var cardInfo = CardID.info[cardID];
      return cardInfo.rankIndex === cardLocations.cardsOnAce(cardInfo.suitIndex) + 1;
   }
   
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

   function startAnimation(cardID) {
      // for now just move it
      cardLocations.moveToAce(cardID);
      
      // update
      cardLocations.repositionAll();
   }
   
   this.enter = function() {
      // either we have something to do or we don't
      var cardToMove = getNextCardToMove();
      if (cardToMove)
         startAnimation(cardToMove);
      else
         stateMachine.setState(new GameIdleState());
   };
}


