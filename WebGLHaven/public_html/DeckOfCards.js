/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */



/* global THREE, CardID */

/**
 * class DeckOfCards constructor
 * @class
 * @param {Object} attributes object with optional contruction attributes, including:
 *    width: width of the card; default 0.5
 *    height: height of the card; default 1.0
 *    cornerRadius: the corner radius; default 0.01
 *    font: the path of the JSON file containing the typeface definition
 *    rankHeight: the height of the rank image
 * @returns DeckOfCards
 */
function DeckOfCards(attributes) {
   // private data... attributes
   const cardWidth = attributes.width ? attributes.width : 0.5;
   const cardHeight = attributes.height ? attributes.height : 1.0;
   const cardCornerRadius = attributes.cornerRadius ? attributes.cornerRadius : 0.01;
   const fontName = attributes.font ? attributes.font : 'three/droid_serif_bold.typeface.json';
   const rankHeight = attributes.rankHeight ? attributes.rankHeight : 0.1 * cardHeight;
   const suitHeight = rankHeight;
   const cardFaceColor = 0xF4FCFF;
   const zOffsetForCardBack = 0.001;

   // XY scale for the back of the card relative to the front of the card;
   // setting the back larger than the front gives a nice outline on the front
   const cardBackScale = 1.02;
   
   // private collections
   const cards = new Array();
   const rankGeometries = new Array();
   const suits = [
      {color: 0x000000},
      {color: 0xD00000},
      {color: 0xD00000},
      {color: 0x000000}
   ];
   
   var isLoaded = false;
   Object.defineProperty(this, 'isLoaded', {
      get: function() { return isLoaded; }
   });

   // create our card shape... upscale it to a height of 100 so that it
   // renders well, because that seems to matter
   var cardShapeScale = 100.0 / cardHeight;
   var cardShape = createCardShape(cardShapeScale * cardWidth, cardShapeScale * cardHeight, cardShapeScale * cardCornerRadius);

   // create our card geometry... we only need one ever
   var cardGeometry = new THREE.ExtrudeGeometry(
           cardShape,
           {depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1}
   );
   cardGeometry.scale(1 / cardShapeScale, 1 / cardShapeScale, 0.001);
   cardGeometry.computeBoundingBox();


   function createRankGeometry(font, text) {
      var geometry = new THREE.TextGeometry(text, {
         font: font,
         size: 80,
         height: 10,
         curveSegments:36
      });
      geometry.computeBoundingBox();
      var heightScale = rankHeight / (geometry.boundingBox.max.y - geometry.boundingBox.min.y);
      geometry.scale(heightScale, heightScale, 0.0001);
      geometry.computeBoundingBox();
      return geometry;
   }

   function createSuitGeometry(suitShape) {
      var extrudeSettings = {depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1};
      var suitGeometry = new THREE.ExtrudeGeometry(suitShape, extrudeSettings);
      suitGeometry.computeBoundingBox();
      var heightScale = suitHeight / (suitGeometry.boundingBox.max.y - suitGeometry.boundingBox.min.y);
      suitGeometry.scale(heightScale, heightScale, 0.001);
      suitGeometry.computeBoundingBox();
      return suitGeometry;
   }

   /**
    * Creates a card Object3D for the given card ID
    * 
    * @param {string} cardID the card ID string
    * @returns {Card3D} an Object3D that contains all the different parts that make up a card
    */
   function createCard3D(cardID) {
      var cardInfo = CardID.info[cardID];
      
      // Our result is a Group (which is the base class for Card3D) that combines
      // a bunch of things.  Presumably you had already guessed that is what a
      // Group does.
      var group = new Card3D(cardID);

      // Add the blank card face
      var cardFaceMaterial = new THREE.MeshLambertMaterial({color: cardFaceColor});
      cardFaceMaterial.shadowSide = THREE.DoubleSide;
      var cardFaceMesh = new THREE.Mesh(cardGeometry, cardFaceMaterial);
      cardFaceMesh.cardObject = group;
      group.add(cardFaceMesh);
      cardFaceMesh.receiveShadow = true;

      // Add the back of the card... for now I just give the front and back
      // the same geometry, which means that the edge of the card is half
      // front color and half back color.  I'll probably change that.
      var cardBackMesh = new THREE.Mesh(cardGeometry, new THREE.MeshBasicMaterial({color: 0}));
      cardBackMesh.scale.x = cardBackMesh.scale.y = cardBackScale;
      cardBackMesh.position.x = (1 - cardBackMesh.scale.x) * cardWidth / 2;
      cardBackMesh.position.y = (1 - cardBackMesh.scale.y) * cardHeight / 2;
      cardBackMesh.position.z = -zOffsetForCardBack;
      cardBackMesh.castShadow = true;
      group.add(cardBackMesh);

      // calculate a margin... the minimum would be cornerRadius * (1 - 0.707)
      var margin = 0.6 * cardCornerRadius;
      
      // add the suit
      var suitGeometry = suits[cardInfo.suitIndex].geometry;
      var suitMesh = new THREE.Mesh(suitGeometry, new THREE.MeshBasicMaterial({color: suits[cardInfo.suitIndex].color}));
      group.add(suitMesh);
      suitMesh.position.z = 0.001;
      suitMesh.position.y = cardHeight - margin - suitGeometry.boundingBox.max.y;
      suitMesh.position.x = cardWidth - margin - suitGeometry.boundingBox.max.x;

      // add the rank
      var rankMesh = new THREE.Mesh(rankGeometries[cardInfo.rankIndex], new THREE.MeshBasicMaterial({color: suits[cardInfo.suitIndex].color}));
      rankMesh.position.z = cardGeometry.boundingBox.max.z;
      rankMesh.position.y = cardHeight - rankGeometries[cardInfo.rankIndex].boundingBox.max.y - margin;
      rankMesh.position.x = margin;
      group.add(rankMesh);

      // and add the larger suit image
      const largeSuitContentArea = 0.9;
      var bigSuitMesh = new THREE.Mesh(suitGeometry, new THREE.MeshBasicMaterial({color: suits[cardInfo.suitIndex].color}));
      bigSuitMesh.scale.x = bigSuitMesh.scale.y = 2.0;
      bigSuitMesh.position.z = 0.001;
      bigSuitMesh.position.y = (largeSuitContentArea*cardHeight -  bigSuitMesh.scale.y*suitGeometry.boundingBox.max.y) / 2;
      bigSuitMesh.position.x = (cardWidth -  bigSuitMesh.scale.x*suitGeometry.boundingBox.max.x) / 2;
      group.add(bigSuitMesh);
      group.castShadow = true;
      group.receiveShadow = true;

      // done
      return group;
   };
   
      
   /**
    * Gets the requested card object
    * @param {string} cardID the card ID string
    * @returns {Card3D} the card
    */
   this.getCard3D = function(cardID) {
      return cards[cardID];
   };
   
   // start creating our rank geometries
   const cardRankStrings = {
      1: 'A',
      2: '2',
      3: '3',
      4: '4',
      5: '5',
      6: '6',
      7: '7',
      8: '8',
      9: '9',
      10: '10',
      11: 'J',
      12: 'Q',
      13: 'K'
   };
   var fontLoader = new THREE.FontLoader();
   fontLoader.load(fontName, function (font) {
      for (var rankNumber in cardRankStrings)
         rankGeometries[rankNumber] = createRankGeometry(font, cardRankStrings[rankNumber]);
      for (var cardID in CardID.info)
         cards[cardID] = createCard3D(cardID);
      isLoaded = true;
   });

   // create our suit geometries
   suits[0].geometry = createSuitGeometry(createClubShape(100, 100));
   suits[1].geometry = createSuitGeometry(createDiamondShape(100, 150));
   suits[2].geometry = createSuitGeometry(createHeartShape(100, 100));
   suits[3].geometry = createSuitGeometry(createSpadeShape(100, 100));
}

/**
 * Indicates whether the deck has finished loading
 * @type Boolean
 */
DeckOfCards.prototype.isLoaded = undefined;
