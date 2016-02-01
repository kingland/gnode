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

package gnode
/*
#cgo pkg-config: gnode.pc
#include "gnode.h"
#include <stdlib.h>
*/
import "C"
import "unsafe"
import "runtime"
import "reflect"

type Engine struct {
  embedable
  self                    unsafe.Pointer
  disposed                bool

  //TODO: static value
  undefinedValue          *Value
  nullValue               *Value
  trueValue               *Value
  falseValue              *Value

  //TODO: Context ID
  //contextId               int
  //scriptId                int
  //unboundScriptId         int
  //valueId                 int

  //TODO:
  aliveContext            map[int]*Context
  aliveScript             map[int]*Script
  aliveUnboundScript      map[int] *UnboundScript
  aliveValue              map[int32]*Value
}

var (
  traceDispose bool = true
  contextId int = 0
  scriptId int = 0
  unboundScriptId int = 0
  valueId int32 = 0
)

//create engine
func NewEngine() *Engine {
  self_ := C.V8_NewEngine(-1,-1)
  engine_ := &Engine{
    self : self_,
    disposed : false,
    aliveContext : make(map[int]*Context),
    aliveScript : make(map[int]*Script),
    aliveUnboundScript : make(map[int] *UnboundScript),
    aliveValue : make(map[int32]*Value),
    //contextId : 0,
    //scriptId : 0,
    //unboundScriptId : 0,
    //valueId : 0,
  }
  engine_.SetOwner()
  runtime.SetFinalizer(engine_, func(e *Engine) {
    if traceDispose {
      println("V8_Engine_Dispose()", e.self)
    }
    if !e.disposed {
      C.V8_Engine_Dispose(e.self)
    }
  })
  return engine_
}

func (engine_ *Engine) SetOwner() {
  C.V8_Go_Engine(engine_.self, unsafe.Pointer(engine_))
}

func (engine_ *Engine) IsDisposed() bool{
  return engine_.disposed
}

func (engine_ *Engine) Disposed(forceAll bool) {
  engine_.checkDisposedException()
  if forceAll {
    //TODO: Disposed context
    /*foreach (var aliveContext in _aliveContexts) {
					JsContext.jscontext_dispose(aliveContext.Value.Handle);
    }
		_aliveContexts.Clear();

    //TODO: Disposed script
		foreach (var aliveScript in _aliveScripts) {
      JsScript.jsscript_dispose(aliveScript.Value.Handle);
		}*/

    /*for _ , ctx_ := range engine_.aliveContext {
      ctx_.Disposed()
    }
    for _, scp_ := range engine_.aliveScript {
      scp_.Disposed()
    }
    for _, uscp_ := range engine_.aliveUnboundScript {
      uscp_.Disposed()
    }
    for _, val_ := range engine_.aliveValue {
      val_.Disposed();
    }*/
  }

  if !engine_.disposed {
    C.V8_Engine_Dispose(engine_.self)
  }
  engine_.disposed = true
}

func (engine_ *Engine) checkDisposedException(){
  if engine_.disposed {
    //throw new ObjectDisposedException("JsEngine:" + _engine.Handle);
    panic("JsEngine Disposed Exception")
  }
}

func (engine_ *Engine) TerminateExecution()  {
  C.V8_Engine_TerminateExecution(engine_.self)
}

/*
TODO:
func (engine_ *Engine) DumpHeapStats()  {
  C.V8_Engine_DumpHeapStats(engine_.self)
}

//TODO: DumpAllocatedItems
func (engine_ *Engine) DumpAllocatedItems()  {
}
*/

func (engine_ *Engine) DisposeValue(val *Value){
  if !val.IsDisposed() {
    val.Disposed();
  }
}

func StringToPointer(value string) unsafe.Pointer {
  valPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&value)).Data)
  return valPtr
}

func ByteToPointer(value []byte) unsafe.Pointer{
  valPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&value)).Data)
  return valPtr
}

//export go_panic
func go_panic(message *C.char) {
	panic(C.GoString(message))
}

//export go_keepalive_value_remove
func go_keepalive_value_remove(engine unsafe.Pointer, context, id int) {
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ != nil {
    engine_ := context_.GetEngine()
    val_ := engine_.GetAliveValue(id)
    val_.Disposed()
  }*/
}

//export go_keepalive_object_remove
func go_keepalive_object_remove(engine unsafe.Pointer, context, id int){
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ != nil {
    engine_ := context_.GetEngine()
    val_ := engine_.GetAliveValue(id)
    val_.Disposed()
  }*/
}

//export go_keepalive_function_remove
func go_keepalive_function_remove(engine unsafe.Pointer, context, id int){
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ != nil {
    engine_ := context_.GetEngine()
    val_ := engine_.GetAliveValue(id)
    val_.Disposed()
  }*/
}

//export go_keepalive_remove
func go_keepalive_remove(engine unsafe.Pointer, context, id int){
  //TODO:
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
}

//export go_keepalive_get_property_value
func go_keepalive_get_property_value(engine unsafe.Pointer, context int, id int, name *C.char){
  //TODO: return  unsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveGetPropertyValue(id, C.GoString(name))
}

//export go_keepalive_set_property_value
func go_keepalive_set_property_value(engine unsafe.Pointer, context int, id int, name *C.char, value *C.jsvalue) {
  //TODO: return unsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveSetPropertyValue(id, C.GoString(name), value)
}

//export go_keepalive_valueof
func go_keepalive_valueof(engine unsafe.Pointer, context, id int) {
  //TODO: return unsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveValueOf(id)
}

//export go_keepalive_invoke
func go_keepalive_invoke(engine unsafe.Pointer, context int, id int, args *C.jsvalue) {
  //TODO: return unsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveInvoke(id, args)
}

//export go_keepalive_delete_property
func go_keepalive_delete_property(engine unsafe.Pointer, context int, id int, name *C.char) {
  //TODO: return unsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveDeleteProperty(id, C.GoString(name))
}

//export go_keepalive_enumerate_properties
func go_keepalive_enumerate_properties(engine unsafe.Pointer, context int, id int) {
  //TODO: retru nunsafe.Pointer
  /*engine_ := (*Engine)(engine)
  context_ := engine_.aliveContext[context]
  if context_ == nil {
    panic("fail")
  }*/
  //context_.keepAliveEnumerateProperties(id)
}
