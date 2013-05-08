// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the general dependency parser.   *
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

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const bool &bRules, const std::string &sSuperPath, const bool bCoNLL) {

   std::cout << "Training iteration is started..." << std::endl ; std::cout.flush();

   CDepParser parser(sFeatureFile, true, bCoNLL);
   parser.setRules(bRules);

   std::ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CDependencyParse ref_sent; 
   CCoNLLOutput ref_conll; 

   depparser::CSuperTag *supertags;
   std::ifstream *is_supertags=0;

   supertags = 0;
   if (!sSuperPath.empty()) {
      supertags = new depparser::CSuperTag();
      is_supertags = new std::ifstream(sSuperPath.c_str());
      parser.setSuperTags(supertags);
   }

   int nCount=0;
 
   // read input  
//   if (bCoNLL) is >> ref_conll; else is >> ref_sent;

   while( bCoNLL ? is>>ref_conll : is>>ref_sent ) {
//   while( (bCoNLL && !ref_conll.empty()) || (!bCoNLL && ! ref_sent.empty()) ) {
      TRACE("Sentence " << nCount);
      ++ nCount ; 

      if (supertags) {
         supertags->setSentenceSize( bCoNLL ? ref_conll.size() : ref_sent.size() );
         (*is_supertags) >> *supertags;
      }

      // example
      if (bCoNLL)
         parser.train_conll( ref_conll, nCount );
      else
         parser.train( ref_sent, nCount );

      // read input  
//      if (bCoNLL) is >> ref_conll; else is >> ref_sent;
 
   }

   parser.finishtraining();

   if (supertags) {
      delete supertags;
      is_supertags->close();
      delete is_supertags;
   }

   std::cout << "Done. " << std::endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("c", "", "process CoNLL format", "");
      configurations.defineConfiguration("p", "path", "supertags", "");
      configurations.defineConfiguration("r", "", "use rules", "");
      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      } 
      configurations.loadConfigurations(options.opts);
   
      int training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cerr << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }
   
      bool bCoNLL = configurations.getConfiguration("c").empty() ? false : true;
      std::string sSuperPath = configurations.getConfiguration("p");
      bool bRules = configurations.getConfiguration("r").empty() ? false : true;

      std::cout << "Training started" << std::endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i) 
         auto_train(options.args[1], options.args[2], bRules, sSuperPath, bCoNLL);
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
   
      return 0;
   } catch (const std::string &e) {
      std::cerr << std::endl << "Error: " << e << std::endl;
      return 1;
   }

}

