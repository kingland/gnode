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
import "time"

type Value struct{
  engine       *Engine
  self         unsafe.Pointer
  //self         C.jsvalue
  disposed     bool
  isType       int
  notType      int
  //fieldOwnerId int64
}

type Object struct {
	*Value
	internalFields []interface{}
	//accessor       *accessorInfo
}

type External struct {
	*Value
	data interface{}
}

type Array struct {
	*Value
}

type RegExpFlags int
type RegExp struct {
	*Value
	pattern       string
	patternCached bool
	flags         RegExpFlags
	flagsCached   bool
}

// Regular expression flag bits. They can be or'ed to enable a set
// of flags.
//
const (
	RF_None       RegExpFlags = 0
	RF_Global                 = 1
	RF_IgnoreCase             = 2
	RF_Multiline              = 4
)

const (
	isUndefined     = 1 << iota
	isNull          = 1 << iota
	isTrue          = 1 << iota
	isFalse         = 1 << iota
	isString        = 1 << iota
	isFunction      = 1 << iota
	isArray         = 1 << iota
	isObject        = 1 << iota
	isBoolean       = 1 << iota
	isNumber        = 1 << iota
	isExternal      = 1 << iota
	isInt32         = 1 << iota
	isUint32        = 1 << iota
	isDate          = 1 << iota
	isBooleanObject = 1 << iota
	isNumberObject  = 1 << iota
	isStringObject  = 1 << iota
	isNativeError   = 1 << iota
	isRegExp        = 1 << iota
)

func NewValue(engine_ *Engine, self_ unsafe.Pointer) *Value{
  if self_ == nil {
    return nil
  }
  res_ := &Value{
    engine : engine_,
    self : self_,
    disposed : false,
  }

  runtime.SetFinalizer(res_, func(v *Value) {
    if traceDispose {
      //println("v8.Value.Dispose()", v.self)
      println("v8.Value.Dispose()")
    }
    if !v.disposed {
      //TODO:
      //C.V8_DisposeValue(v.self)
    }
  })

  return res_
}

//Undefined Value
func (engine_ *Engine) Undefined() *Value{
  if engine_.undefinedValue == nil {
    engine_.undefinedValue = NewValue(engine_, C.V8_Undefined(engine_.self))
  }
   return engine_.undefinedValue
}

//Null Value
func (engine_ *Engine) Null() *Value{
  if engine_.nullValue == nil {
    engine_.nullValue = NewValue(engine_, C.V8_Null(engine_.self))
  }
  return engine_.nullValue
}

//True Value
func (engine_ *Engine) True() *Value{
  if engine_.trueValue == nil{
    engine_.trueValue = NewValue(engine_, C.V8_True(engine_.self))
  }
  return engine_.trueValue
}

//False Value
func (engine_ *Engine) False() *Value{
  if engine_.falseValue == nil{
    engine_.falseValue = NewValue(engine_, C.V8_False(engine_.self))
  }
  return engine_.falseValue
}

func (engine_ *Engine) NewBoolean(value bool) *Value {
  if value {
    return engine_.True()
  }
  return engine_.False()
}

func (engine_ *Engine) NewNumber(value float64) *Value {
	return NewValue(engine_, C.V8_NewNumber(engine_.self, C.double(value)))
}

func (engine_ *Engine) NewDate(value time.Time) *Value {
  return NewValue(engine_, C.V8_NewDate(engine_.self, C.double(value.Unix()*1000)))
}

func (engine_ *Engine) NewInteger(value int64) *Value {
  return NewValue(engine_ , C.V8_NewNumber(engine_.self, C.double(value)))
}

func (engine_ *Engine) NewString(value string) *Value {
  valPtr := StringToPointer(value)
	return NewValue(engine_, C.V8_NewString(engine_.self, (*C.char)(valPtr), C.int(len(value))))
}

func (engine_ *Engine) NewExternal(value interface{}) *External {
	if value == nil {
		panic("value is nil")
	}

	external_ := &External{
		data: value,
	}

	external_.Value = NewValue(engine_, C.V8_NewExternal(
		engine_.self, unsafe.Pointer(&(external_.data)),
	))

  //TODO::
	//external.setOwner(external)

	return external_
}

func (engine_ *Engine) NewObject() *Object {
	value_ := NewValue(engine_, C.V8_NewObject(engine_.self))
  object_ := &Object{}
  object_.Value = value_
  return object_
}

func (engine_ *Engine) NewArray(length int) *Array {
	value_ := NewValue(engine_, C.V8_NewArray(
		engine_.self, C.int(length),
	))
  array_ := &Array{
    //length : length,
  }
  array_.Value = value_

  return array_
}

