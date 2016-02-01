package gnode

import (
  "testing"
  "runtime"
//"sync"
)
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

func Test_NewNull(t *testing.T){
  engine_ := NewEngine()
  null := engine_.Null()
  if null == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

/*func Test_NewArray(t *testing.T){
  engine_ := NewEngine()
  arr := engine_.NewArray(2)
  if arr == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}*/

func Test_NewBoolean(t *testing.T){
  engine_ := NewEngine()
  true_ := engine_.NewBoolean(true)
  false_ := engine_.NewBoolean(false)
  if true_ == nil || false_ == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

func Test_NewInteger(t *testing.T){
  engine_ := NewEngine()
  integer_ := engine_.NewInteger(int64(1000000))
  if integer_ == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

func Test_NewNumber(t *testing.T){
  engine_ := NewEngine()
  number_ := engine_.NewNumber(100000.00003)
  if number_ == nil {
     t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

func Test_NewObject(t *testing.T){
  engine_ := NewEngine()
  obj_ := engine_.NewObject()
  if obj_ == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

/*func Test_NewString(t *testing.T){
  engine_ := NewEngine()
  str_ := engine_.NewString("Hello Word !")
  if str_ == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}

func Test_NewRegExp(t *testing.T){
  engine_ := NewEngine()
  regexp_ := engine_.NewRegExp("[abc]", RF_None)
  if regexp_ == nil {
    t.FailNow()
  }
  engine_.Disposed(false)
  runtime.GC()
}*/


// use one context in different threads
//
/*func Test_ThreadSafe(t *testing.T){
  fail := false
	//context := engine.NewContext(nil)

	wg := new(sync.WaitGroup)
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			context.Scope(func(cs ContextScope) {
				rand_sched(200)

				script := engine.Compile([]byte("'Hello ' + 'World!'"), nil)
				value := cs.Run(script)
				result := value.ToString()
				fail = fail || result != "Hello World!"
				runtime.GC()
				wg.Done()
			})
		}()
	}
	wg.Wait()
	runtime.GC()

	if fail {
		t.FailNow()
	}
}*/

// use one script in different threads
//
/*func Test_ThreadSafe3(t *testing.T) {
	fail := false
	//script := engine.Compile([]byte("'Hello ' + 'World!'"), nil)
	//context := engine.NewContext(nil)
	wg := new(sync.WaitGroup)
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			context.Scope(func(cs ContextScope) {
				rand_sched(200)

				value := cs.Run(script)
				result := value.ToString()
				fail = fail || result != "Hello World!"
				runtime.GC()
				wg.Done()
			})
		}()
	}
	wg.Wait()
	runtime.GC()

	if fail {
		t.FailNow()
	}
}*/

// use one context and one script in different threads
//
/*func Test_ThreadSafe4(t *testing.T) {
	fail := false
	//script := engine.Compile([]byte("'Hello ' + 'World!'"), nil)
	//context := engine.NewContext(nil)

	wg := new(sync.WaitGroup)
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go func() {
			context.Scope(func(cs ContextScope) {
				rand_sched(200)

				value := cs.Run(script)
				result := value.ToString()
				fail = fail || result != "Hello World!"
				runtime.GC()
				wg.Done()
			})
		}()
	}
	wg.Wait()
	runtime.GC()

	if fail {
		t.FailNow()
	}
}*/

//go chanel
/*func Test_ThreadSafe6(t *testing.T) {
	var (
		fail        = false
		gonum       = 100
		scriptChan  = make(chan *Script, gonum)
		contextChan = make(chan *Context, gonum)
	)

	for i := 0; i < gonum; i++ {
		go func() {
			rand_sched(200)

			scriptChan <- engine.Compile([]byte("'Hello ' + 'World!'"), nil)
		}()
	}

	//var context = engine.NewContext(nil)
	for i := 0; i < gonum; i++ {
		go func() {
			rand_sched(200)

			//contextChan <- engine.NewContext(nil)
			contextChan <- context
		}()
	}

	for i := 0; i < gonum; i++ {
		go func() {
			rand_sched(200)

			context := <-contextChan
			script := <-scriptChan

			context.Scope(func(cs ContextScope) {
				result := cs.Run(script).ToString()
				fail = fail || result != "Hello World!"
			})
		}()
	}

	runtime.GC()

	if fail {
		t.FailNow()
	}
}*/
