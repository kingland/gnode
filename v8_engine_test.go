package gnode

import "testing"
import "runtime"
func Test_NewEngine(t *testing.T) {
  //("Test_NewEngine")
  engine_ := NewEngine()
  engine_.Disposed(false)
  runtime.GC()
}

func Test_MultiEngine(t *testing.T){
  //println("Test_MultiEngine")
  var engines_ []*Engine

  for i := 0; i<10; i++ {
    engine_ := NewEngine()
    if engine_ != nil{
      engines_ = append(engines_, engine_)
    }
  }

  for k, engine_ := range engines_ {
    println("Test_MultiEngine Disposed ", k, engine_)
    engine_.Disposed(false)
  }

  runtime.GC()
}
