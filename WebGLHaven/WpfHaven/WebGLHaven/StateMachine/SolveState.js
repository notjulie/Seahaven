/*
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */

/**
 * Constructor for class SolveState, which handles the solving sequence.
 * 
 * @augments State
 * @constructor {GameIdleState}
 * @param {any} solver The object in charge of solving
 */
function SolveState(solver) {
   // inherit State
   State.call(this);

   var subState = 'unEntered';
   var solvePromise;

   this.enter = function () {
      subState = 'waitSolver';
      solvePromise = solver.solve(this.webGLHaven.cardLocations.toJSON());
      solvePromise.then((data) => {
         subState = 'solved';
         this.webGLHaven.stateMachine.setState(new GameIdleState());
         alert(data);
      }, (error) => {
         subState = 'error';
         this.webGLHaven.stateMachine.setState(new GameIdleState());
         alert(error.message);
      });
   };

   this.service = function (time) {
   };
}
SolveState.prototype = Object.create(State.prototype);
