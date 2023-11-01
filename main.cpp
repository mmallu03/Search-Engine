// Program: Mini Search Engine
// Author: Meha Mallu
// Description: This project builds a mini search engine that can index and retrieve information from a set of web pages. 

#include "search.h"
using namespace std;

int main() {
  string filename;
  cout << "Enter the filename: ";
  getline(cin, filename);
  map<string, set<string>> index;  // inverted index to store the relationship between the URL and query
  int numPages = buildIndex(filename, index);
  cout << "Indexed " << numPages << " pages." << endl;
  while (true) {
    string query;
    cout << "Enter a search query (press enter to quit): ";
    getline(cin, query);
    if (query.empty()) {
      break;  // If user presses enter to quit it stops prompting 
    }
    set<string> matchFound = findQueryMatches(index, query); // searches for matching pages
    // output based on if search is found or not
    if (matchFound.empty()) {
      cout << "No matches found." << endl;
    }
    else {
      cout << "Found " << matchFound.size() << " matching pages" << endl;
      for (const string& pageURL : matchFound) {
        cout << pageURL << endl;
      }
    }
  }
  cout << "Thank you for searching!" << endl;
  return 0;
}