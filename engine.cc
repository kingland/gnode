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

//long js_mem_debug_engine_count;
jsvalue CALLINGCONVENTION jsvalue_alloc_array(const int32_t length);

static const int Mega = 1024 * 1024;


static void managed_prop_get(Local< String > property, const PropertyCallbackInfo<Value> &info)
{
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_prop_get" << std::endl;
		#endif
		Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
    //return scope.Close(ref->GetPropertyValue(name));
}

static void managed_prop_set(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value> &info)
{
	#ifdef DEBUG_TRACE_API
		std::cout << "managed_prop_set" << std::endl;
	#endif
		Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
		/*if (ref == NULL) {
			Local<Value> result;
			return scope.Close(result);
		}
    return scope.Close(ref->SetPropertyValue(name, value));*/
}

static void managed_prop_delete (Local< String > property, const PropertyCallbackInfo< Boolean > &info)
{
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_prop_delete" << std::endl;
		#endif
		Isolate* isolate = info.GetIsolate();
	 	HandleScope scope(isolate);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
    //return scope.Close(ref->DeleteProperty(name));
}

static void managed_prop_enumerate(const PropertyCallbackInfo<Array> &info)
{
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_prop_enumerate" << std::endl;
		#endif
		Isolate* isolate = info.GetIsolate();
	 	HandleScope scope(isolate);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
    //return scope.Close(ref->EnumerateProperties());
}

static void managed_call(const FunctionCallbackInfo<Value> &info)
{
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_call" << std::endl;
		#endif
		Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);

    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
    //return scope.Close(ref->Invoke(args));
}

void managed_valueof(const FunctionCallbackInfo<Value> &info) {
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_valueof" << std::endl;
		#endif
		Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
		//return scope.Close(ref->GetValueOf());
}

JsEngine* JsEngine::New(int32_t max_semi_space = -1, int32_t max_old_space = -1)
{
	JsEngine* engine = new JsEngine();
  if (engine != NULL)
	{
		engine->isolate_ = Isolate::New();
		engine->isolate_->Enter();
		if (max_semi_space > 0 && max_old_space > 0) {
			v8::ResourceConstraints constraints;
			constraints.set_max_semi_space_size(max_semi_space * Mega);
			constraints.set_max_old_space_size(max_old_space * Mega);

			v8::SetResourceConstraints(engine->isolate_ ,&constraints);
		}
		engine->isolate_->Exit();

		/*Locker locker(engine->isolate_);
		Isolate::Scope isolate_scope(engine->isolate_);
		HandleScope scope(engine->isolate_);*/
		HANDLE_SCOPE(engine->isolate_);

		// Setup the template we'll use for all managed object references.
    Handle<FunctionTemplate> ftmp_manage = FunctionTemplate::New(engine->isolate_);
		Handle<ObjectTemplate> otmp_manage = ftmp_manage->InstanceTemplate();
    otmp_manage->SetInternalFieldCount(1);
    otmp_manage->SetNamedPropertyHandler(
			managed_prop_get,
			managed_prop_set,
			NULL,
			managed_prop_delete,
			managed_prop_enumerate
		);
		//Sets the callback to be used when calling instances created from this template as a function.
		//If no callback is set, instances behave like normal JavaScript objects that cannot be called as a function.
    otmp_manage->SetCallAsFunctionHandler(managed_call);
		//Persistent<FunctionTemplate> pmanage(engine->isolate_, ftmp_manage);
		engine->managed_template_ = new JsFunctionTemplate(engine, ftmp_manage);

		Local<FunctionTemplate> ftmp_valueof = FunctionTemplate::New(engine->isolate_ ,managed_valueof);
		//Persistent<FunctionTemplate> pvalueof(engine->isolate_ , ftmp_valueof);
		engine->valueof_function_template_ = new JsFunctionTemplate(engine, ftmp_valueof);

		Local<Context> local_context = Context::New(engine->isolate_,NULL , otmp_manage);
		//Persistent<Context> pcontext(engine->isolate_,local_context);

		engine->global_context_ = new JsContext(0, engine, local_context);
		local_context->Enter();

		ftmp_manage->PrototypeTemplate()->Set(
			String::NewFromUtf8(engine->isolate_, "valueOf", String::kInternalizedString),
			ftmp_valueof->GetFunction()
		);

		local_context->Exit();
	}
	return engine;
}

