#ifndef WORLDBUILDER_H
#define WORLDBUILDER_H

#include <JSON/json.h>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <Box2D/Box2D.h>

//class WorldBuilder 
//{
//	public:
//		WorldBuilder();
//	private:
//		~WorldBuilder();
//};

enum EntityType
{
	Circle = 0,
	Polygon,
	Rectangle
};


class Entity
{

public:
	static Json::Value CircleEntity(std::string bodyID, float x, float y, float radius, float angle)
	{
		Json::Value e;
		e["type"] = Circle;
		e["bodyID"] = bodyID;
		e["x"] = x;
		e["y"] = y;
		e["angle"] = angle;
		e["radius"] = radius;
		return e;
	}
	static Json::Value PolygonEntity(std::string bodyID, float xScale, float yScale, std::vector<b2Vec2>* points, float angle)
	{
		Json::Value e;

		e["type"] = Polygon;
		e["bodyID"] = bodyID;
		e["x"] = xScale;
		e["y"] = yScale;
		e["angle"] = angle;

		//map the points in please!
		Json::Value polyPoints(Json::arrayValue);
		int ix = 0;

		//add all the poitns into our new point vector!
		for (std::vector<b2Vec2>::iterator it = points->begin() ; it != points->end(); ++it)
		{
			Json::Value point; 
			point["x"] = it->x;
			point["y"] = it->y;
			polyPoints[ix++] = point;
		}

		return e;
	}
	static Json::Value RectangleEntity(std::string bodyID, float xScale, float yScale, float halfWidth, float halfHeight, float angle)
	{             
		Json::Value e;

		e["type"] = Rectangle;
		e["bodyID"] = bodyID;
		e["x"] = xScale;
		e["y"] = yScale;
		e["angle"] = angle;
		e["halfWidth"] = halfWidth;
		e["halfHeight"] = halfHeight;

		return e;
	}
};


class BodyInformation
{
public:
	BodyInformation();
	//Keep an ID present for each body chunk
	std::string GenomeID;

	//Required Nodes inside the structure
	std::vector<b2Vec2> nodes;

	//All connections required for the sodaracer
	std::vector<b2Vec2> connections;

	//this is for mapping between an index and a 
	//connection number (required for pruning)
	std::map<int,int> indexToConnection;

	//Is LEO used in the generation of this body 
	//(important for output tracking)
	bool useLEO;

private:
	~BodyInformation();
};



#endif //WORLDBUILDER_H