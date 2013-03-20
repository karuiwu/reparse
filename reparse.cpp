#include "reparse.h"

#define DEBUG 0
#define PRINTFEATS 0
int TRAIN = 0;

class PhraseNode {
    int id;
  //int position;
  int child_num;
  int child_num_id;
  // int child_orientation;
  //int child_num_left;
  //int child_num_right;
    string token; 
    PhraseNode *parent;
    deque<PhraseNode *> children;
     deque<PhraseNode *> left_children;
  // deque<PhraseNode *> right_children;
  public:
    friend std::ostream& operator<<(std::ostream&, const PhraseNode&);
    friend class ShiftReduceParser;
    string printStack();
};

string PhraseNode::printStack() {
  return token;
}

std::ostream& operator<<(std::ostream &strm, const PhraseNode &node) {
  /*strm << "(" << node.parent << endl;
  for (int i = 0; i < node.children.size(); i++) {
    strm << node.children[i] << endl;
  }
  return strm << ")";*/
  return strm << node.token;
}

class ShiftReduceParser {
    deque<PhraseNode *> my_stack;
    deque<PhraseNode *> my_queue;
    istringstream iss;
  //int words;
  public:
    ShiftReduceParser();
    void readIn(string sentence);
    void shift(); 
    void reduce(string);
  //void unshift();
    void parse(string);
    void printStack();
};

ShiftReduceParser::ShiftReduceParser() {
  //words = 0;
}

void ShiftReduceParser::readIn(string sentence) {
  iss.str(sentence);
  string token;
  int id = 0;
  while (getline(iss, token, ' ')) {
    cout << token << " ";
    PhraseNode *root = new PhraseNode;
    root->token = token;
    root->id = id++; 
    my_queue.push_back(root);
  }
}

void ShiftReduceParser::shift() {
  //my_queue.front()->position = words++;
  my_stack.push_back(my_queue.front());
  my_queue.pop_front();

  printStack();
}

void ShiftReduceParser::reduce(string direction) {
  PhraseNode *right = my_stack.back();
  my_stack.pop_back();
  PhraseNode *left = my_stack.back();
  my_stack.pop_back();
  
  if (direction == "right") {
    right->parent = left;
    right->child_num_id = left->child_num++;
    left->children.push_back(right);
    my_stack.push_back(left);
  }
  else {
    left->parent = right;
    left->child_num_id = right->child_num++;
    right->children.push_front(left);
    //right->position = --words-1;
    my_stack.push_back(right);
  }

  printStack();
}

/*void ShiftReduceParser::unshift() {
  my_queue.push_front(my_stack.back());
  my_stack.pop_back();

  printStack();
}

void ShiftReduceParser::unreduce(string direction) {
  PhraseNode *root = my_stack.back();
  if (root->parent != NULL) {
    PhraseNode *parent = root->parent;
    parent->children.erase(parent->children.begin()+root->child_num_id);
    root->parent = NULL;
    if (root->position < parent->position) {
      PhraseNode *right = parent;
      PhraseNode *left = root;
    }
    else {
      PhraseNode *right = root;
      PhraseNode *left = parent;
    }
 
    my_queue.push_front(right);
    my_queue.push_front(left);
  }
  else {
    if (direction == "right") {
      
    }
    else {

    }
  }
  PhraseNode *right = my_stack.back();
  my_stack.pop_back();
  PhraseNode *left = my_stack.back();
  my_stack.pop_back();

  if (direction == "right") {
    left->children.push_back(right);
    my_stack.push_back(left);
  }
  else {
    right->children.push_back(left);
    my_stack.push_back(right);
  }

  printStack();
 }*/

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
    /* else if (command == "unshift") {
      unshift();
    }
    else if (command == "unreduce") {
      cout << "left or right:" << flush;
      cin >> direction;
      unreduce(direction);
      }*/
  }
}

void ShiftReduceParser::printStack() {
  cout << "[ ";
  for (int i = 0; i < my_stack.size(); i++) {
    cout << '"' + (*my_stack[i]).printStack() + '"' + ' ';
  }
  cout << "] ";

  cout << "[ ";
  for (int i = 0; i < my_queue.size(); i++) {
    cout << '"' + (*my_queue[i]).printStack() + '"' + ' ';
  }
  cout << "] " << endl;
}

int main(int argc, char* argv[])
{
  ShiftReduceParser parser;

  /*
  // read options                                                                     
  ksopts opt( ":threads 1 :maxst 15 :numactcut 3 :actcut 0 :b 0.01 :m tmp.mod :it 400 t 0 :n 1 :h 0 :f 0 :i 1.0 help 0", argc, argv );
  opt.optset( "t", TRAIN, "Training mode.  The parser runs in parsing mode by default" );

  // open the input file...
  ifstream infile;
  if( opt.args.size() > 0 ) {
    infile.open( opt.args[0].c_str() );

    if( !infile.is_open() ) {
      cerr << "File not found: " << opt.args[0] << endl;
      exit( 1 );
    }
  }

  // ... or use stdin
  istream *istrptr;
  if( infile.is_open() ) {
    istrptr = &infile;
  }
  else {
    istrptr = &std::cin;
  }

  // read in training data
  string str;
  int linenum = 0;
  int sentnum = 0;
  int errnum = 0;

  vector<item> q;
  q.push_back(item());
  q.back().word = "LeftWall";
  q.back().pos = "LW";
  q.back().idx = 0;
  q.back().goldlink = 0;
  q.back().goldlabel = "LW";
  q.back().link = -1;
  q.back().label = "*NONE*";

  while(getline(*istrptr, str)) {
    linenum++;

    // remove new line and carriage return characters
    if( str.find( "\r", 0 ) != string::npos ) {
      str.erase( str.find( "\r", 0 ), 1 );
    }

    if( str.find( "\n", 0 ) != string::npos ) {
      str.erase( str.find( "\n", 0 ), 1 );
    }

    vector<string> tokens;
    Tokenize(str, tokens, " \t");
    if( tokens.size() < 3 ) {
      if( q.size() <= 1 ) {
        continue;
      }

      // process training sentence
      sentnum++;

      if(!(sentnum % 100)) {
	cerr << sentnum << "... ";
      }

      // insert the right wall
      q.push_back(item());
      q.back().word = "RightWall";
      q.back().pos = "RW";
      q.back().idx = q.size() - 1;
      q.back().goldlink = 0;
      q.back().goldlabel = "RW";
      q.back().link = -1;
      q.back().label = "*NONE*";

      for( int i = 0; i < q.size(); i++ ) {
        q[q[i].goldlink].goldin++;
        q[q[i].idx].goldout++;
      }

      

      // clear the queue, and initialize                                                                   
      q.clear();

      q.push_back(item());
      q.back().word = "LeftWall";
      q.back().pos = "LW";
      q.back().idx = 0;
      q.back().goldlink = 0;
      q.back().goldlabel = "LW";
      q.back().link = -1;
      q.back().label = "*NONE*";

      continue;
    }

    // insert the current word in the queue                                                                
    string word = tokens[1];
    string pos = tokens[4];
    string label = tokens[7];
    int link = stringTo<int>( tokens[6] );
    int idx = stringTo<int>( tokens[0] );

    q.push_back(item());
    q.back().word = word;
    q.back().pos = pos;
    q.back().idx = idx;
    q.back().goldlink = link;
    q.back().goldlabel = label;
    q.back().link = -1;
    q.back().label = "*NONE*";
  }
  */
  string sentence; 
  for (int i = 1; i < argc; i++) {
    sentence = argv[i];
  }
  
  parser.parse(sentence);

  return 0;
}
