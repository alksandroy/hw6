#ifndef RECCHECK
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>
#endif

#include "boggle.h"

std::vector<std::vector<char> > genBoard(unsigned int n, int seed) {
    // random number generator
    std::mt19937 r(seed);

    // scrabble letter frequencies
    // A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2,
    // N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
    int freq[26] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2,
                    6, 8, 2, 1, 6,  4, 6, 4, 2, 2, 1, 2, 1};
    std::vector<char> letters;
    for (char c = 'A'; c <= 'Z'; c++) {
        for (int i = 0; i < freq[c - 'A']; i++) {
            letters.push_back(c);
        }
    }
    std::vector<std::vector<char> > board(n);
    for (unsigned int i = 0; i < n; i++) {
        board[i].resize(n);
        for (unsigned int j = 0; j < n; j++) {
            board[i][j] = letters[(r() % letters.size())];
        }
    }
    return board;
}

void printBoard(const std::vector<std::vector<char> >& board) {
    unsigned int n = board.size();
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            std::cout << std::setw(2) << board[i][j];
        }
        std::cout << std::endl;
    }
}

std::pair<std::set<std::string>, std::set<std::string> > parseDict(
    std::string fname) {
    std::ifstream dictfs(fname.c_str());
    if (dictfs.fail()) {
        throw std::invalid_argument("unable to open dictionary file");
    }
    std::set<std::string> dict;
    std::set<std::string> prefix;
    std::string word;
    while (dictfs >> word) {
        dict.insert(word);
        for (unsigned int i = word.size() - 1; i >= 1; i--) {
            prefix.insert(word.substr(0, i));
        }
    }
    prefix.insert("");
    return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string>& dict,
                             const std::set<std::string>& prefix,
                             const std::vector<std::vector<char> >& board) {
    std::set<std::string> result;
    for (unsigned int i = 0; i < board.size(); i++) {
        for (unsigned int j = 0; j < board.size(); j++) {
            boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);
            boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);
            boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);
        }
    }

    return result;
}

bool boggleHelper(const std::set<std::string>& dict,
                  const std::set<std::string>& prefix,
                  const std::vector<std::vector<char> >& board,
                  std::string word,
                  std::set<std::string>& result,
                  unsigned int r,
                  unsigned int c,
                  int dr,
                  int dc) {
    if (r >= board.size() || c >= board.size()) {
        return false;
    }

    word.push_back(board[r][c]);
    if (prefix.find(word) == prefix.end() && dict.find(word) == dict.end()){
        return false;
    }

    // std::cerr << word << std::endl;

    bool isWord = (word.size() >= 2 && dict.find(word) != dict.end());

    bool flag =
        boggleHelper(dict, prefix, board, word, result, r + dr, c + dc, dr, dc);

    if (flag)
        return true;

    if (isWord) {
        result.insert(word);
        // std::cerr << word << std::endl;
        return true;
    }
    return false;

    // size_t curLen = result.size() ? result.begin()->size() : 0;
    // bool isWord = false, isPrefix = false, isRet = false;
    // if (word.size()) {
    //     isWord = (dict.find(word) != dict.end());
    //     isPrefix = (prefix.find(word) != prefix.end());
    // }
    // // INSERT TO RESULT
    // if (isWord) {
    //     if (curLen < word.size()) {
    //         curLen = word.size();
    //         isRet = true;
    //     }
    //     result.insert(word);
    // }
    // // if (isWord && curLen < word.size()) {
    // //     result.clear();
    // //     curLen = word.size();
    // // }
    // // if (isWord && word.size() == curLen) {
    // //     result.insert(word);
    // //     isRet = true;
    // // }

    // // RECURSIVE CALL
    // if (isPrefix || word == "") {
    //     if (r >= board.size() || c >= board.size()) {
    //         return isRet;
    //     }
    //     word += board[r][c];
    //     return isRet || boggleHelper(dict, prefix, board, word, result, r +
    //     dr, c + dc,
    //                         dr, dc);
    // }
    // return false;
}
