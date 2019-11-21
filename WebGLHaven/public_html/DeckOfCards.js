/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


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


function DeckOfCards(attributes) {
    // private data
    var rankGeometries = new Array();
    var cardWidth = 0.5;
    var cardHeight = 1.0;
    var cardCornerRadius = 0.01;

    // process the attributes
    if (attributes.width) cardWidth = attributes.width;
    if (attributes.height) cardHeight = attributes.height;
    if (attributes.cornerRadius) cardCornerRadius = attributes.cornerRadius;
    
    // create our card shape... upscale it to a height of 100 so that it
    // renders well, because that seems to matter
    var cardShapeScale = 100.0 / cardHeight;
    var cardShape = createCardShape(cardShapeScale*cardWidth, cardShapeScale*cardHeight, cardShapeScale*cardCornerRadius);

    // create our card geometry... we only need one ever
    var cardGeometry = new THREE.ExtrudeGeometry(
        cardShape,
        { depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1 }
        );
    cardGeometry.scale(1/cardShapeScale, 1/cardShapeScale, 0.001);

    function createDiamondShape (width, height) {
        var shape = new THREE.Shape();

        shape.moveTo(width/2, 0);
        shape.lineTo(width, height/2);
        shape.lineTo(width/2, height);
        shape.lineTo(0, height/2);

        return shape;
    }
    
    function createRankGeometry(font, text) {
	var geometry = new THREE.TextGeometry(text, {
		font: font,
		size: 80,
		height: 5,
		curveSegments: 12,
		bevelEnabled: true,
		bevelThickness: 10,
		bevelSize: 8,
		bevelOffset: 0,
		bevelSegments: 5
	} );
        geometry.scale(0.004, 0.004, 0.001);
        return geometry;
    }
    
    /// <summary>
    /// Loads resources required by the deck of cards, calls the callback
    /// when loading completes.
    /// </summary>
    this.initialize = function(loadCompleteCallback) {
        var fontLoader = new THREE.FontLoader();
        fontLoader.load('gentilis_bold.typeface.json', function(font){    
            rankGeometries[1] = createRankGeometry(font, 'A');
            rankGeometries[2] = createRankGeometry(font, '2');
            rankGeometries[3] = createRankGeometry(font, '3');
            rankGeometries[4] = createRankGeometry(font, '4');
            rankGeometries[5] = createRankGeometry(font, '5');
            rankGeometries[6] = createRankGeometry(font, '6');
            rankGeometries[7] = createRankGeometry(font, '7');
            rankGeometries[8] = createRankGeometry(font, '8');
            rankGeometries[9] = createRankGeometry(font, '9');
            rankGeometries[10] = createRankGeometry(font, '10');
            rankGeometries[11] = createRankGeometry(font, 'J');
            rankGeometries[12] = createRankGeometry(font, 'Q');
            rankGeometries[13] = createRankGeometry(font, 'K');

            loadCompleteCallback();
        });
    }
    
    this.createCard3D = function() {
        var cardMesh = new THREE.Mesh( cardGeometry, new THREE.MeshPhongMaterial() );

        var diamondMesh;
        {
            var diamondShape = createDiamondShape(100, 150);
            var extrudeSettings = { depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1 };
            var diamondGeometry = new THREE.ExtrudeGeometry( diamondShape, extrudeSettings );
            diamondGeometry.scale(0.004, 0.004, 0.001);

            var mesh = new THREE.Mesh( diamondGeometry, new THREE.MeshPhongMaterial({color:0xFF0000}) );
            //scene.add(mesh);
            //cubes[0] = mesh;
            diamondMesh = mesh;
         }
         
         var rankMesh = new THREE.Mesh( rankGeometries[1], new THREE.MeshPhongMaterial({color:0xFF0000}) );
         rankMesh.position.z=0.001;

         var group = new THREE.Group();
         group.add(cardMesh);
         group.add(diamondMesh);
         diamondMesh.position.z=0.001;
         group.add(rankMesh);
         return group;
    }    
}

