#include "iesorDirector.h"
#include <IESoR/iesor.h>
#include <json.h>


IESoRDirector::IESoRDirector()
{
	//create a generic world for us to manage
	world_ = new IESoRWorld();
}

std::string json2String(Json::Value s)
{
	Json::FastWriter writer;
	//return a non styled json object
	return writer.write(s);
}

//dont use static reader/writer because we might need access from multiple threads
Json::Value string2JSON(std::string s)
{
	Json::Reader reader;
	Json::Value root;
	//return a non styled json object
	reader.parse(s,root);
	return root;
}

//grab the draw list as a json value from the world
Json::Value IESoRDirector::jCurrentDrawFrame()
{
	return world_->jsonWorldDrawList();
}

//same as json version, just return it in string form
std::string IESoRDirector::sCrrentDrawFrame()
{
	//convert to string before sending back
	return json2String(world_->jsonWorldDrawList());
}

//as a string version - for convenience
std::string IESoRDirector::sLongSimulateWorld(double time)
{
	return json2String(longSimulateWorld(time));
}

//as json information
//contains step count and interpolation amount 
Json::Value IESoRDirector::longSimulateWorld(double time)
{
	//returns the number of steps performed by the world (discrete steps)
	Json::Value stepInfo;

	double maxFrame = world_->MaxFrameSizeMS();

	int frameCount = floor(time/maxFrame);
	int tStepCount = 0;
	for(int i=0; i < frameCount; i++)
	{
		//simulate the world for the given max frame time
		//the update function returns how many discrete steps were simulated
		tStepCount += world_->updateWorld(maxFrame); 
		time -= maxFrame;
	}

	//if there is any time left, we simulate that here
	if(time)
		tStepCount += world_->updateWorld(time); 

	//now we have done the simulation in chunks (with the max frame size) -- all done!

	//how many discrete steps did we go?
	stepInfo["stepCount"] = tStepCount;
	//how much extra time was left during simulation?
	//if you simulate 1 step every 100ms, and you pass in 125ms
	//you simulated 1 step discretely, with 25ms leftover
	//interpolation = 25ms/100ms = .25 -- so 1/4 the way to the next step
	stepInfo["interpolation"] = world_->CurrentInterpolation();

	//interpolation is used for adjusting visuals (you linearly interpolate drawing information -- look on google)

	//send back this info as a json object
	return stepInfo;
}


Json::Value IESoRDirector::convertNetworkToBody(std::string& sByteNetwork)
{
	Json::Value val = string2JSON(sByteNetwork);
	return convertNetworkToBody(val);
}
Json::Value IESoRDirector::convertNetworkToBody(Json::Value& byteNetwork)
{
		//build class capable of generating a body from a network
	iesorBody* bodyCreator = new iesorBody(byteNetwork);

	//in the async version, this is the call we would be doing in a worker thread
	Json::Value fullBody = bodyCreator->buildBody();

	//return the body after it's built
	return fullBody;
}
std::string IESoRDirector::sConvertNetworkToBody(std::string& sByteNetwork)
{
	return json2String(convertNetworkToBody(sByteNetwork));
}


//same as inserting with json, we just parse the string to form our json network
std::map<std::string, double> IESoRDirector::insertBodyFromNetwork(std::string& network)
{
	Json::Value val = string2JSON(network);
	return insertBodyFromNetwork(val);
}

std::map<std::string, double> IESoRDirector::insertBodyFromNetwork(Json::Value& byteNetwork)
{

	//build class capable of generating a body from a network
	iesorBody* bodyCreator = new iesorBody(byteNetwork);

	//in the async version, this is the call we would be doing in a worker thread
	Json::Value fullBody = bodyCreator->buildBody();

	//   //This is a temporary piece -- in reality, we should return a local v8 object to JS
	// Json::StyledWriter writer;
	// //For now, we simply write the body to a json string
	// std::string outputConfig = writer.write(fullBody);

	// printf("%s", outputConfig.c_str());

	//now we specifically load the generated body into the world
	std::map<std::string, double> morphology = world_->loadBodyIntoWorld(fullBody);

	//if we aren't check the morph, we can skip the temp variable
	//either way, return the result
	return morphology;
	// for(std::map<std::string, double>::iterator iterator = morphology.begin(); iterator != morphology.end(); iterator++) {

	// 	printf("Morph Key: %s \n", iterator->first.c_str());
	// 	printf("Morph Value: %f \n", iterator->second);
	// }

}
