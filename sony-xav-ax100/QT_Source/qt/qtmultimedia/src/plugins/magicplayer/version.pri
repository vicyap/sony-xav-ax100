
#major version number
MP_VER_MAJOR = "1"

#major version number
MP_VER_SLAVE = "1"

#develope version number: YY + WW = year + week
MP_VER_DEVELOPE = "1603"

#build human name
#MP_VER_MAKER = "$$system(logname)"

#build human tiem
MP_VER_MAKERTIME="$$system(date "+%Y/%m/%d-%H:%M:%S")"

#build data and time
MP_VER_BUILD = "$$system(git show -s --format=%an)"

#version manager number of tool: svn verion number now
MP_VER_COMMITE_ID = "$$system(git show -s --format=%H)"

#version manager number of tool: svn verion number now
#MP_VER_COMMITE_TIME = "$$system(git show -s --format=%ad)"

MP_POINT ="."
MP_DIVID =","
#MP_BUILDER="builder:"
MP_BUILDERTIME="buildtimer:"
#MP_AUTHOR="authorname:"
MP_COMMITID="commitid:"
MP_COMMITTIME="committime:"

#release version string
MPLIB_VER_NUMBER = "$$MP_VER_MAJOR$$MP_POINT$$MP_VER_SLAVE$$MP_POINT$$MP_VER_DEVELOPE"

#develope version string
MPLIB_VER_INFO = "$$MP_BUILDERTIME$$MP_VER_MAKERTIME$$MP_DIVID$$MP_COMMITID$$MP_VER_COMMITE_ID"
message($$MPLIB_VER_INFO)

#release version string
MP_VER_NUMBER = "$$MP_VER_MAJOR$$MP_POINT$$MP_VER_SLAVE$$MP_POINT$$MP_VER_DEVELOPE"

#develope version string
MP_VER_INFO = "$$MP_BUILDERTIME$$MP_VER_MAKERTIME$$MP_DIVID$$MP_COMMITID$$MP_VER_COMMITE_ID"
message($$MP_VER_INFO)


