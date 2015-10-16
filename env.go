package gnode

import (
  v8 "github.com/kingland/go-v8"
)

type Environment struct {
  V8Engine *v8.Engine
  V8Context *v8.Context
  V8ObjTmp *v8.ObjectTemplate
  Contextify *Contextify
  //ProcessFunction *ProcessFunction
}

func NewEnvironment(engine *v8.Engine) *Environment {
  return &Environment{
    V8Engine : engine,
  }
}

func (env *Environment) NewGlobalContext() *v8.Context{
    if env.V8ObjTmp == nil {
      env.NewObjectTemplate()
    }
    env.V8Context = env.V8Engine.NewContext(env.V8ObjTmp)
    return env.V8Context
}

func (env *Environment) NewObjectTemplate() *v8.ObjectTemplate{
  if env.V8ObjTmp != nil{
    return env.V8ObjTmp
  }
  engine := env.V8Engine
  env.V8ObjTmp =  engine.NewObjectTemplate()
  return env.V8ObjTmp
}
