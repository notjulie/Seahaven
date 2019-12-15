/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE */

function Tower3D() {
   // give ourselves an internal unambiguous this
   var thisTower = this;
   
   // inherit THREE.Group
   THREE.Group.call(this);

   // create an object that represents where we want a spotlight to
   // aim when it aims at us
   var spotlightTarget = new THREE.Group();
   spotlightTarget.position.y = 0.8;
   this.add(spotlightTarget);

   function setTower(tower) {
      // normalize its scale so that the height is 1.0
      var boundingBox = new THREE.Box3().setFromObject(tower);
      tower.scale.x = tower.scale.y = tower.scale.z = 1.0 / (boundingBox.max.y - boundingBox.min.y);

      // and shift it so that the origin is bottom center
      tower.position.y = -boundingBox.min.y * tower.scale.y;
      tower.position.x = -(boundingBox.min.x + boundingBox.max.x)/2 * tower.scale.x;

      // and add it as the only actual thing in this group
      thisTower.add(tower);
   }

   // load
   var loader = new THREE.GLTFLoader();
   loader.load('tower/scene.gltf', function (gltf) {
      setTower.call(this, gltf.scene);
   }, undefined, function (error) {
      console.error(error);
   });
   
   this.getSpotlightTarget = function() {
      return spotlightTarget;
   }
}

Tower3D.prototype = Object.assign( Object.create( THREE.Group.prototype ), {
   constructor: Tower3D
} );

