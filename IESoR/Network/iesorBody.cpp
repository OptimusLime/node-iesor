#include "iesorBody.h"

#include <map>
#include <sstream>
#include <cmath>

using namespace std;

static double HyperNEATWeightRange = 3.0;
static double HyperNEATThreshold = .2;

class Point
{
public:
	Point(){x=0;y=0;};
	Point(double dx, double dy)
	{
		x= dx; y=dy;
	}

	string toString()
	{
		ostringstream stringStream;
		stringStream << x << "," << y;
		return stringStream.str();
	}
	Json::Value toJSON()
	{
		Json::Value j;
		j["x"] = x; j["y"] =y;
		return j;
	}

	double x;
	double y;
};
string iToS(long number)
{
    ostringstream convert;   // stream used for the conversion
    convert << number;      // insert the textual representation of 'number' in the characters in the stream
    return convert.str();
}


//simple functions for computing things
double dXDistance(int resolution, float mult)
{
	return mult*2 / ((double)resolution - 1);
}
double dYDistance(int resolution, float mult)
{
	return mult * 2 / ((double)resolution - 1);
}

double maxXDistanceCenter(Point p1, Point p2)
{
	return max(sqrt(pow(p1.x, 2)), sqrt(pow(p2.x, 2)));
}
double minYDistanceGround(Point p1, Point p2)
{
	return min(sqrt(pow((p1.y + 1) / 2, 2)), sqrt(pow((p2.y + 1) / 2, 2)));
}

//new body, simple!
//just parse json from string
iesorBody::iesorBody(std::string sByteNetwork)
{
	//pass in a string will create the network
	Network* net = new Network(sByteNetwork);

	//initalize our clasee with the network
	init(net);
}

iesorBody::iesorBody(Json::Value jByteNetwork)
{
	//pass in a string will create the network
	Network* net = new Network(jByteNetwork);

	//initalize our clasee with the network
	init(net);
}

iesorBody::iesorBody(Network* fNetwork)
{
	init(fNetwork);
}

void iesorBody::init(Network* sNetwork)
{
	//build our body from the network at future time
	network = sNetwork;
}


double* iesorBody::queryCPPNOutputs(double x1, double y1, double x2, double y2, double maxXDist, double minYDist)
{
	double* coordinates = new double[network->InputCount()];

	coordinates[0] = x1;
	coordinates[1] = y1;
	coordinates[2] = x2;
	coordinates[3] = y2;
	//coordinates[4] = maxXDist;
	//coordinates[5] = minYDist;

	//activate using these coordinates -- send back the results
	return network->activate(coordinates);
}

Point* iesorBody::gridQueryPoints(int resolution)
{
	Point* queryPoints = new Point[resolution*resolution];
    double dx = 2 / ((double)resolution - 1);
    double dy = 2 / ((double)resolution - 1);
    double fX = -1; double fY = -1;

    //double threeNodeDistance = sqrt(9.0 * dx * dx + 9.0 * dy * dy);
    double xDistanceThree = 3 * dx;
    double yDistanceThree = 3 * dy;

	int ix=0;
    for (int x = 0; x < resolution; x++)
    {
        for (int y = 0; y < resolution; y++)
        {
			queryPoints[ix].x = fX;
			queryPoints[ix].y = fY;
			ix++;
            //now increment fy and go again
            fY += dy;
        }
        //increment dx, run through again
        //reset fy to -1
        fX += dx;
        fY = -1;
    }

    return queryPoints;
}

map<long, Json::Value> inwardNodeConnections(Json::Value& cgl)
{
    map<long, Json::Value> nodesInward;

    for (int i = 0; i < cgl.size(); i++)
    {
        //grab the connection
        Json::Value cg = cgl[i];
		//get our source and target
		long sourceID = cg["sourceID"].asInt64();
		long targetID = cg["targetID"].asInt64();

        //we actually need to make sure any node that receives a connection must have a connection gene list -- even if it contains nothing
        //i.e. if I go to check how many outgoing connections to a node that only has incoming connections -- if we don't do this step
        //that query will throw a key not found exception in C#
		if (nodesInward.find(sourceID) == nodesInward.end())
		{
			Json::Value aVal(Json::arrayValue);
            nodesInward[sourceID] = aVal;
		}
        if (nodesInward.find(targetID) == nodesInward.end())
        {
            //add source node, and list of possible connections
			Json::Value aVal(Json::arrayValue);
            nodesInward[targetID] = aVal;
        }

        //add this gene to the object holding souce nodes and their outgoing connections
		nodesInward[targetID].append(cg);
    }

    return nodesInward;
}

