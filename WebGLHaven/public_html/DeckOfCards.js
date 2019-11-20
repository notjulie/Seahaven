/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */




function DeckOfCards() {
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

    function createDiamondShape (width, height) {
        var shape = new THREE.Shape();

        shape.moveTo(width/2, 0);
        shape.lineTo(width, height/2);
        shape.lineTo(width/2, height);
        shape.lineTo(0, height/2);

        return shape;
    }
    
    this.createCard3D = function() {
        var cardMesh;
        {
            var cardShape = createCardShape(100, 150, 10);
            var extrudeSettings = { depth: 8, bevelEnabled: true, bevelSegments: 2, steps: 2, bevelSize: 1, bevelThickness: 1 };
            var cardGeometry = new THREE.ExtrudeGeometry( cardShape, extrudeSettings );
            cardGeometry.scale(0.004, 0.004, 0.001);

            var mesh = new THREE.Mesh( cardGeometry, new THREE.MeshPhongMaterial() );
            //scene.add(mesh);
            //cubes[2] = mesh;
            cardMesh = mesh;
         }

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

         var group = new THREE.Group();
         group.add(cardMesh);
         group.add(diamondMesh);
         diamondMesh.position.z=0.001;
         return group;
    }
}

