if(WIN32) # WINDOWS
    add_definitions(-DWINDOWS)
    set(WINDOWS "1")
    set(DLL_EXT ".dll")
    message(FATAL_ERROR "Windows is not supported YET")
elseif(UNIX AND NOT APPLE) # LINUX
    add_definitions(-DLINUX)
    set(LINUX "1")
    set(DLL_EXT ".so")
elseif(UNIX AND APPLE) # MACOS
    add_definitions(-DOSX)
    set(OSX "1")
    set(DLL_EXT ".dylib")
else()
    message(FATAL_ERROR "Platform undetected!")
endif()