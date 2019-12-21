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
function StateMachine(webGLHaven) {
   var currentState = new State();
   var currentTime;
   
   this.getTime = function() {
      return currentTime;
   };
   
   this.newGame = function() {
      currentState.newGame();
   };

   /**
    * Called when a mouse down event occurs on the game canvas
    * 
    * @param {MouseEvent} event
    * @returns {undefined}
    */
   this.onMouseDown = function(event) {
      currentState.onMouseDown(event);
   };
   
   /**
    * Called when a mouse move event occurs on the game canvas
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseMove = function(event) {
      currentState.onMouseMove(event);
   };
      
   /**
    * Called when a mouse up event occurs on the game canvas
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseUp = function(event) {
      currentState.onMouseUp(event);
   };
      
   
   this.setState = function(state) {
      currentState.exit();
      currentState = state;
      state.webGLHaven = webGLHaven;
      currentState.enter();
   };
   
   this.service = function(time) {
      currentTime = time;
      currentState.service();
   };
}
