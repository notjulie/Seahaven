/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, stateMachine, renderer, THREE */

/**
 * Constructor for class GameIdleState, which implements the behaviors of
 * the game when it is idle.
 * 
 * @returns {GameIdleState}
 */
function GameIdleState() {
   // inherit State
   State.call(this);
   
   /**
    * Handles a mouse down event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseDown = function(event) {
      var cardID = renderer.pointToCard(event.clientX, event.clientY);
      if (cardID)
         stateMachine.setState(new DragCardState(event));
   };
}

