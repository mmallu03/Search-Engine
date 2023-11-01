// Program: Mini Search Engine
// Author: Meha Mallu
// Description: This file contains the declarations for the functions implemented to build a mini search engine.

#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
using namespace std;

// This function returns a cleaned version of the token by removing any leading & trailing punctuation and converting it to lowercase.
string cleanToken(string s) {
  string trim;
  int begin = 0;
  int end = s.length() -1;
  // remove leading punctuation and trailing punctuation, respectively
  while (begin <= end && ispunct(s[begin])) {
    begin++;
  }
  while (end >= begin && ispunct(s[end])) {
    end--;
  }
  if (begin > end) {
    return "";   // token is empty
  }
  // token is converted to lowercase and cleaned version is returned
  for (int i=begin; i<=end; i++) {
    if (isalpha(s[i])) {
      trim += tolower(s[i]);
    }
    else {
      trim += ' ';
    }
  }
  return trim;
}

// This function extracts unique tokens, cleans them, stores the cleaned unique tokens and then returns them.
set<string> gatherTokens(string text) {
  set<string> tokens;
  istringstream s(text);
  string token;
  // extract tokens from text
  while (s >> token) {
    string cleanedToken = cleanToken(token);
    if (!cleanedToken.empty()) {  // checks if cleaned token is valid word 
      tokens.insert(cleanedToken);
    }
  }
  return tokens;
}

// This function creates an inverted index after processing a database file and extracting certain information from each text file.
int buildIndex(string filename, map<string, set<string>>& index) {
  int numPages = 0;
  ifstream input(filename);
  string pageURL;
  string pageText;
  // iterates over each line in file and 1 line is read into pageURL and the other is read into pageText
  while (getline(input, pageURL) && getline(input, pageText)) {
    set<string> tokens = gatherTokens(pageText);  // a set of cleaned tokens is returned because gatherTokens is called
    for (const string& token : tokens) {
      index[token].insert(pageURL);
    }
    numPages++;
  }
  return numPages;
}

// This function searches the index for matches for the searched query by parsing each line.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
  // this stream will allow for tokens to be read in from the file
  istringstream ss(sentence);
  string term;
  set<string> searchTerms;
  // the query (sentence) will be extracted from the file
  while (ss >> term) {
    searchTerms.insert(term); 
  }
  set<string> result;
  // stores positive and negative modifiers
  set<string> posMod;
  set<string> negMod;
  set<string> termResults;
  // search through searchTerms to find positive and negative modifiers with "+" or "-"
  for (const string& term : searchTerms) {
    string cleanedToken = cleanToken(term);
    if (cleanedToken[0] == '+') {
      posMod.insert(cleanedToken.substr(1));  // the term excluding the "+" is inserted into posMod
    }
    else if (cleanedToken[0] == '-') {
      negMod.insert(cleanedToken.substr(1));
    }
    // if no modifiers are present
    else {
      set<string> match = index[cleanedToken];
      // calculates the intersection between terms with positive modifiers and current terms to check if there is a match
      if (!posMod.empty()) {
        set<string> intersection;
        set_intersection(match.begin(), match.end(), posMod.begin(), posMod.end(), inserter(intersection, intersection.begin()));
        match = intersection;
      }
      // calculates the intersection between terms with negative modifiers and current terms to check if there is a match
      if (!negMod.empty()) {
        set<string> difference;
        set_difference(match.begin(), match.end(), negMod.begin(), negMod.end(), inserter(difference, difference.begin()));
        match = difference;
      }
      termResults.insert(match.begin(), match.end());  // matching pages for all the terms
    }
  }
  if (!termResults.empty()) {
    result = termResults;
  }
  return result;  // match of search result
}

// This function builds the inverted index to search for queries and provides a user-friendly interface.
void searchEngine(string filename) {
  map<string, set<string>> invertedIndex;
  int pageCt = buildIndex(filename, invertedIndex); // creates inverted index based on the text file
  // error in opening file
  if (pageCt == 0) {
    return;
  }
  // the findQueryMatches function is called if a query is entered to find matching pages and prints the URL if a match is found
  while (true) {
    string query;
    getline(cin, query);
    set<string> matchFound = findQueryMatches(invertedIndex, query);
    for (const string&pageURL : matchFound) {
      cout << pageURL << endl;
    }
  }
}