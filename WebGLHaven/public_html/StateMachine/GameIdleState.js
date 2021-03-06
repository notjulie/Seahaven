/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, THREE, LocationID */

/**
 * Constructor for class GameIdleState, which implements the behaviors of
 * the game when it is idle.
 * 
 * @augments State
 * @constructor {GameIdleState}
 */
function GameIdleState() {
   // inherit State
   State.call(this);
}
GameIdleState.prototype = Object.create(State.prototype);

/**
 * Handles a mouse down event
 * 
 * @param {MouseEvent} event the event
 * @returns {undefined}
 */
GameIdleState.prototype.onMouseDown = function(event) {
   var cardID = this.webGLHaven.renderer.pointToCard(event.clientX, event.clientY);
   if (cardID) {
      var locationID = this.webGLHaven.cardLocations.getCardLocation(cardID);
      if (!LocationID.info[locationID].isAce)
         this.webGLHaven.stateMachine.setState(new DragCardState(event));
   }
};

/**
 * Attempts to solve
 * @param {any} solver The object in charge of solving
 */
GameIdleState.prototype.solve = function (solver) {
   this.webGLHaven.stateMachine.setState(new SolveState(solver));
};
