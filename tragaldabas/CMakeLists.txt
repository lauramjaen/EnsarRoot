# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
add_subdirectory (rpc)
add_subdirectory (data)
#add_subdirectory (db)
#add_subdirectory (gen)
#add_subdirectory (passive)

