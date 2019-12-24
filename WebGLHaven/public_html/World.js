/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global LocationID, THREE */

/**
 * Object that defines the constant characteristics of the world
 * @constructor
 * @returns {WorldProperties}
 */
function WorldProperties() {
}
/**
 * Width of a card in world units
 * @type Number
 */
WorldProperties.prototype.cardWidth = 0.43;
/**
 * Height of a card in world units
 * @type Number
 */
WorldProperties.prototype.cardHeight = 0.65;
/**
 * the spacing between columns relative ot cardWidth
 * @type Number
 */
WorldProperties.prototype.cardRelativeMargin = 0.2;
/**
 * the y coordinate of the ground
 * @type Number
 */
WorldProperties.prototype.groundY = -1.0;



/**
 * Class that is in charge of calculating global points in the
 * 3D coordinate system
 * 
 * @class
 * @param {WorldProperties} properties
 * @returns {World}
 */
function World(properties) {
   // default the properties if we weren't given any
   if (!properties)
      properties = new WorldProperties();
   
   const defaultCameraPosition = new THREE.Vector3(0, 0, 1.9);

   // table dimensions
   const numberOfRowsOnTable = 6;
   const tableAngle = 70;
   const tableHeight = 1.0;
   const tableFrontZ = -0.3;
   const zSpacingOnGround = 0.02;
   const tableLidMargin = 0.05;
   
   // aces
   const yDistanceToAces = 1.0;
   const zDistanceToAces = 2.0;
   const aceZSpacing = 0.05;

   /**
    * Bounding box of the table.
    * @type Box3
    */
   this.tableBox;
   
   /**
    * Plane defining the lid on the table; i.e. the plane that cards
    * being dragged should not sink below.
    * @type Plane
    */
   this.tableLidPlane;
   
   /**
    * Our construction parameters
    * @type WorldProperties
    */
   this.properties;
   
   Object.defineProperty(this, 'properties', {
      value: Object.freeze(properties)
   });

   Object.defineProperty(this, 'tableBox', {
         value: function () {
            var width =
               10.0 * (1 + this.properties.cardRelativeMargin)*this.properties.cardWidth +
               0.5*this.properties.cardWidth;

            var result = new THREE.Box3();
            result.min.x = -width/2;
            result.max.x = width/2;

            result.min.y = this.properties.groundY;
            result.max.y = result.min.y + tableHeight*Math.sin(tableAngle * Math.PI/180);

            result.min.z = tableFrontZ;
            result.max.z = result.min.z + tableHeight*Math.cos(tableAngle * Math.PI/180);
            return Object.freeze(result);
         }.call(this)
      });

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
         this.properties.cardRelativeMargin/2 + 
         (1+this.properties.cardRelativeMargin)*(column - 5)) * this.properties.cardWidth;

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
    * returns the index of the column that contains the given world X
    * coordinate
    * @param {Number} x
    * @returns {Integer}
    */
   this.getColumnForX = function(x) {
      x /= this.properties.cardWidth;
      x -= this.properties.cardRelativeMargin/2;
      x /= (1+this.properties.cardRelativeMargin);
      return Math.floor(x + 5);
   };

   /**
    * Gets the default camera position
    * @returns {THREE.Vector3}
    */
   this.getDefaultCameraPosition = function() {
      return defaultCameraPosition;
   };

   /// <summary>
   /// returns the location on the aces for the given card
   /// </summary>
   this.getAcePosition = function(suit, rank) {
      // start with the position of the top of the card on the top of a
      // column straight in front of the ace
      var result = this.getColumnPosition(3 + suit, 0);
      result.y += this.properties.cardHeight;
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
               y: this.properties.groundY,
               z: -0.3,
               height: 1.0
            };
         case 1:
            return {
               x: -3.3,
               y: this.properties.groundY,
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
            towerPosition.x - this.properties.cardWidth/2,
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
   
   // create our plane that represents the table lid
   Object.defineProperty(this, 'tableLidPlane', {
      value: function() {
         var plane = new THREE.Plane();
         plane.setFromCoplanarPoints(
            this.getColumnPosition(0,0),
            this.getColumnPosition(9,0),
            this.getColumnPosition(1,16),
            );
         plane.translate(new THREE.Vector3(0,this.properties.cardHeight+tableLidMargin,0));
         return plane;
      }.call(this),
      writable: false,
      enumerable: true,
      configurable: false
   });

}

