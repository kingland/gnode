package gnode
import "strconv"

type  Version struct {
  Major int
  Minor int
  Patch int
}

func NewVersion() Version{
  return Version {
    Major : 0,
    Minor : 0,
    Patch : 1,
  }
}

func (v Version) String() string {
  return strconv.Itoa(v.Major) + "." + strconv.Itoa(v.Minor) +"."+strconv.Itoa(v.Patch)
}
