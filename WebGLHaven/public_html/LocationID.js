/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


function generateColumnLocationIDs() {
   var result = {};
   for (var column=0; column<10; ++column) {
      result[column] = {};
      for (var row=0; row<16; ++row) {
         result[column][row] =  'C' + column + '-' + row;
      }
   }
   return result;
}

function generateTowerLocationIDs() {
   var result = {};
   for (var tower=0; tower<4; ++tower)
      result[tower] = 'T' + tower;
   return result;
}

function generateAllLocationIDs() {
   var result = new Object();

   var columns = generateColumnLocationIDs();
   for (var column=0; column<10; ++column) {
      for (var row=0; row<16; ++row) {
         var id = columns[column][row];
         result[id] = {
            row: row,
            column: column
         };
      }
   }
   
   var towers = generateTowerLocationIDs();
   for (var tower=0; tower<4; ++tower) {
      var id = towers[tower];
      result[id] = {
         isTower: true,
         tower: tower
      };
   }
   
   return result;
}

const LocationID =  {
   columns : generateColumnLocationIDs(),
   towers : generateTowerLocationIDs(),
   all : generateAllLocationIDs()
};
