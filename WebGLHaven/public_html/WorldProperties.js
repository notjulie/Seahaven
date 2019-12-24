/* 
 * Project: WebGLHaven
 * Author: Randy Rasmussen
 * Copyright: None
 * Warranty: None
 */


/**
 * Object that defines the fixed characteristics of the world; basically
 * the construction parameter to class World
 * @constructor
 * @returns {WorldProperties}
 */
function WorldProperties() {
}
/**
 * Width of a card in world units
 * @type Number
 */
WorldProperties.prototype.cardWidth = 0.43;
/**
 * Height of a card in world units
 * @type Number
 */
WorldProperties.prototype.cardHeight = 0.65;
/**
 * the spacing between columns relative ot cardWidth
 * @type Number
 */
WorldProperties.prototype.cardRelativeMargin = 0.2;
/**
 * the y coordinate of the ground
 * @type Number
 */
WorldProperties.prototype.groundY = -1.0;
/**
 * the maximum number of cards we will place on a column
 * @type Number
 */
WorldProperties.prototype.columnRowCount = 16;


