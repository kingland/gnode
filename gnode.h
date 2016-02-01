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

#ifndef LIBGNODE_H
#define LIBGNODE_H

//#include <string>
#include <stdlib.h>
#include <stdint.h>
//#include <iostream>
//#include "v8.h"

//using namespace v8;

#define JSOBJECT_MARSHAL_TYPE_DYNAMIC       1
#define JSOBJECT_MARSHAL_TYPE_DICTIONARY    2


// jsvalue (JsValue on the CLR side) is a struct that can be easily marshaled
// by simply blitting its value (being only 16 bytes should be quite fast too).

#define JSVALUE_TYPE_UNKNOWN_ERROR  -1
#define JSVALUE_TYPE_EMPTY			 		 0
#define JSVALUE_TYPE_NULL            1
#define JSVALUE_TYPE_BOOLEAN         2
#define JSVALUE_TYPE_INTEGER         3
#define JSVALUE_TYPE_NUMBER          4
#define JSVALUE_TYPE_STRING          5
#define JSVALUE_TYPE_DATE            6
#define JSVALUE_TYPE_INDEX           7
#define JSVALUE_TYPE_ARRAY          10
#define JSVALUE_TYPE_STRING_ERROR   11
#define JSVALUE_TYPE_MANAGED        12
#define JSVALUE_TYPE_MANAGED_ERROR  13
#define JSVALUE_TYPE_WRAPPED        14
#define JSVALUE_TYPE_DICT           15
#define JSVALUE_TYPE_ERROR          16
#define JSVALUE_TYPE_FUNCTION       17
//#define JSVALUE_TYPE_PERSISTENT		 18

#define IDLE_TIME_IN_MS 1000

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#ifdef _WIN32
#include "Windows.h"
#define CALLINGCONVENTION __stdcall
#define INCREMENT(x) InterlockedIncrement(&x)
#define DECREMENT(x) InterlockedDecrement(&x)
#else
#define CALLINGCONVENTION
#define INCREMENT(x) __sync_fetch_and_add(&x, 1)
#define DECREMENT(x) __sync_fetch_and_add(&x, -1)
#endif


//int32_t js_object_marshal_type;
//long js_mem_debug_engine_count;
//long js_mem_debug_context_count;
//long js_mem_debug_managedref_count;
//long js_mem_debug_script_count;

#define ISOLATE_SCOPE(isolate_ptr) \
	Locker locker(isolate_ptr); \
	Isolate::Scope isolate_scope(isolate_ptr) \

#define HANDLE_SCOPE(isolate_ptr) \
  ISOLATE_SCOPE(isolate_ptr); \
  HandleScope handle_scope(isolate_ptr); \

