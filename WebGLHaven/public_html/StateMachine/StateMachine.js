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
 * @class StateMachine
 * @param {WebGLHaven} webGLHaven 
 * @returns {StateMachine}
 */
function StateMachine(webGLHaven) {
   var currentState = new State();
   var currentTime;
   var lastMouseMove;
   
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
      // just queue this up to be handled on our display loop, so that we
      // don't process a ton of events that never actually get seens on the
      // screen
      lastMouseMove = event;
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
   
   /**
    * Called periodically, i.e. on our screen update loop
    * @param {Number} time
    * @returns {undefined}
    */
   this.service = function(time) {
      // handle any queued up mouse move event
      if (lastMouseMove) {
         currentState.onMouseMove(lastMouseMove);
         lastMouseMove = undefined;
      }
      
      // call the current state's service function
      currentTime = time;
      currentState.service();
   };
   
   // enter initializing state
   this.setState(new InitializingState());
}
