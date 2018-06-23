#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main()
{
  ifstream instream("scene.txt");
  string str;
  int x, y, z;
  
  while(!instream.eof())
  {
    instream >> str >> x>> y>> z;
    cout << str << " " << x << " " << y << " " << z << endl;
  }

  instream.close();
  return 0;
}
