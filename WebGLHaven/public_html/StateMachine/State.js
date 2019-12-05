/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

function State() {
   this.enter = function() {
   }
   
   this.exit = function() {
   }
   
   this.newGame = function() {
      stateMachine.setState(new NewGameState());
   }
}
