/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, renderer, stateMachine */


/**
 * 
 * @param {MouseEvent} mouseDownEvent the mouse down event that begins the drag 
 * @returns {DragCardState}
 */
function DragCardState(mouseDownEvent) {
   // inherit State
   State.call(this);
   
   // get the card that we are going to drag
   var cardID = renderer.pointToCard(mouseDownEvent.clientX, mouseDownEvent.clientY);

   /**
    * Handles a mouse move event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseMove = function(event) {
      stateMachine.setState(new GameIdleState());
      alert('DragCardState.onMouseMove not implemented');
   };
}
