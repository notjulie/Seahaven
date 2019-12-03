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
      tower.scale.x = tower.scale.y = tower.scale.z = 0.04;
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

