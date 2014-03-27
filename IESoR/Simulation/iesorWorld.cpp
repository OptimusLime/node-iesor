#define _USE_MATH_DEFINES

//different build location for different machines
#ifdef __APPLE__
//define something for Windows
#define HOME_DIR "../../../../../"
#elif __WINDOWS__
#define HOME_DIR "../../../../"
#else
#define HOME_DIR "../../../../"
#endif


#include "iesorWorld.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <JSON/src/writer.h>
#include "IESoR/Network/iesorBody.h"

//including our json info for easy access
using namespace Json;
using namespace std;

//HELPER FUNCTIONS
//TODO: 
//To be moved into a singleton later

// number to be converted to a string
string toString(int number)
{
    ostringstream convert;   // stream used for the conversion
    convert << number;      // insert the textual representation of 'number' in the characters in the stream
    return convert.str();
}

#define SIZE 1024

//todo: Put this in a global singleton for calling! Duh....
string getCurrentDirectory()
{
    char buffer[SIZE];
    char *answer = getcwd(buffer, sizeof(buffer));
    string s_cwd;
    if (answer)
    {
        s_cwd = answer;
        return s_cwd;
    }
    else
        return NULL;
}




//Json::Value parseJSON(std::string inString)
//{
//		//pull in our JSON body plz
//	Json::Value inBody;
//	Json::Reader read;
//	read.parse(inString, inBody, true);
//	return inBody;
//}






struct PhysicsID
{
	public:
		PhysicsID(string pid)
		{
			this->id = pid;
		}
		string ID(){return id;}
	private: 
		string id;
		~PhysicsID()
		{
			this->id = "";
		}
};

class Bone
{
	public:
		Bone(std::string muscleID, b2Joint* joint)
		{
			this->boneID = muscleID;
			this->joint = joint;
		}

		b2Joint* GetJoint(){return this->joint;}
		std::string ID(){return this->boneID;}

	private:
		std::string boneID;
		b2Joint* joint;
};

class Muscle
{
public:
	Muscle(std::string muscleID, b2Joint* joint, double phase, double amplitude)
	{
		//store the id, joint, phase, and amplitude -- pretty simple!
		this->muscleID = muscleID;
		this->joint = joint;
		this->phase = phase;
		this->amplitude = amplitude;

		//pull the original length from our distance joint
		this->oLength = ((b2DistanceJoint*)joint)->GetLength();
	}

	//getters to retrieve inner variables
	std::string ID(){return this->muscleID;}
	b2Joint* GetJoint(){return this->joint;}
	double GetOriginalLength(){return this->oLength;}
	double GetPhase(){return this->phase;}
	double GetAmplitude(){return this->amplitude;}

private:
	std::string muscleID;
	double oLength;
	double phase;
	double amplitude;
	b2Joint* joint;
};


static int staticBodyCount = 0;

std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	throw(20);
}

b2Body* IESoRWorld::addBodyToWorld(string bodyID, b2BodyDef* bodyDef)
{
	//add body to world
	b2Body* body = this->world->CreateBody(bodyDef);
	
	//assign the bodyID to a new struct object
	PhysicsID* pid = new PhysicsID(bodyID);
	
	//track all the struct objects
	this->bodyList.push_back(pid);

	//track all body objects
	this->bodyMap[bodyID] = body;
	
	//identify body with user data
	body->SetUserData(pid);

	return body;
}

PhysicsID* IESoRWorld::createShapeID()
{
	//We just keep a static int of all the shapes for all identification
	int bcnt = staticBodyCount++;

	//Track the shape string
	PhysicsID* shapeID = new PhysicsID(toString(bcnt));

	//Save our shape to the list of shapes
	this->shapeList.push_back(shapeID);

	return shapeID;
}

void setShapeID(b2Fixture* fix, PhysicsID* shapeID)
{
	fix->SetUserData(shapeID);
}

