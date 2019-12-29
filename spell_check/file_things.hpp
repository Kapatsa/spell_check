//
//  file_things.hpp
//  spell_check
//
//  Created by David Kapatsa on 29.12.2019.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#ifndef file_things_hpp
#define file_things_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

//TO USE THESE FUNCTIONS WE ASSUME THE TEXT FILE TO BE OPEN IN READING MODE

std::vector<std::string> convertToVect(std::fstream &file, std::vector<bool> &textOrNot);

//all words with Levenstein's distance 1 from the given word
std::vector<std::string> versionsOfWord(std::string word, std::vector<char> &abc);



#endif /* file_things_hpp */
