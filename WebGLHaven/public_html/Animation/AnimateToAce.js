/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global cardLocations, LocationID */


/**
 * 
 * @class Animation class for moving cards to the ace piles
 * 
 * @param {WebGLHaven} webGLHaven the game object
 * @param {string} cardID
 * @param {string} startLocationID
 * @param {string} endLocationID
 * @returns {AnimateToAce}
 */
function AnimateToAce(webGLHaven, cardID, startLocationID, endLocationID) {
   var positionOnCurrentStep = 0;
   var startTime = webGLHaven.stateMachine.getTime();
   var card3D = webGLHaven.deckOfCards.getCard3D(cardID);
   var speed = 6.0; // world units per second
   
   // path depends on if it's from a tower or column
   var path = [ webGLHaven.world.getCardLocation(startLocationID) ];
   var startLocationInfo = LocationID.info[startLocationID];
   if (startLocationInfo.isTower) {
      var next;
      switch (startLocationInfo.tower) {
         case 0:
            var next = webGLHaven.world.getTowerTop(1);
            next.x -= webGLHaven.world.getCardWidth();
            path.push(next);
            break;

         case 1:
         case 2:
            // for the current geometry these guys can just go straight
            // to the aces
            break;

         case 3:
            var next = webGLHaven.world.getTowerTop(2);
            next.x += webGLHaven.world.getCardWidth();
            path.push(next);
            break;
      }
   } else {
      var step1 = webGLHaven.world.getCardLocation(startLocationID);
      step1.y += webGLHaven.world.cardDimensions.height;
      path.push(step1);
      
      var step2 = webGLHaven.world.getCardLocation(startLocationID);
      step2.y = webGLHaven.world.getColumnPosition(0,0).y + webGLHaven.world.cardDimensions.height;
      step2.z = webGLHaven.world.getColumnPosition(0,0).z;
      path.push(step2);
   }
   
   path.push(webGLHaven.world.getCardLocation(endLocationID));
   
   /**
    * Gets the current position of the care we are animating
    * 
    * @returns {.deckOfCards@call;getCard3D.position}
    */
   this.getCurrentPosition = function() {
      return card3D.position;
   };
   
   /**
    * Performs periodic animation updates
    * 
    * @param {Number} minimumZ the farthest back that we are allowed
    * to move this card on this iteration; this prevents us from having
    * cards pass through each other 
    * @returns {undefined}
    */
   this.service = function(minimumZ) {
      // update our clock
      elapsed = webGLHaven.stateMachine.getTime() - startTime;
      startTime = webGLHaven.stateMachine.getTime();

      // update our position along the current segment
      positionOnCurrentStep += elapsed * speed;
      var dx = path[1].x - path[0].x;
      var dy = path[1].y - path[0].y;
      var dz = path[1].z - path[0].z;
      var currentStepLength = Math.sqrt(dx*dx + dy*dy + dz*dz);

      // if we can just finish up then do so
      if (positionOnCurrentStep >= currentStepLength && path[1].z>=minimumZ) {
         card3D.position.x = path[1].x;
         card3D.position.y = path[1].y;
         card3D.position.z = path[1].z;
         path.splice(0, 1);
         positionOnCurrentStep = 0;
         return;
      }

      // calculate the target position
      if (positionOnCurrentStep >= currentStepLength)
         positionOnCurrentStep = currentStepLength;
      var portion = positionOnCurrentStep / currentStepLength;
      card3D.position.x = path[0].x + portion * (path[1].x - path[0].x);
      card3D.position.y = path[0].y + portion * (path[1].y - path[0].y);
      card3D.position.z = path[0].z + portion * (path[1].z - path[0].z);
      
      // adjust the path if the new point will violate our minimum z
      if (card3D.position.z < minimumZ) {
         card3D.position.z = minimumZ;
         path[0] = card3D.position;
         positionOnCurrentStep = 0;
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
