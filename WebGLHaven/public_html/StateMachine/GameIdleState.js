/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, THREE */

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
      var cardID = this.webGLHaven.renderer.pointToCard(event.clientX, event.clientY);
      if (cardID)
         this.webGLHaven.stateMachine.setState(new DragCardState(event));
   };
}

