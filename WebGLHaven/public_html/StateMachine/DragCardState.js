/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, cardLocations, THREE */


/**
 * 
 * @param {MouseEvent} mouseDownEvent the mouse down event that begins the drag 
 * @returns {DragCardState}
 */
function DragCardState(mouseDownEvent) {
   // inherit State
   State.call(this);
   
   var card;
   var worldClickPoint;
   var startPosition;
   var startPositionPlane;
   
   this.enter = function() {
      // get the card that we are going to drag
      var cardID = this.webGLHaven.renderer.pointToCard(mouseDownEvent.clientX, mouseDownEvent.clientY);
      card = this.webGLHaven.deckOfCards.getCard3D(cardID);

      // get its location
      var startLocationID = cardLocations.getCardLocation(cardID);
      startPosition = this.webGLHaven.world.getCardLocation(startLocationID);
      startPositionPlane = new THREE.Plane(new THREE.Vector3(0,0,1), -startPosition.z);

      // calculate the world position that corresponds to the point
      // on the card at the click position... we do this by calculating
      // the x and y of the camera ray where it intersects the card's Z
      // coordinate
      var clickRay = this.webGLHaven.renderer.pointToRay(mouseDownEvent.clientX, mouseDownEvent.clientY);
      worldClickPoint = clickRay.intersectPlane(startPositionPlane, new THREE.Vector3());      
   };

   /**
    * Handles a mouse move event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseMove = function(event) {
      // get the ray that goes through the point on the screen; this is the
      // ray that should go through the point on the card that was originally
      // clicked
      var ray = this.webGLHaven.renderer.pointToRay(event.clientX, event.clientY);
      
      // offset the ray to give us a ray on which the card's origin must fall
      ray.origin.x -= worldClickPoint.x - startPosition.x;
      ray.origin.y -= worldClickPoint.y - startPosition.y;
      
      // we can figure out the 3D location of the card by first seeing where our ray
      // intersects the plane that represents the top of the cards on the table
      
      // intersect it with the plane of the card's original location
      var worldPoint = ray.intersectPlane(startPositionPlane, new THREE.Vector3());
      
      // update the card's position
      card.position.x = worldPoint.x;
      card.position.y = worldPoint.y;
   };
   
   /**
    * Handles a mouse up event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseUp = function(event) {
      cardLocations.repositionAll(this.webGLHaven);
      this.webGLHaven.stateMachine.setState(new GameIdleState());
   };
}
