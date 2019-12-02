/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE */

/// <summary>
/// Creates a rounded rectangle with the given parameters
/// </summary>
function createCardShape(width, height, cornerRadius) {
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
}

/// <summary>
/// Creates a club shape with the given parameters
/// </summary>
function createClubShape (width, height) {
    var shape = new THREE.Shape();

    var baseWidth = 0.3 * width;
    shape.moveTo(width/2 - baseWidth/2, 0);
    shape.splineThru(
        [
            new THREE.Vector2(width/2.0 - baseWidth/3.0, height/4.0),
            new THREE.Vector2(width/2.0, height/2.0)
        ]
    );
    
    // parameters of the leaves
    const leafWidth = 0.4*width;
    const leafHeight = 0.2*width;
    const branchLength = width/2.0 - leafWidth;

    // left leaf
    shape.splineThru(
        [
            new THREE.Vector2(width/2.0 - leafWidth/3.0 - branchLength, height/2.0-leafHeight),
            new THREE.Vector2(width/2.0 - 2.0*leafWidth/3.0 - branchLength, height/2.0-leafHeight),
            new THREE.Vector2(width/2.0 - leafWidth - branchLength, height/2.0),
            new THREE.Vector2(width/2.0 - 2.0*leafWidth/3.0 - branchLength, height/2.0+leafHeight),
            new THREE.Vector2(width/2.0 - leafWidth/3.0 - branchLength, height/2.0+leafHeight),
            new THREE.Vector2(width/2.0 - branchLength, height/2.0),
            new THREE.Vector2(width/2.0, height/2.0)
        ]
    );
    
    // top leaf
    shape.splineThru(
        [
            new THREE.Vector2(width/2.0-leafHeight, height/2.0 + leafWidth/3.0 + branchLength),
            new THREE.Vector2(width/2.0-leafHeight, height/2.0 + 2.0*leafWidth/3.0 + branchLength),
            new THREE.Vector2(width/2.0, height/2.0+leafWidth + branchLength),
            new THREE.Vector2(width/2.0+leafHeight, height/2.0 + 2.0*leafWidth/3.0 + branchLength),
            new THREE.Vector2(width/2.0+leafHeight, height/2.0 + leafWidth/3.0 + branchLength),
            new THREE.Vector2(width/2.0, height/2.0 + branchLength),
            new THREE.Vector2(width/2.0, height/2.0)
        ]
    );
    
    // right leaf
    shape.splineThru(
        [
            new THREE.Vector2(width/2.0 + leafWidth/3.0 + branchLength, height/2.0+leafHeight),
            new THREE.Vector2(width/2.0 + 2.0*leafWidth/3.0 + branchLength, height/2.0+leafHeight),
            new THREE.Vector2(width/2.0 + leafWidth + branchLength, height/2.0),
            new THREE.Vector2(width/2.0 + 2.0*leafWidth/3.0 + branchLength, height/2.0-leafHeight),
            new THREE.Vector2(width/2.0 + leafWidth/3.0 + branchLength, height/2.0-leafHeight),
            new THREE.Vector2(width/2.0 + branchLength, height/2.0),
            new THREE.Vector2(width/2.0, height/2.0)
        ]
    );
    
    shape.lineTo(width/2 + baseWidth/2, 0);

    return shape;
}


/// <summary>
/// Creates a diamond shape with the given parameters
/// </summary>
function createDiamondShape (width, height) {
    var shape = new THREE.Shape();

    shape.moveTo(width/2, 0);
    shape.lineTo(width, height/2);
    shape.lineTo(width/2, height);
    shape.lineTo(0, height/2);

    return shape;
}


/// <summary>
/// Creates a heart shape with the given parameters
/// </summary>
function createHeartShape (width, height) {
    var shape = new THREE.Shape();

    var heartSpline = [
       {x:0.5, y:0.0},
       {x:0.45, y:0.15},
       {x:0.33, y:0.3},
       {x:0.10, y:0.6},
       {x:0.15, y:0.85},
       {x:0.30, y:0.95},
       {x:0.45, y:0.85},
       {x:0.5, y:0.65}
    ];
    var leftSpline = new Array();
    var rightSpline = new Array();
    for (var i=0; i<heartSpline.length; ++i) {
       leftSpline[i] = new THREE.Vector2(heartSpline[i].x*width, heartSpline[i].y*height);
       rightSpline[heartSpline.length-1-i] = new THREE.Vector2((1.0-heartSpline[i].x)*width, heartSpline[i].y*height);
    }

    shape.moveTo(width/2, 0);
    shape.splineThru(leftSpline);
    shape.splineThru(rightSpline);
    return shape;
}


/// <summary>
/// Creates a spade shape with the given parameters
/// </summary>
function createSpadeShape (width, height) {
    var shape = new THREE.Shape();

    var baseWidth = 0.3 * width;
    shape.moveTo(width/2 - baseWidth/2, 0);
    shape.splineThru(
        [
            new THREE.Vector2(width/2.0 - baseWidth/3.0, height/4.0),
            new THREE.Vector2(width/2.0, height/2.0)
        ]
    );
    
    shape.splineThru(
        [
            new THREE.Vector2(0.3*width, 0.25*height),
            new THREE.Vector2(0.1*width, 0.25*height),
            new THREE.Vector2(0.3*width, 0.6*height),
            new THREE.Vector2(0.5*width, 1.0*height)
        ]
    );
    shape.splineThru(
        [
            new THREE.Vector2(0.7*width, 0.6*height),
            new THREE.Vector2(0.9*width, 0.25*height),
            new THREE.Vector2(0.7*width, 0.25*height),
            new THREE.Vector2(0.5*width, 0.5*height)
        ]
    );
    
    shape.lineTo(width/2 + baseWidth/2, 0);

    return shape;
}
