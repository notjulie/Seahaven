/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, stateMachine */

function NewGameState() {
   // inherit State
   State.call(this);

   this.enter = function() {
      // shuffle
      this.webGLHaven.cardLocations.newGame();
      
      // update
      this.webGLHaven.cardLocations.repositionAll(this.webGLHaven);
      
      // start moving to aces
      this.webGLHaven.stateMachine.setState(new MoveToAcesState());
   };
}

NewGameState.prototype = Object.create(State.prototype);
