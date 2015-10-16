package gnode
import(
  v8 "github.com/kingland/go-v8"
  "reflect"
  "os"
  "os/user"
  "strings"
  "runtime"
)

/*type Os struct{
  tmpdir func()
  homedir func()
  endianness func()
  hostname func()
  ttype func()
  platform func()
  arch func()
  release func()
  uptime func()
  loadavg func()
  totalmem func()
  cpus func()
  networkInterfaces func()
}*/

func (env *Environment) bindOS() {
  engine := env.V8Engine
  objTmp := env.NewObjectTemplate()

  objTmp.SetAccessor("os",
    //getter
    func(name string, info v8.AccessorCallbackInfo) {
      osObj := engine.NewObject().ToObject()
      //tmpdir
      osObj.SetAccessor("tmpdir",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.ostmpdir)).Value)
        },
        //setter
        nil,
        //data
        nil,
        //prop attribute
        v8.PA_ReadOnly,
      )

      //homedir
      osObj.SetAccessor("homedir",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.oshomedir)).Value)
        },
        //setter
        nil,
        //data
        nil,
        //prop attribute
        v8.PA_ReadOnly,
      )

      //hostname
      osObj.SetAccessor("hostname",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.oshostname)).Value)
        },
        //setter
        nil,
        //data
        nil,
        //prop attribute
        v8.PA_ReadOnly,
      )

      //type
      osObj.SetAccessor("type",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.ostype)).Value)
        },
        //setter
        nil,
        //data
        nil,
        //prop attribute
        v8.PA_ReadOnly,
      )

      //platform
      osObj.SetAccessor("platform",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.osplatform)).Value)
        },
        //setter
        nil,
        //data
        nil,
        //prop attribute
        v8.PA_ReadOnly,
      )

      //arch
      osObj.SetAccessor("arch",
        //getter
        func(name string, info v8.AccessorCallbackInfo){
          info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
              gofunc := funcInfo.Data().(reflect.Value)
              in := make([]reflect.Value, 0)
              out := gofunc.Call(in)
              funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
            },
            reflect.ValueOf(env.osarch)).Value)
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


func (env *Environment) ostmpdir() string {
  tmpdir := os.TempDir()
  return tmpdir
}

func (env *Environment) oshomedir() string {
  us, err := user.Current()
  if err != nil {
    return ""
  }
  homedir := us.HomeDir
  return homedir
}

func (env *Environment) oshostname() string {
  host, err := os.Hostname()
  if err != nil {
    return ""
  }
  return host
}

func (env *Environment) ostype() string {
  platform := runtime.GOOS
  if strings.ToUpper(platform) == "WINDOWS" || strings.ToUpper(platform) == "WIN" {
    return "Windows_NT"
  }
  platform = strings.ToUpper(platform[0:1]) + platform[1:]
  return platform
}

func (env *Environment) osplatform() string {
  platform := runtime.GOOS
  if strings.ToUpper(platform) == "WINDOWS" || strings.ToUpper(platform) == "WIN" {
    return "win32"
  } else {
    return strings.ToLower(platform)
  }
}

func (env *Environment) osarch() string {
  arch := runtime.GOARCH
  if strings.ToUpper(arch) == "AMD64" {
    arch = "x64"
  } else if arch == "386" {
    arch = "ia32"
  } else{
    arch = strings.ToLower(arch)
  }
  return arch
}
//os endianness
func (env *Environment) osendianness() string {
  //TODO unconplete call C language
  return "LE"
}

//os version
func (env *Environment) osrelease() string {
  //TODO unconplete call C language
  return "6.1.7601"
}

//up time
func (env *Environment) osuptime()  {
  //TODO unconplete call C language
}

//workload [5,10,15] min
func (env *Environment) osloadavg() []float32 {
  //TODO unconplete call C language
  return []float32{0.0,0.0,0.0}
}

//os total memory
func (env *Environment) ostotalmem() int {
  //TODO unconplete call C language
  return 0
}

//os free memory
func (env *Environment) osfreemem() int {
  //TODO unconplete call C language
  return 0
}

//os cpu info
func (env *Environment) oscpus() {
  //TODO unconplete call C language
}

//network interfaces info
func (env *Environment) osnetworkInterfaces()  {
  //TODO unconplete call C language
}
