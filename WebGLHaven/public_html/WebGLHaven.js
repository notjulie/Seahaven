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
 * @param {HTML canvas element} canvasElement
 * @returns {WebGLHaven}
 */
function WebGLHaven(canvasElement) {
   /**
    * Our canvas
    * @type Canvas
    */
   this.canvas;
   
   /**
    * Our stateMachine
    * @type StateMachine
    */
   this.stateMachine;

   /**
    * Our card locations
    * @type CardLocations
    */
   this.cardLocations;

   /**
    * Our deck of cards
    * @type DeckOfCards
    */
   this.deckOfCards;

   /**
    * Our renderer
    * @type Renderer
    */
   this.renderer;

   /**
    * Our global instance of the world geometry
    * @type World
    */
   this.world;
   
   Object.defineProperty(this, 'canvas', {
         value: canvasElement
      });

   Object.defineProperty(this, 'world', {
         value: Object.freeze(new World())
      });   

   Object.defineProperty(this, 'cardLocations', {
         value: new CardLocations()
      });

   Object.defineProperty(this, 'deckOfCards', {
         value: new DeckOfCards({
            width: this.world.properties.cardWidth,
            height: this.world.properties.cardHeight,
            cornerRadius: this.world.properties.cardWidth / 20,
            rankHeight: (0.13 / 0.43) * this.world.properties.cardWidth
         })
      });
   
   Object.defineProperty(this, 'renderer', {
         value: new Renderer(this)
      });   
   
   Object.defineProperty(this, 'stateMachine', {
         value: new StateMachine(this)
      });
}

/**
 * Moves all Card3D objects to the current positions and shows them
 * @returns {undefined}
 */
WebGLHaven.prototype.repositionAllCards = function() {
   for (var cardID in CardID.info) {
      var card3D = this.deckOfCards.getCard3D(cardID);
      var locationID = this.cardLocations.getCardLocation(cardID);
      card3D.position.copy(this.world.getCardLocation(locationID));
      card3D.visible = true;
   }
};


function main(webGLHaven) {   
   var scene = webGLHaven.renderer.getScene();

   var texture = new THREE.TextureLoader().load('Sand002_COLOR.jpg');
   texture.repeat.x = 5000;
   texture.repeat.y = 5000;
   texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
   var material = new THREE.MeshBasicMaterial( { map: texture } );
   var groundGeometry = new THREE.PlaneGeometry(1000, 1000);
   groundGeometry.rotateX(-90 * Math.PI / 180);
   //var ground = new THREE.Mesh(groundGeometry, new THREE.MeshMatcapMaterial({color: 0x7f4f00}));
   var ground = new THREE.Mesh(groundGeometry, material);
   ground.position.y = webGLHaven.world.properties.groundY;
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
   webGLHaven.deckOfCards.initialize(function () {
      // add all the cards to the scene, but hidden
      for (var cardID in CardID.info) {
         var card3D = webGLHaven.deckOfCards.getCard3D(cardID);
         card3D.visible = false;
         scene.add(card3D);
      }

      // start a new game
      webGLHaven.stateMachine.setState(new NewGameState());
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
   webGLHaven.renderer.setSpotLightTarget(towers[2].getSpotlightTarget());

   webGLHaven.renderer.start();

}

