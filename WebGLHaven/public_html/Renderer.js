/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/* global THREE, stateMachine, world */

function Renderer(canvas) {
   var requestedCanvasSize;
   var mouse;
   var spotLight;

   const renderer = new THREE.WebGLRenderer({canvas: canvas, antialias: true});
   const raycaster = new THREE.Raycaster();
   
   canvas.onclick = function(event) {
      mouse = new THREE.Vector2();
      mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
      mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
   };
   
   // create our default camera position
   const fov = 75;
   const aspect = 2;  // the canvas default
   const near = 0.1;
   const far = 5;
   const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
   camera.position.copy(world.getDefaultCameraPosition());

   // create our main scene
   const scene = new THREE.Scene();
   {
      var light = new THREE.DirectionalLight(0xFFFFFF, 10);
      light.position.set(0, 1000, 0);
      scene.add(light);
      
      scene.add(new THREE.AmbientLight(0xFFFFFF, 5));
      
      scene.background = new THREE.Color(0x80A0A0);
   }
   
   spotLight = new THREE.SpotLight(0xFF0000, 15);
   scene.add(spotLight);

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
      stateMachine.service(time);

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

      if (mouse) {
         // update the picking ray with the camera and mouse position
         raycaster.setFromCamera(mouse, camera);

         // calculate objects intersecting the picking ray
         var intersects = raycaster.intersectObjects(scene.children, true);

         for (var i = 0; i < intersects.length; i++) {
            if (intersects[i].object.parent.getClickID)
            {
               stateMachine.cardClicked(intersects[i].object.parent.getClickID());
               break;
            }
         }

         mouse = undefined;
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
}
