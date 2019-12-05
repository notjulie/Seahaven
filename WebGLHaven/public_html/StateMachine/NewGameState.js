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

   this.enter = function() {
      alert("Entering NewGameState");
   };
}

