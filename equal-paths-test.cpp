#include <iostream>
#include <cstdlib>
#include "equal-paths.h"
using namespace std;


Node* a;
Node* b;
Node* c;
Node* d;
Node* e;
Node* f;

void setNode(Node* n, int key, Node* left=NULL, Node* right=NULL)
{
  n->key = key;
  n->left = left;
  n->right = right;
}

void test1(const char* msg)
{
  setNode(a,1,NULL, NULL);
  cout << msg << ": " <<   equalPaths(a) << endl;
}

void test2(const char* msg)
{
  setNode(a,1,b,NULL);
  setNode(b,2,NULL,NULL);
  cout << msg << ": " <<   equalPaths(a) << endl;
}

void test3(const char* msg)
{
  setNode(a,1,b,c);
  setNode(b,2,NULL,NULL);
  setNode(c,3,NULL,NULL);
  cout << msg << ": " <<   equalPaths(a) << endl;
}

void test4(const char* msg)
{
  setNode(a,1,NULL,c);
  setNode(c,3,NULL,NULL);
  cout << msg << ": " <<   equalPaths(a) << endl;
}

void test5(const char* msg)
{
  setNode(a,1,b,c);
  setNode(b,2,NULL,d);
  setNode(c,3,NULL,NULL);
  setNode(d,4,NULL,NULL);
  cout << msg << ": " <<   equalPaths(a) << endl;
}

int main()
{
  a = new Node(1);
  b = new Node(2);
  c = new Node(3);
  d = new Node(4);

  test1("Test1"); // expected 1
  test2("Test2"); // expected 1
  test3("Test3"); // expected 1
  test4("Test4"); // expected 1
  test5("Test5"); // expected 0
 
  delete a;
  delete b;
  delete c;
  delete d;
}

