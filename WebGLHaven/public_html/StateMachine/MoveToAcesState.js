/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, stateMachine */

function MoveToAcesState() {
   // inherit State
   State.call(this);
   
   function getNextCardToMove() {
      
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