b2Fixture* IESoRWorld::addShapeToBody(b2Body*body, b2Shape* shape, float density)
{
	//create physics id based on global shape count
	PhysicsID* pid = createShapeID();

	//add the fixture to our body, 
	b2Fixture* fix = body->CreateFixture(shape, density);

	//Attach this info to the shape for identification
	setShapeID(fix, pid);

	//send back created fixture
	return fix;
}

b2Fixture* IESoRWorld::addShapeToBody(b2Body* body, b2FixtureDef* fixDef)
{
	//create physics id based on global shape count
	PhysicsID* pid = createShapeID();

	//add the fixture to our body using the definition
	b2Fixture* fix = body->CreateFixture(fixDef);

	//Attach this info to the shape for identification
	setShapeID(fix, pid);

	//send back created fixture
	return fix;
}

//laod object from data folder -- simple!!
std::string IESoRWorld::loadDataFile(std::string dataName)
{
	//std::string filePath = "../../../../Physics/Data/" + dataName;
	std::string filePath = HOME_DIR;
    filePath += "Physics/Data/" + dataName;
    
    //printf("Current directory: %s", getCurrentDirectory().c_str());
    
	return get_file_contents(filePath.c_str());
}


IESoRWorld::IESoRWorld()
{
	this->interpolation =0;
	accumulator = 0;
	desiredFPS = 60;
	simulationRate = 1.0 / desiredFPS;
	radians = 0;

	//if we need to simulate a certain amount of time, we refuse above a certain amount
	maxFrameSize = .35;


	//this->bodyList = new vector<PhysicsID*>();
	//this->shapeList = new vector<PhysicsID*>();
	//this->bodyList = new Json::Value(Json::arrayValue);
	//this->shapeList = new Json::Value(Json::arrayValue);

	currentSize = b2Vec2(200, 150);


	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -15.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	this->world = new b2World(gravity);
	this->world->SetAutoClearForces(false);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0.0f, -18.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = this->addBodyToWorld("ground", &groundBodyDef);//this->world->CreateBody(&groundBodyDef);

	//b2Body* groundBody = this->world->CreateBody(&groundBodyDef);


	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(350.0, 10.0);

	// Add the ground fixture to the ground body.
	this->addShapeToBody(groundBody, &groundBox, 0.0f);
	//groundBody->CreateFixture(&groundBox, 0.0f);

	// b2Vec2 canvas(200, 150);
	//this->loadBodyIntoWorld(inBody, canvas);


	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 24.0f);

	//add body to world using definition
	// b2Body* body = this->addBodyToWorld("rect1", &bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(5.0f, 5.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.restitution = .5;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	// this->addShapeToBody(body, &fixtureDef);
	
	// Define the circle body. We set its position and call the body factory.
	b2BodyDef cDef;
	cDef.type = b2_dynamicBody;
	cDef.position.Set(10.0f, 24.0f);

	//add body to world using definition
	// body = this->addBodyToWorld("circleTest", &cDef);

	// Define another box shape for our dynamic body.
	b2CircleShape dCircle;
	dCircle.m_radius = 5.0;

	// Define the dynamic body fixture.
	b2FixtureDef circleDef;
	circleDef.shape = &dCircle;
	circleDef.restitution = .5;

	// Set the box density to be non-zero, so it will be dynamic.
	circleDef.density = 1.0f;

	// Override the default friction.
	circleDef.friction = 0.3f;

	// Add the shape to the body.
	// this->addShapeToBody(body, &circleDef);



	//Add some forces!
	//body->ApplyAngularImpulse(.4, true);
	
	// body->ApplyTorque(150, true);
	b2Vec2 pulse(70, 0);
	b2Vec2 o(0,3);
	// body->ApplyLinearImpulse(pulse, o, true);

}

//converts a b2Vec point into a json object
Json::Value positionToJSONValue(b2Vec2 vec)
{
	Json::Value pos;
	pos["x"] = vec.x;
	pos["y"] = vec.y;
	return pos;
}

