package gnode
import(
  v8 "github.com/kingland/go-v8"
  "runtime"
  "reflect"
  "strings"
  "os"
)

type versions struct {
  gnode string
  v8 string
  modules uint
}

type release struct {
  name string
  libUrl string
  sourceUrl string
  //headersUrl string
}

type config struct {
  host_arch string
  target_arch string
}

/*type ProcessFunction struct{
  argv func()
  abort func()
  chdir func()
  cwd func()
  config func()
  evn func()
  exit func()
  exitCode func()
  execPath func()
  execArgv func()
  getgid func()
  getegid func()
  getgroup func()
  getuid func()
  geteuid func()
  initgroups func()
  kill func()
  memoryUsage func()
  mainModule func()
  nextTick func()
  uptime func()
  hrtime func()
  setgid func()
  setegid func()
  setgroups func()
  setuid func()
  seteuid func()
  stdout func()
  stderr func()
  stdin func()
  title func()
  arch func()
  platform func()
  pid func()
  release func()
  version func()
  versions func()
}*/

func  (env *Environment) bindProcess() {
    engine := env.V8Engine
    objTmp := env.NewObjectTemplate()
    //objTmp.Bind("Process", Process{})
    objTmp.Bind("Versions",versions{})
    objTmp.Bind("Release",release{})
    objTmp.Bind("Config",config{})
    //objTmp.Bind("ProcessFunction", ProcessFunction{})
    //objTmp.Bind("process",env.NewProcess())
    objTmp.SetAccessor("process",
      //getter
      func(name string, info v8.AccessorCallbackInfo) {
          procObj := engine.NewObject().ToObject()
          //version
          procObj.SetAccessor("version",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.version())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )
          //config
          procObj.SetAccessor("config",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.config())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //versions
          procObj.SetAccessor("versions",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.versions())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //pid
          procObj.SetAccessor("pid",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
                  gofunc := funcInfo.Data().(reflect.Value)
                  in := make([]reflect.Value, 0)
                  out := gofunc.Call(in)
                  funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
                },
                reflect.ValueOf(env.pid)).Value)
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )
          //ppid
          procObj.SetAccessor("ppid",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(engine.NewFunction(func (funcInfo v8.FunctionCallbackInfo){
                  gofunc := funcInfo.Data().(reflect.Value)
                  in := make([]reflect.Value, 0)
                  out := gofunc.Call(in)
                  funcInfo.ReturnValue().Set(engine.GoValueToJsValue(out[0]))
                },
                reflect.ValueOf(env.ppid)).Value)
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //release
          procObj.SetAccessor("release",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.release())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //cwd
          procObj.SetAccessor("cwd",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.cwd())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //arch
          procObj.SetAccessor("arch",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.arch())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )

          //platform
          procObj.SetAccessor("platform",
            //getter
            func(name string, info v8.AccessorCallbackInfo){
              info.ReturnValue().Set(env.platform())
            },
            //setter
            nil,
            //data
            nil,
            //prop attribute
            v8.PA_ReadOnly,
          )
          info.ReturnValue().Set(procObj.Value)
      },
      //setter
      nil,
      //data
      nil,
      //prop attribute
      v8.PA_ReadOnly,
    )
    env.V8ObjTmp = objTmp
}

func  (env *Environment) version() *v8.Value {
  ver := "v"+ NewVersion().String()
  engine := env.V8Engine
  return engine.GoValueToJsValue(reflect.ValueOf(ver))
}

func (env *Environment) versions() *v8.Value {
  vers := versions{
    gnode : NewVersion().String(),
    v8 : v8.GetVersion(),
    modules : 0,
  }
  engine := env.V8Engine
  return engine.GoValueToJsValue(reflect.ValueOf(vers))
}

func (env *Environment) cwd() *v8.Value {
  wd,error := os.Getwd();
  if error != nil{
  }
  engine := env.V8Engine
  return engine.GoValueToJsValue(reflect.ValueOf(wd))
}

func (env *Environment) env() {

}

func (env *Environment) config() *v8.Value {
  //TODO : uncomplete
  engine := env.V8Engine
  conf := config{
    host_arch : "x64",
    target_arch : "x64",
  }
  return engine.GoValueToJsValue(reflect.ValueOf(conf))
}

func (env *Environment) release() *v8.Value {
  engine := env.V8Engine
  rel := &release{
    name : "gnode",
  }
  return engine.GoValueToJsValue(reflect.ValueOf(rel))
}

func (env *Environment) pid() int {
  pid := os.Getpid()
  //engine := env.V8Engine
  //return engine.GoValueToJsValue(reflect.ValueOf(pid))
  return pid
}

func (env *Environment) ppid() int {
  ppid := os.Getppid()
  return ppid
}

func (env *Environment) title() {

}

func (env *Environment) arch() *v8.Value{
  arch := runtime.GOARCH
  if strings.ToUpper(arch) == "AMD64" {
    arch = "x64"
  } else if arch == "386" {
    arch = "ia32"
  } else {
    arch = strings.ToLower(arch)
  }
  engine := env.V8Engine
  return engine.GoValueToJsValue(reflect.ValueOf(arch))
}

func (env *Environment) platform() *v8.Value {
  platform := runtime.GOOS
  if strings.ToUpper(platform) == "WINDOWS" || strings.ToUpper(platform) == "WIN"{
    platform = "win32"
  }
  engine := env.V8Engine
  return engine.GoValueToJsValue(reflect.ValueOf(strings.ToLower(platform)))
}
