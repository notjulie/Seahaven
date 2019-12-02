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
   const towerModelNominalHeight = 25.0;
   const cardWidth = 0.43;
   const relativeMarginBetweenCards = 0.2;
   const groundY = -1.0;

   // table dimensions
   const numberOfRowsOnTable = 6;
   const tableAngle = 70;
   const tableHeight = 1.0;
   const tableFrontZ = -0.6;
   const zSpacingOnGround = 0.03;


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

   this.getTowerScale = function () {
      return 0.04;
   };

   this.getTowerPosition = function (tower) {
      switch (tower) {
         case 0:
            return {
               x: -3.4,
               y: groundY,
               z: -0.1
            };
         case 1:
            return {
               x: -3.5,
               y: groundY,
               z: -0.6
            };
            break;
         case 2:
         case 3:
            var result = this.getTowerPosition(3 - tower);
            result.x = -result.x;
            return result;
      }
   };

   this.getTowerTop = function (tower) {
      var result = this.getTowerPosition(tower);
      result.y += towerModelNominalHeight * this.getTowerScale();
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
