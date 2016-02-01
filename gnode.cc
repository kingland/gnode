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

#include <string>
#include <iostream>
#include "v8.h"
#include "gnode.h"
#include "gnodecpp.h"
//#include "bridge.h"
#include "_cgo_export.h"
extern "C" {
	void keepalive_remove (void* engine, int context, int id) {
		go_keepalive_remove(engine, context, id);
	}

	jsvalue keepalive_get_property_value (void* engine, int context, int id, const char* name){
		char * cname = strdup(name);
		go_keepalive_get_property_value(engine, context, id, cname);
		jsvalue v;
		return v;
	}

	jsvalue keepalive_set_property_value (void* engine, int context, int id, const char* name, jsvalue value){
		char * cname = strdup(name);
		go_keepalive_set_property_value(engine, context, id, cname, &value);
		jsvalue v;
		return v;
	}

	jsvalue keepalive_valueof(void* engine, int context, int id){
		go_keepalive_valueof(engine, context, id);
		jsvalue v;
		return v;
	}

	jsvalue keepalive_invoke(void* engine, int context, int id, jsvalue args){
		go_keepalive_invoke(engine, context, id, &args);
		jsvalue v;
		return v;
	}

	jsvalue keepalive_delete_property (void* engine, int context, int id, const char* name){
		char * cname = strdup(name);
		go_keepalive_delete_property(engine, context, id, cname);
		jsvalue v;
		return v;
	}

	jsvalue keepalive_enumerate_properties(void* engine,int context, int id){
		go_keepalive_enumerate_properties(engine, context, id);
		jsvalue v;
		return v;
	}

	EXPORT void* CALLINGCONVENTION V8_NewEngine(int32_t max_young_space, int32_t max_old_space){
		JsEngine *engine = JsEngine::New(max_young_space, max_old_space);
		if (engine != NULL) {
			engine->SetRemoveDelegate(keepalive_remove);
			engine->SetGetPropertyValueDelegate(keepalive_get_property_value);
      engine->SetSetPropertyValueDelegate(keepalive_set_property_value);
			engine->SetValueOfDelegate(keepalive_valueof);
      engine->SetInvokeDelegate(keepalive_invoke);
			engine->SetDeletePropertyDelegate(keepalive_delete_property);
			engine->SetEnumeratePropertiesDelegate(keepalive_enumerate_properties);
    }
		return (void*) engine;
	}

	EXPORT void CALLINGCONVENTION V8_Go_Engine(void* engine, void* prt){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		jsengine->SetGoEngine(prt);
	}

	EXPORT void CALLINGCONVENTION V8_Engine_TerminateExecution(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		//jsengine_terminate_execution(jsengine);
		jsengine->TerminateExecution();
	}

	EXPORT void CALLINGCONVENTION V8_Engine_DumpHeapStats(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		//jsengine_dump_heap_stats(jsengine);
		jsengine->DumpHeapStats();
	}

	//EXPORT void CALLINGCONVENTION V8_Engine_DumpAllocatedItems(){
		//js_dump_allocated_items();
		//std::wcout << "Total allocated Js engines " << js_mem_debug_engine_count << std::endl;
		//std::wcout << "Total allocated Js contexts " << js_mem_debug_context_count << std::endl;
		//std::wcout << "Total allocated Js scripts " << js_mem_debug_script_count << std::endl;
		//std::wcout << "Total allocated Managed Refs " << js_mem_debug_managedref_count << std::endl;
	//}

	EXPORT void CALLINGCONVENTION V8_Engine_Dispose(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		jsengine->Dispose();
    delete jsengine;
	}

	/*EXPORT void CALLINGCONVENTION V8_Engine_DisposeObject(void* engine, jsvalue obj){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		if(obj.type != JSVALUE_TYPE_PERSISTENT){
			return;
		}
		JsObject *jsobj = static_cast<JsObject*>(obj.value.ptr);
		//Persistent<Value> pvalue = jsvalue->value_;
		//Persistent<Object> pobject = Persistent<Object>::Cast(pvalue);
		//jsengine_dispose_object(jsengine, &jsobj->value_);
		if (jsengine != NULL) {
    	//jsengine->DisposeObject(jsobj->value_);
		}
		delete jsobj;
	}*/

	EXPORT void CALLINGCONVENTION V8_Engine_DisposeValue(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		delete jsvalue;
	}

	EXPORT void* CALLINGCONVENTION V8_NewContext(int32_t id, void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		//return (void*) jscontext_new(id, jsengine);
		JsContext* jscontext = JsContext::New(id, jsengine);
    return (void*) jscontext;
	}

	EXPORT void CALLINGCONVENTION V8_Context_ForceGC(void* context){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jscontext_force_gc(jscontext);
		jscontext->IdleNotification();
	}

	EXPORT void CALLINGCONVENTION V8_Context_Dispose(void* context){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jscontext_dispose(jscontext);
		jscontext->Dispose();
    delete jscontext;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_Execute(void* context, const char* str, const char* resourceName){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val = jscontext_execute(jscontext, str, resourceName);
		JsValue* jsval = jscontext->Execute(str, resourceName);
		return (void*) jsval;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_ExecuteScript(void* context, void* script){
		JsContext* jscontext = static_cast<JsContext*>(context);
		JsScript* jsscript = static_cast<JsScript*>(script);
		//jsvalue val = jscontext_execute_script(jscontext,jsscript);
		JsValue* jsval = jscontext->Execute(jsscript);
		return (void*) jsval;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_Global(void* context){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val = jscontext_get_global(jscontext);
		JsValue* v = jscontext->GetGlobal();
		return (void*) v;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_SetVariable(void* context, const char* name, void* value){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val = jscontext_set_variable(jscontext, name, value);
		JsValue* jsvalue = static_cast<JsValue*>(value);
		JsValue* val = jscontext->SetVariable(name, *jsvalue->GetValue());
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_GetVariable(void* context, const char* name){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val = jscontext_get_variable(jscontext, name);
		JsValue* val = jscontext->GetVariable(name);
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_GetPropertyValue(void* context, void* obj, const char* name){
		JsContext* jscontext = static_cast<JsContext*>(context);
		/*jsvalue val;
		val.type = JSVALUE_TYPE_STRING_ERROR;
		if(obj.type != JSVALUE_TYPE_PERSISTENT){
			return val;
		}*/
		JsObject *jsobj = static_cast<JsObject*>(obj);
		//Persistent<Value> pvalue = jsvalue->value_;
		//Persistent<Object> pobject = Persistent<Object>::Cast(pvalue);
		//val = jscontext_get_property_value(jscontext, &jsobj->value_, name);
		JsValue* val = jscontext->GetPropertyValue(&jsobj->value_, name);
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_SetPropertyValue(void* context, void* obj, const char* name, void* value){
		JsContext* jscontext = static_cast<JsContext*>(context);
		/*jsvalue val;
		val.type = JSVALUE_TYPE_STRING_ERROR;
		if(obj.type != JSVALUE_TYPE_PERSISTENT){
			return val;
		}*/
		JsObject *jsobj = static_cast<JsObject*>(obj);
		JsValue *jsval = static_cast<JsValue*>(value);
		//Persistent<Value> pvalue = jsvalue->value_;
		//Persistent<Object> pobject = Persistent<Object>::Cast(pvalue);
		//val = jscontext_set_property_value(jscontext, &jsobj->value_, name, value);
		JsValue* val = jscontext->SetPropertyValue(&jsobj->value_, name, *jsval->GetValue());
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_GetPropertyNames(void* context, void* obj){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val;
		/*val.type = JSVALUE_TYPE_STRING_ERROR;
		if(obj.type != JSVALUE_TYPE_PERSISTENT){
			return val;
		}*/
		JsObject *jsobj = static_cast<JsObject*>(obj);
		//Persistent<Value> pvalue = jsvalue->value_;
		//Persistent<Object> pobject = Persistent<Object>::Cast(pvalue);
		//val = jscontext_get_property_names(jscontext, &jsobj->value_);
		JsValue* val = jscontext->GetPropertyNames(&jsobj->value_);
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_InvokeProperty(void* context, void* obj, const char* name, void* args){
		JsContext* jscontext = static_cast<JsContext*>(context);
		//jsvalue val;
		//val.type = JSVALUE_TYPE_STRING_ERROR;
		//if(obj.type != JSVALUE_TYPE_PERSISTENT){
			//return val;
		//}
		JsObject *jsobj = static_cast<JsObject*>(obj);
		JsValue *jsargs= static_cast<JsValue*>(args);
		//Persistent<Value> pvalue = jsvalue->value_;
		//Persistent<Object> pobject = Persistent<Object>::Cast(pvalue);
		//val = jscontext_invoke_property(jscontext, &jsobj->value_, name, args);
		JsValue *val = jscontext->InvokeProperty(&jsobj->value_, name, *jsargs->GetValue());
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_Context_Invoke(void* context, void* func, void* obj, void* args){
		JsContext* jscontext = static_cast<JsContext*>(context);

		/*jsvalue val;
		val.type = JSVALUE_TYPE_STRING_ERROR;
		if(obj.type != JSVALUE_TYPE_PERSISTENT){
			return val;
		}
		if(func.type != JSVALUE_TYPE_PERSISTENT){
			return val;
		}*/

		JsObject *jsobj = static_cast<JsObject*>(obj);
		JsFunction *jsfunc = static_cast<JsFunction*>(func);
		JsValue	*jsargs = static_cast<JsValue*>(args);
		//Persistent<Object> pobject = jsobj->value_;
		//Persistent<Function> pfunction = jsfunc->value_;
		//val = jscontext_invoke(jscontext, &jsfunc->value_, &jsobj->value_, args);
		JsValue* val = jscontext->InvokeFunction( &jsfunc->value_, &jsobj->value_, *jsargs->GetValue());
		return (void*) val;
	}

	EXPORT void* CALLINGCONVENTION V8_NewScript(void* engine, const char* code, void* goscriptorigin){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		//return (void*) jsscript_new(jsengine);
		//JsScript* script = JsScript::New(jsengine);
    //return script;

		//TODO: extract to origin function
		ScriptOrigin script_origin(String::NewFromUtf8(isolate_, ""));
		if(goscriptorigin){
			char * cstr = go_script_origin_get_name(goscriptorigin);
			int line    = go_script_origin_get_line(goscriptorigin);
			int column  = go_script_origin_get_column(goscriptorigin);

			script_origin = ScriptOrigin(
				String::NewFromUtf8(isolate_, cstr),
				Integer::New(isolate_, line),
				Integer::New(isolate_, column)
			);
			free(cstr);
		}


		jsvalue v;
		v.type = 0;
		JsScript* script = jsengine->CompileScript(code, &script_origin, &v);
		return (void*) script;
	}

	EXPORT void* CALLINGCONVENTION V8_NewUnboundScript(void* engine, const char* code, void* goscriptorigin){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);

		//TODO: extract to origin function
		ScriptOrigin script_origin(String::NewFromUtf8(isolate_, ""));
		if(goscriptorigin){
			char * cstr = go_script_origin_get_name(goscriptorigin);
			int line    = go_script_origin_get_line(goscriptorigin);
			int column  = go_script_origin_get_column(goscriptorigin);

			script_origin = ScriptOrigin(
				String::NewFromUtf8(isolate_, cstr),
				Integer::New(isolate_, line),
				Integer::New(isolate_, column)
			);
			free(cstr);
		}

		jsvalue v;
		v.type = 0;
		JsUnboundScript* script = jsengine->CompileUnboundScript(code, &script_origin, &v);
		return (void*) script;
	}

	EXPORT void CALLINGCONVENTION V8_Script_Dispose(void* script){
		JsScript *jsscript = static_cast<JsScript*>(script);
		//jsscript_dispose(jsscript);
		jsscript->Dispose();
	}

	EXPORT void CALLINGCONVENTION V8_UnboundScript_Dispose(void* script){
		JsUnboundScript *jsscript = static_cast<JsUnboundScript*>(script);
		jsscript->Dispose();
	}

	/*EXPORT jsvalue CALLINGCONVENTION V8_Script_Compile(void* script, const char* str, void *origin){
		JsScript *jsscript = static_cast<JsScript*>(script);
		//TODO:
		//jsvalue val = jsscript_compile(jsscript, str, NULL);
		return jsscript->Compile(str, NULL);
		//return val;
	}*/

	EXPORT jsvalue CALLINGCONVENTION V8_Alloc_String(const char* str){
		//jsvalue val = jsvalue_alloc_string(str);
		//return val;
		jsvalue v;

    int length = 0;
    while (str[length] != '\0')
			length++;

    v.length = length;
    v.value.str = new char[length+1];
    if (v.value.str != NULL) {
        for (int i=0 ; i < length ; i++)
             v.value.str[i] = str[i];
        v.value.str[length] = '\0';
        v.type = JSVALUE_TYPE_STRING;
    }
    return v;
	}

	EXPORT jsvalue CALLINGCONVENTION V8_Alloc_Array(const int32_t length){
		//jsvalue val = jsvalue_alloc_array(length);
		//return val;
		jsvalue v;
    v.value.arr = new jsvalue[length];
    if (v.value.arr != NULL) {
        v.length = length;
        v.type = JSVALUE_TYPE_ARRAY;
    }
		return v;
	}

	EXPORT const char* CALLINGCONVENTION V8_GetVersion(){
		return V8::GetVersion();
	}

	EXPORT void CALLINGCONVENTION V8_SetFlagsFromString(const char* str, int length){
		V8::SetFlagsFromString(str, length);
	}

	EXPORT void CALLINGCONVENTION V8_Value_Dispose(jsvalue value){
		//jsvalue_dispose(value);
		if (value.type == JSVALUE_TYPE_STRING || value.type == JSVALUE_TYPE_STRING_ERROR) {
		 if (value.value.str != NULL) {
			 delete[] value.value.str;
		 }
	 }
	 else if (value.type == JSVALUE_TYPE_ARRAY || value.type == JSVALUE_TYPE_FUNCTION) {
		 for (int i=0 ; i < value.length ; i++) {
			 V8_Value_Dispose(value.value.arr[i]);
		 }
		 if (value.value.arr != NULL) {
			 delete[] value.value.arr;
		 }
	 }
	 else if (value.type == JSVALUE_TYPE_DICT) {
		 for (int i=0 ; i < value.length * 2; i++) {
			 V8_Value_Dispose(value.value.arr[i]);
		 }
		 if (value.value.arr != NULL) {
				 delete[] value.value.arr;
		 }
	 }
	 else if (value.type == JSVALUE_TYPE_ERROR) {
		 jserror *error = (jserror*)value.value.ptr;
		 V8_Value_Dispose(error->resource);
		 V8_Value_Dispose(error->message);
		 V8_Value_Dispose(error->exception);
		 delete error;
	 }
	}

	EXPORT void* CALLINGCONVENTION V8_Undefined(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Primitive> undef = Undefined(isolate_);
		//jsvalue v ;
		//v = jsengine->AnyFromV8(undef);
		JsValue* jsv = new JsValue(jsengine, undef);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_Null(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Primitive> null = Null(isolate_);
		//jsvalue v ;
		//v = jsengine->AnyFromV8(null);
		JsValue* jsv = new JsValue(jsengine, null);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_True(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Boolean> val = True(isolate_);
		//jsvalue v ;
		//v = jsengine->AnyFromV8(val);
		//return &v;
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_False(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Boolean> val = False(isolate_);
		//jsvalue v ;
		//v = jsengine->AnyFromV8(val);
		//return &v;
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_NewNumber(void* engine, double number){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Number> val = Number::New(isolate_, number);
		//jsvalue v;
		//v = jsengine->AnyFromV8(val);
		//return &v;
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_NewDate(void* engine, double number){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> val = Date::New(isolate_, number);
		//jsvalue v;
		//v = jsengine->AnyFromV8(val);
		//return &v;
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_NewString(void* engine, const char* str, int length){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<String> val = String::NewFromUtf8(isolate_, str, String::kInternalizedString, length);
		//jsvalue v;
		//v = jsengine->AnyFromV8(val);
		//return &v;
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_NewExternal(void* engine, void* external){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<External> val = External::New(isolate_, external);
		JsValue* jsv = new JsValue(jsengine, val);
		return (void*) jsv;
	}

	EXPORT void* CALLINGCONVENTION V8_NewObject(void* engine){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();
		std::wcout << "V8_NewObject 1::"  << std::endl;
		HANDLE_SCOPE(isolate_);
		//Local<Context> local_context = Context::New(isolate_, NULL, jsengine->GetGlobalContext()->context_);
		Local<Context> local_context = Local<Context>::New(isolate_, jsengine->GetGlobalContext()->context_);
		Context::Scope context_scope(local_context);
		std::wcout << "V8_NewObject 2::"  << std::endl;
		Local<Object> local_val = Object::New(isolate_);
		//TODO:: manage object
		//Local<Object> val =
		std::wcout << "V8_NewObject 3::"  << std::endl;
		//JsObject* jso = JsObject::New(jsengine, val);
		JsValue* jsval = JsValue::New(jsengine, local_val);
		std::wcout << "V8_NewObject 4::"  << std::endl;
		return (void*) jsval;
	}

	EXPORT void* CALLINGCONVENTION V8_NewArray(void* engine, int length){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Array> val = Array::New(isolate_, length);
		JsValue* jsv = new JsValue(jsengine, val);
		return jsv;
	}

	EXPORT int CALLINGCONVENTION  V8_Array_Length(void* value) {
		JsValue *jsvalue =  static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Array>::Cast(local_value)->Length();
	}

	EXPORT void* CALLINGCONVENTION V8_NewRegExp(void* engine, const char* pattern, int length, int flags){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<RegExp> local_regexp = RegExp::New(
			String::NewFromUtf8(isolate_, pattern, String::kInternalizedString, length),
			(RegExp::Flags)flags
		);

		JsValue* jsv = new JsValue(jsengine, local_regexp);
		return (void*) jsv;
	}

	EXPORT int CALLINGCONVENTION V8_Value_GetManageId(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		return jsvalue->GetManageId();
	}

	EXPORT void CALLINGCONVENTION V8_Value_SetManageId(void* value, int id){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		jsvalue->SetManageId(id);
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsUndefined(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsUndefined();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsNull(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsNull();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsTrue(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsTrue();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsFalse(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsFalse();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsString(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsString();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsFunction(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsFunction();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsArray(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsArray();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsObject(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsObject();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsBoolean(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsBoolean();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsNumber(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsNumber();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsExternal(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsExternal();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsInt32(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsInt32();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsUint32(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsUint32();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsDate(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsDate();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsBooleanObject(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsBooleanObject();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsNumberObject(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsNumberObject();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsStringObject(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsStringObject();
	}

	EXPORT int CALLINGCONVENTION  V8_Value_IsNativeError(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsNativeError();
	}

	EXPORT int CALLINGCONVENTION V8_Value_IsRegExp(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IsRegExp();
	}

	EXPORT int CALLINGCONVENTION V8_Value_ToBoolean(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->BooleanValue();
	}

	EXPORT double CALLINGCONVENTION V8_Value_ToNumber(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->NumberValue();
	}

	EXPORT int64_t CALLINGCONVENTION V8_Value_ToInteger(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->IntegerValue();
	}

	EXPORT uint32_t CALLINGCONVENTION V8_Value_ToUint32(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->Uint32Value();
	}

	EXPORT int32_t CALLINGCONVENTION V8_Value_ToInt32(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return local_value->Int32Value();
	}

	EXPORT char* CALLINGCONVENTION V8_Value_ToString(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Handle<String> string = local_value->ToString();
		char* str = (char*)malloc(string->Length() + 1);
		string->WriteUtf8(str);

		return str;
	}

	EXPORT int CALLINGCONVENTION V8_Object_SetProperty(void* value, const char* key, int keylength, void* propvalue) {
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsValue *jspropvalue = static_cast<JsValue*>(propvalue);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> prop_value = Local<Value>::New(isolate_, jspropvalue->value_);
		return Local<Object>::Cast(local_value)->Set(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength),
			prop_value
		);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> res_value = Local<Object>::Cast(local_value)->Get(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);

		JsValue *jsresvalue = JsValue::New(jsengine, res_value);
		return (void*) jsresvalue;
	}

	EXPORT int CALLINGCONVENTION V8_Object_SetElement(void* value, uint32_t index, void* elemvalue){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsValue *jselemvalue = static_cast<JsValue*>(elemvalue);
		//JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> elem_value = Local<Value>::New(isolate_, jselemvalue->value_);
		return Local<Object>::Cast(local_value)->Set(	index,	elem_value);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetElement(void* value, uint32_t index){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> res_value = Local<Object>::Cast(local_value)->Get(index);

		JsValue *jsresvalue = JsValue::New(jsengine, res_value);
		return (void*) jsresvalue;
	}

	EXPORT int CALLINGCONVENTION V8_Object_InternalFieldCount(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->InternalFieldCount();
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetInternalField(void* value, int index){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Object> local_object = Local<Object>::Cast(local_value);
		Local<Value> res_value = local_object->GetInternalField(index);
		//TODO::
		//return Local<External>::Cast(res_value)->Value();
		JsValue *jsresvalue = JsValue::New(jsengine, res_value);
		return (void*) jsresvalue;
	}

	EXPORT void CALLINGCONVENTION V8_Object_SetInternalField(void* value, int index, void* data){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Object> local_object = Local<Object>::Cast(local_value);
		local_object->SetInternalField(index, External::New(isolate_, data));
	}

	EXPORT int CALLINGCONVENTION V8_Object_HasProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->Has(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT int CALLINGCONVENTION V8_Object_DeleteProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->Delete(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT int CALLINGCONVENTION V8_Object_ForceDeleteProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->ForceDelete(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT int CALLINGCONVENTION V8_Object_HasElement(void* value, uint32_t index){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->Has(index);
	}

	EXPORT int CALLINGCONVENTION V8_Object_DeleteElement(void* value, uint32_t index){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->Delete(index);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetPropertyNames(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Array> local_res = Local<Object>::Cast(local_value)->GetPropertyNames();

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return (void*) jsresvalue;
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetOwnPropertyNames(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Array> local_res = Local<Object>::Cast(local_value)->GetOwnPropertyNames();

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return (void*) jsresvalue;
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetPrototype(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> local_res = Local<Object>::Cast(local_value)->GetPrototype();

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return (void*) jsresvalue;
	}

	EXPORT int CALLINGCONVENTION V8_Object_SetPrototype(void* value, void* proto){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsValue *jsprotovalue = static_cast<JsValue*>(proto);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> local_proto = Local<Value>::New(isolate_, jsprotovalue->value_);

		return Local<Object>::Cast(local_value)->SetPrototype(local_proto);
	}

	EXPORT int CALLINGCONVENTION V8_Object_GetPropertyAttributes(void* value, const char* key, int keylength) {
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();
		//VALUE_SCOPE(value);
		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->GetPropertyAttributes(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT int CALLINGCONVENTION V8_Object_SetHiddenValue(void* value, const char* key, int keylength ,void* hidvalue){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsValue *jshidvalue = static_cast<JsValue*>(hidvalue);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> local_hidvalue = Local<Value>::New(isolate_, jshidvalue->value_);

		return Local<Object>::Cast(local_value) -> SetHiddenValue(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength),
			local_hidvalue
		);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetHiddenValue(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> local_res =  Local<Object>::Cast(local_value)->GetHiddenValue(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return (void*) jsresvalue;
	}

	EXPORT int CALLINGCONVENTION V8_Object_DeleteHiddenValue(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value) -> DeleteHiddenValue(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetConstructorName(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<String> local_res = Local<Object>::Cast(local_value)->GetConstructorName();

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return (void*) jsresvalue;
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetRealNamedProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		JsEngine *jsengine = jsvalue->GetEngine();
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<Value> local_res = Local<Object>::Cast(local_value)->GetRealNamedProperty(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);

		JsValue *jsresvalue = JsValue::New(jsengine, local_res);
		return jsresvalue;
	}

	EXPORT int CALLINGCONVENTION V8_Object_HasRealNamedProperty(void* value, const char* key, int keylength){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->HasRealNamedProperty(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength)
		);
	}

	EXPORT void CALLINGCONVENTION V8_Object_SetAlignedPointerInInternalField(void* value, int index, void* ptr){
			JsValue *jsvalue = static_cast<JsValue*>(value);
			Isolate *isolate_ = jsvalue->GetIsolate();
			//VALUE_SCOPE(value);
			//Local<Object> obj = Local<Object>::Cast(local_value);
			//obj->SetAlignedPointerInInternalField(index, value_ptr);
			HANDLE_SCOPE(isolate_);
			Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
			Local<Object>::Cast(local_value)->SetAlignedPointerInInternalField(index, ptr);
	}

	EXPORT void* CALLINGCONVENTION V8_Object_GetAlignedPointerFromInternalField(void* value, int index) {
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();
		//VALUE_SCOPE(value);
		//Local<Object> obj = Local<Object>::Cast(local_value);
		//return obj->GetAlignedPointerFromInternalField(index);
		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<Object>::Cast(local_value)->GetAlignedPointerFromInternalField(index);
	}

	EXPORT char* CALLINGCONVENTION V8_RegExp_Pattern(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<String> pattern = Local<RegExp>::Cast(local_value)->GetSource();
		char* str = (char*)malloc(pattern->Length() + 1);
		pattern->WriteUtf8(str);
		return str;
	}

	EXPORT int CALLINGCONVENTION V8_RegExp_Flags(void* value){
		JsValue *jsvalue = static_cast<JsValue*>(value);
		Isolate *isolate_ = jsvalue->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		return Local<RegExp>::Cast(local_value)->GetFlags();
	}


	EXPORT void CALLINGCONVENTION V8_FunctionCallback(const FunctionCallbackInfo<Value>& info) {
		Isolate* isolate_ = info.GetIsolate();
		//ISOLATE_SCOPE(isolate_);
		HANDLE_SCOPE(isolate_);

		Local<Array> callback_data = Local<Array>::Cast(info.Data());

		JsEngine *jsengine = (JsEngine*)Local<External>::Cast(callback_data->Get(0))->Value();
		JsFunctionCallbackInfo *jsfunctioncallbackinfo = new JsFunctionCallbackInfo(jsengine, &info);

		/*V8_FunctionCallbackInfo callback_info;
		callback_info.engine = (V8_Context*)Local<External>::Cast(callback_data->Get(0))->Value();
		callback_info.info = &info;
		callback_info.returnValue = NULL;*/

		void* callback = Local<External>::Cast(callback_data->Get(1))->Value();
		void* data = Local<External>::Cast(callback_data->Get(2))->Value();
		//void* context_ptr = V8_Current_ContextPtr(isolate);
		void* context_ptr = NULL;

		//TODO: go callback function
		//go_function_callback(&callback_info, callback, context_ptr, data);

		//if (callback_info.returnValue != NULL)
			//delete callback_info.returnValue;
	}


	EXPORT void* CALLINGCONVENTION V8_NewFunction(void* engine, void* callback, void* data){
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);
		Local<Array> callback_data = Array::New(isolate_, 3);

		if (callback_data.IsEmpty())
			return NULL;

		callback_data->Set(0, External::New(isolate_, engine));
		callback_data->Set(1, External::New(isolate_, callback));
		callback_data->Set(2, External::New(isolate_, data));

		/*return new_V8_Value(V8_Current_Context(isolate),
			Function::New(isolate, V8_FunctionCallback, callback_data)
		);*/
		Local<Function> local_function = Function::New(isolate_, V8_FunctionCallback, callback_data);
		JsFunction *jsfunction = JsFunction::New(jsengine, local_function);
		return (void*)jsfunction;
	}

	EXPORT void* CALLINGCONVENTION V8_NewObjectTemplate(void* engine) {
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();
		//ENGINE_SCOPE(engine);
		//HandleScope handle_scope(isolate);
		HANDLE_SCOPE(isolate_);

		Local<ObjectTemplate> otmp = ObjectTemplate::New(isolate_);
		if (otmp.IsEmpty())
			return NULL;

		JsObjectTemplate *jsobjecttemplate = new JsObjectTemplate(jsengine, otmp);
		return (void*) jsobjecttemplate;
		//return (void*)new V8_ObjectTemplate(the_engine, tpl);
	}

	EXPORT void* CALLINGCONVENTION V8_ObjectTemplate_NewInstance(void* engine, void* otmp) {
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ =jsengine->GetIsolate();
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		//OBJECT_TEMPLATE_SCOPE(tpl);
		//V8_Context* the_engine = static_cast<V8_Context*>(engine);
		//return new_V8_Value(the_engine, local_template->NewInstance());
		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);
		JsValue *jsvalue = JsValue::New(jsengine, local_template->NewInstance());
		return (void*) jsvalue;
	}

	EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetProperty(void* otmp, const char* key, int keylength, void* propvalue, int attribs) {
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		Isolate *isolate_ = jsobjecttemplate->isolate_;
		JsValue *jsvalue = static_cast<JsValue*>(propvalue);
		//OBJECT_TEMPLATE_SCOPE(tpl);
		/*local_template->Set(
			String::NewFromUtf8(isolate, key, String::kInternalizedString, key_length),
			Local<Value>::New(isolate, static_cast<V8_Value*>(prop_value)->self)
		);*/
		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);
		Local<Value> local_value = Local<Value>::New(isolate_, jsvalue->value_);
		Local<String> local_key = String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength);
		local_template->Set(local_key, local_value);
	}

	EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetInternalFieldCount(void* otmp, int count) {
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		Isolate *isolate_ = jsobjecttemplate->isolate_;
		//OBJECT_TEMPLATE_HANDLE_SCOPE(tpl);
		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);
		local_template->SetInternalFieldCount(count);
		//local_template->SetInternalFieldCount(count);
	}

	EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetNamedPropertyHandler(
		void* otmp,
		void* getter,
		void* setter,
		void* query,
		void* deleter,
		void* enumerator,
		void* data
){
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		Isolate *isolate_ = jsobjecttemplate->isolate_;

		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);

		Handle<Array> callbackData_ = Array::New(isolate_, OTP_Num);
		callbackData_->Set(OTP_Context, External::New(isolate_, (void*)jsobjecttemplate->engine_));
		callbackData_->Set(OTP_Getter, External::New(isolate_, getter));
		callbackData_->Set(OTP_Setter, External::New(isolate_, setter));
		callbackData_->Set(OTP_Query, External::New(isolate_, query));
		callbackData_->Set(OTP_Deleter, External::New(isolate_, deleter));
		callbackData_->Set(OTP_Enumerator, External::New(isolate_, enumerator));
		callbackData_->Set(OTP_Data, External::New(isolate_, data));

		local_template->SetNamedPropertyHandler(
			ManageNamePropertyGetCallback,
			setter == NULL ? NULL : ManageNamePropertySetCallback,
			query == NULL ? NULL : ManageNamePropertyQueryCallback,
			deleter == NULL ? NULL : ManageNamePropertyDeleteCallback,
			enumerator == NULL ? NULL : ManageNamePropertyEnumerateCallback,
			callbackData_
		);
	}

	EXPORT void CALLINGCONVENTION  V8_ObjectTemplate_SetIndexedPropertyHandler(
		void* otmp,
		void* getter,
		void* setter,
		void* query,
		void* deleter,
		void* enumerator,
		void* data
	){
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		Isolate *isolate_ = jsobjecttemplate->isolate_;

		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);

		Handle<Array> callbackData_ = Array::New(isolate_, OTP_Num);
		callbackData_->Set(OTP_Context, External::New(isolate_, (void*)jsobjecttemplate->engine_));
		callbackData_->Set(OTP_Getter, External::New(isolate_, getter));
		callbackData_->Set(OTP_Setter, External::New(isolate_, setter));
		callbackData_->Set(OTP_Query, External::New(isolate_, query));
		callbackData_->Set(OTP_Deleter, External::New(isolate_, deleter));
		callbackData_->Set(OTP_Enumerator, External::New(isolate_, enumerator));
		callbackData_->Set(OTP_Data, External::New(isolate_, data));

		local_template->SetIndexedPropertyHandler(
			ManageIndexPropertyGetCallback,
			setter == NULL ? NULL : ManageIndexPropertySetCallback,
			query == NULL ? NULL : ManageIndexPropertyQueryCallback,
			deleter == NULL ? NULL : ManageIndexPropertyDeleteCallback,
			enumerator == NULL ? NULL : ManageIndexPropertyEnumCallback,
	 		callbackData_
		);
	}

	EXPORT void CALLINGCONVENTION  V8_ObjectTemplate_SetAccessor(
		void *otmp,
		const char* key,
		int keylength,
		void* getter,
		void* setter,
		void* data,
		int attribs
	) {
		JsObjectTemplate *jsobjecttemplate = static_cast<JsObjectTemplate*>(otmp);
		Isolate *isolate_ = jsobjecttemplate->isolate_;

		HANDLE_SCOPE(isolate_);
		Local<ObjectTemplate> local_template = Local<ObjectTemplate>::New(isolate_, jsobjecttemplate->value_);

		Handle<Array> callbackData_ = Array::New(isolate_, 5);
		callbackData_->Set(OTA_Context, External::New(isolate_, (void*)jsobjecttemplate->engine_));
		callbackData_->Set(OTA_Getter, External::New(isolate_, getter));
		callbackData_->Set(OTA_Setter, External::New(isolate_, setter));
		callbackData_->Set(OTA_KeyString, External::New(isolate_, (void*)key));
		callbackData_->Set(OTA_KeyLength, Integer::New(isolate_, keylength));
		callbackData_->Set(OTA_Data, External::New(isolate_, data));

		if (callbackData_.IsEmpty())
			return;

		local_template->SetAccessor(
			String::NewFromUtf8(isolate_, key, String::kInternalizedString, keylength),
			ManageAccessorGetterCallback,
			setter == NULL ? NULL : ManageAccessorSetterCallback,
	 		callbackData_
		);
	}

	EXPORT void  CALLINGCONVENTION V8_ObjectTemplate_Dispose(void* otmp) {
		delete static_cast<JsObjectTemplate*>(otmp);
	}

	/*EXPORT void* CALLINGCONVENTION V8_Engine_Compile(void* engine, const char* code, int length, void* goscriptorigin) {
		JsEngine *jsengine = static_cast<JsEngine*>(engine);
		Isolate *isolate_ = jsengine->GetIsolate();

		HANDLE_SCOPE(isolate_);

		//TODO: extract to origin function
		ScriptOrigin script_origin(String::NewFromUtf8(isolate_, ""));
		if(goscriptorigin){
			char * cstr = go_script_origin_get_name(goscriptorigin);
			int line    = go_script_origin_get_line(goscriptorigin);
			int column  = go_script_origin_get_column(goscriptorigin);

			script_origin = ScriptOrigin(
				String::NewFromUtf8(isolate_, cstr),
				Integer::New(isolate_, line),
				Integer::New(isolate_, column)
			);
			free(cstr);
		}

		jsvalue v;

		ScriptCompiler::Source source(
			String::NewFromUtf8(isolate_, code, String::kInternalizedString, length),
			script_origin
		);

		Local<UnboundScript> script = ScriptCompiler::CompileUnbound(isolate_, &source);
		if (script.IsEmpty())
			return NULL;


	}*/
}
