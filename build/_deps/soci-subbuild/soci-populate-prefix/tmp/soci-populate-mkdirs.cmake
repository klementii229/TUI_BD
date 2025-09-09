# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/kernel/Desktop/TUIBD/build/_deps/soci-src")
  file(MAKE_DIRECTORY "/home/kernel/Desktop/TUIBD/build/_deps/soci-src")
endif()
file(MAKE_DIRECTORY
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-build"
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix"
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/tmp"
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/src/soci-populate-stamp"
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/src"
  "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/src/soci-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/src/soci-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/kernel/Desktop/TUIBD/build/_deps/soci-subbuild/soci-populate-prefix/src/soci-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