map<long, Json::Value> outwardNodeConnections(Json::Value& cgl)
{
    map<long, Json::Value> nodesToOut;

    for (int i = 0; i < cgl.size(); i++)
    {
        //grab the connection
        Json::Value cg = cgl[i];
		//get our source and target
		long sourceID = cg["sourceID"].asInt64();
		long targetID = cg["targetID"].asInt64();

        //we actually need to make sure any node that receives a connection must have a connection gene list -- even if it contains nothing
        //i.e. if I go to check how many outgoing connections to a node that only has incoming connections -- if we don't do this step
        //that query will throw a key not found exception in C#
		if (nodesToOut.find(sourceID) == nodesToOut.end())
		{
			Json::Value aVal(Json::arrayValue);
            nodesToOut[sourceID] = aVal;
		}
        if (nodesToOut.find(targetID) == nodesToOut.end())
        {
            //add source node, and list of possible connections
			Json::Value aVal(Json::arrayValue);
            nodesToOut[targetID] = aVal;
        }

        //add this gene to the object holding souce nodes and their outgoing connections
		nodesToOut[sourceID].append(cg);
    }

    return nodesToOut;
}
string jsonPointToString(Json::Value point)
{
	return point["x"].asString() + "," + point["y"].asString(); 
}
string jsonConnToString(Json::Value conn)
{
	return iToS(conn["sourceID"].asInt64()) + "," + iToS(conn["targetID"].asInt64()); 
}
 Json::Value firstConnectionNotInvestigated(Json::Value& connections, Json::Value& allConnectionsSeen)
{
	//source and target should be unique
    for(int c=0; c < connections.size(); c++)
    {
		Json::Value conn = connections[c];
		string s =jsonConnToString(conn);
		Json::Value seen = allConnectionsSeen[s];
		//if we haven't seen this object, it's our first to investigate!
		if(seen.isNull())
			return conn;
    }

	Json::Value n(Json::nullValue);
    return n;
}

 Json::Value findIndex(Point point, Json::Value pointArray)
 {
	 for(int i=0; i < pointArray.size(); i++)
	 {
		 Json::Value p = pointArray[i];
		 if(point.x== p["x"].asDouble() && point.y == p["y"].asDouble())
			 {
			 	Json::Value r(i);
			 	return r;
			 } 
	 }

 	Json::Value r(-1);
 	return r;
 }

 void ensureSingleConnectedStructure(Json::Value& connections,
            map<long, Json::Value>& inwardConnection, map<long, Json::Value>& outwardConnections, 
            Json::Value& hiddenNeurons, map<long, Point>& conSourcePoints, map<long, Point>& conTargetPoints)
{
    int maxChain = 0;

    //no changes for 0 or 1 connections -- nothing could be disconnected
    if (connections.size() <= 1)
        return;

	Json::Value connectionChains(Json::arrayValue);
    //List<ConnectionGeneList> connectionChains = new List<ConnectionGeneList>();

	Json::Value seenConnections(Json::objectValue);
    //List<ConnectionGene> seenConnections = new List<ConnectionGene>();
    //List<long> seenNodes = new List<long>();
	//vector<int> seenNodes;
	Json::Value seenNodes(Json::objectValue);

    while (seenConnections.size() < connections.size())
    {

		Json::Value investigateNodes(Json::arrayValue);
		Json::Value firstInvestigate = firstConnectionNotInvestigated(connections, seenConnections);
        //ConnectionGene firstInvestigate = firstConnectionNotInvestigated(connections, seenConnections);

		if (firstInvestigate.isNull())
        {
            throw 1;//new Exception("Shouldn't be here, the while loop should have failed!");
        }

        //this will mark our chain of connections
		Json::Value conChain(Json::arrayValue);

        //get ready to investigate the node of a connection we haven't seen
		investigateNodes.append(firstInvestigate["sourceID"]);

        //still have nodes left to investigate, we should continue
        while (investigateNodes.size() > 0)
        {

            //this will be the next nodes we take a look at
			Json::Value nextInvestigate(Json::arrayValue);
			map<long, long> investigateMap;

            //for all the nodes we need to look at
            for (int i = 0; i < investigateNodes.size(); i++)
            {
                //grab the node id (this is the starting node, and we want to see all outward connections for that node)
                //i.e. who does this node connect to!
				long sourceNode = investigateNodes[i].asInt64();

                //don't examine nodes you've already seen -- but we shouodln't get here anyways
				if (!seenNodes[iToS(sourceNode)].isNull())// sourceNode.Contains(sourceNode))
                    continue;

                //we mark it as seen, even before looking, in case of recurring connections
                seenNodes[iToS(sourceNode)] = true;

                //grab all of our connections coming out of this node
                //make a copy, we're going to modify it
                Json::Value cgOut = outwardConnections[sourceNode];

                //for each connection
                for (int c = 0; c < cgOut.size(); c++)
                {
                    //grab our connection gene
                    Json::Value cg = cgOut[c];

					long sourceID = cg["sourceID"].asInt64();
					long targetID = cg["targetID"].asInt64();
                   

                    //also note that we've seen this connection -- no duplicates please -- although I don't know if
                    //this will ever be triggered -- i don't think so
					if(seenConnections[jsonConnToString(cg)].isNull()){
                        seenConnections[jsonConnToString(cg)] = cg;
						
                        //add it to our chain, since it's connected but never seen
                    	//recursive chain connections are actually not allowed
						if (sourceID != targetID)
                        	conChain.append(cg);
					}

                    //we need to investigate another node -- no duplicates please!
					if (seenNodes[iToS(targetID)].isNull() && investigateMap.find(targetID) == investigateMap.end())
                    {
						//investigate the json object -- not the int!
						nextInvestigate.append(cg["targetID"]);
						investigateMap[targetID] = 1;
					}
                }

                //and grab all of the connections going inward too -- we need to go both direction
                cgOut = inwardConnection[sourceNode];

                //for each connection
                for (int c = 0; c < cgOut.size(); c++)
                {
                    //grab our connection gene
                    Json::Value cg = cgOut[c];

                    //fetch the source/target info
					long sourceID = cg["sourceID"].asInt64();
					long targetID = cg["targetID"].asInt64();

                    //also note that we've seen this connection -- no duplicates please -- although I don't know if
                    //this will ever be triggered -- i don't think so
                    if(seenConnections[jsonConnToString(cg)].isNull()){
                        seenConnections[jsonConnToString(cg)] = cg;

                        //add it to our chain, since it's connected --but unchecked!
	                    //recursive chain connections are actually not allowed
	                    if (sourceID != targetID)
							conChain.append(cg);
                    }

                    //we need to investigate another node -- no duplicates please!
                    //going for inward goofers
					if (seenNodes[iToS(sourceID)].isNull() && investigateMap.find(sourceID) == investigateMap.end())
                    {
						//investigate the json object -- not the int!
						nextInvestigate.append(cg["sourceID"]);
						investigateMap[sourceID] = 1;
					}
                }

            }

            //we have now investigated all of our previous nodes
            //time to move to the next batch!
            investigateNodes = nextInvestigate;
        }

        //now we have exhausted the chain of possible nodes to investigate

        //that means we have the final chain here
        //so we add it for later investigation
        connectionChains.append(conChain);
		
        maxChain = max<int>(conChain.size(), maxChain);
    }


    //now we simply choose the maximum connection chain -- or the first one with the max found
    Json::Value* maxCGL;
	printf(" Conn chains: %i \n", connectionChains.size());
	for(int i=0; i < connectionChains.size(); i++)
	{
		printf(" Conn chain %i size: %i \n", i, connectionChains[i].size());

		if(connectionChains[i].size() == maxChain)
		{
			maxCGL = &connectionChains[i];
			break;
		}
	}	

    //dump the previous connections
	connections.clear();


    //absorb the correct connections
	for(int i=0; i < maxCGL->size(); i++)
	{
		connections.append((*maxCGL)[i]);
	}

    //now we need to say which hidden neurons actually make sense still
    hiddenNeurons.clear();

	//use map to prevent duplicates
	map<string, int> hiddenAlready;

	printf("Conn final: %i \n", connections.size());

    //make sure points are distinct, and no dupes -- we only need to do this once
	for(int i=0; i < connections.size(); i++)
	{
		Json::Value con = connections[i];
		long cID = con["gid"].asInt64();
        //no duplicates please!
		Point potential = conSourcePoints[cID];
		string s = potential.toString();
		//if we can't find you, we haven't seen you!
		if(hiddenAlready.find(s) == hiddenAlready.end())
		{
			hiddenAlready[s] = 1;
			hiddenNeurons.append(potential.toJSON());
		}

        potential = conTargetPoints[cID];
		s = potential.toString();
		
		if(hiddenAlready.find(s) == hiddenAlready.end())
		{
			hiddenAlready[s] = 1;
			hiddenNeurons.append(potential.toJSON());
		}

	}

    //now we have to correct the indices of the max-chain of objects
	for(int i=0; i < connections.size(); i++)
	{
		Json::Value con = connections[i];
		long cID = con["gid"].asInt64();

		 //readjust connection source/target depending on hiddenNeuron array
       	Point point = conSourcePoints[cID];
		con["sourceID"] = findIndex(point, hiddenNeurons);

		if (con["sourceID"].asInt64() == -1)
			printf("Adjusted con src- %d, tgt- %d \n", con["sourceID"].asInt(), con["targetID"].asInt());

		//now for the target connection
        point = conTargetPoints[cID];
		con["targetID"] = findIndex(point, hiddenNeurons);

		// printf("NSource: %i, NTarget: %i \n", 
		// 	con["sourceID"].asInt64(),
		// 	con["targetID"].asInt64());
		// Json::StyledWriter writer;
		// printf("Conn: %s", writer.write(con).c_str());

		if (con["targetID"].asInt64() == -1)
			printf("Adjusted con src- %d, tgt- %d \n", con["sourceID"].asInt(), con["targetID"].asInt());


		//make sure to overwrite the connection with our new object
		connections[i] = con;

	}


	//done? 
	//Connections should all be setup
}

