#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;

int main(int argc, char* argv[])
{
  vector<string> my_stack;
  queue<string> my_queue;
  double mark;
  string sentence; 
  istringstream iss;
  string val;
  for (int i = 1; i < argc; i++) {
    sentence = argv[i];
  }

  iss.str(sentence);

  cout << iss.str().size() << endl;

  //argv[0] is the name of the file
  while (getline(iss, val, ' ')) {
    std::cout << val << std::endl;
    my_queue.push(val);
  }

  return 0;
}