func (engine_ *Engine) NewRegExp(pattern string, flags RegExpFlags) *Value {
	//patternPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&pattern)).Data)
  patternPtr := StringToPointer(pattern)

	return NewValue(engine_, C.V8_NewRegExp(
		engine_.self, (*C.char)(patternPtr), C.int(len(pattern)), C.int(flags),
	))
}

func (val_ *Value) hasJsType(typeCode int ,check func(unsafe.Pointer) bool) bool{
  if val_.isType&typeCode == typeCode {
    return true
  }
  if val_.notType&typeCode == typeCode {
		return false
	}
  if check(val_.self) {
		val_.isType |= typeCode
		return true
	} else {
		val_.notType |= typeCode
		return false
	}
}

//Value
func (val_ *Value) IsUndefined() bool {
	return val_.hasJsType(isUndefined, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsUndefined(self) == 1
	})
}

func (val_ *Value) IsNull() bool {
	return val_.hasJsType(isNull, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsNull(self) == 1
	})
}

func (val_ *Value) IsTrue() bool {
	return val_.hasJsType(isTrue, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsTrue(self) == 1
	})
}

func (val_ *Value) IsFalse() bool {
	return val_.hasJsType(isFalse, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsFalse(self) == 1
	})
}

func (val_ *Value) IsString() bool {
	return val_.hasJsType(isString, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsString(self) == 1

	})
}

func (val_ *Value) IsFunction() bool {
	return val_.hasJsType(isFunction, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsFunction(self) == 1
	})
}

func (val_ *Value) IsArray() bool {
	return val_.hasJsType(isArray, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsArray(self) == 1
	})
}

func (val_ *Value) IsObject() bool {
	return val_.hasJsType(isObject, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsObject(self) == 1
	})
}

func (val_ *Value) IsBoolean() bool {
	return val_.hasJsType(isBoolean, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsBoolean(self) == 1
	})
}

func (val_ *Value) IsNumber() bool {
	return val_.hasJsType(isNumber, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsNumber(self) == 1
	})
}

func (val_ *Value) IsExternal() bool {
	return val_.hasJsType(isExternal, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsExternal(self) == 1
	})
}

func (val_ *Value) IsInt32() bool {
	return val_.hasJsType(isInt32, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsInt32(self) == 1
	})
}

func (val_ *Value) IsUint32() bool {
	return val_.hasJsType(isUint32, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsUint32(self) == 1
	})
}

func (val_ *Value) IsDate() bool {
	return val_.hasJsType(isDate, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsDate(self) == 1
	})
}

func (val_ *Value) IsBooleanObject() bool {
	return val_.hasJsType(isBooleanObject, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsBooleanObject(self) == 1
	})
}

func (val_ *Value) IsNumberObject() bool {
	return val_.hasJsType(isNumberObject, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsNumberObject(self) == 1
	})
}

func (val_ *Value) IsStringObject() bool {
	return val_.hasJsType(isStringObject, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsStringObject(self) == 1
	})
}

func (val_ *Value) IsNativeError() bool {
	return val_.hasJsType(isNativeError, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsNativeError(self) == 1
	})
}

func (val_ *Value) IsRegExp() bool {
	return val_.hasJsType(isRegExp, func(self unsafe.Pointer) bool {
		return C.V8_Value_IsRegExp(self) == 1
	})
}

func (val_ *Value) ToBoolean() bool {
	return C.V8_Value_ToBoolean(val_.self) == 1
}

func (val_ *Value) ToNumber() float64 {
	return float64(C.V8_Value_ToNumber(val_.self))
}

func (val_ *Value) ToInteger() int64 {
	return int64(C.V8_Value_ToInteger(val_.self))
}

func (val_ *Value) ToUint32() uint32 {
	return uint32(C.V8_Value_ToUint32(val_.self))
}

func (val_ *Value) ToInt32() int32 {
	return int32(C.V8_Value_ToInt32(val_.self))
}

func (val_ *Value) ToString() string {
	cstring_ := C.V8_Value_ToString(val_.self)
	gostring_ := C.GoString(cstring_)
	C.free(unsafe.Pointer(cstring_))
	return gostring_
}

func (val_ *Value) ToTime() time.Time {
	return time.Unix(0, int64(C.V8_Value_ToInteger(val_.self))*1e6)
}

func (val_ *Value) ToObject() *Object {
	if val_ == nil {
		return nil
	}
  obj_ := new(Object)
  obj_.Value = val_
	return obj_
}

func (val_ *Value) ToArray() *Array {
	if val_ == nil {
		return nil
	}
	return &Array{val_}
}

func (val_ *Value) ToRegExp() *RegExp {
	if val_ == nil {
		return nil
	}
	return &RegExp{val_, "", false, RF_None, false}
}

