/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global State, stateMachine, renderer, THREE */

function GameIdleState() {
   // inherit State
   State.call(this);
   
   this.onMouseDown = function(event) {
      var cardID = renderer.pointToCard(event.clientX, event.clientY);
      if (cardID)
         alert(cardID);
   };
}

