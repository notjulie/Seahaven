/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global stateMachine */

function State() {
   this.webGLHaven = undefined;
   
   this.enter = function() {
   };
   
   this.exit = function() {
   };
   
   this.service = function(time) {
   };
   
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

   this.newGame = function() {
      this.webGLHaven.stateMachine.setState(new NewGameState(this.webGLHaven));
   };
}