Json::Value iesorBody::buildBody()//Json::Value compareBody)
{
	Network* net = network;
	bool isEmpty = false;

    //we want the genome, so we can acknowledge the genomeID!

    //now convert a network to a set of hidden neurons and connections
	Json::Value hiddenNeurons(Json::arrayValue);
	Json::Value connections(Json::arrayValue);

    //loop through a grid, defined by some resolution, and test every connection against another using leo
    int resolution = 9;
	int queryPointLength = resolution*resolution;

    //int resolutionHalf = resolution / 2;

    Point* queryPoints = gridQueryPoints(resolution);
    double xDistanceThree = dXDistance(resolution, 3.0f);
    double yDistanceThree = dYDistance(resolution, 3.0f);


    bool useLeo = true;

    int counter = 0;
    
	Json::Value hiddenMap;


	map<long, Point> conSourcePoints;
    map<long, Point> conTargetPoints;

	// Json::Value allOutputs = compareBody["allBodyOutputs"];
	// Json::Value allInputs = compareBody["allBodyInputs"];
	int oCount = 0;

    //Dictionary<string, List<PointF>> pointsChecked = new Dictionary<string, List<PointF>>();
    //List<PointF> pList;
    int src, tgt;
    //for each points we have
    for(int p1=0; p1 < queryPointLength; p1++)
    {
        Point xyPoint = queryPoints[p1];
            
        //query against all other points (possibly limiting certain connection lengths
        for(int p2 = p1; p2 < queryPointLength; p2++)
        {
            Point otherPoint = queryPoints[p2];

            if (p1 != p2 && (abs(xyPoint.x - otherPoint.x) < xDistanceThree && abs(xyPoint.y - otherPoint.y) < yDistanceThree))
            {
                double* outs = queryCPPNOutputs(xyPoint.x, xyPoint.y, otherPoint.x, otherPoint.y, maxXDistanceCenter(xyPoint, otherPoint),  minYDistanceGround(xyPoint, otherPoint));
                double weight = outs[0];

				// Json::Value compareOutputs = allOutputs[oCount];
				// Json::Value compareInputs = allInputs[oCount];

				// Json::Value key = compareInputs["Key"];
				// Json::Value value = compareInputs["Value"];

				// if(abs(key["X"].asDouble() - xyPoint.x) > 0.0)
				// 	printf("in original kx || new inputs: %f   ||   %f \n", key["X"].asDouble(), xyPoint.x);
				// if(abs(key["Y"].asDouble() - xyPoint.y) > 0.0)
				// 	printf("in original ky || new inputs: %f   ||   %f \n", key["Y"].asDouble(), xyPoint.y);

				// if(abs(value["X"].asDouble() - otherPoint.x) > 0.0)
				// 	printf("in original vx || new inputs: %f   ||   %f \n", value["X"].asDouble(), otherPoint.x);
				// if(abs(value["Y"].asDouble() - otherPoint.y) > 0.0)
				// 	printf("in original vy || new inputs: %f   ||   %f \n", value["Y"].asDouble(), otherPoint.y);
				

				// for(int o=0; o < compareOutputs.size(); o++)
				// {
				// 	if(abs(compareOutputs[o].asDouble()- outs[o]) > 0.000001 )
				// 		printf("out original || new outputs || dif : %f   ||   %f   ||   %f \n", compareOutputs[o].asDouble(), outs[o], (abs(compareOutputs[o].asDouble()- outs[o])));
				// }

                if (useLeo )
                {

                    if (outs[1] > 0)
                    {
							
						//made it to connection, save our outputs
						Json::Value cppnOutputs(Json::arrayValue);
						for(int c=0; c < network->OutputCount(); c++)
							cppnOutputs.append(outs[c]);

						//check if we've added these points before
						//we'll need an identifying strign from each poitn (simple, "{x},{y}" uniquely identifies)
						string xyString = xyPoint.toString();
						string otherString = otherPoint.toString();

                        //add to hidden neurons
						if (hiddenMap[xyString].isNull()){
							//store what index we've saved
							hiddenMap[xyString] = hiddenNeurons.size();
							hiddenNeurons.append(xyPoint.toJSON());
							
						}
						src = hiddenMap[xyString].asInt();

						if (hiddenMap[otherString].isNull()){
							//store what index we've saved
							hiddenMap[otherString] = hiddenNeurons.size();
                            hiddenNeurons.append(otherPoint.toJSON());
						}

                        tgt = hiddenMap[otherString].asInt();

                        conSourcePoints[counter] = xyPoint;
                        conTargetPoints[counter] = otherPoint;

						Json::Value conn;
	
						//set identifier as count
						conn["gid"] = counter++;
						//set src
						conn["sourceID"] = src;
						//set tgt
						conn["targetID"] = tgt;
						//set the weight (multiply by range)
						conn["weight"] = weight * HyperNEATWeightRange;
						//and save all our outputs for later interpretation
						conn["cppnOutputs"] = cppnOutputs;

						//add connections to our array
						connections.append(conn);
                    }
                }
                else
                {   
					//TODO: Test this path more -- I don't think it works well without LEO
					//I wouldn't trust this section if you don't use LEO

                    //add to hidden neurons
					//check if we've added these points before
					//we'll need an identifying strign from each poitn (simple, "{x},{y}" uniquely identifies)
					string xyString = xyPoint.toString();
					string otherString = otherPoint.toString();

                    //add to hidden neurons
					if (hiddenMap[xyString].isNull()){
						//store what index we've saved
						hiddenMap[xyString] = hiddenNeurons.size();
						hiddenNeurons.append(xyPoint.toJSON());
							
					}
					src = hiddenMap[xyString].asInt();

					if (hiddenMap[otherString].isNull()){
						//store what index we've saved
						hiddenMap[otherString] = hiddenNeurons.size();
                        hiddenNeurons.append(otherPoint.toJSON());
					}

                    tgt = hiddenMap[otherString].asInt();

                    
                    conSourcePoints[counter] = xyPoint;
                    conTargetPoints[counter] = otherPoint;

					
					if(abs(weight) > HyperNEATThreshold)
					{
						//made it to connection, save our outputs
						Json::Value cppnOutputs(Json::arrayValue);
						for(int c=0; c < network->OutputCount(); c++)
							cppnOutputs.append(outs[c]);

						Json::Value conn;
	
						//set identifier as count
						conn["gid"] = counter++;
						//set src
						conn["sourceID"] = src;
						//set tgt
						conn["targetID"] = tgt;
						//set the weight (multiply by range)
						conn["weight"] = weight * HyperNEATWeightRange;
						//and save all our outputs for later interpretation
						conn["cppnOutputs"] = cppnOutputs;

						//add connections to our array
						connections.append(conn);

					}
                }
				oCount++;
            }
        }

    }

    // int beforeConn = connections.size();
    // int beforeNeuron = hiddenNeurons.size();

	// printf("Before %d\n", beforeConn);
	// printf("tested against %d\n", compareBody["connections"].size());

	
    map<long, Json::Value> connectionsGoingOut = outwardNodeConnections(connections);
    map<long, Json::Value> connectionsGoingInward = inwardNodeConnections(connections);

    ensureSingleConnectedStructure(connections, connectionsGoingInward,  
    	connectionsGoingOut, hiddenNeurons, conSourcePoints, conTargetPoints);

    if (hiddenNeurons.size() > 20 || connections.size() > 100)
    {
		//empty it out -- it's too big -- this is our current fix
		hiddenNeurons.clear();
		connections.clear();
    }


    if (hiddenNeurons.size() == 0 || connections.size() == 0)
        isEmpty = true;

	Json::Value body;

	//store whether or not we used leo
	body["useLEO"] = useLeo;
	//location of our nodes (x,y)
	body["nodes"] = hiddenNeurons;
	//information (src,tgt) of connections
	body["connections"] = connections;
    
	return body;
}

iesorBody::~iesorBody()
{
	
}