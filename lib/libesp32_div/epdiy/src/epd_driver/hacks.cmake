#
# Applies CMAKE_CXX_FLAGS to all targets in the current CMake directory.
# After this operation, CMAKE_CXX_FLAGS is cleared.
#
macro(apply_global_cxx_flags_to_all_targets)
    separate_arguments(_global_cxx_flags_list UNIX_COMMAND ${CMAKE_CXX_FLAGS})
    get_property(_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach(_target ${_targets})
        target_compile_options(${_target} PUBLIC ${_global_cxx_flags_list})
    endforeach()
    unset(CMAKE_CXX_FLAGS)
    set(_flag_sync_required TRUE)
endmacro()

#
# Removes the specified compiler flag from the specified file.
#   _target     - The target that _file belongs to
#   _file       - The file to remove the compiler flag from
#   _flag       - The compiler flag to remove.
#
# Pre: apply_global_cxx_flags_to_all_targets() must be invoked.
#
macro(remove_flag_from_file _target _file _flag)
    get_target_property(_target_sources ${_target} SOURCES)
    # Check if a sync is required, in which case we'll force a rewrite of the cache variables.
    if(_flag_sync_required)
        unset(_cached_${_target}_cxx_flags CACHE)
        unset(_cached_${_target}_${_file}_cxx_flags CACHE)
    endif()
    get_target_property(_${_target}_cxx_flags ${_target} COMPILE_OPTIONS)
    # On first entry, cache the target compile flags and apply them to each source file
    # in the target.
    if(NOT _cached_${_target}_cxx_flags)
        # Obtain and cache the target compiler options, then clear them.
        get_target_property(_target_cxx_flags ${_target} COMPILE_OPTIONS)
        set(_cached_${_target}_cxx_flags "${_target_cxx_flags}" CACHE INTERNAL "")
        set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "")
        # Apply the target compile flags to each source file.
        foreach(_source_file ${_target_sources})
            # Check for pre-existing flags set by set_source_files_properties().
            get_source_file_property(_source_file_cxx_flags ${_source_file} COMPILE_FLAGS)
            if(_source_file_cxx_flags)
                separate_arguments(_source_file_cxx_flags UNIX_COMMAND ${_source_file_cxx_flags})
                list(APPEND _source_file_cxx_flags "${_target_cxx_flags}")
            else()
                set(_source_file_cxx_flags "${_target_cxx_flags}")
            endif()
            # Apply the compile flags to the current source file.
            string(REPLACE ";" " " _source_file_cxx_flags_string "${_source_file_cxx_flags}")
            set_source_files_properties(${_source_file} PROPERTIES COMPILE_FLAGS "${_source_file_cxx_flags_string}")
        endforeach()
    endif()
    list(FIND _target_sources ${_file} _file_found_at)
    if(_file_found_at GREATER -1)
        if(NOT _cached_${_target}_${_file}_cxx_flags)
            # Cache the compile flags for the specified file.
            # This is the list that we'll be removing flags from.
            get_source_file_property(_source_file_cxx_flags ${_file} COMPILE_FLAGS)
            separate_arguments(_source_file_cxx_flags UNIX_COMMAND ${_source_file_cxx_flags})
            set(_cached_${_target}_${_file}_cxx_flags ${_source_file_cxx_flags} CACHE INTERNAL "")
        endif()
        # Remove the specified flag, then re-apply the rest.
        list(REMOVE_ITEM _cached_${_target}_${_file}_cxx_flags ${_flag})
        string(REPLACE ";" " " _cached_${_target}_${_file}_cxx_flags_string "${_cached_${_target}_${_file}_cxx_flags}")
        set_source_files_properties(${_file} PROPERTIES COMPILE_FLAGS "${_cached_${_target}_${_file}_cxx_flags_string}")
    endif()
endmacro()
