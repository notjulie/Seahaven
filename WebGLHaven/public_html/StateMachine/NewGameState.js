/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State */

function NewGameState() {
   // inherit State
   State.call(this);
}

NewGameState.prototype = Object.create(State.prototype);

/**
 * Performs state entry actions
 * @returns {undefined}
 */
NewGameState.prototype.enter = function() {
   // shuffle
   this.webGLHaven.cardLocations.newGame();

   // update
   this.webGLHaven.repositionAllCards();

   // start moving to aces
   this.webGLHaven.stateMachine.setState(new MoveToAcesState());
};
