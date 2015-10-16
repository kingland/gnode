package gnode

import (
  v8 "github.com/kingland/go-v8"
  "reflect"
)

func (env *Environment) bindFS()  {
  engine := env.V8Engine
  objTmp := env.NewObjectTemplate()

  objTmp.SetAccessor("os",
    //getter
    func(name string, info v8.AccessorCallbackInfo) {
        osObj := engine.NewObject().ToObject()

        //tmpdir
        osObj.SetAccessor("rename",
          //getter
          func(name string, info v8.AccessorCallbackInfo){
            info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
                gofunc := funcInfo.Data().(reflect.Value)
                in := make([]reflect.Value, 3)
                gofunc.Call(in)
                funcInfo.ReturnValue().Set(engine.Undefined())
              },
              reflect.ValueOf(env.fsrename)).Value)
          },
          //setter
          nil,
          //data
          nil,
          //prop attribute
          v8.PA_ReadOnly,
        )
    },
    //setter
    nil,
    //data
    nil,
    //property attribute
    v8.PA_ReadOnly,
  )
  env.V8ObjTmp = objTmp
}

func (env *Environment)  fsrename(oldpath,newpath string)  {
}