//converts an array of points into a json array
Json::Value listOfPoints(b2Vec2* points, int length)
{
	Json::Value pArray(Json::arrayValue);

	for(int i=0;i<length;i++)
	{
		pArray[i] = positionToJSONValue(points[i]);
	}

	return pArray;
}
string IESoRWorld::worldDrawList()
{
	//This will be written to json
	//fastwriter is not human readable --> compact
	//exactly what we want to sent to our html files
	Json::FastWriter* writer = new Json::FastWriter();
	Json::Value root = jsonWorldDrawList();
	return writer->write(root);
}

Json::Value IESoRWorld::jsonWorldDrawList()
{
	//root is what we'll be sending back with all the shapes (stored in shapes!)
	Json::Value root;
	Json::Value shapes;

	//we'll loop through all required body information
	b2Body * B = this->world->GetBodyList();

	while(B != NULL)
	{
		if(B->GetUserData())
		{
			//fetch our body identifier
			PhysicsID* pid = static_cast<PhysicsID*>(B->GetUserData());//*((string*)B->GetUserData());
			std::string bodyID = pid->ID();
			
			//we must get all our shapes
			b2Fixture* F = B->GetFixtureList();

			//cycle through the shapes
			while(F != NULL)
			{
				//Hold our shape drawing information
				Json::Value singleShape;

				switch (F->GetType())
				{
				case b2Shape::e_circle:
					{
						b2CircleShape* circle = (b2CircleShape*) F->GetShape();
						/* Do stuff with a circle shape */
						Json::Value center = positionToJSONValue(circle->m_p);
						Json::Value radius = circle->m_radius;
						singleShape["type"] = "Circle";
						singleShape["bodyOffset"] = positionToJSONValue(B->GetPosition());
						singleShape["rotation"] = B->GetAngle();
						singleShape["center"] = center;
						singleShape["radius"] = circle->m_radius;
						singleShape["color"] = "#369";
					}
					break;
				case b2Shape::e_polygon:
					{
						b2PolygonShape* poly = (b2PolygonShape*) F->GetShape();
						/* Do stuff with a polygon shape */

						Json::Value points = listOfPoints(poly->m_vertices, poly->m_count);
						singleShape["type"] = "Polygon";
						singleShape["points"] = points;
						singleShape["bodyOffset"] = positionToJSONValue(B->GetPosition());
						singleShape["rotation"] = B->GetAngle();
						singleShape["color"] = "#38F";

					}
					break;
				}

				//Each shape is the unique combination of 
				pid = static_cast<PhysicsID*>(F->GetUserData());//*((string*)B->GetUserData());
				
				string shapeID = pid->ID();// *((string*)F->GetUserData());

				string fullID = bodyID + "_" + shapeID;
				shapes[fullID] = singleShape;

				F = F->GetNext();
			}
		}

		B = B->GetNext();
	}
	//we set our shapes using the body loops
	root["shapes"] = shapes;

	//we now need to process all of our joints as well
	b2Joint * J = this->world->GetJointList();

	Json::Value joints;

	while(J != NULL)
	{
		if(J->GetUserData())
		{
			//fetch our body identifier
			Bone* jid = static_cast<Bone*>(J->GetUserData());

			//we grab the joint identifier
			std::string bodyID = jid->ID();
		
			//Hold our joint drawing information
			Json::Value singleJoint;

			//we should use the body identifiers
			//but they both need to exist for this to be valid
			if(J->GetBodyA()->GetUserData() && J->GetBodyB()->GetUserData())
			{
				//we need to know what bodies are connected
				PhysicsID* pid = static_cast<PhysicsID*>(J->GetBodyA()->GetUserData());
				singleJoint["sourceID"] =  pid->ID();

				pid = static_cast<PhysicsID*>( J->GetBodyB()->GetUserData());
				singleJoint["targetID"] =  pid->ID();

				//now we need more drawing informtion regarding offsets on the body
				
				//get the anchor relative to body A
				//set that in our json object
				singleJoint["sourceOffset"] = positionToJSONValue(J->GetAnchorA());
				//set the same for our second object
				singleJoint["targetOffset"] = positionToJSONValue(J->GetAnchorB());
			}
			
			//set our joint object using the id, and json values
			joints[bodyID] = singleJoint;
		}

		//loop to the next object
		J = J->GetNext();
	}

	root["joints"] = joints;

	return root;
}

