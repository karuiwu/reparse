#include "reparse.h"

#define DEBUG 0
#define PRINTFEATS 0

double ACTCUTOFF = 0;
int NUMACTCUTOFF = 0;

int TRAIN = 0;
int MAXSTATES = 0;

class PhraseNode {
    int id;
    //int position;
    int child_num;
    int child_num_id;
    //int child_orientation;
    //int child_num_left;
    //int child_num_right;
    string token; 
    PhraseNode *parent;
    deque<PhraseNode *> children;
    //deque<PhraseNode *> left_children;
    //deque<PhraseNode *> right_children;
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

// input: a parser state, a vector that will contain features
// This fills the given vector with features corresponding
// to the given parser state
int makefeats(parserstate& parser_state, vector<string>& feature_vector) {
  feature_vector.clear();

  // gets three items from the stack
  const item *s1 = parser_state.getst(1);
  const item *s2 = parser_state.getst(2);
  const item *s3 = parser_state.getst(3);

  // gets three items from the queue
  const item *q1 = pst.getq(1);
  const item *q2 = pst.getq(2);
  const item *q3 = pst.getq(3);

  int dist1 = s1->idx - s2->idx;
  int dist2 = q1->idx - s1->idx;

  // feature for distance between s1 and s2
  feature_vector.push_back(toString(feature_vector.size()) + "~" + toString(dist1));
  feature_vector.push_back(toString(feature_vector.size()) + "~" + toString(dist2));
  feature_vector.push_back(toString(feature_vector.size()) + "~" + toString(dist1) + "." + toString(dist2));

  // part-of-speech tag between s1 and s2, if any
  if (dist1 > 1) {
    feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s1->idx - 1].pos);
  }
  else {
    feature_vector.push_back(toString(feature_vector.size()) + "~" + "NONE");
  }

  if (dist1 > 2) {
    feature_vector.push_back(toString(feature_vector.size()) + "~" + pst.inputq[s2->idx + 1].pos);
  }
  else {
    feature_vector.push_back(toString(feature_vector.size()) + "~" + "NONE");
  }

  feature_vector.push_back(toString(feature_vector.size()) + "~" + s1->word);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s1->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s1->label);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s1->lch].pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s1->lch].label);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s1->rch].pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s1->rch].label);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + s2->word);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s2->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s2->label);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s2->lch].pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s2->lch].label);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s2->rch].pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.inputq[s2->rch].label);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + s3->word);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s3->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s3->label);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + q1->word);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + q1->pos);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + q2->word);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + q2->pos);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + q3->pos);

  int n = feature_vector.size();
  for (int i = 0; i < n; i++) {
    feature_vector.push_back(toString(feature_vector.size()) + "~" + feature_vector[i] + "~" + s1->pos);
    feature_vector.push_back(toString(feature_vector.size()) + "~" + feature_vector[i] + "~" + s2->pos);
    feature_vector.push_back(toString(feature_vector.size()) + "~" + feature_vector[i] + "~" + q1->pos);
  }

  feature_vector.push_back(toString(feature_vector.size()) + "~" + q1->pos + s1->pos + s2->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s1->pos + s2->pos + s3->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + q1->pos + q2->pos + s1->pos + s2->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + s1->pos + s2->pos + s3->pos + q1->pos);

  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.prevact + s1->pos );
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.prevact + s1->pos + s2->pos);
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.prevact + q1->pos );
  feature_vector.push_back(toString(feature_vector.size()) + "~" + parser_state.prevact + q1->pos + s1->pos);

  return 0;
}

