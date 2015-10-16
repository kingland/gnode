package gnode

import(
  v8 "github.com/kingland/go-v8"
  "fmt"
)

type Gnode struct{
  Environment *Environment
}

var(
  gnode *Gnode
)

func NewGnode() *Gnode {
  engine := v8.NewEngine()
  genv := NewEnvironment(engine)
  gnode = &Gnode{
    genv,
  }
  return gnode
}

func (g *Gnode) Initialize() {
  genv := g.Environment
  engine := genv.V8Engine
  genv.NewObjectTemplate()
  genv.bindProcess()
  genv.bindOS()
  genv.bindFS()
  objTmp := genv.V8ObjTmp
  context := engine.NewContext(objTmp)
  genv.V8Context = context
}

func Start(args []string)  {
    //G = NewGnode()
   //process := GProcess
   //fmt.Println(process.version);
   fmt.Println("Undefined");
}
