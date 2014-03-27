#ifndef IESORWORLD_H
#define IESORWORLD_H

#include <map>
#include <JSON/json.h>
#include <stdio.h>
#include <Box2D/Box2D.h>
#include "IESoR/Simulation/Initialize/initializeWorld.h"

struct PhysicsID;

class Bone;
class Muscle;

class IESoRWorld
{
	public:
		IESoRWorld();
		std::string worldDrawList();
		Json::Value jsonWorldDrawList();
		b2World* world;
		int updateWorld(double msUpdate);

		double CurrentInterpolation() const { return interpolation; }
		double MaxFrameSizeMS();

		b2Body* addBodyToWorld(std::string bodyID, b2BodyDef* bodyDef);
		b2Fixture* addShapeToBody(b2Body* body, b2FixtureDef* fixDef);
		b2Fixture* addShapeToBody(b2Body*body, b2Shape* shape, float density);

		//std::map<std::string, double>* LoadBodyIntoWorld(BodyInformation* inBody, b2Vec2 widthHeight);
		//std::vector<Entity*>* getBodyEntities(BodyInformation* inBody, b2Vec2 widthHeight, std::map<std::string, double>* intitialMorphology);
		std::map<std::string, double> loadBodyIntoWorld(Json::Value& inBody);
		std::vector<Json::Value> getBodyEntities(Json::Value& inBody, b2Vec2 widthHeight, std::map<std::string, double>& intitialMorphology);

		//setting bodies inside the world!
		void setBodies(std::vector<Json::Value>* entities);

		std::string loadDataFile(std::string dataName);
        
		Bone* addDistanceJoint(std::string sourceID, std::string targetID, Json::Value props);
		Muscle* addMuscleJoint(std::string sourceID, std::string targetID, Json::Value props);



	private:
		//Json::Value* bodyList;
		//Json::Value* shapeList;
		double interpolation;
		double radians;
		double accumulator;
		double desiredFPS;
		double simulationRate;
		double maxFrameSize;
		b2Vec2 currentSize;

		double modifyFrameTime(double fTime);

		//keep a list of body identifiers
		std::vector<PhysicsID*> bodyList;
		//keep a list of shape identifiers
		std::vector<PhysicsID*> shapeList;

		//keep a reference to every body according to ID
		std::map<std::string, b2Body*> bodyMap;

		//keep track of all the joints inside the system
		std::vector<Bone*> boneList;

		//we need to keep track of all our muscles too
		std::vector<Muscle*> muscleList;

		//we set a new node body through this function
		void setBody(Json::Value& entity);

		PhysicsID* createShapeID();
		

		~IESoRWorld();
		
};

#endif 
//IESORWORLD_H