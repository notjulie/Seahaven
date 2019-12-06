/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, gameState, stateMachine */

function NewGameState() {
   // inherit State
   State.call(this);

   this.enter = function() {
      // shuffle
      gameState.newGame();
      
      // update
      gameState.repositionAll();
      
      // start moving to aces
      stateMachine.setState(new MoveToAcesState());
   };
}

