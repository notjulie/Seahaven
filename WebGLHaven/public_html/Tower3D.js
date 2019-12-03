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

   function setTower(tower) {
      // normalize its scale so that the height is 1.0
      var boundingBox = new THREE.Box3().setFromObject(tower);
      tower.scale.x = tower.scale.y = tower.scale.z = 1.0 / (boundingBox.max.y - boundingBox.min.y);
      tower.position.y = -boundingBox.min.y * tower.scale.y;
      tower.add(new THREE.SpotLight(0xFFFFFF, 4));
      thisTower.add(tower);
   }

   // load
   var loader = new THREE.GLTFLoader();
   loader.load('tower/scene.gltf', function (gltf) {
      setTower.call(this, gltf.scene);
   }, undefined, function (error) {
      console.error(error);
   });
}

Tower3D.prototype = Object.assign( Object.create( THREE.Group.prototype ), {
   constructor: Tower3D
} );

