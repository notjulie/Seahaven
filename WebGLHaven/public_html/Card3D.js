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
   
   // define the cardID property
   Object.defineProperty(this, 'cardID', {
      value: cardID,
      writable: false,
      enumerable: true,
      configurable: false
   });
}


Card3D.prototype = Object.assign( Object.create( THREE.Group.prototype ), {
   constructor: Card3D
} );
