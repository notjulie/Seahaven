/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


function StateMachine() {
   this.cardClicked = function(cardID) {
      gameState.moveToBottomOfColumn(cardID, 0);
      gameState.repositionAll();
   }
}
