/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

const cardSuitMnemonics = ['C', 'D', 'H', 'S'];

/**
 * Singleton with properties for enumerating and obtaining info about CardIDs
 * 
 * @type Object
 */
const CardID =  {};

// create CardID.info collection
Object.defineProperty(CardID, 'info', {
   value: Object.freeze(function() {
         const cardRankMnemonics = {
            1: 'A',
            2: '2',
            3: '3',
            4: '4',
            5: '5',
            6: '6',
            7: '7',
            8: '8',
            9: '9',
            10: 'T',
            11: 'J',
            12: 'Q',
            13: 'K'
         };

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
      }()),
   writable: false,
   enumerable: true,
   configurable: false
});

// create CardID.all collection
Object.defineProperty(CardID, 'all', {
   value: Object.freeze(function() {
         var result = new Array();
         for (var cardID in CardID.info)
            result.push(cardID);
         return result;
      }()),
   writable: false,
   enumerable: true,
   configurable: false
});

