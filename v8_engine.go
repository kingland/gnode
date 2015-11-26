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
  self unsafe.Pointer
  disposed bool

  //TODO: static value
  undefinedValue *Value
  nullValue *Value
  trueValue *Value
  falseValue *Value

  //TODO: Context ID
  contextId int
  scriptId int
}

var traceDispose = true
var aliveContext map[int]*Context = make(map[int]*Context)

//create engine
func NewEngine() *Engine {
  self_ := C.V8_NewEngine(-1,-1)

  engine := &Engine{
    self : self_,
    disposed : false,
    //mapContext : make(map[int32]*Context),
    contextId : 0,
  }

  runtime.SetFinalizer(engine, func(e *Engine) {
    if traceDispose {
      println("V8_Engine_Dispose()", e.self)
    }
    if !e.disposed {
      C.V8_Engine_Dispose(e.self)
    }
  })
  return engine
}

//map context id
func (engine_ *Engine) GetAliveContext(id int) *Context{
  return aliveContext[id]
}

//map context id
func (engine_ *Engine) SetAliveContext(id int, context *Context){
  aliveContext[id] = context
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

func (engine_ *Engine) DumpHeapStats()  {
  C.V8_Engine_DumpHeapStats(engine_.self)
}

func (engine_ *Engine) DumpAllocatedItems()  {
  //TODO:
}

func (engine_ *Engine) DisposeObject(){
  //TODO:
  if engine_.IsDisposed() {
    //C.V8_Engine_DisposeObject(engine.self,)
  } else {
    //C.V8_Engine_DisposeObject(engine.self,)
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

//export go_keepalive_remove
func go_keepalive_remove(context, id int){
  //TODO:
}

//export go_keepalive_get_property_value
func go_keepalive_get_property_value(context int, id int, name *C.char){
  //TODO: return  unsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveGetPropertyValue(id, C.GoString(name))
}

//export go_keepalive_set_property_value
func go_keepalive_set_property_value(context int, id int, name *C.char, value *C.jsvalue) {
  //TODO: return unsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveSetPropertyValue(id, C.GoString(name), value)
}

//export go_keepalive_valueof
func go_keepalive_valueof(context, id int) {
  //TODO: return unsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveValueOf(id)
}

//export go_keepalive_invoke
func go_keepalive_invoke(context int, id int, args *C.jsvalue) {
  //TODO: return unsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveInvoke(id, args)
}

//export go_keepalive_delete_property
func go_keepalive_delete_property(context int, id int, name *C.char) {
  //TODO: return unsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveDeleteProperty(id, C.GoString(name))
}

//export go_keepalive_enumerate_properties
func go_keepalive_enumerate_properties(context int, id int) {
  //TODO: retru nunsafe.Pointer
  context_ := aliveContext[context]
  if context_ == nil {
    panic("fail")
  }
  context_.keepAliveEnumerateProperties(id)
}
