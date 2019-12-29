//
//  file_things.cpp
//  spell_check
//
//  Created by David Kapatsa on 29.12.2019.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#include "file_things.hpp"

std::vector<std::string> convertToVect(std::fstream &file, std::vector<bool> &textOrNot){
    char charTemp{};
    std::string word{};
    std::string syms{};
    std::string tempWord{};
    std::vector<std::string> textForEditing;

    while (!file.eof()){
        charTemp = file.get();
        if ((charTemp != ' ')&&
            (charTemp != ',')&&
            (charTemp != '.')&&
            (charTemp != '!')&&
            (charTemp != '\377')){
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
    return textForEditing;
}

std::vector<std::string> versionsOfWord(std::string word, std::vector<char> &abc){
    std::vector<std::string> versions;
    int numLetters = static_cast<int>(word.size());
    std::string tempWord{};
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
    return versions;
}
