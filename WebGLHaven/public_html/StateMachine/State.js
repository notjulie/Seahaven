/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/**
 * @constructor Base class for StateMachine states
 * @returns {State}
 */
function State() {   
}


/**
 * Reference to the game object
 * @type WebGLHaven
 */
State.prototype.webGLHaven = undefined;

/**
 * Performs the state's entry actions
 * @returns {undefined}
 */
State.prototype.enter = function() {
};

/**
 * Performs the state's exit actions
 * @returns {undefined}
 */
State.prototype.exit = function() {
};

/**
 * Performs periodic actions while in the state
 * @param time the current timestamp (seconds from some arbitrary initial time)
 * @returns {undefined}
 */
State.prototype.service = function(time) {
};

/**
 * Processes a mouse down event
 * @param {MouseEvent} event
 * @returns {undefined}
 */
State.prototype.onMouseDown = function(event) {
};

/**
 * Handles a mouse move event; does nothing by default
 * 
 * @param {MouseEvent} event the event
 * @returns {undefined}
 */
State.prototype.onMouseMove = function(event) {
};

/**
 * Handles a mouse up event; does nothing by default
 * 
 * @param {MouseEvent} event the event
 * @returns {undefined}
 */
State.prototype.onMouseUp = function(event) {
};

/**
 * Processes a request to start a new game
 * @returns {undefined}
 */
State.prototype.newGame = function() {
   this.webGLHaven.stateMachine.setState(new NewGameState());
};

