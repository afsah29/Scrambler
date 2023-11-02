/* ***********************************
PROGRAM 5: Scrambler
CS 141, Spring 2023.
By; Afsah Ahmed
*********************************** */


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
  public:
    string path;
    int size;
    vector<string> allWords;  
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
    // Constructor: string path is the path to the dictionary file, int size for the [size x size] board
    Scrambler(const string& path, const int size);
    string str() const;
    void scrambler_init();
    void load_all_words_from_dict();
    void jumble_words();
    void try_move(const string& cmd);
    bool is_over() const;
    string display_solution();
    vector<string> get_words();
};

//initialize 
Scrambler::Scrambler(const string& file, const int len){
    size = len;
    path = file;
    allWordsSize = 0;

    scrambler_init();
    jumble_words();
    board2 = board;
  }


// Function str returns the board as a string. Loads all words from the dictionary into a string vector

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
}

// Sets the value of the words array in the Class. This will be the set of words, that are random, this will be used to initialize the board.

void Scrambler::scrambler_init(){
  load_all_words_from_dict();
  for (int i = 0; i < size; i++) {
    board.push_back(vector<string>());
    while (size - board[i].size() > 2) {
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

//Scrambles the words in the grid 

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
          res += " " + to_string(j+1) + "  ";
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
          for (int i = size-1; i > 0; i--) {
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
      return;
    } else {
      return;
    }
  } else {
    return;
  }
}

// is_over checks if game is over. Return true if game is finished.
bool Scrambler::is_over() const{
  if (quit) {
    return true;
  }
  
  vector<string> currentBoard;
  vector<string> tempWords = words;
  for(int i = 0; i<size; i++){
    string str = "";
    for(int j = 0; j <size; j++){
      if(board[i][j] != " "){
        str+=board[i][j];
      }
      else{
        if(str != ""){
          currentBoard.push_back(str);
          str = "";
        }
      }
    }
    if(str != ""){
      currentBoard.push_back(str);
    }
  }

  sort(tempWords.begin(), tempWords.end());
  sort(currentBoard.begin(), currentBoard.end());
  for(int i = 0; i<tempWords.size(); i++){
    if(tempWords[i] != currentBoard[i]){
      return false;
    }
  }
  return true;
}

// display_solution returns the solution to the board as a string. This will un-jumble the scrambled board.

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
    solutions = "*** Move " + to_string(x) + " (" + jumbling[i][0] + "," + jumbling[i][1] + "," + valid[2] + ")\n";
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

int main(){
  Scrambler scr("dictionary.txt", 5);
  cout << scr.str() << endl;
  string cmd;
  while(!scr.is_over()) {
    cout << "enter cmd" << endl;
    cin >> cmd;
    if (cmd == "d") {
      cout << scr.display_solution();
    } else {
      scr.try_move(cmd);
      
    }
  }
  return 0;
}