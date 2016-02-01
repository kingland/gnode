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

#ifndef LIBGNODE_CPP_H
#define LIBGNODE_CPP_H

#include <iostream>
#include "gnode.h"
#include "v8.h"

/*#pragma once
class jsscript
{
public:
	jsscript(void);
	~jsscript(void);
};*/


using namespace v8;

class JsEngine;
class JsContext;
class JsValue;
class ManagedRef;
class JsScript;
class JsUnboundScript;
class JsFunctionTemplate;
class JsObjectTemplate;
//class JsNamePropertyHandler;

//WEAK CALLBACK
void ManageWeakValueCallback(const WeakCallbackData<Value, ManagedRef> &data);
void ManageWeakObjectCallback(const WeakCallbackData<Object, ManagedRef> &data);
void ManageWeakFunctionCallback(const WeakCallbackData<Function, ManagedRef> &data);
void ManageWeakFunctionTemplateCallback(const WeakCallbackData<Value, ManagedRef> &data);
void ManageWeakObjectTemplateCallback(const WeakCallbackData<Value, ManagedRef> &data);

//NAME PROPERTY CALLBACK
void ManageNamePropertyGetCallback(Local< String > property, const PropertyCallbackInfo<Value> &info);
void ManageNamePropertySetCallback(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value> &info);
void ManageNamePropertyDeleteCallback(Local< String > property, const PropertyCallbackInfo< Boolean > &info);
void ManageNamePropertyEnumerateCallback(const PropertyCallbackInfo<Array> &info);
void ManageNamePropertyQueryCallback(Local<String> property, const PropertyCallbackInfo<Integer> &info);

//INDEX PROPERTY CALLBACK
void ManageIndexPropertyGetCallback(uint32_t index, const PropertyCallbackInfo<Value> &info);
void ManageIndexPropertySetCallback(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value> &info);
void ManageIndexPropertyDeleteCallback(uint32_t index, const PropertyCallbackInfo<Boolean> &info);
void ManageIndexPropertyQueryCallback(uint32_t index, const PropertyCallbackInfo<Integer> &info);
void ManageIndexPropertyEnumCallback(const PropertyCallbackInfo<Array> &info);