double speedFrameMultiplier = 4.0;

double IESoRWorld::modifyFrameTime(double mseconds)
{
	//return multiplier * mseconds / 1000 == seconds
	return (mseconds*speedFrameMultiplier)/1000.f;
}

double IESoRWorld::MaxFrameSizeMS()
{
	return maxFrameSize*1000.0/speedFrameMultiplier;
}

//pass in the amount of time you want to simulate
//in real time, this is the delta time between called to update
//in simulation time, this can be any amount -- but this function won't return until all updates are processed
//so it holds up the thread
int IESoRWorld::updateWorld(double msUpdate)
{
	//we'll keep track of the number of times we simulated during this update
    int stepCount = 0;

    //# of seconds since last time
    double frameTime =  modifyFrameTime(msUpdate);

    //maximum frame time, to prevent what is called the spiral of death
    if (frameTime > maxFrameSize)
        frameTime = maxFrameSize;

    //we accumulate all the time we haven't rendered things in
    this->accumulator += frameTime;

	//as long as we have a chunk of time to simulate, we need to do the simulation
    while (accumulator >= simulationRate)
    {
		//how many times did we run this loop, we shoudl keep track --
		//that's the number of times we ran the physics engine
        stepCount++;

        //move the muscles quicker using this toggle
        float speedup = 3;

		//we loop through all our muscles, and update the lengths associated with the connectionsj
		for (std::vector<Muscle*>::iterator it = muscleList.begin() ; it != muscleList.end(); ++it)
		{
			//grab our muscle pointer from the list
			Muscle* muscle = *it;
			
			//get our distance joint -- holder of physical joint info
			b2DistanceJoint* dJoint = (b2DistanceJoint*)muscle->GetJoint();

			//Javascript version
			//muscle.SetLength(muscle.m_length + muscle.amplitude/this.scale*Math.cos(rad + muscle.phase*2*Math.PI));
			//double lengthCalc = (dJoint->GetLength() + muscle->GetAmplitude()*cos(radians + muscle->GetPhase() * 2 * M_PI));

			//fetch the original length of the distance joint, and add some fraction of that amount to the length
			//depending on the current location in the muscle cycle
            double lengthCalc = (1.0 + muscle->GetAmplitude() * cos(radians + muscle->GetPhase() * 2 * M_PI)) * muscle->GetOriginalLength();
			
			//we set our length as the calculate value
			dJoint->SetLength(lengthCalc);
		}

        //step the physics world
        this->world->Step(
            this->simulationRate   //frame-rate
            , 10       //velocity iterations
            , 10       //position iterations
        );

		//manually clear forces when doing fixed time steps, 
		//NOTE: that we disabled auto clear after step inside of the b2World
		this->world->ClearForces();

        //increment the radians for the muscles
        radians += speedup * this->simulationRate;

        //decrement the accumulator - we ran a chunk just now!
        accumulator -= this->simulationRate;
    }
            
	//interpolation is basically a measure of how far into the next step we should have simulated
	//it's meant to ease the drawing stages 
	//(you linearly interpolate the last frame and the current frame using this value)
    this->interpolation = accumulator / this->simulationRate;
    // printf("Interp: %f , %f, final- %f \n", accumulator, this->simulationRate, this->interpolation);

	//how many times did we run the simulator?
    return stepCount;
}


