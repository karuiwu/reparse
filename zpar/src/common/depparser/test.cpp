// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * test.cpp - the testing of English dependency parser.         *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"

using namespace english;

/*===============================================================
 *
 * test_weight - just load and save weights to see if the same
 *
 *===============================================================*/

void test_weight(const std::string &sFeatureFile) {

   std::cout << "Testing weights..." << std::endl ; std::cout.flush();

   CDepParser parser(sFeatureFile, true);

   parser.finishtraining();

   std::cout << "Done. " << std::endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   const std::string hint = " feature_file";
   if (argc < 2) {
      std::cout << "\nUsage: " << argv[0] << hint << std::endl;
      return 1;
   }

   test_weight(argv[1]);

   return 0;

}