//ACCESSOR CALLBACK
void ManageAccessorGetterCallback(Local<String> property, const PropertyCallbackInfo<Value>& info);
void ManageAccessorSetterCallback(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

typedef struct V8_ReturnValue {
	int32_t  						contextId;
	ReturnValue<Value> 	value;
} V8_ReturnValue;

// JsEngine is a single isolated v8 interpreter and is the referenced as an IntPtr
// by the JsEngine on the CLR side.
class JsEngine {
public:
	static JsEngine *New(int32_t max_semi_space, int32_t max_old_space);
	void TerminateExecution();

	inline void SetRemoveDelegate(keepalive_remove_f delegate) { keepalive_remove_ = delegate; }
  inline void SetGetPropertyValueDelegate(keepalive_get_property_value_f delegate) { keepalive_get_property_value_ = delegate; }
  inline void SetSetPropertyValueDelegate(keepalive_set_property_value_f delegate) { keepalive_set_property_value_ = delegate; }
  inline void SetValueOfDelegate(keepalive_valueof_f delegate) { keepalive_valueof_ = delegate; }
	inline void SetInvokeDelegate(keepalive_invoke_f delegate) { keepalive_invoke_ = delegate; }
	inline void SetDeletePropertyDelegate(keepalive_delete_property_f delegate) { keepalive_delete_property_ = delegate; }
	inline void SetEnumeratePropertiesDelegate(keepalive_enumerate_properties_f delegate) { keepalive_enumerate_properties_ = delegate; }

	// Call delegates into managed code.
  inline void CallRemove(int32_t context, int id) {
			if (keepalive_remove_ == NULL) {
				return;
			}
			keepalive_remove_(ptr_, context, id);
	}

	inline jsvalue CallGetPropertyValue(int32_t context, int32_t id, const char* name) {
			if (keepalive_get_property_value_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_get_property_value_(ptr_, context, id, name);
			return value;
	}

	inline jsvalue CallSetPropertyValue(int32_t context, int32_t id, const char* name, jsvalue value) {
			if (keepalive_set_property_value_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_set_property_value_(ptr_, context, id, name, value);
	}

	inline jsvalue CallValueOf(int32_t context, int32_t id) {
			if (keepalive_valueof_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_valueof_(ptr_, context, id);
	}

  inline jsvalue CallInvoke(int32_t context, int32_t id, jsvalue args) {
			if (keepalive_invoke_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_invoke_(ptr_ ,context, id, args);
	}

	inline jsvalue CallDeleteProperty(int32_t context, int32_t id, const char* name) {
			if (keepalive_delete_property_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_delete_property_(ptr_, context, id, name);
			return value;
	}

	inline jsvalue CallEnumerateProperties(int32_t context, int32_t id) {
			if (keepalive_enumerate_properties_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_enumerate_properties_(ptr_, context, id);
			return value;
	}

		// Conversions. Note that all the conversion functions should be called
    // with an HandleScope already on the stack or sill misarabily fail.
    jsvalue ErrorFromV8(TryCatch& trycatch);
    jsvalue StringFromV8(Handle<Value> value);
    jsvalue WrappedFromV8(Handle<Object> obj);
    jsvalue ManagedFromV8(Handle<Object> obj);
    jsvalue AnyFromV8(Handle<Value> value, Handle<Object> thisArg = Handle<Object>());

		JsScript* CompileScript(const char* str, ScriptOrigin *origin, jsvalue *error);
		JsUnboundScript* CompileUnboundScript(const char* str, ScriptOrigin *origin, jsvalue *error);

		// Converts JS function Arguments to an array of jsvalue to call managed code.
    //jsvalue ArrayFromArguments(const Arguments& args);

		Handle<Value> AnyToV8(jsvalue value, int32_t contextId);
    // Needed to create an array of args on the stack for calling functions.
    int32_t ArrayToV8Args(jsvalue value, int32_t contextId, Handle<Value> preallocatedArgs[]);
		// Dispose a Persistent<Object> that was pinned on the CLR side by JsObject.
    void DisposeObject(Persistent<Object> obj);
		void Dispose();
		void DumpHeapStats();
		void CreateContext(JsObjectTemplate* jsotmp = NULL);
		void CreateManageOtmp(JsObjectTemplate* jsotmp = NULL);

		Isolate* GetIsolate() { return isolate_; }
		JsContext* GetGlobalContext(){return global_context_;}
		JsObjectTemplate* GetGlobalOtmp(){return global_otmp_;}
		//JsFunctionTemplate* GetManageFtmp(){return managed_template_; }
		//JsFunctionTemplate* GetValueOfFtmp(){return valueof_function_template_;}
		void SetGoEngine(void* ptr){ptr_ = ptr;}
		void* GetGoEngine(){return ptr_;}
		inline virtual ~JsEngine() {
			//DECREMENT(js_mem_debug_engine_count);
		}


private:
		inline JsEngine() {
			//INCREMENT(js_mem_debug_engine_count);
		}

		Isolate *isolate_;

		JsContext* global_context_;
		JsObjectTemplate* global_otmp_;
		//JsFunctionTemplate* managed_template_;
		//JsFunctionTemplate* valueof_function_template_;
		//Persistent<Context> global_context_;
		//Persistent<FunctionTemplate> managed_template_;
		//Persistent<FunctionTemplate> valueof_function_template_;
		void* ptr_;

		keepalive_remove_f keepalive_remove_;
    keepalive_get_property_value_f keepalive_get_property_value_;
    keepalive_set_property_value_f keepalive_set_property_value_;
		keepalive_valueof_f keepalive_valueof_;
  	keepalive_invoke_f keepalive_invoke_;
		keepalive_delete_property_f keepalive_delete_property_;
		keepalive_enumerate_properties_f keepalive_enumerate_properties_;
};

class JsContext {
 public:
    static JsContext* New(int id, JsEngine *engine, JsObjectTemplate *otmp = NULL);

    // Called by bridge to execute JS from managed code.
    JsValue* Execute(const char* str, const char* resourceName);
		JsValue* Execute(JsScript *script);

		JsValue* GetGlobal();
    JsValue* GetVariable(const char* name);
    JsValue* SetVariable(const char* name, jsvalue value);
		JsValue* GetPropertyNames(Persistent<Object>* obj);
    JsValue* GetPropertyValue(Persistent<Object>* obj, const char* name);
    JsValue* SetPropertyValue(Persistent<Object>* obj, const char* name, jsvalue value);
    JsValue* InvokeProperty(Persistent<Object>* obj, const char* name, jsvalue args);
    JsValue* InvokeFunction(Persistent<Function>* func, Persistent<Object>* thisArg, jsvalue args);

		void IdleNotification();
		void Dispose();

		inline int32_t GetId() {
			return id_;
		}

		inline virtual ~JsContext() {
			//DECREMENT(js_mem_debug_context_count);
			context_.Reset();
		}
		inline JsContext(int id, JsEngine *engine, Handle<Context> context) {
			//INCREMENT(js_mem_debug_context_count);
			id_ = id;
			engine_ = engine;
			isolate_ = engine->GetIsolate();
			context_.Reset(isolate_, context);
		}
		Persistent<Context> context_;
 private:
		int32_t id_;
    Isolate *isolate_;
		JsEngine *engine_;
};


class ManagedRef {
 public:
    inline explicit ManagedRef(JsEngine *engine, int32_t contextId, int id) : engine_(engine), contextId_(contextId), id_(id) {
			//INCREMENT(js_mem_debug_managedref_count);
		}

    inline int32_t GetId() { return id_; }
		inline int32_t GetContextId(){return contextId_;}
		JsEngine* GetEngine(){
			return engine_;
		}
		JsContext* GetContext(){
			return context_;
		}

		//Deprecate in GO
    Handle<Value> GetPropertyValue(Local<String> name);
		//Deprecate in GO
    Handle<Value> SetPropertyValue(Local<String> name, Local<Value> value);
		//Deprecate in GO
		Handle<Value> GetValueOf();
    //Handle<Value> Invoke(const Arguments& args);
		//Deprecate in GO
		Handle<Value> Invoke(const jsvalue& args);
		//Deprecate in GO
    Handle<Boolean> DeleteProperty(Local<String> name);
		//Deprecate in GO
		Handle<Array> EnumerateProperties();

    ~ManagedRef() {
			engine_->CallRemove(contextId_, id_);
			//DECREMENT(js_mem_debug_managedref_count);
		}

 private:
  ManagedRef() {
		//INCREMENT(js_mem_debug_managedref_count);
	}
	int32_t contextId_;
	int32_t id_;
	JsEngine *engine_;
	JsContext *context_;
};

class JsUnboundScript{
public:
	static JsUnboundScript *New(JsEngine *engine, Handle<UnboundScript> script);
	//void Compile(const char* str, ScriptOrigin *origin);
	void Dispose();
	inline virtual ~JsUnboundScript() {
		//DECREMENT(js_mem_debug_script_count);
		script_.Reset();
	}
	Persistent<UnboundScript> script_;

private:
	inline JsUnboundScript(JsEngine *engine, Handle<UnboundScript> script) {
		//INCREMENT(js_mem_debug_script_count);
		engine_ = engine;
		isolate_ = engine->GetIsolate();
		script_.Reset(isolate_, script);
	}
	JsEngine *engine_;
	Isolate *isolate_;
};

class JsScript {
public:
	static JsScript *New(JsEngine *engine, Handle<Script> script);
	//void Compile(const char* str, ScriptOrigin *origin);
	void Dispose();
	//Persistent<Script> GetScript() { return script_; }

	inline virtual ~JsScript() {
		//DECREMENT(js_mem_debug_script_count);
		script_.Reset();
	}
	Persistent<Script> script_;
private:
	inline JsScript(JsEngine *engine, Handle<Script> script) {
		//INCREMENT(js_mem_debug_script_count);
		engine_ = engine;
		isolate_ = engine->GetIsolate();
		script_.Reset(isolate_, script);
	}
	JsEngine *engine_;
	Isolate *isolate_;
};

class JsData {
	public:
		JsData(JsEngine *engine, Handle<Value> value){
			//std::wcout << "JsData::Contructor 1::"  << std::endl;
			engine_ = engine;
			//std::wcout << "JsData::Contructor 2::"  << std::endl;
			isolate_ = engine->GetIsolate();
			//std::wcout << "JsData::Contructor 3::"  << std::endl;
			val_ = engine->AnyFromV8(value);
			//std::wcout << "JsData::Contructor 4::"  << std::endl;
		}
		Isolate* GetIsolate(){
			return isolate_;
		}
		JsEngine* GetEngine(){
			return engine_;
		}
		jsvalue* GetValue(){
			return &val_;
		}
		int32_t GetType(){
			return val_.type;
		}
		int GetManageId(){
			/*if(val_.type == JSVALUE_TYPE_MANAGED || val_.type == JSVALUE_TYPE_MANAGED_ERROR)
				return val_.length;
			else
				return 0;*/
				return id_;
		}
		void SetManageId(int id){
			/*if(val_.type == JSVALUE_TYPE_MANAGED || val_.type == JSVALUE_TYPE_MANAGED_ERROR)
				val_.length = id;*/
				id_ = id;
		}
		ManagedRef* GetManage(){
			return manage_;
		}
		void SetManage(ManagedRef* manage){
			manage_ = manage;
		}

		~JsData(){
			//delete manage_;
		}
	private:
		Isolate *isolate_;
		JsEngine *engine_;
		ManagedRef *manage_;
		int id_;
		jsvalue	val_;
};

class JsValue : public JsData {
	public:
		static JsValue* New(JsEngine *engine, Handle<Value> value){
			JsValue* val = new JsValue(engine, value);
			return val;
		}

		JsValue(JsEngine *engine, Handle<Value> value): JsData(engine, value) {
		 	Isolate* isolate_ = GetIsolate();
			value_.Reset(isolate_, value);
		}

		~JsValue() {
			Locker locker(GetIsolate());
			Isolate::Scope isolate_scope(GetIsolate());
			value_.SetWeak<ManagedRef>(GetManage(), ManageWeakValueCallback);
		}
		Persistent<Value>   value_;
};

class JsObject : public JsData{
	public:
		static JsObject* New(JsEngine *engine, Handle<Object> value){
			std::wcout << "JsObject::NEW 1::"  << std::endl;
			JsObject* val = new JsObject(engine, value);
			return val;
		}

		JsObject(JsEngine *engine, Handle<Object> value): JsData(engine, value) {
			std::wcout << "JsObject::Contructor 1::"  << std::endl;
			Isolate* isolate_ = GetIsolate();
			value_.Reset(isolate_, value);
			std::wcout << "JsObject::Contructor 2::"  << std::endl;
			if(GetType() == JSVALUE_TYPE_MANAGED || GetType() == JSVALUE_TYPE_MANAGED_ERROR){
				std::wcout << "JsObject::Contructor 3::"  << std::endl;
				Local<External> local_external = Local<External>::Cast(value->GetInternalField(0));
				std::wcout << "JsObject::Contructor 4::"  << std::endl;
				if(!local_external.IsEmpty()) SetManage((ManagedRef*)local_external->Value());
				std::wcout << "JsObject::Contructor 5::"  << std::endl;
			}
		}

		~JsObject() {
			Locker locker(GetIsolate());
			Isolate::Scope isolate_scope(GetIsolate());
			value_.SetWeak<ManagedRef>(GetManage(), ManageWeakObjectCallback);
			//value_.Reset();
		}
		Persistent<Object>   value_;
};

class JsFunction :public JsData{
	public:
		static JsFunction* New(JsEngine *engine, Handle<Function> value){
			JsFunction* val = new JsFunction(engine, value);
			return val;
		}

		JsFunction(JsEngine *engine, Handle<Function> value):JsData(engine, value) {
			Isolate* isolate_ = GetIsolate();
			value_.Reset(isolate_, value);
		}

		~JsFunction() {
			Locker locker(GetIsolate());
			Isolate::Scope isolate_scope(GetIsolate());
			value_.SetWeak<ManagedRef>(GetManage(), ManageWeakFunctionCallback);
			//value_.Reset();
		}
		Persistent<Function>   value_;
};

class JsFunctionTemplate {
public:
	JsFunctionTemplate(JsEngine *engine, Handle<FunctionTemplate> tmp){
		engine_ = engine;
		isolate_ = engine->GetIsolate();
		value_.Reset(isolate_, tmp);
	}
	~JsFunctionTemplate() {
		//Locker locker(GetIsolate());
		//Isolate::Scope isolate_scope(GetIsolate());
		//value_.SetWeak<ManagedRef>(GetManage(), ManageWeakFunctionTemplateCallback);
		value_.Reset();
	}

	JsEngine* engine_;
	Isolate* isolate_;
	Persistent<FunctionTemplate> value_;
};

class JsObjectTemplate {
public:
	JsObjectTemplate(JsEngine *engine, Handle<ObjectTemplate> otmp){
		engine_ = engine;
		isolate_ = engine->GetIsolate();
		value_.Reset(isolate_, otmp);
	}
	~JsObjectTemplate() {
		//Locker locker(GetIsolate());
		//Isolate::Scope isolate_scope(GetIsolate());
		//value_.SetWeak<ManagedRef>(GetManage(), ManageWeakObjectTemplateCallback);
		value_.Reset();
	}

	/*void SetNamedPropertyHandler(JsNamePropertyHandler* handle){
		namepropertyhandle_ = handle;
	}

	JsNamePropertyHandler* SetNamedPropertyHandler(){
		return namepropertyhandle_;
	}*/

	//JsNamePropertyHandler* namepropertyhandle_;
	JsEngine* engine_;
	Isolate* isolate_;
	Persistent<ObjectTemplate> value_;
};

/*class JsNamePropertyHandler {
public:
	JsNamePropertyHandler(void* getter, void* setter, void* query, void* deleter, void* enumerater, void* data) : getter_(getter), setter_(setter), query_(query), deleter_(deleter), enumerater_(enumerater) , data_(data) {

	}
	void* GetSetter(){
		return setter_;
	}
	void * GetGetter(){
		return getter_;
	}
	void* GetDeleter(){
		return deleter_;
	}
	void* GetQuery(){
		return query_;
	}
	void* GetEnumerater(){
		return enumerater_;
	}
	void* GetData(){
		return data_;
	}
private:
	void* setter_;
	void* getter_;
	void* deleter_;
	void* query_;
	void* enumerater_;
	void* data_;
};*/

class JsReturnValue{
public:
	JsReturnValue(JsEngine *engine, ReturnValue<Value> val) : engine_(engine),value_(val) {
		//engine_ = engine;
		//isolate_ = engine->GetIsolate();
		//value_.Reset(isolate_, tmp);
		//value_ = val;
	}
	~JsReturnValue() {
		//value_.Reset();
	}

	JsEngine* engine_;
	//Isolate* isolate_;
	ReturnValue<Value> value_;
};

class JsFunctionCallbackInfo{
public:
	JsFunctionCallbackInfo(JsEngine *engine,const FunctionCallbackInfo<Value>* info){
		engine_ = engine;
		//isolate_ = engine->GetIsolate();
		//value_.Reset(isolate_, tmp);
		info_ = info;
	}
	~JsFunctionCallbackInfo() {
		//value_.Reset();
	}

	JsEngine* engine_;
	//Isolate* isolate_;
	JsReturnValue* returnVal_;
	const FunctionCallbackInfo<Value>* info_;
};
#endif
