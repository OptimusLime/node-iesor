#ifndef WORLDWRAPPER_H
#define WORLDWRAPPER_H

#include <node.h>
#include <IESoR/iesor.h>

using namespace v8;

class IESoRWrap : node::ObjectWrap {
  static Persistent<Function> constructor;

  public:
  static void Init(Handle<Object> exports);
  
  IESoRDirector* Director() const { return director_; }


 private:
  explicit IESoRWrap();
  ~IESoRWrap();

  IESoRDirector* director_;

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> LoadBodyFromNetwork(const Arguments& args);
  static Handle<Value> SimulateWorldMS(const Arguments& args);
  static Handle<Value> GetWorldDrawList(const Arguments& args);
  static Handle<Value> ConvertNetworkToBody(const Arguments& args);
};

#endif