std::vector<Json::Value> IESoRWorld::getBodyEntities(Json::Value& inBody, b2Vec2 widthHeight, std::map<std::string, double>& initMorph)
{
	//We're going to first convert our body into a collection of physics objects that need to be added to the environment
	std::vector<Json::Value> entityVector;

	//We'll be using our window width/height for adjusting relative body location
	//remember that the CPPN is going to paint information onto a grid, we need to interpret that information according the appropriate sizes
	double canvasWidth = widthHeight.x;
	double canvasHeight = widthHeight.y;

	printf(" Widht %f, height: %f \n", canvasWidth, canvasHeight);

	//Grab an array of nodes from our json object
	Json::Value oNodes = inBody["nodes"];

	//this is the starting body identifier for each node (every node has a unique ID for the source connections)
	//so node 5 is always node 5, but if you already have objects in the physical world
	//object 5 != node 5, so the number of objects + nodeID = true nodeID
	int bodyCount = this->world->GetBodyCount();
	int bodyID = bodyCount;

	bool useLEO = inBody["useLEO"].asBool();

	//manipulated values of each node, adjusted for screen coorindates
    double xScaled, yScaled;

	//Adjusting the total size of the object according to window size
	double divideForMax = 2.2;
    double divideForMaxHeight = 2.5f;

	//Part of a calculation for determinig the x,y values of each node
    double maxAllowedWidth = canvasWidth / divideForMax;
    double maxAllowedHeight = canvasHeight / divideForMaxHeight;

	//starting values, these will be adjusted
    double minX = canvasWidth; double maxX = 0.0f;
    double minY = canvasHeight; double maxY = 0.0f;

	//We loop through all of our nodes
	for (int i=0; i < oNodes.size(); i++)
	{
		//for each our nodes, we have a location in the world
		Json::Value nodeLocation = oNodes[i];

		//We pull 
		double nodeX = nodeLocation["x"].asDouble();
        double nodeY = nodeLocation["y"].asDouble();

		//here we actually modify the x and y values to fit into a certain sized box depending on the initial screen size/physics world size
        xScaled = (nodeX)/divideForMax* maxAllowedWidth;
        yScaled = (1.0f - nodeY) / divideForMaxHeight* maxAllowedHeight;

        //FOR each node, we make a body with certain properties, then increment count
		entityVector.push_back(Entity::CircleEntity(toString(bodyID), xScaled, yScaled, maxAllowedWidth/35.0, 0));
		
		///what's the min/max xValues we've seen? We can determine how wide the creature is
        minX = min(minX, xScaled);
        maxX = max(maxX, xScaled);

		///what's the min/max y values we've seen? We can determine how tall the creature is
        minY = min(minY, yScaled);
        maxY = max(maxY, yScaled);

        //need to increment the body id so we don't overwrite previous object
        bodyID++;
    }
	
	//We use movex, movey to center our creature in world space after creation 
    double moveX = (maxX - minX) / 2.0;
    double moveY = (maxY - minY) / 2.0;

    //Need to move x,y coordinates for entities
    //no one should get an unfair advantage being at a certain screen location
	for (std::vector<Json::Value>::iterator it = entityVector.begin() ; it != entityVector.end(); ++it)
	{
		Json::Value e = *it;
		e["x"] = (e["x"].asDouble() - minX) + canvasWidth/2 - moveX;
		e["y"] = (e["y"].asDouble() - minY) + canvasHeight/2 - moveY;
	}
	
	//We now have everything we need to identify our initial morphology
	//we can see the inital width/height of the object
	//as well as the offset (startx, starty) of the object
	initMorph["width"] = maxX - minX;
	initMorph["height"] = maxY - minY;
	initMorph["startX"] = minX;
	initMorph["startY"] = minY;
	initMorph["totalNodes"] = oNodes.size();

	return entityVector;
}


