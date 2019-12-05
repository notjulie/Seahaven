/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global LocationID, THREE */

/// <summary>
/// Class that is in charge of calculating global points in the
/// 3D coordinate system.
/// </summary>
function World() {
   const cardWidth = 0.43;
   const relativeMarginBetweenCards = 0.2;
   const groundY = -1.0;

   // table dimensions
   const numberOfRowsOnTable = 6;
   const tableAngle = 70;
   const tableHeight = 1.0;
   const tableFrontZ = -0.3;
   const zSpacingOnGround = 0.02;


   this.getCardWidth = function() {
      return cardWidth;
   };

   /// <summary>
   /// Gets the card location associated with a card at a given
   /// row and column
   /// </summary>
   this.getColumnPosition = function (column, row) {
      var table = this.getTableGeometry();
      var tableSize = table.getSize(new THREE.Vector3());

      var x = (relativeMarginBetweenCards/2 + (1+relativeMarginBetweenCards)*(column - 5)) * cardWidth;

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

   this.getGroundY = function () {
      return groundY;
   };

   this.getTableGeometry = function () {
      var width = 10.0 * (1 + relativeMarginBetweenCards)*cardWidth + 0.5*cardWidth;
      
      var result = new THREE.Box3();
      result.min.x = -width/2;
      result.max.x = width/2;
      
      result.min.y = groundY;
      result.max.y = result.min.y + tableHeight*Math.sin(tableAngle * Math.PI/180);

      result.min.z = tableFrontZ;
      result.max.z = result.min.z + tableHeight*Math.cos(tableAngle * Math.PI/180);
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

   /// <summary>
   /// returns the location at which to place a card that sits on
   /// a tower
   /// </summary>
   this.getTowerTop = function (tower) {
      var result = this.getTowerPosition(tower);
      result.y += result.height;
      result.x -= cardWidth/2;
      return result;
   };
   
   this.getCardLocation = function(locationID) {
      var locationInfo = LocationID.all[locationID];
      if (locationInfo.isTower)
         return this.getTowerTop(locationInfo.tower);
      else
         return this.getColumnPosition(locationInfo.column, locationInfo.row);
   }
}