JsScript* JsEngine::CompileScript(const char* str, ScriptOrigin *origin, jsvalue *error) {
	/*Locker locker(isolate_);
	Isolate::Scope isolate_scope(isolate_);
  HandleScope scope(isolate_);*/

	HANDLE_SCOPE(isolate_);

	TryCatch trycatch;
	Local<Context> global_context = Local<Context>::New(isolate_, global_context_->context_);
	global_context->Enter();

	//Handle<String> source = String::New(str);
	Handle<String> source = 	String::NewFromUtf8(isolate_, str, String::kInternalizedString);
	Handle<Script> script;

	if (origin != NULL) {
		//Handle<String> name = String::New(resourceName);
		//Handle<String> name = String::NewFromUtf8(isolate_, resourceName, String::kInternalizedString);
		script = Script::Compile( source, origin);
	} else {
		script = Script::Compile(source);
	}

	if (script.IsEmpty()) {
		*error = ErrorFromV8(trycatch);
	}

	//(*global_context_)->Exit();
	global_context->Exit();

	//Persistent<Script>  pScript(isolate_, script);
	//Persistent<Script> *pScript = new Persistent<Script>(Persistent<Script>::New(script));
	//return &pScript;
	if(script.IsEmpty())
		return NULL;

	JsScript* jsscript = JsScript::New(this, script);
	return jsscript;
}

JsUnboundScript* JsEngine::CompileUnboundScript(const char* str, ScriptOrigin *origin, jsvalue *error){
	HANDLE_SCOPE(isolate_);

	TryCatch trycatch;
	//Local<Context> global_context = Local<Context>::New(isolate_, global_context_->context_);
	//global_context->Enter();

	//Handle<String> source = String::New(str);
	Handle<String> source = 	String::NewFromUtf8(isolate_, str, String::kInternalizedString);
	Handle<UnboundScript> script;

	if (origin != NULL) {
		//Handle<String> name = String::New(resourceName);
		//Handle<String> name = String::NewFromUtf8(isolate_, resourceName, String::kInternalizedString);
		ScriptCompiler::Source scriptcompiler_source(source, *origin);
		script = ScriptCompiler::CompileUnbound(isolate_, &scriptcompiler_source);
	} else {
		ScriptCompiler::Source scriptcompiler_source(source);
		script = ScriptCompiler::CompileUnbound(isolate_, &scriptcompiler_source);
	}

	if (script.IsEmpty()) {
		*error = ErrorFromV8(trycatch);
	}

	//(*global_context_)->Exit();
	//global_context->Exit();
	if(script.IsEmpty())
		return NULL;

	JsUnboundScript* jsunboundscript = JsUnboundScript::New(this, script);
	return jsunboundscript;
}

void JsEngine::TerminateExecution(){
	V8::TerminateExecution(isolate_);
}

void JsEngine::DumpHeapStats(){
	/*Locker locker(isolate_);
  Isolate::Scope isolate_scope(isolate_);*/
	ISOLATE_SCOPE(isolate_);

	// gc first.
	//while(!V8::IdleNotification()) {};
	while(!isolate_->IdleNotification(IDLE_TIME_IN_MS)) {};

	HeapStatistics stats;
	isolate_->GetHeapStatistics(&stats);
	std::wcout << "Heap size limit " << (stats.heap_size_limit() / Mega) << std::endl;
	std::wcout << "Total heap size " << (stats.total_heap_size() / Mega) << std::endl;
	std::wcout << "Heap size executable " << (stats.total_heap_size_executable() / Mega) << std::endl;
	std::wcout << "Total physical size " << (stats.total_physical_size() / Mega) << std::endl;
	std::wcout << "Used heap size " << (stats.used_heap_size() / Mega) << std::endl;
}

