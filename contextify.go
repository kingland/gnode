package gnode

import(
  v8 "github.com/kingland/go-v8"
)

type Contextify struct{
  //env *Environment
  sandbox_ *v8.Object
  context_ *v8.Context
  proxy_global_ *v8.Object
}

func (env *Environment) NewContextify(sandbox *v8.Object) *Contextify {
  if env.Contextify != nil {
    return env.Contextify
  }

  return &Contextify{
    sandbox_ : sendbox
  }
}

func (env *Environment) NewV8Context() *v8.Context {
  engine := env.V8Engine
  gcontext := env.V8Context
  contextify := env.Contextify
  sendbox_ := contextify.sendbox_
  if sendbox_ == nil{
    return nil
  }

  if gcontext == nil{
    env.NewV8Context()
    gcontext = env.V8Context
  }

  gcontext.EscapableScope(func (es EscapableScope){
    ftmp = engine.NewFunctionTemplate(nil,nil)
    ftmp.SetHiddenPrototype(true)
    
  })
}
