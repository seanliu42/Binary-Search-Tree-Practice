#
# Makefile for Binary Search Tree
#
# Homework 3 (fall 2021)
#

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 

hw3: hw3.o BinarySearchTree.o
	${CXX} $(CXXFLAGS) -o $@ $^
	
# This rule builds BinarySearchTree.o
BinarySearchTree.o: BinarySearchTree.cpp BinarySearchTree.h
	$(CXX) $(CXXFLAGS) -c BinarySearchTree.cpp

# The below rule will be used by unit_test.
unit_test: unit_test_driver.o BinarySearchTree.o
	$(CXX) $(CXXFLAGS) $^ 

%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf hw3 *.o *.dSYM

# Don't forget to add your testing files to this rule if you use it!
provide:
	provide comp15 hw3_BST hw3.cpp BinarySearchTree.cpp BinarySearchTree.h \
	        README Makefile unit_tests.h

