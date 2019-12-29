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
    * the inherited game object, redeclared here to help intellisense
    * @type WebGLHaven
    */
   this.webGLHaven;
   
   /**
    * Performs calculations to determine where the card should be assuming
    * that it is moving in a plane in front of the cards on the table.
    * @param {THREE.Ray} ray ray on which the origin of the card sits
    * @returns {THREE.Vector3}
    */
   function calculatePositionInFrontOfColumns(ray) {
      var position = new THREE.Vector3();
      
      /** @type World */
      var world = this.webGLHaven.world;
      
      // for each row on the table we intersect the ray with that row's plane and
      // determine if the resulting point:
      //  1) is far enough forward to be in front of the cards
      //  2) is high enough not to be clipped by the table or the ground
      for (var rowIndex=0; rowIndex<world.properties.columnRowCount; ++rowIndex) {
         // find where our ray intersects that row
         ray.intersectPlane(world.getRowPlane(rowIndex), position);
         
         // find what column that would be
         var columnIndex = world.getColumnForX(position.x);
         if (columnIndex > 9 || columnIndex < 0)
            continue;
         
         // since the card origins are bottom-left, we might be in front of either
         // this column or the one to its right
         var margin = world.properties.cardWidth * world.properties.cardRelativeMargin / 3;
         var minRowIndex = 0;
         var leftColumnFrontRow = cardLocations.getMaxOccupiedRowOnColumn(columnIndex);
         var leftColumnPosition = world.getColumnPosition(columnIndex, leftColumnFrontRow);
         if (leftColumnPosition.x + world.properties.cardWidth + margin >= position.x)
            minRowIndex = Math.max(minRowIndex, 1 + leftColumnFrontRow);
         var rightColumnFrontRow = cardLocations.getMaxOccupiedRowOnColumn(columnIndex + 1);
         var rightColumnPosition = world.getColumnPosition(columnIndex + 1, rightColumnFrontRow);
         if (position.x + world.properties.cardWidth + margin >= rightColumnPosition.x)
            minRowIndex = Math.max(minRowIndex, 1 + rightColumnFrontRow);
         
         // see if we are in front far enough of those columns that we can descend
         // in front of them; we do that by seeing if we can be one row in front of it
         if (rowIndex >= minRowIndex) {
            // since at this point we are moving the card downward we need to stop
            // it if it reaches the table
            var minY = world.getColumnPosition(columnIndex, rowIndex).y;
            if (position.y < minY)
               position.y = minY;
            card.position.copy(position);
            return position;
         }
      }
   }
   
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
      
      // figure out our position if we assume that we are dragging the card in
      // front of the column cards
      var positionInFrontOfColumns = calculatePositionInFrontOfColumns.call(this, ray);
            
      // figure out our position assuming that we are dragging the card above the
      // column cards
      var pointOnTableLid = ray.intersectPlane(this.webGLHaven.world.tableLidPlane, new THREE.Vector3());
      
      // triage
      var targetPosition = pointOnTableLid;
      if (positionInFrontOfColumns && positionInFrontOfColumns.z < targetPosition.z)
         targetPosition = positionInFrontOfColumns;
      
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
