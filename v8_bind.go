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
import (
  //"errors"
	"reflect"
	//"time"
)

var (
	typeOfValue    = reflect.TypeOf(new(Value))
	typeOfObject   = reflect.TypeOf(new(Object))
	typeOfArray    = reflect.TypeOf(new(Array))
	typeOfRegExp   = reflect.TypeOf(new(RegExp))
	typeOfFunction = reflect.TypeOf(new(Function))
)

func (engine_ *Engine) GoValueToJsValue(value reflect.Value) *Value {
	switch value.Kind() {
	case reflect.Bool:
		return engine_.NewBoolean(value.Bool())
	case reflect.String:
		return engine_.NewString(value.String())
	case reflect.Int8, reflect.Int16, reflect.Int32:
		return engine_.NewInteger(value.Int())
	case reflect.Uint8, reflect.Uint16, reflect.Uint32:
		return engine_.NewInteger(int64(value.Uint()))
	case reflect.Int, reflect.Int64:
		return engine_.NewNumber(float64(value.Int()))
	case reflect.Uint, reflect.Uint64:
		return engine_.NewNumber(float64(value.Uint()))
	case reflect.Float32, reflect.Float64:
		return engine_.NewNumber(value.Float())
	// TODO: avoid data copy
	case reflect.Array, reflect.Slice:
		arrayLen := value.Len()
		jsArrayVal := engine_.NewArray(value.Len())
		jsArray := jsArrayVal.ToArray()
		for i := 0; i < arrayLen; i++ {
			jsArray.SetElement(i, engine_.GoValueToJsValue(value.Index(i)))
		}
		return jsArrayVal
	// TODO: avoid data copy
	case reflect.Map:
		jsObjectVal := engine_.NewObject()
		jsObject := jsObjectVal.ToObject()
		for _, key := range value.MapKeys() {
			switch key.Kind() {
			case reflect.String:
				jsObject.SetProperty(key.String(), engine_.GoValueToJsValue(value.MapIndex(key)))
			case reflect.Int8, reflect.Int16, reflect.Int32,
				reflect.Uint8, reflect.Uint16, reflect.Uint32,
				reflect.Int, reflect.Uint, reflect.Int64, reflect.Uint64:
				jsObject.SetElement(int(key.Int()), engine_.GoValueToJsValue(value.MapIndex(key)))
			}
		}
		return jsObjectVal
    //TODO :: function callback
	/*case reflect.Func:
		return engine_.NewFunction(bindFuncCallback, value).Value*/
	case reflect.Interface:
		return engine_.GoValueToJsValue(reflect.ValueOf(value.Interface()))
    //TODO ::
	/*case reflect.Ptr:
		valType := value.Type()
		if valType == typeOfValue {
			return value.Interface().(*Value)
		}
		elemType := valType.Elem()
		if elemType.Kind() == reflect.Struct {
			if bindInfo, exits := engine_.bindTypes[elemType]; exits {
				objectVal := engine_.NewInstanceOf(bindInfo.Template)
				object := objectVal.ToObject()
				object.SetInternalField(0, &DynamicObject{
					Target:     value,
					SpecFields: bindInfo.SpecFields,
				})
				return objectVal
			}
		}
	case reflect.Struct:
		switch value.Interface().(type) {
		case time.Time:
			return engine_.NewDate(value.Interface().(time.Time))
		default:
			if bindInfo, exits := engine_.bindTypes[value.Type()]; exits {
				objectVal := engine_.NewInstanceOf(bindInfo.Template)
				object := objectVal.ToObject()
				object.SetInternalField(0, &DynamicObject{
					Target:     value,
					SpecFields: bindInfo.SpecFields,
				})
				return objectVal
			}
		}*/
	}
	return engine_.Undefined()
}

func (engine_ *Engine) SetJsValueToGo(field reflect.Value, jsvalue *Value) {
	goType := field.Type()
	switch goType.Kind() {
	case reflect.Bool:
		field.SetBool(jsvalue.ToBoolean())
	case reflect.String:
		field.SetString(jsvalue.ToString())
	case reflect.Int8, reflect.Int16, reflect.Int32:
		field.SetInt(int64(jsvalue.ToInt32()))
	case reflect.Uint8, reflect.Uint16, reflect.Uint32:
		field.SetUint(uint64(jsvalue.ToUint32()))
	case reflect.Int, reflect.Uint, reflect.Int64, reflect.Uint64:
		field.SetInt(jsvalue.ToInteger())
	case reflect.Float32, reflect.Float64:
		field.SetFloat(jsvalue.ToNumber())
	case reflect.Slice:
		jsArray := jsvalue.ToArray()
		jsArrayLen := jsArray.Length()
		field.Set(reflect.MakeSlice(goType, jsArrayLen, jsArrayLen))
		fallthrough
	case reflect.Array:
		jsArray := jsvalue.ToArray()
		jsArrayLen := jsArray.Length()
		for i := 0; i < jsArrayLen; i++ {
			engine_.SetJsValueToGo(field.Index(i), jsArray.GetElement(i))
		}
	case reflect.Map:
		if jsvalue.IsUndefined() || jsvalue.IsBoolean() || jsvalue.IsNumber() || jsvalue.IsString() {
			// GetPropertyNames() causes SIGSEGV.
			break
		}
		jsObject := jsvalue.ToObject()
		jsObjectKeys := jsObject.GetPropertyNames()
		jsObjectKeysLen := jsObjectKeys.Length()
		field.Set(reflect.MakeMap(goType))
		itemType := goType.Elem()
		for i := 0; i < jsObjectKeysLen; i++ {
			mapKey := jsObjectKeys.GetElement(i).ToString()
			mapValue := reflect.Indirect(reflect.New(itemType))
			engine_.SetJsValueToGo(mapValue, jsObject.GetProperty(mapKey))
			field.SetMapIndex(reflect.ValueOf(mapKey), mapValue)
		}
	case reflect.Interface:
		field.Set(reflect.ValueOf(jsvalue))

  //TODO:: function callback
	/*case reflect.Func:
		function := jsvalue.ToFunction()
		field.Set(reflect.MakeFunc(goType, func(args []reflect.Value) []reflect.Value {
			jsargs := make([]*Value, len(args))
			for i := 0; i < len(args); i++ {
				jsargs[i] = engine_.GoValueToJsValue(args[i])
			}
			jsresult := function.Call(jsargs...)

			outNum := goType.NumOut()

			if outNum == 1 {
				var result = reflect.Indirect(reflect.New(goType.Out(0)))
				engine_.SetJsValueToGo(result, jsresult)
				return []reflect.Value{result}
			}

			results := make([]reflect.Value, outNum)
			jsresultArray := jsresult.ToArray()

			for i := 0; i < outNum; i++ {
				results[i] = reflect.Indirect(reflect.New(goType.Out(i)))
				engine_.SetJsValueToGo(results[i], jsresultArray.GetElement(i))
			}

			return results
		}))*/
	default:
		switch goType {
		case typeOfValue:
			field.Set(reflect.ValueOf(jsvalue))
		case typeOfObject:
			field.Set(reflect.ValueOf(jsvalue.ToObject()))
		case typeOfArray:
			field.Set(reflect.ValueOf(jsvalue.ToArray()))
		case typeOfRegExp:
			field.Set(reflect.ValueOf(jsvalue.ToRegExp()))
		case typeOfFunction:
			field.Set(reflect.ValueOf(jsvalue.ToFunction()))
		}
	}
}
