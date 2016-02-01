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
#include "gnodecpp.h"
#include "v8.h"
#include "_cgo_export.h"
using namespace v8;
void ManageWeakValueCallback(const WeakCallbackData<Value, ManagedRef> &data){
  ManagedRef* m = data.GetParameter();
  delete m;
}

void ManageWeakObjectCallback(const WeakCallbackData<Object, ManagedRef> &data){
  ManagedRef* m = data.GetParameter();
  delete m;
}

void ManageWeakFunctionCallback(const WeakCallbackData<Function, ManagedRef> &data){
  ManagedRef* m = data.GetParameter();
  delete m;
}

void ManageWeakFunctionTemplateCallback(const WeakCallbackData<Value, ManagedRef> &data){
  ManagedRef* m = data.GetParameter();
  delete m;
}

void ManageWeakObjectTemplateCallback(const WeakCallbackData<Value, ManagedRef> &data){
  ManagedRef* m = data.GetParameter();
  delete m;
}

void ManageNamePropertyGetCallback(Local< String > property, const PropertyCallbackInfo<Value> &info){
  Isolate* isolate_ = info.GetIsolate();
  //HandleScope scope(isolate);
  HANDLE_SCOPE(isolate_);

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  if(!wrap.IsEmpty()){
    ManagedRef* ref = (ManagedRef*)wrap->Value();

    Local<Array> callbackData_ =  Local<Array>::Cast(info.Data());
    V8_PropertyCallbackInfo callbackInfo_;
    callbackInfo_.engine = Local<External>::Cast(callbackData_->Get(OTP_Context))->Value();
    callbackInfo_.info = (void*) &info;
    callbackInfo_.returnValue = NULL;
    callbackInfo_.data = Local<External>::Cast(callbackData_->Get(OTP_Data))->Value();
    callbackInfo_.callback = Local<External>::Cast(callbackData_->Get(OTP_Getter))->Value();
    callbackInfo_.key = NULL;
    callbackInfo_.contextId = ref->GetContextId();
    callbackInfo_.id = ref->GetId();

    //if (OTP_Getter != OTP_Enumerator) {
		char* key = (char*)malloc(property->Length() + 1);
		property->WriteUtf8(key);
		callbackInfo_.key = key;
  	//}

  	/*if (OTP_Getter == OTP_Setter) {
  		callback_info.setValue = new_V8_Value(
  			V8_Current_Context(isolate_ptr),
  			value
  		);
  	}*/

    JsEngine* engine = ref->GetEngine();

    go_named_property_callback(OTP_Getter, &callbackInfo_, engine->GetGoEngine());

  	if (OTP_Getter != OTP_Enumerator) {
  		free(callbackInfo_.key);
  	}

  	if (callbackInfo_.returnValue != NULL)
  		delete static_cast<V8_ReturnValue*>(callbackInfo_.returnValue);
  }
  //return scope.Close(ref->GetPropertyValue(name));
}

void ManageNamePropertySetCallback(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value> &info){
  Isolate* isolate_ = info.GetIsolate();
  //HandleScope scope(isolate);
  HANDLE_SCOPE(isolate_);

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  ManagedRef* ref = (ManagedRef*)wrap->Value();
  /*if (ref == NULL) {
    Local<Value> result;
    return scope.Close(result);
  }
  return scope.Close(ref->SetPropertyValue(name, value));*/
}

void ManageNamePropertyDeleteCallback(Local< String > property, const PropertyCallbackInfo< Boolean > &info){

  Isolate* isolate_ = info.GetIsolate();
  //HandleScope scope(isolate);
  HANDLE_SCOPE(isolate_)

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  ManagedRef* ref = (ManagedRef*)wrap->Value();
  //return scope.Close(ref->DeleteProperty(name));
}

void ManageNamePropertyEnumerateCallback(const PropertyCallbackInfo<Array> &info){
  Isolate* isolate_ = info.GetIsolate();
  //HandleScope scope(isolate);
  HANDLE_SCOPE(isolate_);

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  ManagedRef* ref = (ManagedRef*)wrap->Value();
  //return scope.Close(ref->EnumerateProperties());
}

void ManageNamePropertyQueryCallback(Local<String> property, const PropertyCallbackInfo<Integer> &info){
  Isolate* isolate_ = info.GetIsolate();
  //HandleScope scope(isolate);
  HANDLE_SCOPE(isolate_);

  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  ManagedRef* ref = (ManagedRef*)wrap->Value();
  //return scope.Close(ref->EnumerateProperties());
}

void ManageIndexPropertyGetCallback(uint32_t index, const PropertyCallbackInfo<Value> &info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageIndexPropertySetCallback(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value> &info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageIndexPropertyDeleteCallback(uint32_t index, const PropertyCallbackInfo<Boolean> &info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageIndexPropertyQueryCallback(uint32_t index, const PropertyCallbackInfo<Integer> &info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageIndexPropertyEnumCallback(const PropertyCallbackInfo<Array> &info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}

void ManageAccessorSetterCallback(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info){
  Isolate* isolate_ = info.GetIsolate();
  HANDLE_SCOPE(isolate_);
}
