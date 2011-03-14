//-*-C++-*-

/** \ingroup main.JsonParser */
/*@{*/

/*! \file jsonParser.cpp 
 *
 *  
 *
 */

#include "JsonParser.h"
#include "DefaultContext.h"
#include <wchar.h>
#include <iostream>
#include <fstream>

int main(int argc,char *argv[]) {
  
  if (argc < 2) {
    std::cout << "Usage: "<<argv[0]<<" inputFileName\n";
    return -1;
  }

  std::string    fileName(argv[1]);
  std::wifstream file;
  
  file.open(fileName.c_str());
  
  JsonParser::JsonParser<JsonParser::DefaultContext> parser;
  parser.filename = fileName;		
  while(parser.parseChar(file));
  
  std::wcout << parser.ctx;
  std::wcout <<"\n";  
  return 0;
}


/*@}*/

