// Copyright(c) 2015
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE

#include <iostream>

#include "gnode.h"
#include "gnodecpp.h"

extern "C" {

using namespace v8;

//long js_mem_debug_managedref_count;

Handle<Value> ManagedRef::GetPropertyValue(Local<String> name){
  Handle<Value> res;

	#ifdef DEBUG_TRACE_API
	std::cout << "GetPropertyValue" << std::endl;
	#endif
	jsvalue s = engine_->StringFromV8(name);
  jsvalue r = engine_->CallGetPropertyValue(contextId_, id_, s.value.str);
	if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::cout << "cleaning up result from getproperty value" << std::endl;
	#endif
  // We don't need the jsvalue anymore and the CLR side never reuse them.
  V8_Value_Dispose(r);
	return res;
}

Handle<Boolean> ManagedRef::DeleteProperty(Local<String> name){
  Handle<Value> res;

	#ifdef DEBUG_TRACE_API
	std::cout << "DeleteProperty" << std::endl;
	#endif
	jsvalue s = engine_->StringFromV8(name);
  jsvalue r = engine_->CallDeleteProperty(contextId_, id_, s.value.str);
	if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::cout << "cleaning up result from DeleteProperty" << std::endl;
	#endif
  // We don't need the jsvalue anymore and the CLR side never reuse them.
	V8_Value_Dispose(s);
  V8_Value_Dispose(r);

	return res->ToBoolean();
}

Handle<Value> ManagedRef::SetPropertyValue(Local<String> name, Local<Value> value){
  Handle<Value> res;
	#ifdef DEBUG_TRACE_API
	std::cout << "SetPropertyValue" << std::endl;
	#endif
	jsvalue s = engine_->StringFromV8(name);
  jsvalue v = engine_->AnyFromV8(value);
  jsvalue r = engine_->CallSetPropertyValue(contextId_, id_, s.value.str, v);
  if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::cout << "cleaning up result from setproperty value" << std::endl;
	#endif
  // We don't need the jsvalues anymore and the CLR side never reuse them.
	V8_Value_Dispose(s);
  V8_Value_Dispose(v);
  V8_Value_Dispose(r);

  return res;
}

Handle<Value> ManagedRef::GetValueOf(){
	#ifdef DEBUG_TRACE_API
	std::wcout << "GETTING VALUE OF..........." << std::endl;
	#endif
	Handle<Value> res;
  jsvalue r = engine_->CallValueOf(contextId_, id_);
  if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::wcout << "cleaning up result from getting value of" << std::endl;
	#endif
  // We don't need the jsvalue anymore and the CLR side never reuse them.
	V8_Value_Dispose(r);
  return res;
}

Handle<Value> ManagedRef::Invoke(const jsvalue& args){
	#ifdef DEBUG_TRACE_API
	std::wcout << "INVOKING..........." << std::endl;
	#endif
	Handle<Value> res;
    //jsvalue a = engine_->ArrayFromArguments(args);
  jsvalue r = engine_->CallInvoke(contextId_, id_, args);
  if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::wcout << "cleaning up result from invoke" << std::endl;
	#endif
  // We don't need the jsvalue anymore and the CLR side never reuse them.
	//jsvalue_dispose(a);
  V8_Value_Dispose(r);
  return res;
}

Handle<Array> ManagedRef::EnumerateProperties(){
  Handle<Value> res;

	#ifdef DEBUG_TRACE_API
	std::cout << "EnumerateProperties" << std::endl;
	#endif
  jsvalue r = engine_->CallEnumerateProperties(contextId_, id_);
	if (r.type == JSVALUE_TYPE_MANAGED_ERROR)
        res = engine_->GetIsolate()->ThrowException(engine_->AnyToV8(r, contextId_));
  else
        res = engine_->AnyToV8(r, contextId_);

	#ifdef DEBUG_TRACE_API
	std::cout << "cleaning up result from EnumerateProperties" << std::endl;
	#endif
  // We don't need the jsvalue anymore and the CLR side never reuse them.
  V8_Value_Dispose(r);

	return Handle<Array>::Cast(res);
}

}
