#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>

#define VNAME(value) (#value)
#define R 5
#define C 7

#define dis(i1, i2, j1, j2) abs(i1 - i2) + abs(j1 - j2)
using namespace std;

enum status {
  unwalkable,
  walkable,
  inCloselist,
  inOpenlist,
  startNode,
  endNode
};
class Node {
  Node *parent;
  status flag;
  int G;
  int H;
  int i;
  int j;

public:
  Node() : parent(NULL), flag(walkable), G(0), H(0), i(0), j(0) {}
  int get_F() { return G + H; }
  status get_status() { return flag; }
  int get_G() { return G; }
  int get_H() { return H; }
  int get_i() { return i; }
  int get_j() { return j; }
  Node *get_parent() { return parent; }

  void print() { cout << "(" << i << ", " << j << ")"; }

  void set(int _i, int _j) {
    i = _i;
    j = _j;
  }
  void set_parent(Node *p) { parent = p; }

  void set_status(status _flag) { flag = _flag; }
  void set_G(int g) { G += g; }
  void set_H(int h) { H += h; }

  void reset_G(int g) { G = g; }
  void reset_H(int h) { H = h; }
  friend bool operator<(Node n1, Node n2) { return n1.get_F() < n2.get_F(); }
};

class Map {
  int row, column; //行与列
  Node *_map;

  priority_queue<Node> openList;
  //  priority_queue<Node> closeList;

public:
  Node *s;
  Node *e;
  const char *name;
  Map(const char *n, int _row, int _column)
      : row(_row), column(_column), s(NULL), e(NULL) {
    name = n;
    _map = new Node[row * column];
    cout << "Constructor function of " << name << " has been called." << endl;
  }
  Map(int _row, int _column) : row(_row), column(_column), s(NULL), e(NULL) {
    name = NULL;
    _map = new Node[row * column];
    cout << "Constructor function has been called, no name has been assigned "
            "yet."
         << endl;
  }

  int init();
  void assign_name(const char *n) { name = n; }
  void assign_row(int _row) { row = _row; }
  void assign_column(int _column) { column = _column; }

  void test_s() { s->print(); }

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
    (!name) ? cout << "Destructor function has been called, no name has been "
                      "assigned."
                   << endl
            : cout << "Destructor function of " << name << " has been called"
                   << endl;
    delete _map;
  }
};

int main() {
  Map shahe(VNAME(shahe), R, C);
  shahe.init();
  shahe.print();
  // shahe.test_s();
  // for (int i = 0; i < 20; i++) {
  shahe.update();
  //}
  Node *temp = shahe.e;
  cout << "The path found by algorithm is :" << endl;
  while (temp != NULL) {
    temp->print();
    cout << "<-";
    temp = temp->get_parent();
  }
  cout << endl;
  shahe.print();
  //   shahe(0, 3).get_parent()->print();
  //   cout<<endl;
  return 0;
}

