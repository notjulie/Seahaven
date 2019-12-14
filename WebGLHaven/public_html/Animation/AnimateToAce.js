/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global cardLocations, stateMachine, world, deckOfCards, LocationID */


/**
 * 
 * @class Animation class for moving cards to the ace piles
 * 
 * @param {string} cardID
 * @param {string} startLocationID
 * @param {string} endLocationID
 * @returns {AnimateToAce}
 */
function AnimateToAce(cardID, startLocationID, endLocationID) {
   var positionOnCurrentStep = 0;
   var startTime = stateMachine.getTime();
   var card3D = deckOfCards.getCard3D(cardID);
   var speed = 1.0; // world units per second
   
   // Create our path... this depends on whether we are moving from
   // a tower or a column.  Basically our strategy is to choose a specific
   // Z location that each card needs to move to before starting the next
   // animation, which will prevent any pair of cards from passing through
   // each other.
   var safeZ = world.getColumnPosition(0,0).z;
   
   // path depends on if it's from a tower or column
   var path = [ world.getCardLocation(startLocationID) ];
   var startLocationInfo = LocationID.info[startLocationID];
   if (startLocationInfo.isTower) {
      var next;
      switch (startLocationInfo.tower) {
         case 0:
            var next = world.getTowerTop(1);
            next.x -= world.getCardWidth();
            break;

         case 1:
            var next = world.getTowerTop(1);
            break;

         case 2:
            var next = world.getTowerTop(2);
            break;

         case 3:
            var next = world.getTowerTop(2);
            next.x += world.getCardWidth();
            break;
      }
      next.z = safeZ;
      path.push(next);
   } else {
      var step1 = world.getCardLocation(startLocationID);
      step1.y += world.getCardHeight();
      path.push(step1);
      
      var step2 = world.getCardLocation(startLocationID);
      step2.y = world.getColumnPosition(0,0).y + world.getCardHeight();
      step2.z = safeZ;
      path.push(step2);
   }
   
   path.push(world.getCardLocation(endLocationID));
   
   /**
    * Performs periodic animation updates
    * 
    * @returns {undefined}
    */
   this.service = function() {
      // update our clock
      elapsed = stateMachine.getTime() - startTime;
      startTime = stateMachine.getTime();

      // update our position along the current segment
      positionOnCurrentStep += elapsed * speed;
      var dx = path[1].x - path[0].x;
      var dy = path[1].y - path[0].y;
      var dz = path[1].z - path[0].z;
      var currentStepLength = Math.sqrt(dx*dx + dy*dy + dz*dz);
      if (positionOnCurrentStep >= currentStepLength) {
         card3D.position.x = path[1].x;
         card3D.position.y = path[1].y;
         card3D.position.z = path[1].z;
         path.splice(0, 1);
         positionOnCurrentStep = 0;
      } else {
         var portion = positionOnCurrentStep / currentStepLength;
         card3D.position.x = path[0].x + portion * (path[1].x - path[0].x);
         card3D.position.y = path[0].y + portion * (path[1].y - path[0].y);
         card3D.position.z = path[0].z + portion * (path[1].z - path[0].z);
      }
   };
   
   /**
    * 
    * returns a value indicating whether the animation is completed
    * 
    * @returns {Boolean}
    */
   this.isFinished = function() {
      return path.length < 2;
   };
   
   /**
    * 
    * Returns a value indicating whether the next card-to-ace animation may
    * be started; this is here to prevent cards from travelling through each
    * other as they move.
    * 
    * @returns {Boolean}
    */
   this.canStartNext = function() {
      return path.length <= 2;
   };
}
