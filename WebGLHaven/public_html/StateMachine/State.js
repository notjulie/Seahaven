/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global stateMachine */

function State() {
   this.enter = function() {
   };
   
   this.exit = function() {
   };
   
   this.service = function(time) {
   };
   
   this.onMouseDown = function(event) {
   };

   this.newGame = function() {
      stateMachine.setState(new NewGameState());
   };
}