int parse (vector<item>& train_sentence, ME_Model &memod, int nparses) {
  int n = train_sentence.size()-1;
  vector<double> lenbeam((n * 2) + 10, 0.0);
  vector<parserstate> results;
  priority_queue<parserstate> parser_states;
  parserstate new_parser_state(train_sentence);
  new_parser_state.score = 1;
  new_parser_state.shift();
  parser_states.push(new_parser_state);

  parserstate current_parser_state;
  
  while (parser_states.size() > 0) {
    // finished parsing one sentence
    if ((parser_states.top().i >= n) && (parser_states.top().s.size() <= 1)) {
      results.push_back(parser_states.top());
      if (results.size() >= nparses) {
        break;
      }
    }

    // have we reached the maximum parser states on the priority queue?
    if (MAXSTATES && (parser_states.size() > (MAXSTATES * 2))) {
      priority_queue<parserstate> tmp_parser_states;
      for (int tt = 0; tt < MAXSTATES; tt++) {
        tmp_parser_states.push(parser_states.top());
        parser_states.pop();
      }
      parser_states = tmp_parser_states;
    }

    current_parser_state = parser_states.top();
    parser_states.pop();

    if (DEBUG) cout << "\nGOT PARSER STATE: " << current_parser_state.score << endl;

    string act = "S";
    string label = "NONE";

    // get items on top of stack
    const item* s1 = current_parser_state.getst(1);
    const item* s2 = current_parser_state.getst(2);

    if (DEBUG) cout << "S: " << current_parser_state.s.size() << " "
                    << "Q: " << current_parser_state.i << endl;

    if (DEBUG) cout << "S1: " << s1->word << " IDX: "
                    << " " << s1->idx << " GOLDIN: " << s1->goldin
                    << " LINK: " << s1->goldlink << endl;
    if (DEBUG) cout << "S2: " << s2->word << " IDX: "
                    << " " << s2->idx << " GOLDIN: " << s2->goldin
                    << " LINK: " << s2->goldlink << endl;

    // make features from parser state
    vector<string> features;
    makefeats(current_parser_state, features);
    vector<double> vp;

    // choose action
    if (TRAIN) {
      // is s1 a dependent of s2?
      if( (s1->goldlink == s2->idx) && (s1->goldin == 0) && current_parser_state.s.size() > 1) {
        act = "L-" + s1->goldlabel;
        label = s1->goldlabel;
      }
      // is s2 a dependent of s1?
      else if( (s2->goldlink == s1->idx) && (s2->goldin == 0) && current_parser_state.s.size() > 1) {
        act = "R-" + s2->goldlabel;
        label = s2->goldlabel;
      }
      else if (current_parser_state.i < n) {
        act = "S";
      }
      else {
        act = "F";
      }
      vp.push_back(1.0);

      /*
      // add training example                                                                      
      if(!(act == "F")) {
        me_train( memod, feats, act );
      }
      */

      if (PRINTFEATS) {
        cout << act;

        for (int i = 0; i < features.size(); i++) {
          cout << " " << features[i];
        }
        cout << endl;
      }
    }

    if (!TRAIN) {
      // classify action                                                                           
      vp = me_classify( memod, feats );
    }

    // put all possible actions in a priority queue                                                
    priority_queue<pact> actions;
    for (int i = 0; i < vp.size(); i++) {
      if (vp[i] < ACTCUTOFF) {
        continue;
      }
      if (TRAIN) {
        actions.push(pact(vp[i], act));
      }
      else {
        actions.push(pact(vp[i], memod.get_class_label(i)));
      }
    }

    int actcnt = 0;
    while (actions.size()) {
      if (NUMACTCUTOFF && (actcnt == NUMACTCUTOFF)) {
        break;
      }
      act = actions.top().label;
      double score = actions.top().score;
      actions.pop();

      parserstate new_parser_state(current_parser_state);

      if (DEBUG) cout << "   " << act << " : " << score << endl;

      if (act[0] == 'L') {
        if (DEBUG) cout << ">>> LEFT\n";
        label = act.substr(2);
        if(!new_parser_state.reduceleft(label)) {
          act = "F";
          if (DEBUG) cout << "NO!\n";
        }
        else {
          new_parser_state.score *= score;
        }
      }
      else if (act[0] == 'R') {
        if (DEBUG) cout << ">>> RIGHT\n";
        label = act.substr(2);
        if (!new_parser_state.reduceright(label)) {
          act = "F";
          if (DEBUG) cout << "NO!\n";
        }
        else {
          new_parser_state.score *= score;
        }
      }
      else if (act == "S") {
        if (DEBUG) cout << ">>> SHIFT\n";
        if (!new_parser_state.shift()) {
          act = "F";
          if (DEBUG) cout << "NO!\n";
        }
        else {
          new_parser_state.score *= score;
        }
      }
      else {
        act = "F";
      }

      if( act == "F" ) {
        if (DEBUG) cout << ">>> ACT-FAIL\n";
        continue;
      }
      actcnt++;

      if (DEBUG) cout << "PUSH NPST\n";
      if (actcnt==1 || new_parser_state.score > (lenbeam[new_parser_state.numacts] * LENBEAMFACTOR ) ) {
        parser_states.push(new_parser_state);
        if (new_parser_state.score > (lenbeam[new_parser_state.numacts])) {
          lenbeam[new_parser_state.numacts] = new_parser_state.score;
        }
      }
    }
  }

  // if we are training, don't print out a parse                                                   
  if (!TRAIN) {
    // if we are doing 1-best parsing                                                              
    // just use the CoNLL-X output format                                                          
    if (nparses == 1) {
      for (int i = 1; i < n; i++) {
        if (parser_states.size() > 0) {
          parser_states.top().inputq[i].print();
        }
        else {
          errflg = 1;
          current_parser_state.inputq[i].print();
        }
      }
    }
    // n-best output                                                                               
    else {
      cout << results.size() << " parses\n";
      for (int j = 0; j < results.size(); j++) {
        cout << results[j].score << endl;
        for (int i = 1; i < results[j].inputq.size() - 1; i++) {
          results[j].inputq[i].print();
        }
        cout << endl;
      }
    }

    cout << endl;
  }

  // we didn't find a complete parse                                                               
  if( errflg ) {
    return -1;
  }

  // we found at least one complete parse                                                          
  return 0;
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
  
  // read in parameter options                                                           
  ksopts opt( ":threads 1 :maxst 15 :numactcut 3 :actcut 0 :b 0.01 :m tmp.mod :it 400 t 0 :n 1 :h 0 :f 0 :i 1.0 help 0", argc, argv );
  opt.optset( "t", TRAIN, "Training mode.  The parser runs in parsing mode by default" );

  // open file containing training data
  ifstream train_file;
  if (opt.args.size() > 0) {
    train_file.open(opt.args[0].c_str());

    if (!train_file.is_open()) {
      cerr << "File not found: " << opt.args[0] << endl;
      exit(1);
    }
  }
  
  // read in training data
  string str;
  int linenum = 0;
  int sentnum = 0;
  int errnum = 0;
  
  vector<item> train_sentence;
  train_sentence.push_back(item());
  train_sentence.back().word = "LeftWall";
  train_sentence.back().pos = "LW";
  train_sentence.back().idx = 0;
  train_sentence.back().goldlink = 0;
  train_sentence.back().goldlabel = "LW";
  train_sentence.back().link = -1;
  train_sentence.back().label = "*NONE*";

  while (getline(train_file, str)) {
    linenum++;

    // remove new line and carriage return characters
    if (str.find("\r", 0) != string::npos) {
      str.erase(str.find("\r", 0), 1);
    }

    if (str.find("\n", 0) != string::npos) {
      str.erase(str.find("\n", 0), 1);
    }

    vector<string> tokens;
    Tokenize(str, tokens, " \t");
    // we're at the end of a sentence
    if (tokens.size() < 3) {
      // we didn't find a sentence
      if (train_sentence.size() <= 1) {
        continue;
      }

      // keeping track of number of training sentences
      sentnum++;

      // show progress of training
      if (!(sentnum % 100)) {
	cerr << sentnum << "... ";
      }

      // insert the right wall
      train_sentence.push_back(item());
      train_sentence.back().word = "RightWall";
      train_sentence.back().pos = "RW";
      train_sentence.back().idx = train_sentence.size()-1;
      train_sentence.back().goldlink = 0;
      train_sentence.back().goldlabel = "RW";
      train_sentence.back().link = -1;
      train_sentence.back().label = "*NONE*";

      for (int i = 0; i < train_sentence.size(); i++) {
        train_sentence[train_sentence[i].goldlink].goldin++;
        train_sentence[train_sentence[i].idx].goldout++;

	cout << train_sentence[i].word << endl; 
      }

      // add the sentence to the model, and check if failed
      if (parse(q, memod, nparses ) == -1) {
        cerr << "Sentence " << sentnum << ": parse failed.\n";
        errnum++;
      }

      // clear and reinitialize for the next training sentence
      train_sentence.clear();

      train_sentence.push_back(item());
      train_sentence.back().word = "LeftWall";
      train_sentence.back().pos = "LW";
      train_sentence.back().idx = 0;
      train_sentence.back().goldlink = 0;
      train_sentence.back().goldlabel = "LW";
      train_sentence.back().link = -1;
      train_sentence.back().label = "*NONE*";

      continue;
    }

    // insert the current word in the queue                                                                
    string word = tokens[1];
    string pos = tokens[4];
    string label = tokens[7];
    int link = stringTo<int>( tokens[6] );
    int idx = stringTo<int>( tokens[0] );

    train_sentence.push_back(item());
    train_sentence.back().word = word;
    train_sentence.back().pos = pos;
    train_sentence.back().idx = idx;
    train_sentence.back().goldlink = link;
    train_sentence.back().goldlabel = label;
    train_sentence.back().link = -1;
    train_sentence.back().label = "*NONE*";
  }

  /*
  string test_sentence; 
  for (int i = 1; i < argc; i++) {
    test_sentence = argv[i];
  }
  
  parser.parse(test_sentence);

  return 0;
  */
}