int Map::init() {
  cout << "Now we are initializing the Map:" << name << endl;
  int r = get_row();
  int c = get_column();
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      char st;
      cin >> st;
      if (st == 'u') {
        (*this)(i, j).set_status(unwalkable);
        (*this)(i, j).set(i, j);
      } else if (st == 'w') {
        (*this)(i, j).set_status(walkable);
        (*this)(i, j).set(i, j);
      } else if (st == 's') {
        if (s != NULL) {
          cout << "You have already set a start postion, please set another "
                  "value here ("
               << i << ", " << j << ")." << endl;
          // dec(i, j);
          j--;
          continue;
        }
        (*this)(i, j).set_status(startNode);
        s = &(*this)(i, j);
        s->set(i, j);
        openList.push(*s);
      } else if (st == 'e') {
        if (e != NULL) {
          cout << "You have already set a end postion, please set another "
                  "value here ("
               << i << ", " << j << ")." << endl;
          j--;
          continue;
        }
        (*this)(i, j).set_status(endNode);
        e = &(*this)(i, j);
        e->set(i, j);
      } else {
        cout << "Illegal input, fail to set up at postion (" << i << ", " << j
             << ")." << endl;
        cout << "Please try again" << endl;
        j--;
      }
    }
  }

  if (s == NULL) {
    cout << "You didn't set a start postion, algorithm would only fail."
         << endl;
    exit(EXIT_FAILURE);
  } else if (e == NULL) {
    cout << "You didn't set a end postion, algorithm would only fail." << endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

int Map::print() {
  cout << "Outputting the Map:" << name << endl;
  for (int i = 0; i < row; i++) {
    cout << "[ ";
    for (int j = 0; j < column; j++) {
      cout << setw(3) << (*this)(i, j).get_status() << " ";
    }
    cout << " ]" << endl;
  }
  cout << endl;
  return 0;
}

Map &Map::operator=(Map a) {
  if (this == &a) {
    return *this;
  }
  int l = a.get_row();
  int r = a.get_column();
  memcpy(this, &a, sizeof(Map));
  _map = new Node[l * r];
  memcpy(_map, a._map, sizeof(int) * (l * r));
  // cout<<_map[0]<<endl;
  return *this;
}

void Map::update() {
  Node temp = openList.top();
  int temp_i = temp.get_i();
  int temp_j = temp.get_j();
  Node &now =(*this)(temp_i,temp_j);
//   cout << "now at ";
//   now.print();
//   cout << endl;
  openList.pop();

  if (now.get_status() != startNode) {
    now.set_status(inCloselist);
  }
  int I = now.get_i();
  int J = now.get_j();
  for (int i = I - 1; i <= I + 1; i++) {
    if (i < 0 || i > get_row())
      continue;
    for (int j = J - 1; j <= J + 1; j++) {
      if (j < 0 || J > get_column() || (i == I && j == J))
        continue;
      int d = (dis(i, I, j, J) == 1) ? 10 : 14; //注意这里有没有正确赋值
      if (((*this)(i, j).get_status() == unwalkable) ||
          ((*this)(i, j).get_status() == inCloselist) ||
          ((*this)(i, j).get_status() == startNode)) {
        // cout << "need to continue, status at ";
        // (*this)(i, j).print();
        // cout << " is " << (*this)(i, j).get_status() << endl;
        continue;
      } else if ((*this)(i, j).get_status() == walkable) {
        (*this)(i, j).set_status(inOpenlist);
        // cout << "Adding node ";
        // (*this)(i, j).print();
        // cout << endl;
        openList.push((*this)(i, j));
        (*this)(i, j).set_parent(&now);
        // cout << "Now the parent of ";
        // (*this)(i, j).print();
        // cout << " is ";
        // now.print();
        // // printf(" %ud and size of node is %lx\n", (*this)(i, j).get_parent(),
        // //        sizeof(Node));

        // // (*this)(i, j).print();
        // // cout << "<-";
        // // (*this)(i, j).get_parent()->print();
        // // if ((*this)(i, j).get_parent()->get_parent()!=NULL) {
        // //   cout << "<-";
        // //   (*this)(i, j).get_parent()->get_parent()->print();
        // // }
        // cout << endl;

        (*this)(i, j).set_G(d);
        int de = dis(i, e->get_i(), j, e->get_j());
        (*this)(i, j).set_H(de);
      } else if ((*this)(i, j).get_status() == inOpenlist) {
        if (now.get_G() + d < (*this)(i, j).get_G()) {
          (*this)(i, j).reset_G(now.get_G() + d);
          (*this)(i, j).set_parent(&now);
        //   cout << "Now the parent of ";
        //   (*this)(i, j).print();
        //   cout << " is ";
        //   (*this)(i, j).get_parent()->print();
          cout<<endl;
          //   printf(" %ud and size of node is %lx\n", (*this)(i,
          //   j).get_parent(),
          //          sizeof(Node));
        }
      } else if ((*this)(i, j).get_status() == endNode) {
        // cout << "Reaching endNode ";
        // (*this)(i, j).print();
        // cout << " status is " << (*this)(i, j).get_status();
        // cout << endl;
        (*this)(i, j).set_parent(&now);
        return;
      } else if (openList.empty()) {
        cout << "Failed to find a path." << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
  update();
}
/*
w w w w w w w
w w w u w w w
w s w u w e w
w w w u w w w
w w w w w w w
*/
