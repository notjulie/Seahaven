/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

const cardRankMnemonics = {
   1:'A',
   2:'2',
   3:'3',
   4:'4',
   5:'5',
   6:'6',
   7:'7',
   8:'8',
   9:'9',
   10:'10',
   11:'J',
   12:'Q',
   13:'K'
};

const cardSuitMnemonics = ['C', 'D', 'H', 'S'];


function generateAllCardIDs() {
   var result = new Object();
   for (var suitIndex in cardSuitMnemonics) {
      for (var cardRankIndex in cardRankMnemonics) {
         var id = cardRankMnemonics[cardRankIndex] + cardSuitMnemonics[suitIndex];
         result[id] = {
            suitIndex: parseInt(suitIndex),
            rankIndex: parseInt(cardRankIndex)
         };
      }
   }
   return result;
}


const CardID =  {
   all : generateAllCardIDs()
};
