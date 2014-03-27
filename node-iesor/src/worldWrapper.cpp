#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "worldWrapper.h"
#include <json.h>
#include <string>
#include <sstream>

using namespace v8;

Persistent<Function> IESoRWrap::constructor;

IESoRWrap::IESoRWrap() {
  director_ = new IESoRDirector();
}

IESoRWrap::~IESoRWrap() {
}

void IESoRWrap::Init(Handle<Object> exports) {
   HandleScope scope;
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("iesorWorld"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  // Prototype
  //load body into our iesor world (convert from network to body and insert into box2d world)
  tpl->PrototypeTemplate()->Set(String::NewSymbol("loadBodyFromNetwork"),
      FunctionTemplate::New(LoadBodyFromNetwork)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("convertNetworkToBody"),
      FunctionTemplate::New(ConvertNetworkToBody)->GetFunction());
  
  //simulate a time period inside the world (returns information about simulation)
  tpl->PrototypeTemplate()->Set(String::NewSymbol("simulateWorldMS"),
      FunctionTemplate::New(SimulateWorldMS)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("getWorldDrawList"),
      FunctionTemplate::New(GetWorldDrawList)->GetFunction());
  


  

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("iesorWorld"), constructor);
}

Handle<Value> IESoRWrap::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new IESoRWrap(...)`
    IESoRWrap* obj = new IESoRWrap();
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `IESoRWrap(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}
////////////////
/// Methods inside javascript object
////////////////

std::string d2String(double number)
{
    std::ostringstream convert;   // stream used for the conversion
    convert << number;      // insert the textual representation of 'number' in the characters in the stream
    return convert.str();
}

std::string map2String(std::map<std::string, double> map)
{ 
  std::string mapToString = "{";
  
  for(std::map<std::string, double>::iterator it = map.begin(); it != map.end(); ++it)
  {
      mapToString += ("\"" + it->first + "\"" + ":" + "\"" + d2String(it->second) + "\"");
      if(it->first != map.rbegin()->first)
      {
        mapToString += ",";
      }
  }
  mapToString += "}";

  return mapToString;
}

//TODO: Make this asynchronous
Handle<Value> IESoRWrap::LoadBodyFromNetwork(const Arguments& args) {
  HandleScope scope;

  IESoRWrap* obj = ObjectWrap::Unwrap<IESoRWrap>(args.This());
  
  std::string byteNetwork(*v8::String::Utf8Value(args[0]->ToString()));
  
  //grab director from our object
  //director wraps all the necessary calls to iesorWorld -- it'll handle body loading from networks
  IESoRDirector* director = obj->Director();

  //send the body into our director (which will in turn load into a iesorWorld
  std::map<std::string, double> morph = director->insertBodyFromNetwork(byteNetwork);

  //for now, send back morphology information
  return scope.Close(String::New(map2String(morph).c_str()));
}


//TODO: Make this asynchronous
Handle<Value> IESoRWrap::ConvertNetworkToBody(const Arguments& args) {
  HandleScope scope;

  IESoRWrap* obj = ObjectWrap::Unwrap<IESoRWrap>(args.This());
  
  std::string byteNetwork(*v8::String::Utf8Value(args[0]->ToString()));
  
  //grab director from our object
  //director wraps all the necessary calls to iesorWorld -- it'll handle body loading from networks
  IESoRDirector* director = obj->Director();

  //send the body into our director (which will in turn load into a iesorWorld
  std::string bodyString = director->sConvertNetworkToBody(byteNetwork);

  //for now, send back morphology information
  return scope.Close(String::New(bodyString.c_str()));
}



Handle<Value> IESoRWrap::SimulateWorldMS(const Arguments& args) {
  
  HandleScope scope;

  IESoRWrap* obj = ObjectWrap::Unwrap<IESoRWrap>(args.This());
  
  double simTime = args[0]->NumberValue();
  
  IESoRDirector* director = obj->Director();

  //send the time into our director (which will in turn run the iesorWorld)
  std::string worldStep = director->sLongSimulateWorld(simTime);

  //we send back the info as a string (again json object -- figure out later what to do about this)
  return scope.Close(String::New(worldStep.c_str()));
}

Handle<Value> IESoRWrap::GetWorldDrawList(const Arguments& args) {
  
  HandleScope scope;

  IESoRWrap* obj = ObjectWrap::Unwrap<IESoRWrap>(args.This());
  
  IESoRDirector* director = obj->Director();

  //no arguments, just fetch the worldly information, and send it back!
  std::string worldDraw = director->sCrrentDrawFrame();

  //we send back the info as a string (again json object -- figure out later what to do about this)
  return scope.Close(String::New(worldDraw.c_str()));
}






