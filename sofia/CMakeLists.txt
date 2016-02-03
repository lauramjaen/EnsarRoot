# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
#add_subdirectory (base)  
add_subdirectory (data)
add_subdirectory (detectors)
add_subdirectory (evtvis)
add_subdirectory (experimentdata)
add_subdirectory (field)
add_subdirectory (gen)
add_subdirectory (macros)
add_subdirectory (Mbs)
add_subdirectory (mcstack)
add_subdirectory (passive) 
add_subdirectory (plists)

