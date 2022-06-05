#include "astar.h"

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

/*
w w w w w w w
w w w u w w w
w s w u w e w
w w w u w w w
w w w w w w w

w w w w w w w
w w w u u w w
w s w u w e w
w w w u u w w
w w w w w w w
*/
