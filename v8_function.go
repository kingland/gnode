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

type Function struct {
	*Value
	callback FunctionCallback
	data     interface{}
}

type FunctionCallback func(FunctionCallbackInfo)

// Function and property return value
//
type ReturnValue struct {
	self unsafe.Pointer
}

// Function callback info
//
type FunctionCallbackInfo struct {
	self        unsafe.Pointer
	returnValue ReturnValue
	context     *Context
	data        interface{}
}


//FUNCTION
func (engine_ *Engine) NewFunction(callback FunctionCallback, data interface{}) *Function {
  fun_ := new(Function)
  fun_.data = data
	fun_.callback = callback

  fun_.Value = NewValue(engine_, C.V8_NewFunction(
		engine_.self, unsafe.Pointer(&fun_.callback), unsafe.Pointer(&fun_.data),
	))

  //TODO alive object
  //function.setOwner(function)
  return fun_
}

/*
TODO:
func (f *Function) Call(args ...*Value) *Value {
	argv := make([]unsafe.Pointer, len(args))
	for i, arg := range args {
		argv[i] = arg.self
	}
	return newValue(f.engine, C.V8_Function_Call(
		f.self, C.int(len(args)),
		unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&argv)).Data),
	))
}

func (f *Function) NewInstance(args ...*Value) *Value {
	argv := make([]unsafe.Pointer, len(args))
	for i, arg := range args {
		argv[i] = arg.self
	}
	return newValue(f.engine, C.V8_Function_NewInstance(
		f.self, C.int(len(args)),
		unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&argv)).Data),
	))
}
*/

//RETURNVALUE
/*
func (rv ReturnValue) Set(value *Value) {
	C.V8_ReturnValue_Set(rv.self, value.self)
}

func (rv ReturnValue) SetBoolean(value bool) {
	valueInt := 0
	if value {
		valueInt = 1
	}
	C.V8_ReturnValue_SetBoolean(rv.self, C.int(valueInt))
}

func (rv ReturnValue) SetNumber(value float64) {
	C.V8_ReturnValue_SetNumber(rv.self, C.double(value))
}

func (rv ReturnValue) SetInt32(value int32) {
	C.V8_ReturnValue_SetInt32(rv.self, C.int32_t(value))
}

func (rv ReturnValue) SetUint32(value uint32) {
	C.V8_ReturnValue_SetUint32(rv.self, C.uint32_t(value))
}

func (rv ReturnValue) SetString(value string) {
	valuePtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&value)).Data)
	C.V8_ReturnValue_SetString(rv.self, (*C.char)(valuePtr), C.int(len(value)))
}

func (rv ReturnValue) SetNull() {
	C.V8_ReturnValue_SetNull(rv.self)
}

func (rv ReturnValue) SetUndefined() {
	C.V8_ReturnValue_SetUndefined(rv.self)
}
*/

//FUNCTION CALLBACK INFO
/*
func (fc FunctionCallbackInfo) CurrentScope() ContextScope {
	return ContextScope{fc.context}
}

func (fc FunctionCallbackInfo) Get(i int) *Value {
	return newValue(fc.context.engine, C.V8_FunctionCallbackInfo_Get(fc.self, C.int(i)))
}

func (fc FunctionCallbackInfo) Length() int {
	return int(C.V8_FunctionCallbackInfo_Length(fc.self))
}

func (fc FunctionCallbackInfo) Callee() *Function {
	return newValue(fc.context.engine, C.V8_FunctionCallbackInfo_Callee(fc.self)).ToFunction()
}

func (fc FunctionCallbackInfo) This() *Object {
	return newValue(fc.context.engine, C.V8_FunctionCallbackInfo_This(fc.self)).ToObject()
}

func (fc FunctionCallbackInfo) Holder() *Object {
	return newValue(fc.context.engine, C.V8_FunctionCallbackInfo_Holder(fc.self)).ToObject()
}

func (fc FunctionCallbackInfo) Data() interface{} {
	return fc.data
}

func (fc *FunctionCallbackInfo) ReturnValue() ReturnValue {
	if fc.returnValue.self == nil {
		fc.returnValue.self = C.V8_FunctionCallbackInfo_ReturnValue(fc.self)
	}
	return fc.returnValue
}
*/
