//this will partially test our genomeToBody construction logic (build appropriate GPU shaders for the job)
var gpu = require('./gpu.js');
var fs = require('fs');
var ng = JSON.parse(fs.readFileSync(__dirname + '/jsGenome142856.json'));

var shadersSoFar = gpu.NeatGenomeToByteCode(ng);

module.exports = shadersSoFar;

// console.log(shadersSoFar.nodeArrays);
//console.log('Frag shader: \n\n\n');
//console.log(shadersSoFar.fragment);
