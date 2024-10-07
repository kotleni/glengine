if(WIN32) # WINDOWS
    set(WINDOWS "1")
    set(DLL_EXT ".dll")
    add_definitions(-DWINDOWS -DDLL_EXT=${DLL_EXT})
    message(FATAL_ERROR "Windows is not supported YET")
elseif(UNIX AND NOT APPLE) # LINUX
    set(LINUX "1")
    set(DLL_EXT ".so")
    add_definitions(-DLINUX -DDLL_EXT=${DLL_EXT})
elseif(UNIX AND APPLE) # MACOS
    set(OSX "1")
    set(DLL_EXT ".dylib")
    add_definitions(-DOSX -DDLL_EXT=${DLL_EXT})
else()
    message(FATAL_ERROR "Platform undetected!")
endif()