void JsEngine::Dispose(){
	if (isolate_ != NULL) {
		isolate_->Enter();

		//managed_template_->Dispose();
		managed_template_ ->value_.Reset();
		delete managed_template_;
		managed_template_ = NULL;

		//valueof_function_template_->Dispose();
		valueof_function_template_->value_.Reset();
		delete valueof_function_template_;
		valueof_function_template_ = NULL;

		//global_context_->Dispose();
		global_context_->context_.Reset();
    delete global_context_;
		global_context_ = NULL;

		isolate_->Exit();
		isolate_->Dispose();
		isolate_ = NULL;
	  keepalive_remove_ = NULL;
		keepalive_get_property_value_ = NULL;
		keepalive_set_property_value_ = NULL;
		keepalive_valueof_ = NULL;
		keepalive_invoke_ = NULL;
		keepalive_delete_property_ = NULL;
		keepalive_enumerate_properties_ = NULL;
	}
}

void JsEngine::DisposeObject(Persistent<Object> obj){
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
		//obj->Dispose(isolate_);
		ISOLATE_SCOPE(isolate_);
		obj.Reset();
}

jsvalue JsEngine::ErrorFromV8(TryCatch& trycatch){
    jsvalue v;

    HandleScope scope(isolate_);
    Local<Value> exception = trycatch.Exception();
		v.type = JSVALUE_TYPE_UNKNOWN_ERROR;
		v.value.str = 0;
    v.length = 0;

	// If this is a managed exception we need to place its ID inside the jsvalue
    // and set the type JSVALUE_TYPE_MANAGED_ERROR to make sure the CLR side will
    // throw on it.

  if (exception->IsObject()) {
    Local<Object> obj = Local<Object>::Cast(exception);
    if (obj->InternalFieldCount() == 1) {
			Local<External> wrap = Local<External>::Cast(obj->GetInternalField(0));
			ManagedRef* ref = (ManagedRef*)wrap->Value();
	        v.type = JSVALUE_TYPE_MANAGED_ERROR;
            v.length = ref->Id();
			return v;
		}
	}

	jserror *error = new jserror();
	memset(error, 0, sizeof(jserror));

	Local<Message> message = trycatch.Message();

	if (!message.IsEmpty()) {
		error->line = message->GetLineNumber();
		error->column = message->GetStartColumn();
		error->resource = AnyFromV8(message->GetScriptResourceName());
		error->message = AnyFromV8(message->Get());
	}
	 if (exception->IsObject()) {
        Local<Object> obj2 = Local<Object>::Cast(exception);
		error->type = AnyFromV8(obj2->GetConstructorName());
	 }

	error->exception = AnyFromV8(exception);
	v.type = JSVALUE_TYPE_ERROR;
	v.value.ptr = error;

	return v;
}

jsvalue JsEngine::StringFromV8(Handle<Value> value){
    jsvalue v;

    Local<String> s = value->ToString();
    v.length = s->Length();
    v.value.str = new char[v.length+1];
    if (v.value.str != NULL) {
				//TODO:
        //s->Write(v.value.str);
				s->WriteUtf8(v.value.str);
        v.type = JSVALUE_TYPE_STRING;
    }

    return v;
}

