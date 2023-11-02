
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
#include <sstream>
#include <fstream>
#include <random>

using namespace std; 
class Scrambler{
  private:
    string path;
    int size;
    vector<string> allWords;  // dictionary.txt
    vector<string> words;
    int allWordsSize;
    vector<vector<string>> board;
    vector<vector<string>> board2;
    vector<string> jumbling;
    string moving;
    string randomWord;
    bool quit = false;
    string initial_board;
    string last_board;

  public:
    // Constructor: string path is the path to the dictionary file
    //              int size for the [size x size] board
    Scrambler(const string& path, const int size);
    string str() const;
    void scrambler_init();
    void load_all_words_from_dict();
    void jumble_words();
    void try_move(const string& cmd);
    bool is_over();
    string display_solution();
    vector<string> get_words();
};

Scrambler::Scrambler(const string& file, const int len){
    size = len;
    path = file;
    allWordsSize = 0;
    
    scrambler_init();
    jumble_words();
    board2 = board;
  }

  // void display_board(){
  //   for (int i = 0; i < size; i++) {
  //     for (int j = 0; j < size; j++) {
  //       cout << board[i][j] << " ";
  //     }
  //     cout << endl;
  //   }
  // }

// Function str returns the board as a string.
// Loads all words from the dictionary into a string vector
void Scrambler::load_all_words_from_dict(){
  ifstream infile;
  infile.open(path);
  
  if(infile.is_open()){
    string str;
    
    while(getline(infile, str)){
      allWords.push_back(str);
      allWordsSize++;
    }
  }
  infile.close();
  // cout << "reaches here2" << endl;
}

// Sets the value of the words array in the Class.
// This will be the set of words that will be used in the board.
void Scrambler::scrambler_init(){
  // cout << "reaches here" << endl;
  load_all_words_from_dict();
  for (int i = 0; i < size; i++) {
    board.push_back(vector<string>());
    // cout << "reaches 111" << endl;
    while (size - board[i].size() > 2) {
      // cout << "reaches 222" << endl;
      randomWord = allWords[rand() % (allWordsSize-1)];
      if (randomWord.size() <= (size - board[i].size()) && ((size - board[i].size()) - randomWord.size()) != 3) {
        words.push_back(randomWord);
        for (int j = 0; j < randomWord.size(); j++) {
          string empty = "";
          empty += randomWord[j];
          board[i].push_back(empty);
        }
        if (size > board[i].size()) {
          board[i].push_back(" ");
        }
    //     if (!board[i].empty()) {
    //       string last_word = board[i].back();
    //       if (last_word != " " && last_word != "\n" && last_word != "\t") {
    //         randomWord = " " + randomWord;
    //         wordSize++;
    //       }
    //     }
    //     board[i].push_back(randomWord);
    //     remaining -= wordSize;
      }
    } 
  }
  for (int i = 0; i < size; i++) {
    int remaining = size - board[i].size();
    for (int j = 0; j < remaining; j++) {
      board[i].push_back(" ");
    }
  }
  initial_board = str();
}


//
// Randomly pick an integer x between [0, 7]
// 2 nmoves = x + 3
// 3 for i = 1 → nmoves
// 4 pick a random index idx
// 5 if i is even
// 6 pick ’u’ or ’d’ at random (50/50) = dir
// 7 shift column with index idx and direction dir
// 8 else
// 9 pick ’r’ or ’l’ at random (50/50) = dir
// 10 shift row with index idx and direction dir
//

void Scrambler::jumble_words(){
  int randomNum = rand() % 8;
  int nMoves = randomNum + 3;
  int index;
  string moves;
  for (int i = 0; i < nMoves; i++) {
    index = (rand() % size) + 1;
    if (i % 2 == 0) {
      moves = "c";
      moves += to_string(index);
      if (rand() % 2 == 0) {
        moves+= "u";
      } else {
        moves += "d";
      }
    } else {
      moves = "r";
      moves += to_string(index);
      if (rand() % 2 == 0) {
        moves+= "r";
      } else {
        moves += "l";
      }
    }
    jumbling.push_back(moves);
  }
  for (int i = 0; i < jumbling.size(); i++) {
    try_move(jumbling[i]);
  }
  last_board = str();
  
}

