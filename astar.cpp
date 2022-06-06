#include "astar.h"
#include <fstream>

int Node::is_same(Node a) { return (i == a.get_i()) && (j == a.get_j()); }
int Map::init() {
  cout << "Now we are initializing the Map:" << name << endl;
  int r = get_row();
  int c = get_column();
  file_flow.open(file_name);

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      int st;
      file_flow >> st;
      (*this)(i, j).set_status(st);
      (*this)(i, j).set(i, j);
    }
  }
  file_flow.close();
  return 0;
}
int Map::set_target() {
  while (1) {
    cout << "Please input your start location, must be walkable" << endl;
    int i, j;
    cin >> i >> j;
    if ((*this)(i, j).is_walkable()) {
      start_node = &(*this)(i, j);
      openList.push(*start_node);
      break;
    } else {
      cout << "Invalid location, try again." << endl;
    }
  }
  while (1) {
    cout << "Please input your end location, must be walkable" << endl;
    int i, j;
    cin >> i >> j;
    if ((*this)(i, j).is_walkable()) {
      end_node = &(*this)(i, j);
      break;
    } else {
      cout << "Invalid location, try again." << endl;
    }
  }
  return 0;
}
int Map::print() {
  cout << "Outputting the Map:" << name << endl;
  file_flow.open("road.txt");
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      file_flow << ((*this)(i, j).get_listed() == ROAD) << " ";
    }
    file_flow << endl;
  }
  file_flow.close();
  return 0;
}

Map &Map::operator=(Map a) {
  if (this == &a) {
    return *this;
  }
  int l = a.get_row();
  int r = a.get_column();
  memcpy((void *)this, (void *)&a, sizeof(Map));
  _map = new Node[l * r];
  memcpy(_map, a._map, sizeof(int) * (l * r));
  // cout<<_map[0]<<endl;
  return *this;
}

void Map::update() {
  Node temp = openList.top();
  int temp_i = temp.get_i();
  int temp_j = temp.get_j();
  Node &now = (*this)(temp_i, temp_j);
  // cout << "now at ";
  // now.print();
  // cout << endl;
  openList.pop();

  now.set_listed(IN_CLOSE_LIST);
  int I = now.get_i();
  int J = now.get_j();
  for (int i = I - 1; i <= I + 1; i++) {
    if (i < 0 || i >= get_row())
      continue;
    for (int j = J - 1; j <= J + 1; j++) {
      if (j < 0 || J >= get_column() || (i == I && j == J))
        continue;
      int d = (dis(i, I, j, J) == 1) ? 10 : 14;
      // cout<<"i="<<i<<", j="<<j<<endl;
      if ((!(*this)(i, j).is_walkable()) ||
          ((*this)(i, j).get_listed() == IN_CLOSE_LIST)) { //
        // cout << "need to continue, status at ";
        // (*this)(i, j).print();
        // cout << " is " << (*this)(i, j).get_status() << endl;
        continue;
      } else if ((*this)(i, j).is_same(*end_node)) {
        // cout << "Reaching endNode ";
        // (*this)(i, j).print();
        // cout << " status is " << (*this)(i, j).get_status();
        // cout << endl;
        (*this)(i, j).set_parent(&now);
        return;
      } else if ((*this)(i, j).is_walkable() &&
                 (*this)(i, j).get_listed() == NOT_IN_LIST) {
        (*this)(i, j).set_listed(IN_OPEN_LIST);
        // cout << "Adding node ";
        // (*this)(i, j).print();
        // cout << endl;
        openList.push((*this)(i, j));
        (*this)(i, j).set_parent(&now);
        // cout << "Now the parent of ";
        // (*this)(i, j).print();
        // cout << " is ";
        // now.print();
        // printf(" %ud and size of node is %lx\n", (*this)(i,
        // j).get_parent(),
        //        sizeof(Node));

        // (*this)(i, j).print();
        // cout << "<-";
        // (*this)(i, j).get_parent()->print();
        // if ((*this)(i, j).get_parent()->get_parent()!=NULL) {
        //   cout << "<-";
        //   (*this)(i, j).get_parent()->get_parent()->print();
        // }
        // cout << endl;

        (*this)(i, j).set_G(d);
        int de = dis(i, end_node->get_i(), j, end_node->get_j());
        (*this)(i, j).set_H(de);
      } else if ((*this)(i, j).get_listed() == IN_OPEN_LIST) {
        if (now.get_G() + d < (*this)(i, j).get_G()) {
          (*this)(i, j).reset_G(now.get_G() + d);
          (*this)(i, j).set_parent(&now);
          // cout << "Now the parent of ";
          // (*this)(i, j).print();
          // cout << " is ";
          // (*this)(i, j).get_parent()->print();
          // cout << endl;
          //   printf(" %ud and size of node is %lx\n", (*this)(i,
          //   j).get_parent(),
          //          sizeof(Node));
        }

      } else if (openList.empty()) {
        cout << "Failed to find a path." << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
  update();
}