void IESoRWorld::setBody(Json::Value& entity)
{
	//we create a generic body def, to hold our body
	b2BodyDef* bodyDef = new b2BodyDef();
	
	//we are either adding "ground" (static) or we're adding everything else (dynamic)
	if (entity["bodyID"].asString() == "ground")
    {
		bodyDef->type = b2_staticBody;
    }
    else
    {
		bodyDef->type = b2_dynamicBody;
    }

	//we set the position to the intial x,y coordinate
	bodyDef->position.Set(entity["x"].asDouble(), entity["y"].asDouble());

	//we set physical properties of this body
	//we don't want any rotation on the nodes (they would slide when moving)
	bodyDef->linearDamping = 1.1f;
	
    //bodies have fixed rotation in iesor -- no move please k thx
	bodyDef->fixedRotation = true;

	if(!entity["angle"].isNull())
		bodyDef->angle = entity["angle"].asDouble();

	//we add our body to the world, and register it's identifying information
	//that's useful for drawing
	b2Body* body = this->addBodyToWorld(entity["bodyID"].asString(), bodyDef);
		
	//we check if we have a radius (that makes us a circle entity)
	if (!entity["radius"].isNull())
    {
		//this filter prevents circle nodes from colliding on our bodies
		b2Filter* filter = new b2Filter();
		filter->categoryBits = 0x0002;
		filter->maskBits = 0x0001;
		filter->groupIndex = 0;

		//we create a fixture def for our new shape
		b2FixtureDef fixture;

		//We need high density objects so they don't float!
		fixture.density = 25.0;
		//lots of friction please!
		fixture.friction = 1.0;
		//less bouncing, more staying
		fixture.restitution = .1;

		//we're a circle shape, make it so
		b2CircleShape node;
		
		//and we establish our radius
		node.m_radius = entity["radius"].asDouble();

		//attach the shape to our fixtureDef
		fixture.shape = &node;
		fixture.filter = *filter;

		//add this fixture to our body (and track it in this class)
		this->addShapeToBody(body, &fixture);
    }
	//now we're checking if we're a polygonal shape
	else if (!entity["polyPoints"].isNull())
    {
		//we know how many points we need to add 
		int pointCount = entity["polyPoints"].size();

		//so we create an array to hold those points in a form Box2D undertstands
		b2Vec2* polyPoints = new b2Vec2[pointCount];
		//loop through our json points
		for (int j = 0; j < pointCount; j++)
        {
			//pull the object from our json array
			Json::Value point = entity["polyPoints"][j];

			//convert to box2d coords
			b2Vec2 vec(point["x"].asDouble(), point["y"].asDouble());

			//set our coordinate inside the array
			polyPoints[j] = vec;
		}

		//create our polygon shape
		b2PolygonShape poly;

		//set our box2d points as the polygonal shape
		poly.Set(polyPoints, pointCount);
	
		//defalult density to 0, and add the sucker
		this->addShapeToBody(body, &poly, 0.0);
    }
    else
    {
		//we're a rectangle entity! 
		b2PolygonShape poly;

		//we've got a box, we simply put in the half height/ hafl width info for our polygon
		//it does the rest
		poly.SetAsBox(entity["halfWidth"].asDouble(),entity["halfHeight"].asDouble());

		//defalult density to 0, and add the sucker
		this->addShapeToBody(body, &poly, 0.0);
    }

    //we've created our body and our shape, and it's all registered in the class
}

//pretty simple, we loop through and call the set body function that does all the work
void IESoRWorld::setBodies(vector<Json::Value>* entities)
{
	//look through our vector of entities, and add each entity to the world
	for (std::vector<Json::Value>::iterator it = entities->begin() ; it != entities->end(); ++it)
	{
		Json::Value e = *it;
		string bodyID = e["bodyID"].asString();

		this->setBody(e);
	}
}


