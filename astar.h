#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>

#define VNAME(value) (#value)
#define R 65
#define C 105

#define dis(i1, i2, j1, j2) abs(i1 - i2) + abs(j1 - j2)
using namespace std;

typedef int status;
#define ENTRY 0
#define MAIN 1
#define RELIEF 2
#define GREEN 3
#define TBULIDING 4
#define DORMITORY 5
#define ASSIST 6
#define GYM 7
#define WATER 8
#define NOT_ALLOW 9

#define IN_OPEN_LIST 1
#define NOT_IN_LIST 0
#define IN_CLOSE_LIST -1

class Node {
  Node *parent;
  status flag;
  int listed;//0表示没有加入任何list，1表示加入了open list，-1表示加入了close list
  int G;
  int H;
  int i;
  int j;

public:
  Node() : parent(NULL), flag(MAIN), G(0), H(0), i(0), j(0) {}
  int get_F() { return G + H; }
  status get_status() { return flag; }
  int is_walkable() { return !(flag / 4); }
  int get_G() { return G; }
  int get_H() { return H; }
  int get_i() { return i; }
  int get_j() { return j; }
  int get_listed(){return listed;}
  Node *get_parent() { return parent; }
  void print() { cout << "(" << i << ", " << j << ")"; }
  void set(int _i, int _j) { i = _i, j = _j; }
  void set_parent(Node *p) { parent = p; }
  void set_status(status _flag) { flag = _flag; }
  void set_G(int g) { G += g; }
  void set_H(int h) { H += h; }
  void set_listed(int l) { listed = l; }
  void reset_G(int g) { G = g; }
  void reset_H(int h) { H = h; }
  int is_same(Node a);
  friend bool operator<(Node n1, Node n2) { return n1.get_F() < n2.get_F(); }
};

class Map {
  int row, column; //行与列
  Node *_map;
  fstream file_flow;
  string file_name;
  priority_queue<Node> openList;

public:
  Node *start_node;
  Node *end_node;
  const char *name;
  Map(const char *n, string fname, int _row, int _column)
      : row(_row), column(_column), start_node(NULL), end_node(NULL) {
    name = n;
    file_name = fname;
    _map = new Node[row * column];
   // cout << "Constructor function of " << name << " has been called." << endl;
  }
  Map(int _row, int _column)
      : row(_row), column(_column), start_node(NULL), end_node(NULL) {
    name = NULL;
    _map = new Node[row * column];
    // cout << "Constructor function has been called, no name has been assigned "
    //         "yet."
    //      << endl;
  }

  int init();
  int set_target();
  void assign_name(const char *n) { name = n; }
  void assign_row(int _row) { row = _row; }
  void assign_column(int _column) { column = _column; }

  void test_s() { start_node->print(); }

  int print();
  Node &operator()(int _row, int _column) {
    return _map[_row * column + _column];
  }
  Map &operator=(Map a);
  int get_row() { return row; }
  int get_column() { return column; }

  void update();
  void update_G();
  void update_H();

  ~Map() {
    // (!name) ? cout << "Destructor function has been called, no name has been "
    //                   "assigned."
    //                << endl
    //         : cout << "Destructor function of " << name << " has been called"
    //                << endl;
    delete _map;
  }
};