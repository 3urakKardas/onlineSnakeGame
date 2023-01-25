#include "command.h"

Command::Command(){

}

std::string Command::getHeader(){


    return header;

}

std::string Command::getData(){

    return data;

}

char Command::getPrimaryChar(){

    return primaryChar;

}

char Command::getSecondaryChar(){

    return secondaryChar;

}

void Command::setHeader(std::string hdr){


    header = hdr;

}

void Command::setData(std::string dta){

    data = dta;

}

void Command::setPrimaryChar(char primary){

    primaryChar = primary;

}

void Command::setSecondaryChar(char secondary){

    secondaryChar = secondary;

}
