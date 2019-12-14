/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/* global cardLocations, stateMachine, world, deckOfCards */

function AnimateToAce(cardID, startLocationID, endLocationID) {
   var position = 0;
   var startTime = stateMachine.getTime();
   var startLocation = world.getCardLocation(startLocationID);
   var endLocation = world.getCardLocation(endLocationID);
   var card3D = deckOfCards.getCard3D(cardID);
   var elapsed = 0;
   
   this.service = function() {
      elapsed = stateMachine.getTime() - startTime;
      if (elapsed > 1.0)
         elapsed = 1.0;
      card3D.position.x = startLocation.x + elapsed * (endLocation.x - startLocation.x);
      card3D.position.y = startLocation.y + elapsed * (endLocation.y - startLocation.y);
      card3D.position.z = startLocation.z + elapsed * (endLocation.z - startLocation.z);
   };
   
   this.isFinished = function() {
      return elapsed >= 1.0;
   };
   
   this.canStartNext = function() {
      return elapsed >= 1.0;
   };
}
