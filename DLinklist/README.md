# Doubly Linked List Implementation

This is the tutorial for a doubly linked list implementation and the corresponding unit test.

dlinkedlist.c - source code for the doubly linked list implementation
dlinkedlist.h - header file for the doubly linked list implementation
dlinkedlist_unittest.c - source code for the doubly linked list unit test
Makefile - makefile for the build and testing

The unit test is implemented under the cmocka framework. cmocka is included in this repository as a git subtree.
See 3rd-party/README.md for details.

Note that 'cmake' is required for building the unit test.

## Build
- building cmocka
  make cmocka

- building and running the unit test
  make test

- clean up local build artifacts:
  make clean

- removing all generated files, including 3rd-party builds
  make clobber


  ### Reference
  https://github.com/afosdick/ecen5813/tree/develop/tutorial/unit_tests
  
