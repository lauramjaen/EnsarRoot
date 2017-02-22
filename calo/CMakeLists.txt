# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
add_subdirectory (base)
add_subdirectory (data)
add_subdirectory (gen)
add_subdirectory (passive)
add_subdirectory (cal)
if(FAIRDB_FOUND)
add_subdirectory (db)
add_subdirectory (commondb)
endif(FAIRDB_FOUND)
