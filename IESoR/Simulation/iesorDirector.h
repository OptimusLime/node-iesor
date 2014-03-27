#ifndef IESORDIRECTOR_H
#define IESORDIRECTOR_H
#include <JSON/json.h>
#include <map>

class IESoRWorld;

//this class should handle pretty much all your WIN needs
class IESoRDirector
{
	public:
		IESoRDirector();

		//access the world from the outside, if need be
		IESoRWorld* World() const { return world_; }

		//here, all the legwork is done in converting a network into a body, then inserting into the object
		std::map<std::string, double> insertBodyFromNetwork(std::string& network);
		std::map<std::string, double> insertBodyFromNetwork(Json::Value& network);

		Json::Value convertNetworkToBody(std::string& network);
		Json::Value convertNetworkToBody(Json::Value& network);
		std::string sConvertNetworkToBody(std::string& sByteNetwork);

		Json::Value longSimulateWorld(double time);
		std::string sLongSimulateWorld(double time);

		Json::Value jCurrentDrawFrame();
		std::string sCrrentDrawFrame();  

	private:
		
  		IESoRWorld* world_;

		~IESoRDirector();
		
};

#endif //IESORDIRECTOR_H