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
      cardLocations[cardID] = undefined;

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
      
      // see which column we are over
      var z = pointOnTableLid.z;
      var columnIndex = this.webGLHaven.world.getColumnForX(pointOnTableLid.x);
      if (columnIndex>=0 && columnIndex<=9) {
         // get the z coordinate of the bottom card on the column
         var bottomColumnCardID = this.webGLHaven.cardLocations.getBottomColumnCardID(columnIndex);
         if (bottomColumnCardID) {
            var bottomColumnCard = this.webGLHaven.deckOfCards.getCard3D(bottomColumnCardID);
            z = bottomColumnCard.position.z;
         }
      }
      
      // update the card's position
      card.position.x = pointOnTableLid.x;
      card.position.y = pointOnTableLid.y;
      card.position.z = z;
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
