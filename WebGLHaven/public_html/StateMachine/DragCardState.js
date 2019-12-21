/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, renderer, stateMachine, cardLocations, world, THREE, deckOfCards */


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
   var card = deckOfCards.getCard3D(cardID);
   
   // get its location
   var startLocationID = cardLocations.getCardLocation(cardID);
   var startPosition = world.getCardLocation(startLocationID);
   var startPositionPlane = new THREE.Plane(new THREE.Vector3(0,0,1), startPosition.z);
   
   // calculate the world position that corresponds to the point
   // on the card at the click position... we do this by calculating
   // the x and y of the camera ray where it intersects the card's Z
   // coordinate
   var clickRay = renderer.pointToRay(mouseDownEvent.clientX, mouseDownEvent.clientY);
   var worldClickPoint = clickRay.intersectPlane(startPositionPlane, new THREE.Vector3());
   

   /**
    * Handles a mouse move event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseMove = function(event) {
      // get the ray that goes through the point on the screen
      var ray = renderer.pointToRay(event.clientX, event.clientY);
      
      // intersect it with the plane of the card's original location
      var worldPoint = ray.intersectPlane(startPositionPlane, new THREE.Vector3());
      
      // update the card's position
      card.position.x = startPosition.x + (worldPoint.x - worldClickPoint.x);
      card.position.y = startPosition.y + (worldPoint.y - worldClickPoint.y);
   };
   
   /**
    * Handles a mouse up event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseUp = function(event) {
      cardLocations.repositionAll();
      stateMachine.setState(new GameIdleState());
   };
}
