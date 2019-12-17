/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/**
 * Constructor for class StateMachine, which is the main state machine for the
 * game.
 * 
 * @returns {StateMachine}
 */
function StateMachine() {
   var currentState = new State();
   var currentTime;
   
   this.getTime = function() {
      return currentTime;
   };
   
   this.newGame = function() {
      currentState.newGame();
   };

   /**
    * Called when a mouse down even occurs on the game canvas
    * 
    * @param {MouseEvent} event
    * @returns {undefined}
    */
   this.onMouseDown = function(event) {
      currentState.onMouseDown(event);
   };
   
   this.setState = function(state) {
      currentState.exit();
      currentState = state;
      currentState.enter();
   };
   
   this.service = function(time) {
      currentTime = time;
      currentState.service();
   };
}
