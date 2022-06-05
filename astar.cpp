#include "astar.h"

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
  Node &now = (*this)(temp_i, temp_j);
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
        // // printf(" %ud and size of node is %lx\n", (*this)(i,
        // j).get_parent(),
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
          cout << endl;
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