#ifdef __cplusplus
extern "C" {
#endif
	typedef enum {
		OTP_Context = 0,
		OTP_Getter,
		OTP_Setter,
		OTP_Query,
		OTP_Deleter,
		OTP_Enumerator,
		OTP_Data,
		OTP_Num
	} V8_PropertyDataEnum;

	typedef enum {
    OTA_Context = 0,
    OTA_Getter,
    OTA_Setter,
    OTA_KeyString,
    OTA_KeyLength,
    OTA_Data,
    OTA_Num
	} V8_AccessorDataEnum;

	typedef struct {
	  void*     engine;
	  void*     info;
	  void*     callback;
	  void*     setValue;
	  void*     data;
	  char*	  	key;
		uint32_t  index;
	  void*     returnValue;
		int32_t 	contextId;
		int32_t		id;
	} V8_PropertyCallbackInfo;

	typedef struct {
    void*        engine;
    void*        info;
    void*        setValue;
    const char*  key;
    int          key_length;
    void*        data;
    void*        callback;
    void*        returnValue;
		int32_t 	contextId;
		int32_t		id;
} V8_AccessorCallbackInfo;

	typedef struct{
	  void*   engine;
	  void*   host;
	  void*   key;
	  uint32_t  index;
	  void*   data;
	  void*   callback;
		int32_t 	contextId;
		int32_t		id;
	} V8_AccessCheckCallbackInfo;

	typedef struct jsvalue
	{
        // 8 bytes is the maximum CLR alignment; by putting the union first and a
        // int64_t inside it we make (almost) sure the offset of 'type' will always
        // be 8 and the total size 16. We add a check to JsContext_new anyway.
        union
        {
            int32_t     i32;
            int64_t     i64;
            double      num;
            void       *ptr;
            char       *str;
            struct jsvalue    *arr;
        } value;

        int32_t         type;
        int32_t         length; // Also used as slot index on the CLR side.
	} jsvalue;

	typedef struct
	{
		jsvalue type;
		int32_t line;
		int32_t column;
		jsvalue resource;
		jsvalue message;
		jsvalue exception;
	} jserror;

	EXPORT void CALLINGCONVENTION jsvalue_dispose(jsvalue value);
	 // We don't have a keepalive_add_f because that is managed on the managed side.
    // Its definition would be "int (*keepalive_add_f) (ManagedRef obj)".

	typedef void (CALLINGCONVENTION *keepalive_remove_f) (void* engine, int context, int id);
  typedef jsvalue (CALLINGCONVENTION *keepalive_get_property_value_f) (void* engine, int context, int id, const char* name);
  typedef jsvalue (CALLINGCONVENTION *keepalive_set_property_value_f) (void* engine, int context, int id, const char* name, jsvalue value);
  typedef jsvalue (CALLINGCONVENTION *keepalive_valueof_f) (void* engine,int context, int id);
	typedef jsvalue (CALLINGCONVENTION *keepalive_invoke_f) (void* engine,int context, int id, jsvalue args);
	typedef jsvalue (CALLINGCONVENTION *keepalive_delete_property_f) (void* engine,int context, int id, const char* name);
	typedef jsvalue (CALLINGCONVENTION *keepalive_enumerate_properties_f) (void* engine,int context, int id);

	EXPORT void* CALLINGCONVENTION V8_NewEngine(
		int32_t max_young_space,
		int32_t max_old_space
	);

	EXPORT void CALLINGCONVENTION V8_Go_Engine(void* engine, void* prt);

	EXPORT void CALLINGCONVENTION V8_Engine_TerminateExecution(void* engine);

	//EXPORT void CALLINGCONVENTION V8_Engine_DumpHeapStats(void* engine);

	EXPORT void CALLINGCONVENTION V8_Engine_DumpAllocatedItems();

	EXPORT void CALLINGCONVENTION V8_Engine_Dispose(void* engine);

	//EXPORT void CALLINGCONVENTION V8_Engine_DisposeObject(void* engine, jsvalue obj);

	EXPORT void CALLINGCONVENTION V8_Engine_DisposeValue(void* value);

  //EXPORT void* CALLINGCONVENTION V8_Engine_Compile(void* engine, const char* code, int length, void* goscriptorigin) ;

	EXPORT void* CALLINGCONVENTION V8_NewContext(int id, void* engine);

	EXPORT void CALLINGCONVENTION V8_Context_ForceGC(void* context);

	EXPORT void CALLINGCONVENTION V8_Context_Dispose(void* context);

	EXPORT void* CALLINGCONVENTION V8_Context_Execute(void* context, const char* str, const char* resourceName);

	EXPORT void* CALLINGCONVENTION V8_Context_ExecuteScript(void* context, void* script);

	EXPORT void* CALLINGCONVENTION V8_Context_Global(void* context);

	EXPORT void* CALLINGCONVENTION V8_Context_SetVariable(void* context, const char* name, void* value);

	EXPORT void* CALLINGCONVENTION V8_Context_GetVariable(void* context, const char* name);

	EXPORT void* CALLINGCONVENTION V8_Context_GetPropertyValue(void* context, void* obj, const char* name);

	EXPORT void* CALLINGCONVENTION V8_Context_SetPropertyValue(void* context, void* obj, const char* name, void* value);

	EXPORT void* CALLINGCONVENTION V8_Context_GetPropertyNames(void* context, void* obj);

	EXPORT void* CALLINGCONVENTION V8_Context_InvokeProperty(void* context, void* obj, const char* name, void* args);

	EXPORT void* CALLINGCONVENTION V8_Context_Invoke(void* context, void* func, void* obj, void* args);

	EXPORT void* CALLINGCONVENTION V8_NewScript(void* engine, const char* code, void* goscriptorigin );

  EXPORT void* CALLINGCONVENTION V8_NewUnboundScript(void* engine, const char* code, void* goscriptorigin );

	EXPORT void CALLINGCONVENTION V8_Script_Dispose(void* script);

  EXPORT void CALLINGCONVENTION V8_UnboundScript_Dispose(void* script);

	//EXPORT jsvalue CALLINGCONVENTION V8_Script_Compile(void* script, const char* str, void *origin);

	EXPORT jsvalue CALLINGCONVENTION V8_Alloc_String(const char* str);

	EXPORT jsvalue CALLINGCONVENTION V8_Alloc_Array(const int32_t length);

  EXPORT const char* CALLINGCONVENTION V8_GetVersion();

  EXPORT void CALLINGCONVENTION V8_SetFlagsFromString(const char* str, int length);

	EXPORT void CALLINGCONVENTION V8_Value_Dispose(jsvalue value);

  EXPORT void* CALLINGCONVENTION V8_Undefined(void* engine);

  EXPORT void* CALLINGCONVENTION V8_Null(void* engine);

  EXPORT void* CALLINGCONVENTION V8_True(void* engine);

  EXPORT void* CALLINGCONVENTION V8_False(void* engine);

  EXPORT void* CALLINGCONVENTION V8_NewNumber(void* engine, double number);

  EXPORT void* CALLINGCONVENTION V8_NewDate(void* engine, double number);

  EXPORT void* CALLINGCONVENTION V8_NewString(void* engine, const char* str, int length);

  EXPORT void* CALLINGCONVENTION V8_NewExternal(void* engine, void* external);

  EXPORT void* CALLINGCONVENTION V8_NewObject(void* engine);

  EXPORT void* CALLINGCONVENTION V8_NewArray(void* engine, int length);

  EXPORT int CALLINGCONVENTION  V8_Array_Length(void* value);

  EXPORT void* CALLINGCONVENTION V8_NewRegExp(void* engine, const char* pattern, int length, int flags);

  EXPORT int CALLINGCONVENTION V8_Value_GetManageId(void* value);

  EXPORT void CALLINGCONVENTION V8_Value_SetManageId(void* value, int id);

  EXPORT int CALLINGCONVENTION V8_Value_IsUndefined(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsNull(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsTrue(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsFalse(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsString(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsFunction(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsArray(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsObject(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsBoolean(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsNumber(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsExternal(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsInt32(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsUint32(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsDate(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsBooleanObject(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsNumberObject(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsStringObject(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsNativeError(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_IsRegExp(void* value);

  EXPORT int CALLINGCONVENTION V8_Value_ToBoolean(void* value);

  EXPORT double CALLINGCONVENTION V8_Value_ToNumber(void* value);

  EXPORT int64_t CALLINGCONVENTION V8_Value_ToInteger(void* value);

  EXPORT uint32_t CALLINGCONVENTION V8_Value_ToUint32(void* value);

  EXPORT int32_t CALLINGCONVENTION V8_Value_ToInt32(void* value);

  EXPORT char* CALLINGCONVENTION V8_Value_ToString(void* value);

  //EXPORT int CALLINGCONVENTION V8_Value_ToBoolean(void* value);

  EXPORT int CALLINGCONVENTION V8_Object_SetProperty(void* value, const char* key, int key_length, void* prop_value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetProperty(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_SetElement(void* value, uint32_t index, void* elem_value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetElement(void* value, uint32_t index);

  EXPORT int CALLINGCONVENTION V8_Object_InternalFieldCount(void* value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetInternalField(void* value, int index);

  EXPORT void CALLINGCONVENTION V8_Object_SetInternalField(void* value, int index, void* data);

  EXPORT int CALLINGCONVENTION V8_Object_HasProperty(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_DeleteProperty(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_ForceDeleteProperty(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_HasElement(void* value, uint32_t index);

  EXPORT int CALLINGCONVENTION V8_Object_DeleteElement(void* value, uint32_t index);

  EXPORT void* CALLINGCONVENTION V8_Object_GetPropertyNames(void* value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetOwnPropertyNames(void* value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetPrototype(void* value);

  EXPORT int CALLINGCONVENTION V8_Object_SetPrototype(void* value, void* proto);

  EXPORT int CALLINGCONVENTION V8_Object_SetHiddenValue(void* value, const char* key , int keylength, void* hidvalue);

  EXPORT void* CALLINGCONVENTION V8_Object_GetHiddenValue(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_DeleteHiddenValue(void* value, const char* key, int keylength);

  EXPORT void* CALLINGCONVENTION V8_Object_GetConstructorName(void* value);

  EXPORT void* CALLINGCONVENTION V8_Object_GetRealNamedProperty(void* value, const char* key, int keylength);

  EXPORT int CALLINGCONVENTION V8_Object_HasRealNamedProperty(void* value, const char* key, int keylength);

	EXPORT void CALLINGCONVENTION V8_Object_SetAlignedPointerInInternalField(void* value, int index, void* ptr);

	EXPORT void* CALLINGCONVENTION V8_Object_GetAlignedPointerFromInternalField(void* value, int index);

	EXPORT int CALLINGCONVENTION V8_Object_GetPropertyAttributes(void* value, const char* key, int keylength);

  EXPORT char* CALLINGCONVENTION V8_RegExp_Pattern(void* value);

  EXPORT int CALLINGCONVENTION V8_RegExp_Flags(void* value);

  EXPORT void* CALLINGCONVENTION V8_NewFunction(void* engine, void* callback, void* data);

  EXPORT void* CALLINGCONVENTION V8_NewObjectTemplate(void* engine);

  EXPORT void* CALLINGCONVENTION V8_ObjectTemplate_NewInstance(void* engine, void* otmp);

  EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetProperty(void* otmp, const char* key, int keylength, void* propvalue, int attribs);

  EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetInternalFieldCount(void* otmp, int count);

	EXPORT void CALLINGCONVENTION V8_ObjectTemplate_SetNamedPropertyHandler(void* otmp, void* getter,	void* setter,	void* query,	void* deleter,	void* enumerator,	void* data);

	EXPORT void CALLINGCONVENTION  V8_ObjectTemplate_SetIndexedPropertyHandler(void* otmp, void* getter,	void* setter,	void* query,	void* deleter,	void* enumerator,	void* data);

	EXPORT void CALLINGCONVENTION  V8_ObjectTemplate_SetAccessor(void *otmp, const char* key, int keylength, void* getter, void* setter, void* data, int attribs) ;

  EXPORT void  CALLINGCONVENTION V8_ObjectTemplate_Dispose(void* otmp);

#ifdef __cplusplus
}
#endif

/*template <class TypeName>
inline v8::Local<TypeName> PersistentToLocal(
    v8::Isolate* isolate,
    const v8::Persistent<TypeName>& persistent) {
  if (persistent.IsWeak()) {
    return WeakPersistentToLocal(isolate, persistent);
  } else {
    return StrongPersistentToLocal(persistent);
  }
}

template <class TypeName>
inline v8::Local<TypeName> StrongPersistentToLocal(
    const v8::Persistent<TypeName>& persistent) {
  return *reinterpret_cast<v8::Local<TypeName>*>(
      const_cast<v8::Persistent<TypeName>*>(&persistent));
}

template <class TypeName>
inline v8::Local<TypeName> WeakPersistentToLocal(
    v8::Isolate* isolate,
    const v8::Persistent<TypeName>& persistent) {
  return v8::Local<TypeName>::New(isolate, persistent);
}*/

#endif
