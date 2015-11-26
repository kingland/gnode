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

#pragma once
class jsscript
{
public:
	jsscript(void);
	~jsscript(void);
};


using namespace v8;

class JsEngine;
class JsContext;
class JsValue;
class JsScript;
class JsUnboundScript;
class JsFunctionTemplate;
class JsObjectTemplate;



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
			keepalive_remove_(context, id);
	}

	inline jsvalue CallGetPropertyValue(int32_t context, int32_t id, const char* name) {
			if (keepalive_get_property_value_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_get_property_value_(context, id, name);
			return value;
	}

	inline jsvalue CallSetPropertyValue(int32_t context, int32_t id, const char* name, jsvalue value) {
			if (keepalive_set_property_value_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_set_property_value_(context, id, name, value);
	}

	inline jsvalue CallValueOf(int32_t context, int32_t id) {
			if (keepalive_valueof_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_valueof_(context, id);
	}

  inline jsvalue CallInvoke(int32_t context, int32_t id, jsvalue args) {
			if (keepalive_invoke_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			return keepalive_invoke_(context, id, args);
	}

	inline jsvalue CallDeleteProperty(int32_t context, int32_t id, const char* name) {
			if (keepalive_delete_property_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_delete_property_(context, id, name);
			return value;
	}

	inline jsvalue CallEnumerateProperties(int32_t context, int32_t id) {
			if (keepalive_enumerate_properties_ == NULL) {
				jsvalue v;
				v.type == JSVALUE_TYPE_NULL;
				return v;
			}
			jsvalue value = keepalive_enumerate_properties_(context, id);
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

		Isolate *GetIsolate() { return isolate_; }
		JsContext* GetGlobalContext(){return global_context_;}
		JsFunctionTemplate* GetManageFtmp(){return managed_template_; }
		JsFunctionTemplate* GetValueFtmp(){return valueof_function_template_;}

		inline virtual ~JsEngine() {
			//DECREMENT(js_mem_debug_engine_count);
		}


private:
		inline JsEngine() {
			//INCREMENT(js_mem_debug_engine_count);
		}

		Isolate *isolate_;

		JsContext* global_context_;
		JsFunctionTemplate* managed_template_;
		JsFunctionTemplate* valueof_function_template_;
		//Persistent<Context> global_context_;
		//Persistent<FunctionTemplate> managed_template_;
		//Persistent<FunctionTemplate> valueof_function_template_;

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
    static JsContext* New(int id, JsEngine *engine);

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

    inline int32_t Id() { return id_; }

    Handle<Value> GetPropertyValue(Local<String> name);
    Handle<Value> SetPropertyValue(Local<String> name, Local<Value> value);
		Handle<Value> GetValueOf();
    //Handle<Value> Invoke(const Arguments& args);
		Handle<Value> Invoke(const jsvalue& args);
    Handle<Boolean> DeleteProperty(Local<String> name);
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
	JsEngine *engine_;
	int32_t id_;
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
			std::wcout << "JsData::Contructor 1::"  << std::endl;
			engine_ = engine;
			std::wcout << "JsData::Contructor 2::"  << std::endl;
			isolate_ = engine->GetIsolate();
			std::wcout << "JsData::Contructor 3::"  << std::endl;
			val_ = engine->AnyFromV8(value);
			std::wcout << "JsData::Contructor 4::"  << std::endl;
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
		int32_t GetManageId(){
			if(val_.type == JSVALUE_TYPE_MANAGED || val_.type == JSVALUE_TYPE_MANAGED_ERROR)
				return val_.length;
			else
				return 0;
		}
		void SetManageId(int32_t id){
			if(val_.type == JSVALUE_TYPE_MANAGED || val_.type == JSVALUE_TYPE_MANAGED_ERROR)
				val_.length = id;
		}
	private:
		Isolate *isolate_;
		JsEngine *engine_;
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
			value_.Reset();
			/*Locker locker(isolate_);
			Isolate::Scope isolate_scope(isolate_);

			if (fieldOwnerInfo == NULL) {
				self.Reset();
			} else {
				self.SetWeak<V8_FieldOwnerInfo>(fieldOwnerInfo, FieldOwnerWeakCallback);
			}
			context_handler.Reset();*/
		}
		Persistent<Value>   value_;
		//Persistent<Context> *context_;
};

class JsObject : public JsData{
	public:
		static JsObject* New(JsEngine *engine, Handle<Object> value){
			JsObject* val = new JsObject(engine, value);
			return val;
		}

		JsObject(JsEngine *engine, Handle<Object> value): JsData(engine, value) {
			Isolate* isolate_ = GetIsolate();
			value_.Reset(isolate_, value);
		}

		~JsObject() {
			value_.Reset();
			/*Locker locker(isolate_);
			Isolate::Scope isolate_scope(isolate_);

			if (fieldOwnerInfo == NULL) {
				self.Reset();
			} else {
				self.SetWeak<V8_FieldOwnerInfo>(fieldOwnerInfo, FieldOwnerWeakCallback);
			}
			context_handler.Reset();*/
		}

		Persistent<Object>   value_;
		//Persistent<Context> *context_;
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
			value_.Reset();
			/*Locker locker(isolate_);
			Isolate::Scope isolate_scope(isolate_);

			if (fieldOwnerInfo == NULL) {
				self.Reset();
			} else {
				self.SetWeak<V8_FieldOwnerInfo>(fieldOwnerInfo, FieldOwnerWeakCallback);
			}
			context_handler.Reset();*/
		}
		Persistent<Function>   value_;
		//Persistent<Context> *context_;
};

class JsFunctionTemplate {
public:
	JsFunctionTemplate(JsEngine *engine, Handle<FunctionTemplate> tmp){
		engine_ = engine;
		isolate_ = engine->GetIsolate();
		value_.Reset(isolate_, tmp);
	}
	~JsFunctionTemplate() {
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
		value_.Reset();
	}

	JsEngine* engine_;
	Isolate* isolate_;
	Persistent<ObjectTemplate> value_;
};

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
