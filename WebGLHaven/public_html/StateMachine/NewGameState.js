/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, cardLocations, stateMachine */

function NewGameState() {
   // inherit State
   State.call(this);

   this.enter = function() {
      // shuffle
      cardLocations.newGame();
      
      // update
      cardLocations.repositionAll(this.webGLHaven);
      
      // start moving to aces
      this.webGLHaven.stateMachine.setState(new MoveToAcesState());
   };
}

