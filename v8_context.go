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
#include "gnode.h"
#include <stdlib.h>
*/
import "C"
import "unsafe"
import "runtime"

type Context struct {
	embedable
  id       int
	disposed bool
	self     unsafe.Pointer
	engine   *Engine
}

//var count_context_id int32 = 0
//var aliveContext map[int]*Context = make(map[int]*Context)

//create context
func (engine_ *Engine) NewContext() *Context {
  contextId++
  id_ := contextId
  self_ := C.V8_NewContext(C.int(id_), engine_.self)

  context_ :=  &Context{
    id : id_,
    self : self_,
    engine : engine_,
  }

	engine_.SetAliveContext(id_, context_)

  //gc
  runtime.SetFinalizer(context_, func(c *Context) {
		if traceDispose {
			println("v8.Context.Dispose()", c.self)
		}
		C.V8_Context_Dispose(c.self)
	})

  return context_
}

//map context id
func (engine_ *Engine) GetAliveContext(id int) *Context{
  return engine_.aliveContext[id]
}

//map context id
func (engine_ *Engine) SetAliveContext(id int, context *Context){
  engine_.aliveContext[id] = context
}

func (context_ *Context) GetEngine() *Engine{
  return context_.engine
}

func (context_ *Context) IsDisposed() bool{
	return context_.disposed
}

func (context_ *Context) Dispose(){
	context_.checkDisposedException()
	//TODO
	/*jscontext_dispose(_context);
	if (disposing) {
		_keepalives.Clear();
	}
	_notifyDispose(_id);*/
	context_.disposed = true

}

func (context_ *Context) checkDisposedException(){
	engine_ := context_.GetEngine()
	if engine_.IsDisposed() {
		//TODO:
		//throw new ObjectDisposedException("JsContext: engine has been disposed");
	}
	if context_.IsDisposed() {
		//throw new ObjectDisposedException("JsContext:" + _context.Handle);
	}
}

func (context_ *Context) Execute(code , name string) *Value {
	//TODO:
	codePtr_ := StringToPointer(code)
	namePtr_ := StringToPointer(name)
	val_ := NewValue(context_.GetEngine(), C.V8_Context_Execute(context_.self,(*C.char)(codePtr_),(*C.char)(namePtr_)))
	return val_
}

func (context_ *Context) Stats(){
	//TODO:
}

func (context_ *Context) Global() *Value{
	context_.checkDisposedException()
	val_ := NewValue(context_.GetEngine() , C.V8_Context_Global(context_.self))
	return val_
}

func (context_ *Context) GetVariable(name string) *Value{
	context_.checkDisposedException()
	namePtr_ := StringToPointer(name)
	val_ := NewValue(context_.GetEngine(), C.V8_Context_GetVariable(context_.self , (*C.char)(namePtr_)))
	return val_
}

func (context_ *Context) SetVariable(name string, val *Value) *Value{
	context_.checkDisposedException()
	namePtr_ := StringToPointer(name)
	val_ := NewValue(context_.GetEngine(), C.V8_Context_SetVariable(context_.self, (*C.char)(namePtr_), val.self))
	return val_
}

func (context_ *Context) keepAliveValueOf(id int){

}

func (context_ *Context) keepAliveInvoke( id int, args *C.jsvalue){

}

func (context_ *Context) keepAliveSetPropertyValue(id int, name string, value *C.jsvalue){

}

func (context_ *Context) keepAliveGetPropertyValue(id int, name string){

}

func (context_ *Context) keepAliveDeleteProperty(id int, name string){

}

func (context_ *Context)  keepAliveEnumerateProperties(id int){

}
