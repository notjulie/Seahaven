/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE */

function Card3D(clickID) {
   // inherit THREE.Group
   THREE.Group.call(this);
   
   this.getClickID = function() {
      return clickID;
   }
}

Card3D.prototype = Object.assign( Object.create( THREE.Group.prototype ), {

   constructor: Card3D,

} );
