/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */



/* global THREE, CardID */

/// <summary>
/// class DeckOfCards
///    Represents a deck of 3D card objects
/// construction attributes:
///    width: width of the card; default 0.5
///    height: height of the card; default 1.0
///    cornerRadius: the corner radius; default 0.01
///    font: the path of the JSON file containing the typeface definition
///    rankHeight: the height of the rank image
/// </summary>
function DeckOfCards(attributes) {
   // private data... attributes
   const cardWidth = attributes.width ? attributes.width : 0.5;
   const cardHeight = attributes.height ? attributes.height : 1.0;
   const cardCornerRadius = attributes.cornerRadius ? attributes.cornerRadius : 0.01;
   const fontName = attributes.font ? attributes.font : 'three/droid_serif_bold.typeface.json';
   const rankHeight = attributes.rankHeight ? attributes.rankHeight : 0.1 * cardHeight;
   const suitHeight = rankHeight;
   const cardFaceColor = 0xFFFFF0;

   const cards = new Array();

   // private collections
   const rankGeometries = new Array();
   const suits = [
      {color: 0x000000},
      {color: 0xFF0000},
      {color: 0xFF0000},
      {color: 0x000000}
   ];

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
         height: 1
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

   /// <summary>
   /// Creates a card Object3D for the given card ID
   /// </summary>
   function createCard3D(cardID) {
      var cardInfo = CardID.all[cardID];
      
      // Our result is a Group that combines a bunch of things.  Presumably
      // you had already guessed that is what a Group does.
      var group = new Card3D(cardID);

      // Add the blank card face
      var cardFaceMesh = new THREE.Mesh(cardGeometry, new THREE.MeshBasicMaterial({color: cardFaceColor}));
      cardFaceMesh.cardObject = group;
      group.add(cardFaceMesh);

      // Add the back of the card... for now I just give the front and back
      // the same geometry, which means that the edge of the card is half
      // front color and half back color.  I'll probably change that.
      var cardBackMesh = new THREE.Mesh(cardGeometry, new THREE.MeshPhongMaterial({color: 0x7F7FC0}));
      cardBackMesh.scale.x = cardBackMesh.scale.y = 1.015;
      cardBackMesh.position.z = (cardGeometry.boundingBox.min.z - cardGeometry.boundingBox.max.z) / 1000;
      group.add(cardBackMesh);

      // add the suit
      var suitGeometry = suits[cardInfo.suitIndex].geometry;
      var suitMesh = new THREE.Mesh(suitGeometry, new THREE.MeshPhongMaterial({color: suits[cardInfo.suitIndex].color}));
      group.add(suitMesh);
      suitMesh.position.z = 0.001;
      suitMesh.position.y = cardHeight - (1 - 0.707) * cardCornerRadius - suitGeometry.boundingBox.max.y;
      suitMesh.position.x = cardWidth - (1 - 0.707) * cardCornerRadius - suitGeometry.boundingBox.max.x;

      // add the rank
      var rankMesh = new THREE.Mesh(rankGeometries[cardInfo.rankIndex], new THREE.MeshPhongMaterial({color: suits[cardInfo.suitIndex].color}));
      rankMesh.position.z = cardGeometry.boundingBox.max.z;
      rankMesh.position.y = cardHeight - rankGeometries[cardInfo.rankIndex].boundingBox.max.y - (1 - 0.707) * cardCornerRadius;
      rankMesh.position.x = (1 - 0.707) * cardCornerRadius;
      group.add(rankMesh);

      // and add the larger suit image
      var bigSuitMesh = new THREE.Mesh(suitGeometry, new THREE.MeshPhongMaterial({color: suits[cardInfo.suitIndex].color}));
      bigSuitMesh.scale.x = bigSuitMesh.scale.y = 2.0;
      bigSuitMesh.position.z = 0.001;
      bigSuitMesh.position.y = (cardHeight -  bigSuitMesh.scale.y*suitGeometry.boundingBox.max.y) / 2;
      bigSuitMesh.position.x = (cardWidth -  bigSuitMesh.scale.x*suitGeometry.boundingBox.max.x) / 2;
      group.add(bigSuitMesh);

      // done
      return group;
   };
   
   
   /// <summary>
   /// Loads resources required by the deck of cards, calls the callback
   /// when loading completes.
   /// </summary>
   this.initialize = function (loadCompleteCallback) {
      // start creating our rank geometries
      var fontLoader = new THREE.FontLoader();
      fontLoader.load(fontName, function (font) {
         for (var rankNumber in cardRankMnemonics)
            rankGeometries[rankNumber] = createRankGeometry(font, cardRankMnemonics[rankNumber]);
         for (var cardID in CardID.all)
            cards[cardID] = createCard3D(cardID);
         loadCompleteCallback();
      });

      // create our suit geometries
      suits[0].geometry = createSuitGeometry(createClubShape(100, 100));
      suits[1].geometry = createSuitGeometry(createDiamondShape(100, 150));
      suits[2].geometry = createSuitGeometry(createHeartShape(100, 100));
      suits[3].geometry = createSuitGeometry(createSpadeShape(100, 100));
   };
   
   /// <summary>
   /// Gets the requested card object
   /// </summary>
   this.getCard3D = function(cardID) {
      return cards[cardID];
   }
}
