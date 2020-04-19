/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include <unordered_map>
#include "unordered_set"

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    Queue<string> qu;
    qu.enqueue(start);
    unordered_map<string, string> dict;
    unordered_set<string> mark;
    mark.insert(start);
    dict[start] = "";
    bool exit = false;
    int cnt = 0;
    while(!qu.isEmpty()) {
        cnt++;
        int size = qu.size();
        for(int i = 0; i < size; i++) {
            string word = qu.dequeue();
            if(word == end) {
                exit = true;
                break;
            }
            string newWord = word;
            for(size_t j = 0; j < word.size(); j++) {
                for(int k = 0; k < 26; k++) {
                    char c = newWord[j];
                    newWord[j] = 'a' + k;
                    if(english.contains(newWord) && mark.find(newWord) == mark.end()) {
                        qu.enqueue(newWord);
                        dict[newWord] = word;
                        mark.insert(newWord);
                    }
                    newWord[j] = c;
                }
            }
        }
        if(exit) {
            break;
        }
    }
    Vector<string> words(cnt);
    for(string word = end; word != ""; word = dict[word]) {
        words[--cnt] = word;
    }
    for(const auto& word : words) {
        cout << word << " ";
    }
    cout << endl;
}

static const string kEnglishLanguageDatafile = "dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
