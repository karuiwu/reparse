#include <iostream>
#include <sstream>
#include <deque>
using namespace std;

struct PhraseNode {
  string token; 
  PhraseNode *parent;
  deque<PhraseNode*> children;
};

class ShiftReduceParser {
    deque<string> my_stack;
    deque<string> my_queue;
    istringstream iss;
  public:
    void readIn(string sentence);
    void shift(); 
    void reduce(string);
    void parse(string);
    void printStack();
};

void ShiftReduceParser::readIn(string sentence) {
  iss.str(sentence);
  string token;
  while (getline(iss, token, ' ')) {
    cout << token << endl;
    my_queue.push_back(token);
  }
}

void ShiftReduceParser::shift() {
  my_stack.push_back(my_queue.front());
  my_queue.pop_front();

  printStack();
}

void ShiftReduceParser::reduce(string direction) {
  string second_element = my_stack.back();
  my_stack.pop_back();
  string first_element = my_stack.back();
  my_stack.pop_back();
  string reduced_element;

  if (direction == "left") {
    reduced_element = first_element + " -> " + second_element;
  }
  else if (direction == "right") {
    reduced_element = "[ " + first_element + " <- " + second_element + " ]";
  }
  my_stack.push_back(reduced_element);

  printStack();
}

void ShiftReduceParser::parse(string sentence) {
  readIn(sentence);
  string command;
  string direction;

  while (my_queue.size() > 0 || my_stack.size() > 1) {
    cout << "shift or reduce:" << flush;
    cin >> command;
    if (command == "shift") {
      shift();
    }
    else if (command == "reduce") {
      cout << "left or right:" << flush;
      cin >> direction;
      reduce(direction);
    }
    else {

    }
  }
}

void ShiftReduceParser::printStack() {
  cout << "[ ";
  for (int i = 0; i < my_stack.size(); i++) {
    cout << '"' + my_stack[i] + '"' + ' ';
  }
  cout << "] ";

  cout << "[ ";
  for (int i = 0; i < my_queue.size(); i++) {
    cout << '"' + my_queue[i] + '"' + ' ';
  }
  cout << "] " << endl;
}

int main(int argc, char* argv[])
{
  ShiftReduceParser parser;
  string sentence; 
  for (int i = 1; i < argc; i++) {
    sentence = argv[i];
  }
  
  parser.parse(sentence);

  return 0;
}
