#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "src/worldWrapper.h"

using namespace v8;

// Persistent<FunctionTemplate> IESoRWrap::constructor;

void InitAll(Handle<Object> exports) {
  IESoRWrap::Init(exports);
}

NODE_MODULE(iesorworld, InitAll)