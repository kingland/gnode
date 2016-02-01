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

#include "gnode.h"
#include "gnodecpp.h"

//long js_mem_debug_script_count;
extern "C" {

	JsScript *JsScript::New(JsEngine *engine, Handle<Script> script) {
		 JsScript *jsscript = new JsScript(engine, script);
		 //jsscript->engine_ = engine;
		 //jsscript->script_.Reset(engine->GetIsolate(), script);
		 return jsscript;
	}

	/*void JsScript::Compile(const char* str, ScriptOrigin *origin = NULL) {
		jsvalue v;
		v.type = 0;
		engine_->CompileScript(str, origin, &v);
		//return v;
	}*/

	void JsScript::Dispose() {
		//Isolate *isolate = engine_->GetIsolate();
		//if(isolate != NULL) {
				//Locker locker(isolate);
	   	 	//Isolate::Scope isolate_scope(isolate);
				//script_->Dispose();
	    	//delete script_;
		//}
		script_.Reset();
	}

	JsUnboundScript *JsUnboundScript::New(JsEngine* engine, Handle<UnboundScript> script){
		JsUnboundScript* jsunboundscript = new JsUnboundScript(engine, script);
		return jsunboundscript;
	}

	/*void JsUnboundScript::Compile(const char* str, ScriptOrigin *origin = NULL) {
		jsvalue v;
		v.type = 0;
		engine_->CompileScript(str, origin, &v);
		//return v;
	}*/

	void JsUnboundScript::Dispose() {
		script_.Reset();
	}
}