/*
TODO::
func (v *Value) ToFunction() *Function {
	if v == nil {
		return nil
	}
	return &Function{&Object{v, nil, nil}, nil, nil}
}
*/

func (val_ *Value) ToExternal() *External {
	if val_ == nil {
		return nil
	}
	return &External{val_, nil}
}

func (val_ *Value) String() string {
	return val_.ToString()
}

// Keep the Object alive when it refence by JS
/*
TODO: manage id
func (v *Value) setOwner(self interface{}) {
	// the object reference by engine
	if v.fieldOwnerId == 0 {
		v.engine.fieldOwnerId += 1
		v.fieldOwnerId = v.engine.fieldOwnerId
		v.engine.fieldOwners[v.fieldOwnerId] = self
		C.V8_Value_SetFieldOwnerInfo(v.self, unsafe.Pointer(v.engine), C.int64_t(v.fieldOwnerId))
	}
}
*/

//External
func (ex_ *External) GetValue() interface{} {
	if ex_.data == nil {
    //TODO::
		//ptr := C.V8_External_Value(ex.self)
		//ex_.data = *(*interface{})(ptr)
	}

	return ex_.data
}

//object
func (object_ *Object) SetProperty(key string, value *Value) bool {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key);
	return C.V8_Object_SetProperty(
		object_.self, (*C.char)(keyPtr), C.int(len(key)), value.self,
	) == 1
}

func (object_ *Object) GetProperty(key string) *Value {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return NewValue(object_.engine, C.V8_Object_GetProperty(
		object_.self, (*C.char)(keyPtr), C.int(len(key)),
	))
}

func (object_ *Object) SetElement(index int, value *Value) bool {
	return C.V8_Object_SetElement(object_.self, C.uint32_t(index), value.self) == 1
}

func (object_ *Object) GetElement(index int) *Value {
	return NewValue(object_.engine, C.V8_Object_GetElement(object_.self, C.uint32_t(index)))
}

/*
func (o *Object) GetPropertyAttributes(key string) PropertyAttribute {
	keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
	return PropertyAttribute(C.V8_Object_GetPropertyAttributes(
		o.self, (*C.char)(keyPtr), C.int(len(key)),
	))
}
*/

func (object_ *Object) InternalFieldCount() int {
	return int(C.V8_Object_InternalFieldCount(object_.self))
}

func (object_ *Object) GetInternalField(index int) interface{} {
	data := C.V8_Object_GetInternalField(object_.self, C.int(index))
	if data == nil {
		return nil
	}
	return *(*interface{})(data)
}

func (object_ *Object) SetInternalField(index int, value interface{}) {
	C.V8_Object_SetInternalField(
		object_.self,
		C.int(index),
		unsafe.Pointer(&value),
	)

	// the value reference by object so the value can't destory by GC
  //TODO::
	object_.internalFields = append(object_.internalFields, value)
	//object_.setOwner(object_)
}

/*
TODO::
func (o *Object) SetAccessor(
	key string,
	getter AccessorGetterCallback,
	setter AccessorSetterCallback,
	data interface{},
	attribs PropertyAttribute,
) {
	o.setAccessor(&accessorInfo{
		key:     key,
		getter:  getter,
		setter:  setter,
		data:    data,
		attribs: attribs,
	})
}

func (o *Object) setAccessor(info *accessorInfo) {
	keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&info.key)).Data)
	var getterPointer, setterPointer unsafe.Pointer
	if info.getter != nil {
		getterPointer = unsafe.Pointer(&info.getter)
	}

	if info.setter != nil {
		setterPointer = unsafe.Pointer(&info.setter)
	}

	o.accessor = info
	o.setOwner(o)

	C.V8_Object_SetAccessor(
		o.self,
		(*C.char)(keyPtr), C.int(len(info.key)),
		getterPointer,
		setterPointer,
		unsafe.Pointer(&info.data),
		C.int(info.attribs),
	)
}

// Sets a local property on this object bypassing interceptors and
// overriding accessors or read-only properties.
//
// Note that if the object has an interceptor the property will be set
// locally, but since the interceptor takes precedence the local property
// will only be returned if the interceptor doesn't return a value.
//
// Note also that this only works for named properties.
func (o *Object) ForceSetProperty(key string, value *Value, attribs PropertyAttribute) bool {
	keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
	return C.V8_Object_ForceSetProperty(o.self,
		(*C.char)(keyPtr), C.int(len(key)), value.self, C.int(attribs),
	) == 1
}
*/

func (object_ *Object) HasProperty(key string) bool {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_HasProperty(
		object_.self, (*C.char)(keyPtr), C.int(len(key)),
	) == 1
}

