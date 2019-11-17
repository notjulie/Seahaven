/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */




function DeckOfCards() {
}

DeckOfCards.prototype = {
    createCardShape : function(width, height, cornerRadius) {
        var shape = new THREE.Shape();

        shape.moveTo(0, height-cornerRadius);
        shape.arc(cornerRadius, 0, cornerRadius, Math.PI, Math.PI/2, true);
        shape.lineTo(width - cornerRadius, height);
        shape.arc(0, -cornerRadius, cornerRadius, Math.PI/2, 0, true);
        shape.lineTo(width, cornerRadius);
        shape.arc(-cornerRadius, 0, cornerRadius, 0, -Math.PI/2, true);
        shape.lineTo(cornerRadius, 0);
        shape.arc(0, cornerRadius, cornerRadius, -Math.PI/2, -Math.PI, true);

        return shape;
    },

    createDiamondShape : function(width, height) {
        var shape = new THREE.Shape();

        shape.moveTo(width/2, 0);
        shape.lineTo(width, height/2);
        shape.lineTo(width/2, height);
        shape.lineTo(0, height/2);

        return shape;
    }
}
