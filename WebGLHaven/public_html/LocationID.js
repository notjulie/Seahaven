/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


function generateAllLocationIDs() {
   var result = new Object();
   for (var column=0; column<10; ++column) {
      for (var row=0; row<16; ++row) {
         var id = 'C' + column + '-' + row;
         result[id] = {
            row: row,
            column: column
         };
      }
   }
   
   for (var tower=0; tower<4; ++tower) {
      var id = 'T' + tower;
      result[id] = {
         isTower: true,
         tower: tower
      };
   }
   
   return result;
}

const LocationID =  {
   all : generateAllLocationIDs()
};
