package gnode
/*
#include "gnode.h"
#include <stdlib.h>
*/
import "C"
import "unsafe"

type embedable struct {
	data interface{}
}

func (emdata_ *embedable) GetPrivateData() interface{} {
	return emdata_.data
}

func (emdata_ *embedable) SetPrivateData(data interface{}) {
	emdata_.data = data
}

func GetV8Version() string {
	return C.GoString(C.V8_GetVersion())
}

func SetFlagsFromString(cmd string) {
	cs := C.CString(cmd)
	defer C.free(unsafe.Pointer(cs))
	C.V8_SetFlagsFromString(cs, C.int(len(cmd)))
}

// Set default array buffer allocator to V8 for
// ArrayBuffer, ArrayBufferView, Int8Array...
// If you want to use your own allocator. You can implement it in C++
// and invoke v8::SetArrayBufferAllocator by your self
//TODO:: Allocate array buffer
/*func UseDefaultArrayBufferAllocator() {
	C.V8_UseDefaultArrayBufferAllocator()
}*/