jsvalue JsEngine::WrappedFromV8(Handle<Object> obj)
{
    jsvalue v;

	//if (js_object_marshal_type == JSOBJECT_MARSHAL_TYPE_DYNAMIC) {
	//TODO::
	if(false){
		v.type = JSVALUE_TYPE_WRAPPED;
		v.length = 0;
        // A Persistent<Object> is exactly the size of an IntPtr, right?
		// If not we're in deep deep trouble (on IA32 and AMD64 should be).
		// We should even cast it to void* because C++ doesn't allow to put
		// it in a union: going scary and scarier here.
		Persistent<Object> pobject(isolate_, obj);
		//v.value.ptr = new Persistent<Object>(Persistent<Object>::New(obj));
		v.value.ptr = &pobject;
	} else {
		v.type = JSVALUE_TYPE_DICT;
		Local<Array> names = obj->GetOwnPropertyNames();
		v.length = names->Length();
		jsvalue* values = new jsvalue[v.length * 2];
		if (values != NULL) {
			for(int i = 0; i < v.length; i++) {
				int indx = (i * 2);
				Local<Value> key = names->Get(i);
				values[indx] = AnyFromV8(key);
				values[indx+1] = AnyFromV8(obj->Get(key));
			}
			v.value.arr = values;
		}
	}

	return v;
}

jsvalue JsEngine::ManagedFromV8(Handle<Object> obj)
{
    jsvalue v;

	Local<External> wrap = Local<External>::Cast(obj->GetInternalField(0));
    ManagedRef* ref = (ManagedRef*)wrap->Value();
	v.type = JSVALUE_TYPE_MANAGED;
    v.length = ref->Id();
    v.value.str = 0;

    return v;
}

jsvalue JsEngine::AnyFromV8(Handle<Value> value, Handle<Object> thisArg)
{
    jsvalue v;

    // Initialize to a generic error.
    v.type = JSVALUE_TYPE_UNKNOWN_ERROR;
    v.length = 0;
    v.value.str = 0;

    if (value->IsNull() || value->IsUndefined()) {
        v.type = JSVALUE_TYPE_NULL;
    }
    else if (value->IsBoolean()) {
        v.type = JSVALUE_TYPE_BOOLEAN;
        v.value.i32 = value->BooleanValue() ? 1 : 0;
    }
    else if (value->IsInt32()) {
        v.type = JSVALUE_TYPE_INTEGER;
        v.value.i32 = value->Int32Value();
    }
    else if (value->IsUint32()) {
        v.type = JSVALUE_TYPE_INDEX;
        v.value.i64 = value->Uint32Value();
    }
    else if (value->IsNumber()) {
        v.type = JSVALUE_TYPE_NUMBER;
        v.value.num = value->NumberValue();
    }
    else if (value->IsString()) {
        v = StringFromV8(value);
    }
    else if (value->IsDate()) {
        v.type = JSVALUE_TYPE_DATE;
        v.value.num = value->NumberValue();
    }
    else if (value->IsArray()) {
        Handle<Array> object = Handle<Array>::Cast(value->ToObject());
        v.length = object->Length();
        jsvalue* array = new jsvalue[v.length];
        if (array != NULL) {
            for(int i = 0; i < v.length; i++) {
                array[i] = AnyFromV8(object->Get(i));
            }
            v.type = JSVALUE_TYPE_ARRAY;
            v.value.arr = array;
        }
    }
    else if (value->IsFunction()) {
			Handle<Function> function = Handle<Function>::Cast(value);
			jsvalue* array = new jsvalue[2];
	    if (array != NULL) {
				Persistent<Function> pfunction(isolate_, function);
				//array[0].value.ptr = new Persistent<Object>(Persistent<Function>::New(isolate_, function));
				array[0].value.ptr = &pfunction;
				array[0].length = 0;
				array[0].type = JSVALUE_TYPE_WRAPPED;
				if (!thisArg.IsEmpty()) {
					Persistent<Object> pobject(isolate_, thisArg);
					//array[1].value.ptr = new Persistent<Object>(Persistent<Object>::New(isolate_, thisArg));
					array[1].value.ptr = &pobject;
					array[1].length = 0;
					array[1].type = JSVALUE_TYPE_WRAPPED;
				} else {
					array[1].value.ptr = NULL;
					array[1].length = 0;
					array[1].type = JSVALUE_TYPE_NULL;
				}
		        v.type = JSVALUE_TYPE_FUNCTION;
	            v.value.arr = array;
        }
    }
    else if (value->IsObject()) {
        Handle<Object> obj = Handle<Object>::Cast(value);
        if (obj->InternalFieldCount() == 1)
            v = ManagedFromV8(obj);
        else
            v = WrappedFromV8(obj);
    }

    return v;
}

