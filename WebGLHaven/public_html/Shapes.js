/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


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
