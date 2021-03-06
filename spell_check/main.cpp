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
#include "file_things.hpp"
const int MIN_SIZE = 4;

int main(int argc, const char * argv[]) {
    
    std::string filename_txt =
    "text.txt";
    std::string filename_dic =
"dictionary.txt";
    std::string filename_abc =
    "abc.txt";

    std::fstream txt(filename_txt, std::ios::in);
    std::fstream dictionary(filename_dic, std::ios::in);
    std::ifstream alphabet(filename_abc, std::ios::in);
    
    ErrorHandler err;
    char charTemp{};
    
    try{
        
        //IF FILES ARE NOT OPEN THROWS AN ERROR
        if (txt.is_open() == 0 || dictionary.is_open() == 0 || alphabet.is_open() == 0){
            err.set(FILEOPEN_ERR);
            throw err;
        }
        
        txt.get();
        alphabet.get();
        if ( txt.eof() || alphabet.eof() ){
            err.set(EMPTY_FILE_ERR);
            throw err;
        }
        txt.clear();
        txt.seekg(0L, std::ios::beg);
        alphabet.clear();
        alphabet.seekg(0L, std::ios::beg);
        
        //ALPHABET AS A VECTOR OF CHARS
        std::vector<char> abc;
        while (!alphabet.eof()) {
            charTemp = alphabet.get();
            if (charTemp != ' ' && charTemp != '\377') {
                abc.push_back(charTemp);
            }
        }
        
        std::string strTemp{};
        //DICTIONARY AS A SET
        std::set<std::string> dic;
        std::set<std::string>::iterator it{};
        std::vector<std::set<std::string>::iterator> iterators;
        while(!dictionary.eof()){
            getline(dictionary, strTemp, '\n');
            dic.insert(strTemp);
            //std::cout << strTemp << std::endl;
        }
        
        std::vector<std::string> textForEditing;
        std::vector<bool> textOrNot{};
        textForEditing = convertToVect(txt, textOrNot);
        
        //VECTOR OF DIFFERENT VERSIONS OF THE UNKNOWN WORD
        std::string word{};
        std::string tempWord{};
        std::vector<std::string> versions;
        char change{};
        char add{};
        //GOING THROUGH EVERY WORD
        for(int l = 0; l < textForEditing.size(); ++l){
            word = textForEditing[l];
            if (textOrNot[l]==1 &&
               (dic.find(word) == dic.end())&&
                static_cast<int>(word.size()) >= MIN_SIZE){
                //CREATING ALL WORDS WITH LEVENSTEIN DISTANCE LESS THAN ONE
                //FROM THE GIVEN ONE
                versions = versionsOfWord(word, abc);
                iterators.clear();
                for(int i = 0; i < versions.size(); ++i){
                    it = dic.find(versions[i]);
                    if(*it != ""){
                    iterators.push_back(it);
                    }
                    //it = dic.find(versions[i])
                    //'it' is an iterator to the word which matched to the dictionary
                    //if (it != dic.end()) break;
                }
                std::cout << "Number of proposed changes: " <<  iterators.size() << std::endl;
                for(int i = 0; i < iterators.size(); ++i){
                    it = iterators[i];
//                    if (*it == "") {
//                        std::cout << "No word \'" << word << "\' in dictionary. Add? (y/n): ";
//                        std::cin >> add;
//                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
//                        if(add == 'y'){
//                        dictionary.close();
//                        dictionary.open(filename_dic, std::ios::app);
//                        dictionary << word << '\n';
//                        dic.insert(word);
//                        dictionary.close();
//                        dictionary.open(filename_dic, std::ios::in);
//                        }
//                    }
                    if (*it != word) {
                        std::cout << word << " -> " << *it  << " ? (y/n) ";
                        std::cin >> change;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                        if (change == 'y'){
                            textForEditing[l] = *it;
                            break;
                        }
                        else if (change == 'n' && i == iterators.size() - 1) {
                            std::cout << "No word \'" << word << "\' in dictionary. Add? (y/n): ";
                            std::cin >> add;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                            if(add == 'y'){
                                dictionary.close();
                                dictionary.open(filename_dic, std::ios::app);
                                dictionary << word << '\n';
                                dic.insert(word);
                                dictionary.close();
                                dictionary.open(filename_dic, std::ios::in);
                            }
                        }
                    }
                }
                versions.clear();
            }
        } //for
        txt.close();
        txt.open(filename_txt, std::ios::out);
        for(int i = 0; i < textForEditing.size(); ++i){
            txt << textForEditing[i];
        }
        txt.close();
        dictionary.close();
        alphabet.close();
    } //try
    
    catch (ErrorHandler err){
        switch (err.errCode) {
            case FILEOPEN_ERR:
                std::cout <<
                "ERROR: One of the files cannot be opened" << std::endl;
                break;
            case EMPTY_FILE_ERR:
                std::cout <<
                "ERROR: One of the files is empty" << std::endl;
                break;
            default:
                break;
        }
        return -1;
    }
    
    return 0;
}