Bone* IESoRWorld::addDistanceJoint(std::string sourceID, std::string targetID, Json::Value props)
{
	//we're connecting two body objects together
	//they should be defined by their IDs in our bodymap
	b2Body* body1 = this->bodyMap[sourceID];
	b2Body* body2 = this->bodyMap[targetID];

	//Create a basic distance joint
	b2DistanceJointDef* joint = new b2DistanceJointDef();

	//initialize the joints where they're attached at the center of the objects
	joint->Initialize(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());

	//check if we defined frequencyHz value, if so fetch it from json
	//this has to do with how the distance joint responds to stretching
	if(!props["frequencyHz"].isNull())
	{
		joint->frequencyHz = props["frequencyHz"].asDouble();
	}

	//check if we defined dampingRatio value, if so fetch it from json -- check docs for dampingRatio usage
	if(!props["dampingRatio"].isNull())
	{
		joint->dampingRatio = props["dampingRatio"].asDouble();
	}

	//let's use our definition to make a real live joint!
	b2Joint* wJoint = world->CreateJoint(joint);

	//we identify our bones by the count in the list
	Bone* bone = new Bone(toString(boneList.size()), wJoint);

	//we push our joint into a list of joints created
	boneList.push_back(bone);

	//we store this distance joint info inside of the joint!
	wJoint->SetUserData(bone);

	//finished, return our bone object created
    return bone;
}


Muscle* IESoRWorld::addMuscleJoint(std::string sourceID, std::string targetID, Json::Value props)
{
	//we add a standard distance joint
	Bone* addedJoint = addDistanceJoint(sourceID, targetID, props);

	//But our muscles have phase and amplitude, which adjust length during updates
	double phase = 0;

	//check if we defined phase value, if so fetch it from json
	if(!props["phase"].isNull())
	{
		phase = props["phase"].asDouble();
	}

	//amplitude is how much change each muscles exhibits
	double amplitude = 1;

	//check if it's in our props object
	if(!props["amplitude"].isNull())
	{
		phase = props["amplitude"].asDouble();
	}

	//muscle is just a container for our 
	Muscle* ms = new Muscle(toString(this->muscleList.size()), addedJoint->GetJoint(), amplitude, phase);

	//track the muscle objects this way, pretty easy
	this->muscleList.push_back(ms);

	//send back the joint we added to physical world
	return ms;
}

