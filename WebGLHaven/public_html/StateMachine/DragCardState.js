/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, THREE */


/**
 * State class active when the user is dragging a card
 * 
 * @constructor
 * @extends State
 * @param {MouseEvent} mouseDownEvent the mouse down event that begins the drag 
 * @returns {DragCardState}
 */
function DragCardState(mouseDownEvent) {
   // inherit State
   State.call(this);
   
   var card;
   var worldClickPoint;
   var startPosition;
   
   /**
    * our private copy of the cardLocations collection with the cards that
    * we are dragging removed
    * @type CardLocations
    */
   var cardLocations;
   
   /**
    * @type WebGLHaven
    */
   this.webGLHaven;
   
   /**
    * Performs the state's entry actions
    * @returns {undefined}
    */
   this.enter = function() {
      // clone the card locations
      cardLocations = this.webGLHaven.cardLocations.clone();
      
      // get the card that we are going to drag
      var cardID = this.webGLHaven.renderer.pointToCard(mouseDownEvent.clientX, mouseDownEvent.clientY);
      card = this.webGLHaven.deckOfCards.getCard3D(cardID);

      // get its location
      var startLocationID = this.webGLHaven.cardLocations.getCardLocation(cardID);
      startPosition = this.webGLHaven.world.getCardLocation(startLocationID);
      var startPositionPlane = new THREE.Plane(new THREE.Vector3(0,0,1), -startPosition.z);

      // move it to nowhere
      cardLocations.removeCard(cardID);

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
      var pointOnTableLid = ray.intersectPlane(this.webGLHaven.world.tableLidPlane, new THREE.Vector3());
      var targetPosition = pointOnTableLid;
      
      // if our calculated z position is such that we would be in front of the
      // frontmost row, change to intersect our ray with that row's plane
      for (var rowIndex=0; rowIndex<this.webGLHaven.world.properties.columnRowCount; ++rowIndex) {
         // find where our ray intersects that row
         var position = ray.intersectPlane(this.webGLHaven.world.getRowPlane(rowIndex), new THREE.Vector3());
         
         // find what column that would be
         var columnIndex = this.webGLHaven.world.getColumnForX(position.x);
         
         // see if we are in front far enough of that column that we can descend
         // in front of it; we do that by seeing if we can be one row in front of it
         var minRowIndex = 1 + cardLocations.getMaxOccupiedRowOnColumn(columnIndex);
         if (rowIndex >= minRowIndex) {
            // since at this point we are moving the card downward we need to stop
            // it if it reaches the table
            var minY = this.webGLHaven.world.getColumnPosition(columnIndex, rowIndex).y;
            if (position.y < minY)
               position.y = minY;
            card.position.copy(position);
            return;
         }
      }
      
      // update the card's position
      card.position.copy(targetPosition);
   };
   
   /**
    * Handles a mouse up event
    * 
    * @param {MouseEvent} event the event
    * @returns {undefined}
    */
   this.onMouseUp = function(event) {
      this.webGLHaven.repositionAllCards();
      this.webGLHaven.stateMachine.setState(new GameIdleState());
   };
}
DragCardState.prototype = Object.create(State.prototype);
