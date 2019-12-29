/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global State, CardID */

/**
 * State that is active during initialization
 * @constructor
 * @augments State
 * @returns {InitializingState}
 */
function InitializingState() {
   // inherit State
   State.call(this);
   
   /**
    * the inherited game object, redeclared here to help intellisense
    * @type WebGLHaven
    */
   this.webGLHaven;
}
InitializingState.prototype = Object.create(State.prototype);

/**
 * Performs periodic actions while the state is active
 * @param {Number} time game elapsed time (in seconds)
 * @returns {undefined}
 */
InitializingState.prototype.service = function(time) {
   if (this.webGLHaven.deckOfCards.isLoaded) {
      // add all the cards to the scene, but hidden
      for (var cardID in CardID.info) {
         var card3D = this.webGLHaven.deckOfCards.getCard3D(cardID);
         card3D.visible = false;
         this.webGLHaven.renderer.getScene().add(card3D);
      }

      // start a new game
      this.webGLHaven.stateMachine.setState(new NewGameState());
   }
};
