/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE, CardID */

/**
 * Our main object that aggregates the whole game
 * 
 * @constructor
 * @returns {WebGLHaven}
 */
function WebGLHaven() {
   /**
    * Our global instance of the world geometry
    * @type World
    */
   const world = {
         value: Object.freeze(new World()),
         writable: false,
         enumerable: true,
         configurable: false
      };
   Object.defineProperty(this, 'world', world);   
}

/**
 * 
 * @type WebGLHaven
 */
var webGLHaven = new WebGLHaven();
var stateMachine = new StateMachine();
var deckOfCards = new DeckOfCards({
   width: webGLHaven.world.cardDimensions.width,
   height: webGLHaven.world.cardDimensions.height,
   cornerRadius: webGLHaven.world.cardDimensions.width / 20,
   rankHeight: (0.13 / 0.43) * webGLHaven.world.cardDimensions.width
});


var cardLocations = new CardLocations();

// create our renderer
const canvas = document.querySelector('#c');
var renderer = new Renderer(canvas);


function main() {
   var scene = renderer.getScene();

   var texture = new THREE.TextureLoader().load('Sand002_COLOR.jpg');
   texture.repeat.x = 5000;
   texture.repeat.y = 5000;
   texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
   var material = new THREE.MeshBasicMaterial( { map: texture } );
   var groundGeometry = new THREE.PlaneGeometry(1000, 1000);
   groundGeometry.rotateX(-90 * Math.PI / 180);
   //var ground = new THREE.Mesh(groundGeometry, new THREE.MeshMatcapMaterial({color: 0x7f4f00}));
   var ground = new THREE.Mesh(groundGeometry, material);
   ground.position.y = webGLHaven.world.getGroundY();
   scene.add(ground);

   var tableTexture = new THREE.TextureLoader().load('grass.jpg');
   tableTexture.repeat.x = 10;
   tableTexture.repeat.y = 10;
   tableTexture.wrapS = tableTexture.wrapT = THREE.RepeatWrapping;
   var tableMaterial = new THREE.MeshBasicMaterial( { map: tableTexture } );
   var tableWorldGeometry = webGLHaven.world.tableBox;
   var tableWorldSize = tableWorldGeometry.getSize(new THREE.Vector3());
   var tableGeometry = new THREE.PlaneGeometry(
           tableWorldSize.x,
           Math.sqrt(tableWorldSize.y * tableWorldSize.y + tableWorldSize.z * tableWorldSize.z)
           );
   tableGeometry.computeBoundingBox();
   tableGeometry.translate(0, -tableGeometry.boundingBox.min.y, 0);
   tableGeometry.rotateX(-Math.atan2(tableWorldSize.z, tableWorldSize.y));
   var table = new THREE.Mesh(tableGeometry, tableMaterial);
   table.position.y = tableWorldGeometry.min.y;
   table.position.z = tableWorldGeometry.max.z;
   scene.add(table);


   // initialize our deck of cards; on completion add cards to the scene
   deckOfCards.initialize(function () {
      // add all the cards to the scene, but hidden
      for (var cardID in CardID.info) {
         var card3D = deckOfCards.getCard3D(cardID);
         card3D.visible = false;
         scene.add(card3D);
      }

      // start a new game
      stateMachine.setState(new NewGameState());
   });


   var towers = new Array();
   var loader = new THREE.GLTFLoader();

   for (var i = 0; i < 4; ++i) {
      towers[i] = new Tower3D();
      var position = webGLHaven.world.getTowerPosition(i);
      towers[i].position.x = position.x;
      towers[i].position.y = position.y;
      towers[i].position.z = position.z;
      towers[i].scale.y = position.height;
      scene.add(towers[i]);
   }
   renderer.setSpotLightTarget(towers[2].getSpotlightTarget());

   renderer.start();

}

