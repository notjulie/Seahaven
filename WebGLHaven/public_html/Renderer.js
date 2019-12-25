/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE */

/**
 * Constructor for class Renderer
 * 
 * @param {WebGLHaven} webGLHaven
 * @returns {Renderer}
 */
function Renderer(webGLHaven) {
   var requestedCanvasSize;
   var spotLight;

   const renderer = new THREE.WebGLRenderer({canvas: webGLHaven.canvas, antialias: true});
   renderer.shadowMap.enabled = true;
   const raycaster = new THREE.Raycaster();
   
   // add our event listeners to the canvas
   webGLHaven.canvas.addEventListener('mousedown', function(event) {
      webGLHaven.stateMachine.onMouseDown(event);
   });
   webGLHaven.canvas.addEventListener('mousemove', function(event) {
      webGLHaven.stateMachine.onMouseMove(event);
   });
   webGLHaven.canvas.addEventListener('mouseup', function(event) {
      webGLHaven.stateMachine.onMouseUp(event);
   });
   
   // create our default camera position
   const fov = 75;
   const aspect = 2;  // the canvas default
   const near = 0.1;
   const far = 5;
   const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
   camera.position.copy(webGLHaven.world.getDefaultCameraPosition());

   // create our main scene
   const scene = new THREE.Scene();
   
   var backgroundLoader = new THREE.TextureLoader();
   backgroundLoader.setCrossOrigin("");

   //var bgTexture = backgroundLoader.load('adventure_arid_background_desert_desolate_dry_dunes_600264.jpg',
   var bgTexture = backgroundLoader.load('Beach-With-Sunset-And-Waves-Of-The-Atlantic-Ocean-Portugal-The-Coast-C11.jpg',
       function ( texture ) {
           //var img = texture.image;
           //bgWidth= img.width;
           //bgHeight = img.height;
           //resize();
       }
   );
   scene.background = bgTexture;
   bgTexture.wrapS = THREE.RepeatWrapping;
   bgTexture.wrapT = THREE.RepeatWrapping;
   bgTexture.repeat.set(0.8, 0.65);
   bgTexture.offset.x = 0;
   bgTexture.offset.y = 0.35;
   
   // add our directional light so that we can have some shadows
   var light = new THREE.DirectionalLight(0xFFFFFF, 5);
   light.position.set(0, 0, 300);
   light.castShadow = true;
   light.shadow.mapSize.width = 1024;
   light.shadow.mapSize.height = 1024;
   scene.add(light);

   // add ambient light so that our shadows aren't too exaggerated
   scene.add(new THREE.AmbientLight(0xFFFFFF, 0.8));
      
   // background color
   //scene.background = new THREE.Color(0x80A0A0);

   // this is a spotlight to highlight a tower that's going to receive a
   // card... I think
   spotLight = new THREE.SpotLight(0xFF0000, 15);
   scene.add(spotLight);

   function pointToRaycaster(x, y) {
      var screenPoint = new THREE.Vector2(
         x / renderer.domElement.clientWidth * 2  - 1,
         1 - y / renderer.domElement.clientHeight * 2
         );
      raycaster.setFromCamera(screenPoint, camera);
   }

   this.setSpotLightTarget = function(target) {
      spotLight.target = target;
      spotLight.position.x = target.position.x;
      spotLight.position.y = target.position.y + 1.5;
      spotLight.position.z = target.position.z + 0.005;
      spotLight.angle *= 0.17;
   };

   function render(time) {
      time *= 0.001;  // convert time to seconds

      // give the state machine its timeslice
      webGLHaven.stateMachine.service(time);

      if (requestedCanvasSize) {
         // update the size of the canvas to fit the window
         renderer.setSize(requestedCanvasSize.width, requestedCanvasSize.height);

         // We want a constant horizontal view angle.  This gives is the distance
         // to the projector.
         const horizontalViewAngle = 120 * Math.PI / 180;
         var l = requestedCanvasSize.width / 2.0 / Math.tan(horizontalViewAngle / 2);

         // then that gives us the vertical viewAngle
         var verticalViewAngle = 2 * Math.atan(requestedCanvasSize.height / 2.0 / l) * 180 / Math.PI;

         // update our camera view... this is not the final form of
         // this... it's just an approximation for now
         camera.fov = verticalViewAngle;
         camera.aspect = requestedCanvasSize.width / requestedCanvasSize.height;
         camera.updateProjectionMatrix();

         // all good now
         requestedCanvasSize = undefined;
      }

      renderer.render(scene, camera);

      requestAnimationFrame(render);
   }
   
   this.start = function() {
      requestAnimationFrame(render);
   };
   
   this.resizeCanvas = function(width, height) {
      requestedCanvasSize = {
         width:width,
         height:height
      };
   };
   
   this.getScene = function() { return scene; };
   
   /**
    * Gets the ID of the card at the given point
    * 
    * @param {number} x x coordinate within the canvas
    * @param {number} y y coordinate within the canvas
    * @returns {string} the CardID string
    */
   this.pointToCard = function(x, y) {
      // update the picking ray with the camera and mouse position
      pointToRaycaster(x, y);

      // calculate objects intersecting the picking ray
      var intersects = raycaster.intersectObjects(scene.children, true);

      // return the first one that intersects that belongs to a Card3D
      for (var i = 0; i < intersects.length; i++)
         if (intersects[i].object.parent.cardID)
            return intersects[i].object.parent.cardID;
   };
   
   /**
    * Gets a ray that goes through the given screen point
    * 
    * @param {number} x x coordinate within the canvas
    * @param {number} y y coordinate within the canvas
    * @returns {Ray} the ray
    */
   this.pointToRay = function(x, y) {
      // update the picking ray with the camera and mouse position
      pointToRaycaster(x, y);
      
      // return its ray
      return raycaster.ray;
   };
}
