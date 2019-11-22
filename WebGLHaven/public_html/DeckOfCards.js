/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */



/// <summary>
/// class DeckOfCards
///    Represents a deck of 3D card objects
/// construction attributes:
///    width: width of the card; default 0.5
///    height: height of the card; default 1.0
///    cornerRadius: the corner radius; default 0.01
/// </summary>
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
    cardGeometry.computeBoundingBox();

    
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
    
    /// <summary>
    /// Creates a card Object3D of the given suit and rank
    /// </summary>
    this.createCard3D = function(suit, rank) {
        // Our result is a Group that combines a bunch of things.  Presumably
        // you had already guessed that is what a Group does.
        var group = new THREE.Group();         

        // grab the card dimensions
        var cardSize = new THREE.Vector3();
        cardGeometry.boundingBox.getSize(cardSize);

        // Add the blank card face
        var cardFaceMesh = new THREE.Mesh( cardGeometry, new THREE.MeshPhongMaterial() );
        group.add(cardFaceMesh);
         
        // Add the back of the card... for now I just give the front and back
        // the same geometry, which means that the edge of the card is half
        // front color and half back color.  I'll probably change that.
        var cardBackMesh = new THREE.Mesh( cardGeometry, new THREE.MeshPhongMaterial({color:0x0000FF}) );
        cardBackMesh.position.z = -cardSize.z;
        group.add(cardBackMesh);
        
        // add the suit
        var diamondShape = createDiamondShape(100, 150);
        var extrudeSettings = { depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1 };
        var diamondGeometry = new THREE.ExtrudeGeometry( diamondShape, extrudeSettings );
        diamondGeometry.scale(0.004, 0.004, 0.001);
        var suitMesh = new THREE.Mesh( diamondGeometry, new THREE.MeshPhongMaterial({color:0xFF0000}) );
        group.add(suitMesh);
        suitMesh.position.z=0.001;

        // add the rank
        var rankMesh = new THREE.Mesh(rankGeometries[rank], new THREE.MeshPhongMaterial({color:0xFF0000}) );
        rankMesh.position.z=0.001;
        group.add(rankMesh);

        // done
        return group;
    }    
}

