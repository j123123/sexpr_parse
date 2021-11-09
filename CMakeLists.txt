cmake_minimum_required(VERSION 3.5)

project(sexpr_parse LANGUAGES C)

set(HEADERS alloc_routines.h  my_struct.h  my_struct_utils.h)
set(SOURCES alloc_routines.c  main.c  my_struct.c  my_struct_utils.c)
add_executable(sexpr_parse  ${SOURCES} ${HEADERS})
