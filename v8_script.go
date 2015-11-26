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
//import "reflect"
import "runtime"

// A compiled JavaScript script.
type Script struct {
	self unsafe.Pointer
}

var aliveScript map[int]*Script = make(map[int]*Script)
/*
func NewScript(ptr unsafe.Pointer) *Script{
  script_ := &Script{
    self : ptr,
  }

  runtime.SetFinalizer(script_, func(s *Script) {
    if traceDispose {
      println("v8.Script.Dispose()", s.self)
    }
    //TODO:
    //C.V8_DisposeScript(s.self)
  })

  return script_
}
*/

func (engine_ *Engine) Compile(code []byte, origin *ScriptOrigin)  *Script {
	codePtr := ByteToPointer(code)
	self := C.V8_NewScript(engine_.self, (*C.char)(codePtr), unsafe.Pointer(origin))

	if self == nil {
		return nil
	}

	script := &Script{
		self: self,
	}

	runtime.SetFinalizer(script, func(s *Script) {
		if traceDispose {
			println("v8.Script.Dispose()", s.self)
		}
		//C.V8_DisposeScript(s.self)
		C.V8_Script_Dispose(s.self)
	})

	return script
}

func (engine_ *Engine) GetAliveScript(id int) *Script{
	return aliveScript[id]
}

func (engine_ *Engine) SetAliveScript(id int, script *Script){
	aliveScript[id] = script
}

type UnboundScript struct {
	self unsafe.Pointer
}

func (engine_ *Engine) CompileUnbound(code []byte, origin *ScriptOrigin)  *UnboundScript {
	codePtr := ByteToPointer(code)
	self := C.V8_NewUnboundScript(engine_.self, (*C.char)(codePtr), unsafe.Pointer(origin))

	if self == nil {
		return nil
	}

	script := &UnboundScript{
		self: self,
	}

	runtime.SetFinalizer(script, func(s *Script) {
		if traceDispose {
			println("v8.Script.Dispose()", s.self)
		}
		//C.V8_DisposeScript(s.self)
		C.V8_UnboundScript_Dispose(s.self)
	})

	return script
}

type ScriptOrigin struct {
	Name         string
	LineOffset   int
	ColumnOffset int
}

func NewScriptOrigin(name string, lineOffset, columnOffset int) *ScriptOrigin {
	return &ScriptOrigin{
		Name:         name,
		LineOffset:   lineOffset,
		ColumnOffset: columnOffset,
	}
}

//export go_script_origin_get_name
func go_script_origin_get_name(p unsafe.Pointer) *C.char {
	if p == nil {
		return C.CString("")
	}
	o := (*ScriptOrigin)(p)
	return C.CString(o.Name)
}

//export go_script_origin_get_line
func go_script_origin_get_line(p unsafe.Pointer) int {
	if p == nil {
		return 0
	}
	o := (*ScriptOrigin)(p)
	return o.LineOffset
}

//export go_script_origin_get_column
func go_script_origin_get_column(p unsafe.Pointer) int {
	if p == nil {
		return 0
	}
	o := (*ScriptOrigin)(p)
	return o.ColumnOffset
}