string Scrambler::str() const {
  string res = "";
  for (int i = 0; i < size; i++) {
    if (i == 0) {
      res += "   ";
      for (int j = 0; j < size; j++) {
        if (j != (size-1)) {
          res += " " + to_string(j+1) + " ";
        } else {
          res += " " + to_string(j+1);
        }
      }
      res += "\n   ";
      for (int j = 0; j < size; j++) {
        res += "----";
      }
      res += "\n";
    }
    res += to_string(i+1) + " |";
    for (int j = 0; j < size; j++) {
      res+= " " + board[i][j] + " |";
    }
    res+="\n   ";
    for (int j = 0; j < size; j++) {
      res += "----";
    }
    if (i != size-1) {
      res += "\n";
    }
  }
  return res;
}

// try_move takes in an input and makes the move for player if it is valid.
//  input format: 3 characters -> 1st char - 'c' or 'r' (column or row)
//                             -> 2nd char - Which number of row or column (ie. 3)
//                             -> 3rd char - 'u' or 'd' (up or down) if selected column
//                                         - 'l' or 'r' (left or right) if selected row
// Example input: c3d (translation: Column 3, move Down)
void Scrambler::try_move(const string& cmd){
  if (cmd.empty()) {
    return;
  }
  string lower = "";
  for (char c : cmd) {
    lower+= tolower(c);
  }
  if (lower.size() == 3) {
    char first = lower[0];
    char sec = lower[1] - '0';
    char third = lower[2];

    if ((first == 'c' || first == 'r') && (sec >= 1 && sec <= size)) {
      if (first == 'c') {
        if (third == 'u') {
          moving = board[0][sec-1];
          for (int i = 0; i < size-1; i++) {
            board[i][sec-1] = board[i+1][sec-1];
          }
          board[size-1][sec-1] = moving;
        } else if (third == 'd') {
          moving = board[size-1][sec-1];
          for (int i = size-1; i > 0; i--) {
            board[i][sec-1] = board[i-1][sec-1];
          }
          board[0][sec-1] = moving;
        } else {
          return;
        }
      } else {
        if (third == 'l') {
          moving = board[sec-1][0];

          for (int i = 0; i < size-1; i++) {
            board[sec-1][i] = board[sec-1][i+1];
          }
          board[sec-1][size-1] = moving;
        } else if (third == 'r'){
          moving = board[sec-1][size-1];
          for (int i = size-1; i < 0; i--) {
            board[sec-1][i] = board[sec-1][i-1];
          }
          board[sec-1][0] = moving;
        } else {
          return;
        }
      }
    } else {
      return;
    }
  } else if (lower.size() == 1) {
    if (lower[0] == 'r') {
      board = board2;
    } else if (lower[0] == 'q') {
      quit = true;
      is_over();
    } else {
      return;
    }
  } else {
    return;
  }
}

// is_over checks if game is over. Return true if game is finished.
bool Scrambler::is_over(){
  if (quit) {
    return true;
  } else {
    return false;
  }
}

// display_solution returns the solution to the board as a string.
// This will un-jumble the scrambled board.
string Scrambler::display_solution(){
  string result, solutions, valid;
  int x;
  result += last_board + "\n";

  for (int i = jumbling.size()-1; i >= 0; i--) {
    x += 1;
    if (jumbling[i][2] == 'u') {
      valid = jumbling[i].substr(0,2) + "d";
      try_move(valid);
    } else if (jumbling[i][2] == 'd') {
      valid = jumbling[i].substr(0,2) + "u";
      try_move(valid);
    } else if (jumbling[i][2] == 'l') {
      valid = jumbling[i].substr(0,2) + "r";
      try_move(valid);
    } else {
      valid = jumbling[i].substr(0,2) + "l";
      try_move(valid);
    }
    solutions = "*** Move " + to_string(x) + "( " + jumbling[i][0] + "," + jumbling[i][1] + "," + valid[2] + ")\n";
    result += solutions;
    result += str();
    if (i != 0) {
      result += "\n";
    }
  }
  return result;
}

// get_words returns a vector of strings which contains all of the words present on the board
vector<string> Scrambler::get_words(){
  return words;
}