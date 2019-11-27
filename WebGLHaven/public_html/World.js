/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


function World() {
    const towerModelNominalHeight = 25.0;
    
    this.getColumnPosition = function(column, row) {
        return {
            x:(column - 5)*0.5,
            y:-row * 0.2,
            z:0.1 * row
        };
    }
    
    this.getGroundY = function() {
        return -1.5;
    }
    
    this.getTowerScale = function() {
        return 0.04;
    }
    
    this.getTowerPosition = function(tower) {
        switch(tower) {
            case 0:
                return {
                    x:-3.4,
                    y:-0.7,
                    z:-0.1
                };
            case 1:
                return {
                    x:-3.5,
                    y:-0.4,
                    z:-0.6
                };
                break;
            case 2:
            case 3:
                var result = this.getTowerPosition(3-tower);
                result.x = -result.x;
                return result;
        }
    }
    
    this.getTowerTop = function(tower) {
        var result = this.getTowerPosition(tower);
        result.y += towerModelNominalHeight * this.getTowerScale();
        return result;
    }
}