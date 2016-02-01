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
import "sync"
import "reflect"

type ObjectTemplate struct {
	sync.Mutex
	id                 int
	engine             *Engine
	//accessors          map[string]*accessorInfo
	//namedInfo          *namedPropertyInfo
	indexedInfo        *indexedPropertyInfo
	properties         map[string]*propertyInfo
	self               unsafe.Pointer
	internalFieldCount int
}

type PropertyCallbackInfo struct {
	self        			unsafe.Pointer
	typ         			C.V8_PropertyDataEnum
	data        			interface{}
	returnValue 			ReturnValue
	context     			*Context
}

type AccessorCallbackInfo struct {
	self        			unsafe.Pointer
	data        			interface{}
	returnValue 			ReturnValue
	context     			*Context
	typ         			C.V8_AccessorDataEnum
}

type AccessCheckCallbackInfo struct{
	self 							unsafe.Pointer
	data  						interface{}
	context 					*Context
	host 							unsafe.Pointer
	key 							unsafe.Pointer
	index 						uint32
	//typ 	C.AccessCheckDataEnum
}

type namedPropertyInfo struct {
	getter     				NamedPropertyGetterCallback
	setter     				NamedPropertySetterCallback
	deleter    				NamedPropertyDeleterCallback
	query      				NamedPropertyQueryCallback
	enumerator 				NamedPropertyEnumeratorCallback
	data       				interface{}
}

type indexedPropertyInfo struct {
	getter     				IndexedPropertyGetterCallback
	setter     				IndexedPropertySetterCallback
	deleter    				IndexedPropertyDeleterCallback
	query      				IndexedPropertyQueryCallback
	enumerator 				IndexedPropertyEnumeratorCallback
	data       				interface{}
}


type accessorInfo struct {
	key     					string
	getter  					AccessorGetterCallback
	setter  					AccessorSetterCallback
	data    					interface{}
	attribs 					PropertyAttribute
}

type propertyInfo struct {
	key     					string
	value   					*Value
	attribs 					PropertyAttribute
}

//Name Property Callback
type NamedPropertyGetterCallback func(string, PropertyCallbackInfo)
type NamedPropertySetterCallback func(string, *Value, PropertyCallbackInfo)
type NamedPropertyDeleterCallback func(string, PropertyCallbackInfo)
type NamedPropertyQueryCallback func(string, PropertyCallbackInfo)
type NamedPropertyEnumeratorCallback func(PropertyCallbackInfo)

//Index Property Callback
type IndexedPropertyGetterCallback func(uint32, PropertyCallbackInfo)
type IndexedPropertySetterCallback func(uint32, *Value, PropertyCallbackInfo)
type IndexedPropertyDeleterCallback func(uint32, PropertyCallbackInfo)
type IndexedPropertyQueryCallback func(uint32, PropertyCallbackInfo)
type IndexedPropertyEnumeratorCallback func(PropertyCallbackInfo)

//Accessor Callback
type AccessorGetterCallback func(name string, info AccessorCallbackInfo)
type AccessorSetterCallback func(name string, value *Value, info AccessorCallbackInfo)

type NamedSecurityCallback func(info AccessCheckCallbackInfo) bool
type IndexedSecurityCallback func(info AccessCheckCallbackInfo) bool

type AccessControl int
const (
	AC_DEFAULT               AccessControl = 0
	AC_ALL_CAN_READ                        = 1
	AC_ALL_CAN_WRITE                       = 1 << 1
	AC_PROHIBITS_OVERWRITING               = 1 << 2
)

type PropertyAttribute int
const (
	PA_None       PropertyAttribute = 0
	PA_ReadOnly                     = 1 << 0
	PA_DontEnum                     = 1 << 1
	PA_DontDelete                   = 1 << 2
)

func (engine_ *Engine) NewObjectTemplate() *ObjectTemplate {
	self_ := C.V8_NewObjectTemplate(engine_.self)

  if self_ == nil {
		return nil
	}

  //TODO::
	otmp_ := &ObjectTemplate{
		//id:         e.objectTemplateId + 1,
		engine:     engine_,
		//accessors:  make(map[string]*accessorInfo),
		//properties: make(map[string]*propertyInfo),
		self:       self_,
	}
	//e.objectTemplateId += 1
	//e.objectTemplates[ot.id] = ot
	return otmp_
}

/*
TODO::
func (ot *ObjectTemplate) Dispose() {
	ot.Lock()
	defer ot.Unlock()

	if ot.id > 0 {
		delete(ot.engine.objectTemplates, ot.id)
		ot.id = 0
		ot.engine = nil
		C.V8_ObjectTemplate_Dispose(ot.self)
	}
}
*/
func (otmp_ *ObjectTemplate) NewInstanceOf(ot *ObjectTemplate) *Value {
	otmp_.Lock()
	defer otmp_.Unlock()

	if otmp_.engine == nil {
		return nil
	}
  engine_ := otmp_.engine
	return NewValue(engine_, C.V8_ObjectTemplate_NewInstance(engine_.self, otmp_.self))
}

