/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global body, THREE, CardID */

/**
 * The global world geometry
 * @type World
 */
const world = new World();

var stateMachine = new StateMachine();
var deckOfCards = new DeckOfCards({
   width: world.cardDimensions.width,
   height: world.cardDimensions.height,
   cornerRadius: world.cardDimensions.width / 20,
   rankHeight: (0.13 / 0.43) * world.cardDimensions.width
});


var cardLocations = new CardLocations();

// create our renderer
const canvas = document.querySelector('#c');
var renderer = new Renderer(canvas);

// add our handler to resize it
body.onresize = function () {
   renderer.resizeCanvas(body.clientWidth, body.clientHeight);
};
body.onresize();

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
   ground.position.y = world.getGroundY();
   scene.add(ground);

   var tableTexture = new THREE.TextureLoader().load('grass.jpg');
   tableTexture.repeat.x = 10;
   tableTexture.repeat.y = 10;
   tableTexture.wrapS = tableTexture.wrapT = THREE.RepeatWrapping;
   var tableMaterial = new THREE.MeshBasicMaterial( { map: tableTexture } );
   var tableWorldGeometry = world.tableBox;
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
      var position = world.getTowerPosition(i);
      towers[i].position.x = position.x;
      towers[i].position.y = position.y;
      towers[i].position.z = position.z;
      towers[i].scale.y = position.height;
      scene.add(towers[i]);
   }
   renderer.setSpotLightTarget(towers[2].getSpotlightTarget());

   renderer.start();

}

main();
