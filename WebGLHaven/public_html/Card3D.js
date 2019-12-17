/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE */


/**
 * Constructor for Card3D, which is an Object3D that represents a card
 * 
 * @param {string} cardID
 * @returns {Card3D}
 */
function Card3D(cardID) {
   // inherit THREE.Group
   THREE.Group.call(this);
   
   this.getClickID = function() {
      return cardID;
   };
}


Card3D.prototype = Object.assign( Object.create( THREE.Group.prototype ), {
   constructor: Card3D
} );
