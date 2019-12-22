/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, CardID */

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
      return cardInfo.rankIndex === this.webGLHaven.cardLocations.cardsOnAce(cardInfo.suitIndex) + 1;
   }
   
   /// <summary>
   /// Gets the next card to move
   /// </summary>
   function getNextCardToMove() {
      // try towers
      for (var tower=0; tower<4; ++tower) {
         var cardID = this.webGLHaven.cardLocations.getTowerCardID(tower);
         if (cardID)
            if (canMoveToAce.call(this, cardID))
               return cardID;
      }
      
      // try columns
      for (var column=0; column<10; ++column) {
         var cardID = this.webGLHaven.cardLocations.getBottomColumnCardID(column);
         if (cardID)
            if (canMoveToAce.call(this, cardID))
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
      var startLocation = this.webGLHaven.cardLocations.getCardLocation(cardID);
      
      // move the card's static location
      this.webGLHaven.cardLocations.moveToAce(cardID);
      var endLocation = this.webGLHaven.cardLocations.getCardLocation(cardID);
      
      // start animating it to that position
      animations.push(new AnimateToAce(this.webGLHaven, cardID, startLocation, endLocation));
   }
   
   /// <summary>
   /// Performs actions associated with state entry
   /// </summary>
   this.enter = function() {
      // either we have something to do or we don't
      var cardToMove = getNextCardToMove.call(this);
      if (cardToMove)
         startAnimation.call(this, cardToMove);
      else
         this.webGLHaven.stateMachine.setState(new GameIdleState());
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
      var cardToMove = getNextCardToMove.call(this);
      if (cardToMove) {
         var startPosition = this.webGLHaven.world.getCardLocation(this.webGLHaven.cardLocations.getCardLocation(cardToMove));
         if (startPosition.z > maximumZ + zDistanceBetweenCards)
            startAnimation.call(this, cardToMove);
      }
      
      // if all our animations are done we can move on to the next state
      if (animations.length === 0)
         this.webGLHaven.stateMachine.setState(new GameIdleState());
   };
}

MoveToAcesState.prototype = Object.create(State.prototype);

