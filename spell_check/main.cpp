//
//  main.cpp
//  spell_check
//
//  Created by David Kapatsa on 28.12.2019.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include "errorHandler.hpp"
const int MIN_SIZE = 5;

int main(int argc, const char * argv[]) {
    
    std::string filename_txt =
    "/Users/davidkapatsa/Documents/Programming/4th_year/spell_check/spell_check/text.txt";
    std::string filename_dic =
    "/Users/davidkapatsa/Documents/Programming/4th_year/spell_check/spell_check/dictionary.txt";
    std::string filename_abc =
    "/Users/davidkapatsa/Documents/Programming/4th_year/spell_check/spell_check/abc.txt";

    std::fstream txt(filename_txt, std::ios::in);
    std::fstream dictionary(filename_dic, std::ios::in);
    std::ifstream alphabet(filename_abc, std::ios::in);
    
    ErrorHandler err;
    char charTemp{};
    
    try{
        
        if (txt.is_open() == 0 || dictionary.is_open() == 0 || alphabet.is_open() == 0){
            err.set(FILEOPEN_ERR);
            throw err;
        }
        
        //alphabet as a vector of chars
        std::vector<char> abc;
        while (!alphabet.eof()) {
            charTemp = alphabet.get();
            if (charTemp != ' ' && charTemp != '\377') {
                abc.push_back(charTemp);
            }
        }
        
        std::string strTemp{};
        //dictionary as a set
        std::set<std::string> dic;
        while(!dictionary.eof()){
            getline(dictionary, strTemp, '\n');
            dic.insert(strTemp);
            //std::cout << strTemp << std::endl;
        }
        
        //vector of different versions of the word
        std::vector<std::string> versions;
        
        //GETTING A WORD
        std::string word{};
        std::string syms{};
        int numLetters{};
        std::string tempWord{};
        std::set<std::string>::iterator it;

        std::vector<std::string> textForEditing;
        std::vector<bool> textOrNot{};
        
        char change{};
        char add{};
        
        while (!txt.eof()){
            charTemp = txt.get();
            if ((charTemp != ' ')&&(charTemp != ',')&&(charTemp != '.')&&(charTemp != '!')&&(charTemp != '\377')){
                //letter
                if(syms != ""){
                    //std::cout << "symbols: " << syms << std::endl;
                    textOrNot.push_back(0);
                    textForEditing.push_back(syms);
                    syms = "";
                }
                word += charTemp;
            }
            else{
                //symbols
                if(word != ""){
                    //std::cout << "word: " << word << std::endl;
                    textOrNot.push_back(1);
                    textForEditing.push_back(word);
                    word = "";
                }
                syms += charTemp;
            }
        };
        
        //GOING THROUGH EVERY WORD
        for(int l = 0; l < textForEditing.size(); ++l){
            word = textForEditing[l];
            if (textOrNot[l]==1 &&
               (dic.find(word) == dic.end())&&
                static_cast<int>(word.size()) >= MIN_SIZE){
                //CREATING ALL WORDS WITH LEVENSTEIN DISTANCE LESS THAN ONE
                //FROM THE GIVEN ONE
                numLetters = static_cast<int>(word.size());
                //PART I. deleting
                for (int j = 0; j < numLetters; ++j){
                    for (int i = 0; i < numLetters; ++i){
                        if (i != j){
                            tempWord += word[i];
                        }
                    }
                    versions.push_back(tempWord);
                    //std::cout << tempWord << std::endl;
                    tempWord = "";
                }
                //PART II. addition
                for (int i = 0; i < numLetters + 1; ++i){ //where
                    for (int j = 0; j < abc.size(); ++j){ //what
                        for (int k = 0; k < numLetters + 1; ++k){
                            if (k < i) tempWord += word[k];
                            else if (k == i) tempWord += abc[j];
                            else if (k > i) tempWord += word[k-1];
                            //std::cout << tempWord[k] << std::endl;
                        }
                        versions.push_back(tempWord);
                        //std::cout << tempWord << std::endl;
                        tempWord = "";
                    }
                }
                //PART III. changing
                for (int i = 0; i < numLetters; ++i){ //where
                    for (int j = 0; j < abc.size(); ++j){ //what
                        for (int k = 0; k < numLetters; ++k){
                            if (k < i) tempWord += word[k];
                            else if (k == i) tempWord += abc[j];
                            else if (k > i) tempWord += word[k];
                        }
                        versions.push_back(tempWord);
                        //std::cout << tempWord << std::endl;
                        tempWord = "";
                    }
                }
                //std::cout << "Size of versions vector: " << versions.size() << std::endl;
                for(int i = 0; i < versions.size(); ++i){
                    it = dic.find(versions[i]);
                    if (it != dic.end()) break;
                }
                if (*it == "") {
                    std::cout << "No word \'" << word << "\' in dictionary. Add? (y/n): ";
                    std::cin >> add;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    if(add == 'y'){
                    dictionary.close();
                    dictionary.open(filename_dic, std::ios::app);
                    dictionary << word << '\n';//problem: *it does not show in txt file
                    dictionary.close();
                    dictionary.open(filename_dic, std::ios::in);
                    }
                }
                else if (*it != word) {
                    std::cout << word << " -> " << *it  << " ? (y/n) ";
                    std::cin >> change;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    if (change == 'y') textForEditing[l] = *it;
                }
                versions.clear();
            }
        } //for
        txt.close();
        txt.open(filename_txt, std::ios::out);
        for(int i = 0; i < textForEditing.size(); ++i){
            txt << textForEditing[i];
        }
        
    } //try
    
    catch (ErrorHandler err){
        switch (err.errCode) {
            case FILEOPEN_ERR:
                std::cout <<
                "ERROR: The text file cannot be opened" << std::endl;
                break;
            default:
                break;
        }
    }
    
    txt.close();
    dictionary.close();
    alphabet.close();
    
    return 0;
}
