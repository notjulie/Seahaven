/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


function CardPositioner(attributes) {
    this.moveToColumn = function(column, row, card) {
        card.position.x = (column - 5)*0.5;
        card.position.y = -row * 0.2;
        card.position.z = 0.1 * row;
    }

    this.moveToTower = function(tower, card) {
        card.position.x = (tower - 2)*0.5;
        card.position.y = 0.8;
        card.position.z = -1;
    }
}
