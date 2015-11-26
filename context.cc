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
#include <vector>
#include "gnode.h"
#include "gnodecpp.h"

extern "C" {
using namespace v8;

//long js_mem_debug_context_count;

JsContext* JsContext::New(int id, JsEngine *engine)
{
    //JsContext* context_ = new JsContext(id, engine, context);
    //if (context != NULL) {
  		//context->id_ = id;
  		//context->engine_ = engine;
  		//context->isolate_ = engine->GetIsolate();
      Isolate* isolate_ = engine->GetIsolate();

  		/*Locker locker(engine->GetIsolate());
      Isolate::Scope isolate_scope(engine->GetIsolate());
      HandleScope scope(engine->GetIsolate());*/

      HANDLE_SCOPE(isolate_);
      JsFunctionTemplate *ftmp_ = engine->GetManageFtmp();
      Local<FunctionTemplate> local_ftmp = Local<FunctionTemplate>::New(isolate_, ftmp_->value_);
      //Handle<FunctionTemplate> local_ftmp = FunctionTemplate::New(engine->GetIsolate());
      Local<ObjectTemplate> local_otmp = local_ftmp->InstanceTemplate();
      Local<Context> local_context = Context::New(isolate_, NULL, local_otmp);
  		//Persistent<Context> pc(context->isolate_, Context::New(context->isolate_));
      //std::wcout << "5:: "  << std::endl;
  		//context_->context_.Reset(context_->isolate_, local_context);
  		//context->context_ = new Persistent<Context>(Context::New(context->isolate_));
	 //}
   JsContext* context_ = new JsContext(id, engine, local_context);
    return context_;
}

void JsContext::Dispose()
{
	//if(engine_->GetIsolate() != NULL) {
		//Locker locker(isolate_);
   	//Isolate::Scope isolate_scope(isolate_);
		//Local<Context> local_context = Local<Context>::New(isolate_, *context_);
		//local_context->Dispose();
    //delete context_;
	//}
    context_.Reset();
}

void JsContext::IdleNotification()
{
	if(engine_->GetIsolate() != NULL) {
		//Locker locker(isolate_);
		//Isolate::Scope isolate_scope(isolate_);
    ISOLATE_SCOPE(isolate_);
		while(!isolate_->IdleNotification(IDLE_TIME_IN_MS)) {};
	}
}

JsValue* JsContext::Execute(const char* str, const char* resourceName = NULL)
{
    //jsvalue v;
    JsValue *v;
      /*Locker locker(isolate_);
      Isolate::Scope isolate_scope(isolate_);
      HandleScope handle_scope(isolate_);*/
      HANDLE_SCOPE(isolate_);

      //std::wcout << "JsContext::Execute 1:: "  << std::endl;
  		Local<Context> local_context = Local<Context>::New(isolate_, context_);
      //Handle<ObjectTemplate> global = ObjectTemplate::New(isolate_);
      //Local<Context> local_context = Context::New(isolate_, NULL, global);
      //Context::Scope context_scope(local_context);
      //HandleScope scope(isolate_);

      //Context::Scope context_scope(local_context);
      //HandleScope scope(isolate_);
  		local_context->Enter();
      //(*context_)->Enter();
      TryCatch trycatch;

      //Handle<String> source =  String::NewFromUtf8(isolate_, "'Hello' + ', World!'", String::kNormalString );
  		Local<String> source = String::NewFromUtf8(isolate_, str, String::kInternalizedString);


      ScriptOrigin scriptorigin = ScriptOrigin(
  			String::NewFromUtf8(isolate_, "cstr"),
  			Integer::New(isolate_, 0),
  			Integer::New(isolate_, 0)
  		);

      /*ScriptCompiler::Source scriptsource(
    		String::NewFromUtf8(isolate_, "'Hello' + ', World!'", String::kNormalString),
    		scriptorigin
    	);*/

      ScriptCompiler::Source scriptsource(source);
    	//Handle<UnboundScript> ssssscript = ScriptCompiler::CompileUnbound(isolate_, &scriptsource);
      ///std::wcout << "JsContext::Execute 2:: "  << std::endl;
  		//Handle<Script> script = ScriptCompiler::Compile(isolate_, &scriptsource);

      Local<Script> script = Script::Compile(source, &scriptorigin);
  		/*if (resourceName != NULL) {
  			//Handle<String> name = String::New(resourceName);
  			Handle<String> name = String::NewFromUtf8(isolate_, resourceName, String::kInternalizedString);
  			script = Script::Compile(source, name);
  		} else {
  			script = Script::Compile(source);
  		}*/
      //std::wcout << "JsContext::Execute 3:: "  << std::endl;
  		if (!script.IsEmpty()) {
        //std::wcout << "JsContext::Execute 4:: "  << std::endl;
  			Local<Value> result = script->Run();
  			if (result.IsEmpty()){
            //TODO:
            //v = engine_->ErrorFromV8(trycatch);
            //std::wcout << "JsContext::Execute 5:: "  << std::endl;
        }
        else{
            //v = engine_->AnyFromV8(result);
              v = new JsValue(engine_, result);
              //std::wcout << "JsContext::Execute 6:: "  << std::endl;
        }
      }
      else {
        //TODO:
          //v = engine_->ErrorFromV8(trycatch);
          //std::wcout << "JsContext::Execute 7:: "  << std::endl;
      }

      //(*context_)->Exit();
  		local_context->Exit();
      //std::wcout << "JsContext::Execute 8:: "  << std::endl;
	return v;
}

JsValue* JsContext::Execute(JsScript *jsscript)
{
    //jsvalue v;
    JsValue* v;
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

		//Persistent<Script> script = jsscript->GetScript();
		Local<Script> local_script = Local<Script>::New(isolate_, jsscript->script_);

		if (!local_script.IsEmpty()) {
			Local<Value> result = local_script->Run();

			if (result.IsEmpty()){
        //TODO:
				//v = engine_->ErrorFromV8(trycatch);
      }
			else{
				//v = engine_->AnyFromV8(result);
        v = new JsValue(engine_, result);
      }
		}

    //(*context_)->Exit();
		local_context->Exit();
	return v;
}

JsValue* JsContext::SetVariable(const char* name, jsvalue value)
{
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();
    Local<Value> v_ = engine_->AnyToV8(value, id_);
    if (local_context->Global()->Set(String::NewFromUtf8(isolate_, name, String::kInternalizedString), v_) == false) {
        // TODO: Return an error if set failed.
    }

    //(*context_)->Exit();
		local_context->Exit();
    //return engine_->AnyFromV8(Null(isolate_));
    JsValue* v = new JsValue(engine_, Null(isolate_));
    return v;
}

JsValue* JsContext::GetGlobal() {
	 //jsvalue v;

    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

    Local<Value> value = local_context->Global();
    JsValue* v = NULL;
    if (!value.IsEmpty()) {
        //v = engine_->AnyFromV8(value);
        v = new JsValue(engine_, value);
    }
    else {
        //v = engine_->ErrorFromV8(trycatch);
        //TODO:
    }
    //(*context_)->Exit();
		local_context->Exit();
    return v;
}

JsValue* JsContext::GetVariable(const char* name)
{
    //jsvalue v;
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

    Local<Value> value_ = local_context->Global()->Get(String::NewFromUtf8(isolate_, name, String::kInternalizedString));
    JsValue* v;
    if (!value_.IsEmpty()) {
        //v = engine_->AnyFromV8(value);
        v = new JsValue(engine_, value_);
    }
    else {
      //TODO::
        //v = engine_->ErrorFromV8(trycatch);
    }

    //(*context_)->Exit();
		local_context->Exit();

    return v;
}

JsValue* JsContext::GetPropertyNames(Persistent<Object>* obj) {
	  //jsvalue v;
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

		Local<Object> local_object = Local<Object>::New(isolate_, *obj);
    Local<Value> value_ = local_object->GetPropertyNames();

    JsValue* v = NULL;
    if (!value_.IsEmpty()) {
        //v = engine_->AnyFromV8(value);
        v = new JsValue(engine_, value_);
    } else {
      //TODO::
      //v = engine_->ErrorFromV8(trycatch);
    }
    //(*context_)->Exit();
		local_context->Exit();

    return v;
}

JsValue* JsContext::GetPropertyValue(Persistent<Object>* obj, const char* name)
{
    //jsvalue v;
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();
    //(*context_)->Enter();

    TryCatch trycatch;

		Local<Object> local_object = Local<Object>::New(isolate_, *obj);
    Local<Value> value_ = local_object->Get(String::NewFromUtf8(isolate_, name, String::kInternalizedString) /*String::New(name)*/);

    JsValue* v = NULL;
    if (!value_.IsEmpty()) {
        //v = engine_->AnyFromV8(value);
        v = new JsValue(engine_, value_);
    }
    else {
        //v = engine_->ErrorFromV8(trycatch);
    }
    //(*context_)->Exit();
		local_context->Exit();

    return v;
}


JsValue* JsContext::SetPropertyValue(Persistent<Object>* obj, const char* name, jsvalue value)
{
    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    Handle<Value> v_ = engine_->AnyToV8(value, id_);
		Local<Object> local_object = Local<Object>::New(isolate_, *obj);
    if (local_object->Set(String::NewFromUtf8(isolate_, name, String::kInternalizedString) /*String::New(name)*/, v_) == false) {
        // TODO: Return an error if set failed.
    }

    //(*context_)->Exit();
		local_context->Exit();
    //return engine_->AnyFromV8(Null(isolate_));
    JsValue* v = new JsValue(engine_, Null(isolate_));
    return v;
}

JsValue* JsContext::InvokeFunction(Persistent<Function>* func, Persistent<Object>* thisArg, jsvalue args) {
		//jsvalue v;

    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

		//Local<Value> prop = *(*func);
		Local<Function> prop = Local<Function>::New(isolate_, *func);
    if (prop.IsEmpty() || !prop->IsFunction()) {
        //v = engine_->StringFromV8(String::NewFromUtf8(isolate_, "isn't a function", String::kInternalizedString) /*String::New("isn't a function")*/);
        //v.type = JSVALUE_TYPE_STRING_ERROR;
        //TODO::
    }

		//Local<Object> reciever = *(*thisArg);
		Local<Object> reciever = Local<Object>::New(isolate_, *thisArg);
    JsValue* v = NULL;
		if (reciever.IsEmpty()) {
			//reciever = (*context_)->Global();
			reciever = local_context->Global();
		}

    else {
        std::vector<Local<Value> > argv(args.length);
        engine_->ArrayToV8Args(args, id_, &argv[0]);
        // TODO: Check ArrayToV8Args return value (but right now can't fail, right?)
        Local<Function> func = Local<Function>::Cast(prop);
				Local<Value> value = func->Call(reciever, args.length, &argv[0]);
        if (!value.IsEmpty()) {
            //v = engine_->AnyFromV8(value);
            v = new JsValue(engine_, value);
        }
        else {
            //v = engine_->ErrorFromV8(trycatch);
            //TODO:
        }
    }

    //(*context_)->Exit();
		local_context->Exit();

    return v;

}

JsValue* JsContext::InvokeProperty(Persistent<Object>* obj, const char* name, jsvalue args)
{
    //jsvalue v;
    JsValue* v;

    //Locker locker(isolate_);
    //Isolate::Scope isolate_scope(isolate_);
    //(*context_)->Enter();
    HANDLE_SCOPE(isolate_);
		Local<Context> local_context = Local<Context>::New(isolate_, context_);
		local_context->Enter();

    TryCatch trycatch;

		Local<Object> local_object = Local<Object>::New(isolate_, *obj);
    Local<Value> prop = local_object->Get(String::NewFromUtf8(isolate_, name, String::kInternalizedString) /*String::New(name)*/);
    if (prop.IsEmpty() || !prop->IsFunction()) {
        //v = engine_->StringFromV8(String::NewFromUtf8(isolate_, "property not found or isn't a function", String::kInternalizedString));
        //v.type = JSVALUE_TYPE_STRING_ERROR;
        //TODO::
    }
    else {
        //std::vector<Local<Value> > argv(args.length);
				Handle<Value>* real_argv = new Handle<Value>[args.length];
        engine_->ArrayToV8Args(args, id_, real_argv);
        // TODO: Check ArrayToV8Args return value (but right now can't fail, right?)
        Local<Function> func = Local<Function>::Cast(prop);
        Local<Value> value = func->Call(local_object, args.length, real_argv);

        if (!value.IsEmpty()) {
            //v = engine_->AnyFromV8(value);
            v = new JsValue(engine_, value);
        }
        else {
            //v = engine_->ErrorFromV8(trycatch);
            //TODO::
        }
    }

    //(*context_)->Exit();
		local_context->Exit();

    return v;
}

}
