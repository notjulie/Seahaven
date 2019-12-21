/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/**
 * Base class for StateMachine states
 * 
 * @constructor
 * @returns {State}
 */
function State() {
   /**
    * Reference to the game object
    * @type WebGLHaven
    */
   this.webGLHaven = undefined;
   
   /**
    * Performs the state's entry actions
    * @returns {undefined}
    */
   this.enter = function() {
   };
   
   /**
    * Performs the state's exit actions
    * @returns {undefined}
    */
   this.exit = function() {
   };
   
   /**
    * Performs periodic actions while in the state
    * @param time the current timestamp (seconds from some arbitrary initial time)
    * @returns {undefined}
    */
   this.service = function(time) {
   };
   
   /**
    * Processes a mouse down event
    * @param {MouseEvent} event
    * @returns {undefined}
    */
   this.onMouseDown = function(event) {
   };

   /**
    * Handles a mouse move event; does nothing by default
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseMove = function(event) {
   };

   /**
    * Handles a mouse up event; does nothing by default
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseUp = function(event) {
   };

   /**
    * Processes a request to start a new game
    * @returns {undefined}
    */
   this.newGame = function() {
      this.webGLHaven.stateMachine.setState(new NewGameState());
   };
}
