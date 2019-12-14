/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global gameState */

function StateMachine() {
   var currentState = new State();
   var currentTime;
   
   this.cardClicked = function(cardID) {
      gameState.moveToBottomOfColumn(cardID, 0);
      gameState.repositionAll();
   };
   
   this.getTime = function() {
      return currentTime;
   }
   
   this.newGame = function() {
      currentState.newGame();
   };
   
   this.setState = function(state) {
      currentState.exit();
      currentState = state;
      currentState.enter();
   };
   
   this.service = function(time) {
      currentTime = time;
      currentState.service();
   }
}
