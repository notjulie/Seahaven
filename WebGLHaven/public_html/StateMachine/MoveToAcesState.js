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
   const zDistanceBetweenCards = 0.01;
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
      var endLocation = cardLocations.getCardLocation(cardID);
      
      // start animating it to that position
      animations.push(new AnimateToAce(cardID, startLocation, endLocation));
   }
   
   /// <summary>
   /// Performs actions associated with state entry
   /// </summary>
   this.enter = function() {
      // either we have something to do or we don't
      var cardToMove = getNextCardToMove();
      if (cardToMove)
         startAnimation(cardToMove);
      else
         stateMachine.setState(new GameIdleState());
   };
   
   /// <summary>
   /// Carries out periodic actions
   /// </summary>
   this.service = function() {
      // give all our current animations their timeslice
      var maximumZ = -100000;
      for (var i=0; i<animations.length; ++i) {
         animations[i].service(maximumZ + zDistanceBetweenCards);
         var z = animations[i].getCurrentPosition().z;
         if (z > maximumZ)
            maximumZ = z;
      }
      
      // remove any that have finished
      for (var i=animations.length-1; i>=0; --i) {
         if (animations[i].isFinished())
            animations.splice(i,1);
      }
      
      // if all the cards' Z positions are further back than
      // the next card to move we can start moving it
      var cardToMove = getNextCardToMove();
      if (cardToMove) {
         var startPosition = world.getCardLocation(cardLocations.getCardLocation(cardToMove));
         if (startPosition.z > maximumZ + zDistanceBetweenCards)
            startAnimation(cardToMove);
      }
      
      // if all our animations are done we can move on to the next state
      if (animations.length === 0)
         stateMachine.setState(new GameIdleState());
   };
}