/*
TODO::
func (ot *ObjectTemplate) Plugin(pluginInit unsafe.Pointer) {
	C.V8_ObjectTemplate_Plugin(ot.self, pluginInit)
}

func (ot *ObjectTemplate) WrapObject(value *Value) {
	ot.Lock()
	defer ot.Unlock()

	object := value.ToObject()

	for _, info := range ot.accessors {
		object.setAccessor(info)
	}

	for _, info := range ot.properties {
		object.SetProperty(info.key, info.value)
	}
}
*/
func (otmp_ *ObjectTemplate) SetProperty(key string, value *Value, attribs PropertyAttribute) {
	info_ := &propertyInfo{
		key:     key,
		value:   value,
		attribs: attribs,
	}

	otmp_.properties[key] = info_

	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&otmp_.key)).Data)
  keyPtr := StringToPointer(key)

	C.V8_ObjectTemplate_SetProperty(
		otmp_.self, (*C.char)(keyPtr), C.int(len(key)), value.self, C.int(attribs),
	)
}

func (otmp_ *ObjectTemplate) SetInternalFieldCount(count int) {
	C.V8_ObjectTemplate_SetInternalFieldCount(otmp_.self, C.int(count))
	otmp_.internalFieldCount = count
}

func (otmp_ *ObjectTemplate) InternalFieldCount() int {
	return otmp_.internalFieldCount
}


//export go_named_property_callback
func go_named_property_callback(typ C.V8_PropertyDataEnum, info *C.V8_PropertyCallbackInfo, engine unsafe.Pointer) {
	gname := ""
	if info.key != nil {
		gname = C.GoString(info.key)
	}
	gengine := (*Engine)(engine)
	//gcontext := (*Context)(context)
	gcontext := gengine.GetAliveContext(int(info.contextId));
	switch typ {
	case C.OTP_Getter:
		(*(*NamedPropertyGetterCallback)(info.callback))(
			gname, PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Setter:
		(*(*NamedPropertySetterCallback)(info.callback))(
			gname,
			NewValue(gengine, info.setValue),
			PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Deleter:
		(*(*NamedPropertyDeleterCallback)(info.callback))(
			gname, PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Query:
		(*(*NamedPropertyQueryCallback)(info.callback))(
			gname, PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Enumerator:
		(*(*NamedPropertyEnumeratorCallback)(info.callback))(
			PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	}
}


//export go_indexed_property_callback
func go_indexed_property_callback(typ C.V8_PropertyDataEnum, info *C.V8_PropertyCallbackInfo, engine unsafe.Pointer) {
	gengine := (*Engine)(engine)
	//gcontext := (*Context)(context)
	gcontext := gengine.GetAliveContext(int(info.contextId))
	switch typ {
	case C.OTP_Getter:
		(*(*IndexedPropertyGetterCallback)(info.callback))(
			uint32(info.index), PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Setter:
		(*(*IndexedPropertySetterCallback)(info.callback))(
			uint32(info.index),
			NewValue(gcontext.engine, info.setValue),
			PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Deleter:
		(*(*IndexedPropertyDeleterCallback)(info.callback))(
			uint32(info.index), PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Query:
		(*(*IndexedPropertyQueryCallback)(info.callback))(
			uint32(info.index), PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	case C.OTP_Enumerator:
		(*(*IndexedPropertyEnumeratorCallback)(info.callback))(
			PropertyCallbackInfo{unsafe.Pointer(info), typ, *(*interface{})(info.data), ReturnValue{}, gcontext})
	}
}

//export go_accessor_callback
func go_accessor_callback(typ C.V8_AccessorDataEnum, info *C.V8_AccessorCallbackInfo, engine unsafe.Pointer) {
	gengine := (*Engine)(engine)
	name := reflect.StringHeader{
		Data: uintptr(unsafe.Pointer(info.key)),
		Len:  int(info.key_length),
	}
	gname := *(*string)(unsafe.Pointer(&name))
	//gcontext := (*Context)(context)
	gcontext := gengine.GetAliveContext(int(info.contextId))
	switch typ {
	case C.OTA_Getter:
		(*(*AccessorGetterCallback)(info.callback))(
			gname,
			AccessorCallbackInfo{unsafe.Pointer(info), *(*interface{})(info.data), ReturnValue{}, gcontext, typ})
	case C.OTA_Setter:
		(*(*AccessorSetterCallback)(info.callback))(
			gname,
			NewValue(gcontext.engine, info.setValue),
			AccessorCallbackInfo{unsafe.Pointer(info), *(*interface{})(info.data), ReturnValue{}, gcontext, typ})
	default:
		panic("impossible type")
	}
}