std::map<std::string, double> IESoRWorld::loadBodyIntoWorld(Json::Value& inBody)
{
	//we use bodycount as a way to identify each physical object being added from the body information
	long oBodyCount = (long)world->GetBodyCount();

	//This mapping will be what we return from the function while adding the body
	//we measure the morphological properties
    map<string, double> morphology;

    //take our current size as the size to add entities into the space
    b2Vec2 widthHeight = currentSize;

    printf("Body prep: %i, nodeCount: %i, connCount: %i \n", 
    	oBodyCount, 
    	inBody["nodes"].size(), 
    	inBody["connections"].size());

	//here we use our body information to create all the necessary body additions to the world
    vector<Json::Value> entities = getBodyEntities(inBody, widthHeight, morphology);

	//we'll quickly map and ID into a json point value
	map<string, Json::Value> entityMap;
    int cnt=0;
	//this was we can access locations by the bodyID while determining connection distance
	for (std::vector<Json::Value>::iterator it = entities.begin() ; it != entities.end(); ++it)
	{
		Json::Value e = *it;
		printf("bID %s \n", e["bodyID"].asString().c_str());
		entityMap[e["bodyID"].asString()] = e;
		cnt++;
	}

    printf("Body Map prepared: %i \n", cnt);


    //push our bodies into the system so that our joints have bodies to connect to
	this->setBodies(&entities);

	//Did we use LEO to calculate our body information -- affects the indexing procedure
	bool useLEO = inBody["useLEO"].asBool();
            
	//this is the json connection array
	Json::Value connections = inBody["connections"];

	//this determines if we should be a fixed connection (bone) or a moving connection (muscle)
    double amplitudeCutoff = .2;

	//we like to measure the total connection distance as part of our mass calculation
	//mass = # of nodes + length of connections
	double connectionDistanceSum = 0.0;

    printf("Conns to eval: %i \n", connections.size());
    int connSize = connections.size();

	//loop through all our connections
	for(int i=0; i < connSize; i++)
	{
		printf("Conn %i start \n", i);
		//grab connection from our array
		Json::Value connectionObject = connections[i];
		Json::Value cppnOutputs = connectionObject["cppnOutputs"];

		Json::StyledWriter write;

    	printf("Conn to eval: %s sID: %i, tID: %i \n", 
    		write.write(connectionObject).c_str(),
    		connectionObject["sourceID"].asInt64(),
    		connectionObject["targetID"].asInt64());

		long sID = connectionObject["sourceID"].asInt64();//atoi(connectionObject["sourceID"].asString().c_str());
		long tID = connectionObject["targetID"].asInt64();//atoi(connectionObject["targetID"].asString().c_str());

    	printf("Conn to eval: %i - %i \n", sID, tID);


		//To identify a given object in the physical world, we need to start with the current body count, and add the source id number
		//This allows us to add multiple bodies to the same world (though this is recommended against, since it's easier to create separate worlds)
		string sourceID = toString(oBodyCount + sID);
		string targetID = toString(oBodyCount + tID);
               
		 //we ignore connections that loop to themselves 
        if (sID == tID)
        {
            continue;
        }
               

        try
        {
			//depending on whether LEO was used or not, dictates what outputs we'll be looking at for what
            int phaseIx = (useLEO ? 2 : 1);
            int ampIx = (useLEO ? 3 : 2);

			//sample the amplitude output to know what's up -- we convert from [-1,1] to [0,1]
			double amp = (cppnOutputs[ampIx].asDouble() + 1) / 2;

			// printf("Ent source: %s \n, ent targ: %s \n", 
			// 	write.write(entityMap[sourceID]).c_str(),
			// 	write.write(entityMap[targetID]).c_str());

			//what's the distance been the source (x,y) and distance (x,y) -- that's the length of our connection
			double connectionDistance = sqrt(
				pow(entityMap[sourceID]["x"].asDouble() - entityMap[targetID]["x"].asDouble(), 2) 
				+ pow(entityMap[sourceID]["y"].asDouble() - entityMap[targetID]["y"].asDouble(), 2));

			//add the connection distance to our sum
            connectionDistanceSum += connectionDistance;

			//this will hold our custom props for the distance/muscle joints
			Json::Value props;

			printf("Amp: %d, cutoff: %d \n", amp, amplitudeCutoff);

            if (amp < amplitudeCutoff)
				this->addDistanceJoint(sourceID, targetID, props);//, {frequencyHz: 3, dampingRatio:.3});
            else{				

				//these are our hardcoded spring behaviors, could be altered by CPPN, but that seems risky
				props["frequencyHz"] = 3;
				props["dampingRatio"] = .3;

				//Phase/Amplitude set by our cppn outputs
				props["phase"] = cppnOutputs[phaseIx].asDouble();
				//JS Version
				//props["amplitude"] = .3f*connectionDistance*amp;
				props["amplitude"] = .3f*amp;

                //need to scale joints based on size of the screen - this is a bit odd, but should help multiple sizes behave the same!
                this->addMuscleJoint(sourceID, targetID, props);
			}
        }
		catch (exception e)
        {
			printf("Oops error %s", e.what());
            throw e;
        }

    }

	//add the concept of mass
	morphology["mass"] = morphology["totalNodes"] + connectionDistanceSum/2.0f;

	//remove the concept of totalNodes since we have mass
	//morphology should now have width/height, startX/startY, and mass
	morphology.erase("totalNodes");

	return morphology;
}


IESoRWorld::~IESoRWorld()
{

}