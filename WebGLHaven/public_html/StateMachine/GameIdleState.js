/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, stateMachine */

function GameIdleState() {
   // inherit State
   State.call(this);
   
   this.onMouseDown = function(event) {
      alert('GameIdleState.onMouseDown not implemented');
   };
}

