//
//  errorHandler.hpp
//  spell_check
//
//  Created by David Kapatsa on 28.12.2019.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#ifndef errorHandler_hpp
#define errorHandler_hpp

#include <stdio.h>

const int FILEOPEN_ERR = 1;
const int EMPTY_FILE_ERR = 2;

struct ErrorHandler{
    int posRow;
    int posCol;
    int errCode;
    ErrorHandler(){};
    void set(int err, int row = 0, int col = 0){
        errCode = err;
        posRow = row;
        posCol = col;
    };
};

#endif /* errorHandler_hpp */