/*jsvalue JsEngine::ArrayFromArguments(const Arguments& args)
{
    jsvalue v = jsvalue_alloc_array(args.Length());
    Local<Object> thisArg = args.Holder();

    for (int i=0 ; i < v.length ; i++) {
        v.value.arr[i] = AnyFromV8(args[i], thisArg);
    }

    return v;
}*/

static void managed_destroy(Persistent<Value> object, void* parameter)
{
		#ifdef DEBUG_TRACE_API
		std::cout << "managed_destroy" << std::endl;
		#endif
		//TODO::
    /*HandleScope scope;
    Persistent<Object> self = Persistent<Object>::Cast(object);
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
		ManagedRef* ref = (ManagedRef*)wrap->Value();
    delete ref;
    object.Dispose();*/
}

Handle<Value> JsEngine::AnyToV8(jsvalue v, int32_t contextId)
{
	if (v.type == JSVALUE_TYPE_EMPTY) {
		return Handle<Value>();
	}
	if (v.type == JSVALUE_TYPE_NULL) {
        return Null(isolate_);
    }
    if (v.type == JSVALUE_TYPE_BOOLEAN) {
        return Boolean::New(isolate_, v.value.i32);
    }
    if (v.type == JSVALUE_TYPE_INTEGER) {
        return Int32::New(isolate_, v.value.i32);
    }
    if (v.type == JSVALUE_TYPE_NUMBER) {
        return Number::New(isolate_, v.value.num);
    }
    if (v.type == JSVALUE_TYPE_STRING) {
        //return String::New(isolate_ ,v.value.str);
				return String::NewFromUtf8(isolate_, v.value.str, String::kInternalizedString);
    }
    if (v.type == JSVALUE_TYPE_DATE) {
        return Date::New(isolate_, v.value.num);
    }

    // Arrays are converted to JS native arrays.

    if (v.type == JSVALUE_TYPE_ARRAY) {
        Local<Array> a = Array::New(isolate_, v.length);
        for(int i = 0; i < v.length; i++) {
            a->Set(i, AnyToV8(v.value.arr[i], contextId));
        }
        return a;
    }

    // This is an ID to a managed object that lives inside the JsContext keep-alive
    // cache. We just wrap it and the pointer to the engine inside an External. A
    // managed error is still a CLR object so it is wrapped exactly as a normal
    // managed object.
    if (v.type == JSVALUE_TYPE_MANAGED || v.type == JSVALUE_TYPE_MANAGED_ERROR) {
			ManagedRef* ref = new ManagedRef(this, contextId, v.length);
			Local<FunctionTemplate> local_ftmp = Local<FunctionTemplate>::New(isolate_, managed_template_->value_);
			Local<Object> object = local_ftmp->InstanceTemplate()->NewInstance();
			if (object.IsEmpty()) {
				return Null(isolate_);
			}

			//Persistent<Object> persistent = Persistent<Object>::New(object);
			//persistent->SetInternalField(0, External::New(ref));
			//persistent.MakeWeak(NULL, managed_destroy);
      //return persistent;
			object->SetInternalField(0, External::New(isolate_, ref));
			return Local<Value>::Cast(object);
    }

    return Null(isolate_);
}

int32_t JsEngine::ArrayToV8Args(jsvalue value, int32_t contextId, Handle<Value> preallocatedArgs[])
{
    if (value.type != JSVALUE_TYPE_ARRAY)
        return -1;

    for (int i=0 ; i < value.length ; i++) {
        preallocatedArgs[i] = AnyToV8(value.value.arr[i], contextId);
    }

    return value.length;
}

}