func (object_ *Object) DeleteProperty(key string) bool {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_DeleteProperty(
		object_.self, (*C.char)(keyPtr), C.int(len(key)),
	) == 1
}


// Delete a property on this object bypassing interceptors and
// ignoring dont-delete attributes.
func (object_ *Object) ForceDeleteProperty(key string) bool {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_ForceDeleteProperty(
		object_.self, (*C.char)(keyPtr), C.int(len(key)),
	) == 1
}

func (object_ *Object) HasElement(index int) bool {
	return C.V8_Object_HasElement(
		object_.self, C.uint32_t(index),
	) == 1
}

func (object_ *Object) DeleteElement(index int) bool {
	return C.V8_Object_DeleteElement(
		object_.self, C.uint32_t(index),
	) == 1
}

/*
TODO: ToArray
// Returns an array containing the names of the enumerable properties
// of this object, including properties from prototype objects.  The
// array returned by this method contains the same values as would
// be enumerated by a for-in statement over this object.
//
func (object_ *Object) GetPropertyNames() *Array {
	return newValue(object_.engine, C.V8_Object_GetPropertyNames(object_.self)).ToArray()
}
*/

/*
TODO:: ToArray
// This function has the same functionality as GetPropertyNames but
// the returned array doesn't contain the names of properties from
// prototype objects.
//
func (o *Object) GetOwnPropertyNames() *Array {
	return newValue(o.engine, C.V8_Object_GetOwnPropertyNames(o.self)).ToArray()
}
*/


/*
TODO:: ToObject
// Get the prototype object.  This does not skip objects marked to
// be skipped by __proto__ and it does not consult the security
// handler.
//
func (o *Object) GetPrototype() *Object {
	return newValue(o.engine, C.V8_Object_GetPrototype(o.self)).ToObject()
}
*/

// Set the prototype object.  This does not skip objects marked to
// be skipped by __proto__ and it does not consult the security
// handler.
//
func (object_ *Object) SetPrototype(proto *Object) bool {
	return C.V8_Object_SetPrototype(object_.self, proto.self) == 1
}


func (object_ *Object) SetHiddenValue(key string, value *Value) bool{
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_SetHiddenValue(object_.self, (*C.char)(keyPtr), C.int(len(key)), value.self) == 1
}

func (object_ *Object) GetHiddenValue(key string) *Value {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return NewValue(object_.engine, C.V8_Object_GetHiddenValue(object_.self, (*C.char)(keyPtr), C.int(len(key))))
}

func (object_ *Object) DeleteHiddenValue(key string) bool{
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_DeleteHiddenValue(object_.self, (*C.char)(keyPtr), C.int(len(key))) == 1
}

/*
TODO:: ToString
func (object_ *Object) GetConstructorName() string{
	return NewValue(object_.engine, C.V8_Object_GetConstructorName(o.self)).ToString()
}
*/

/*
//TODO:
func (o *Object) SetAlignedPointerInInternalField(index int, val_ptr unsafe.Pointer) {
	C.V8_Object_SetAlignedPointerInInternalField(o.self, C.int(index), val_ptr)
}

func (o *Object) GetAlignedPointerFromInternalField(index int) unsafe.Pointer{
	return C.V8_Object_GetAlignedPointerFromInternalField(o.self, C.int(index))
}
*/

func (object_ *Object) GetRealNamedProperty(key string) *Value {
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return NewValue(object_.engine, C.V8_Object_GetRealNamedProperty(object_.self, (*C.char)(keyPtr), C.int(len(key))))
}

func (object_ *Object) HasRealNamedProperty(key string) bool{
	//keyPtr := unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&key)).Data)
  keyPtr := StringToPointer(key)
	return C.V8_Object_HasRealNamedProperty(object_.self,(*C.char)(keyPtr),C.int(len(key))) == 1
}

//Array
func (array_ *Array) Length() int {
	return int(C.V8_Array_Length(array_.self))
}

//Reg
// Returns the value of the source property: a string representing
// the regular expression.
func (reg_ *RegExp) Pattern() string {
	if !reg_.patternCached {
		cstring := C.V8_RegExp_Pattern(reg_.self)
		reg_.pattern = C.GoString(cstring)
		reg_.patternCached = true
		C.free(unsafe.Pointer(cstring))
	}
	return reg_.pattern
}

// Returns the flags bit field.
//
func (reg_ *RegExp) Flags() RegExpFlags {
	if !reg_.flagsCached {
		reg_.flags = RegExpFlags(C.V8_RegExp_Flags(reg_.self))
		reg_.flagsCached = true
	}
	return reg_.flags
}
