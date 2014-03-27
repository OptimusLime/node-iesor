// This loads our extension in the iesor variable. 
// It will only load a constructor function, notify.notification().
var iesor = require("../build/Release/iesorworld.node"); // path to our extension
var fs = require('fs');
var gpu = require('./gpu.js');

//setup the test info
var genome = JSON.parse(fs.readFileSync(__dirname + '/jsGenome112.json'));
var testNetwork = gpu.NeatGenomeToByteCode(genome);

var realBody = JSON.parse(fs.readFileSync(__dirname + '/jsBody112.json'));


var tgString = JSON.stringify(testNetwork);

var fs = require('fs');
var iWorld = new iesor.iesorWorld();

//here we load a json genome, and inject it into the world
//var sGenome = fs.readFileSync('./sampleGenome.json');

var s = iWorld.loadBodyFromNetwork(tgString);//sGenome.toString());

var body = JSON.parse(s);
console.log("Generated body: ", body);


//simulate for 1/2 second
var simTimeMS = 500;

//tell the wrold to simulate
var simInfo = iWorld.simulateWorldMS(simTimeMS);

console.log('Sim info: ', JSON.parse(simInfo));

var drawInfo = iWorld.getWorldDrawList();

console.log("Draw it: ", JSON.parse(drawInfo));

var bodyJSON = JSON.parse(iWorld.convertNetworkToBody(tgString));//sGenome.toString());

console.log("Gen nodes: ", bodyJSON.nodes.length, " conns: ", bodyJSON.connections.length);
console.log("Real nodes: ", realBody.nodes.length, " conns: ", realBody.connections.length);

console.log("Gen body: ", bodyJSON.nodes);
console.log("Real body: ", realBody.nodes);

console.log("Gen conn: ", bodyJSON.connections);
console.log("Real conn: ", realBody.connections);

// console.log("Ret val: ", s);

//set up a timer for simulation
//simulate, then ask it for a world description each time step. 


