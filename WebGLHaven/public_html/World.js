/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global LocationID, THREE */

/**
 * Class that is in charge of calculating global points in the
 * 3D coordinate system
 * 
 * @class
 * @returns {World}
 */
function World() {
   const groundY = -1.0;
   const defaultCameraPosition = new THREE.Vector3(0, 0, 1.9);

   // table dimensions
   const numberOfRowsOnTable = 6;
   const tableAngle = 70;
   const tableHeight = 1.0;
   const tableFrontZ = -0.3;
   const zSpacingOnGround = 0.02;
   
   // aces
   const yDistanceToAces = 1.0;
   const zDistanceToAces = 2.0;
   const aceZSpacing = 0.05;

   /**
    * Object defining world dimensions of a card
    * @type Object
    */
   const cardDimensions = {
      value: Object.freeze({
         width:0.43,
         height:0.65,
         relativeMargin:0.2
      }),
      writable: false,
      enumerable: true,
      configurable: false
   };
   Object.defineProperty(this, 'cardDimensions', cardDimensions);

   /**
    * Bounding box of the table.
    * @type Box3
    */
   const tableBox = {
         value: function () {
            var width =
               10.0 * (1 + this.cardDimensions.relativeMargin)*this.cardDimensions.width +
               0.5*this.cardDimensions.width;

            var result = new THREE.Box3();
            result.min.x = -width/2;
            result.max.x = width/2;

            result.min.y = groundY;
            result.max.y = result.min.y + tableHeight*Math.sin(tableAngle * Math.PI/180);

            result.min.z = tableFrontZ;
            result.max.z = result.min.z + tableHeight*Math.cos(tableAngle * Math.PI/180);
            return Object.freeze(result);
         }.call(this),
         writable: false,
         enumerable: true,
         configurable: false
      };
   Object.defineProperty(this, 'tableBox', tableBox);

   /**
    * Gets the card location associated with a card at a given
    * row and column
    * 
    * @param {Integer} column the column (0-9)
    * @param {Integer} row the row (0-16)
    * @returns {THREE.Vector3} the world position of a card at the position
    * in the given column
    */
   this.getColumnPosition = function (column, row) {
      var table = this.tableBox;
      var tableSize = table.getSize(new THREE.Vector3());

      var x = (
         this.cardDimensions.relativeMargin/2 + 
         (1+this.cardDimensions.relativeMargin)*(column - 5)) * this.cardDimensions.width;

      // see if it's on the table or on the ground
      if (row < numberOfRowsOnTable) {
         return new THREE.Vector3(
            x,
            table.max.y - row * tableSize.y / (numberOfRowsOnTable - 1),
            table.min.z + row * tableSize.z / (numberOfRowsOnTable - 1)
         );         
      } else {
         return new THREE.Vector3(
            x,
            table.min.y,
            table.max.z + (1 + row - numberOfRowsOnTable) * zSpacingOnGround
         );         
      }

   };

   /**
    * Gets the default camera position
    * @returns {THREE.Vector3}
    */
   this.getDefaultCameraPosition = function() {
      return defaultCameraPosition;
   };

   this.getGroundY = function () {
      return groundY;
   };

   /// <summary>
   /// returns the location on the aces for the given card
   /// </summary>
   this.getAcePosition = function(suit, rank) {
      // start with the position of the top of the card on the top of a
      // column straight in front of the ace
      var result = this.getColumnPosition(3 + suit, 0);
      result.y += this.cardDimensions.height;
      result.y += yDistanceToAces;
      
      // move it back in Z
      result.z -= zDistanceToAces;
      result.z += rank*aceZSpacing;
      return result;
   };

   /// <summary>
   /// returns the location at which to place the given Tower3D object
   /// </summary>
   this.getTowerPosition = function (tower) {
      switch (tower) {
         case 0:
            return {
               x: -3.3,
               y: groundY,
               z: -0.3,
               height: 1.0
            };
         case 1:
            return {
               x: -3.3,
               y: groundY,
               z: -0.7,
               height: 2.0
            };
            break;
         case 2:
         case 3:
            var result = this.getTowerPosition(3 - tower);
            result.x = -result.x;
            return result;
      }
   };

   /**
    * returns the location at which to place a card that sits on a tower
    * @param tower {integer} index of tower
    * @returns {Vector3}
    */
   this.getTowerTop = function (tower) {
      var towerPosition = this.getTowerPosition(tower);
      return new THREE.Vector3(
            towerPosition.x - this.cardDimensions.width/2,
            towerPosition.y + towerPosition.height,
            towerPosition.z
            );
   };
   
   /**
    * returns the location associated with the given location ID
    * 
    * @param {string} locationID the location ID
    * @returns {THREE.Vector3} the world coordinates of the location
    */
   this.getCardLocation = function(locationID) {
      var locationInfo = LocationID.info[locationID];
      if (locationInfo.isTower)
         return this.getTowerTop(locationInfo.tower);
      else if (locationInfo.isAce)
         return this.getAcePosition(locationInfo.suit, locationInfo.rank);
      else
         return this.getColumnPosition(locationInfo.column, locationInfo.row);
   };
}

