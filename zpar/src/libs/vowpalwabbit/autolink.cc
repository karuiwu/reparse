#include "simple_label.h"
#include <float.h>
#include "parser.h"
#include "vw.h"

namespace ALINK {
  const int autoconstant = 524267083;
  
  struct autolink {
    uint32_t d;
    uint32_t stride;
    learner base;
  };

  void learn(void* d, example* ec)
  {
    autolink* b = (autolink*)d;

    float label = ((label_data*)ec->ld)->label;
    float weight = ((label_data*)ec->ld)->weight;
    ((label_data*)ec->ld)->label = FLT_MAX;
    ((label_data*)ec->ld)->weight = 0;
    b->base.learn(ec);
    ((label_data*)ec->ld)->label = label;
    ((label_data*)ec->ld)->weight = weight;
    float base_pred = ec->final_prediction;
    
    ec->indices.push_back(autolink_namespace);
    
    float sum_sq = 0;
    for (size_t i = 0; i < b->d; i++)
      if (base_pred != 0.)
	{
	  feature f = { base_pred, (uint32_t) (autoconstant + i * b->stride) };
	  ec->atomics[autolink_namespace].push_back(f);
	  sum_sq += base_pred*base_pred;
	  base_pred *= ec->final_prediction;
	}
    ec->total_sum_feat_sq += sum_sq;
    b->base.learn(ec);
   
    ec->atomics[autolink_namespace].erase();
    ec->indices.pop();
    ec->total_sum_feat_sq -= sum_sq;
  }
  
  void finish(void* d)
  {
    autolink* b = (autolink*)d;
    b->base.finish();
    free(b);
  }

  void drive(vw* all, void* d)
  {
    example* ec = NULL;
    while ( true )
      {
        if ((ec = VW::get_example(all->p)) != NULL)//semiblocking operation.
          {
            learn(d, ec);
	    return_simple_example(*all, ec);
          }
        else if (parser_done(all->p))
	  return;
        else 
          ;
      }
  }

  learner setup(vw& all, std::vector<std::string>&opts, po::variables_map& vm, po::variables_map& vm_file)
  {
    autolink* data = (autolink*)calloc(1,sizeof(autolink));
    data->base = all.l;
    data->d = (uint32_t)vm["autolink"].as<size_t>();
    data->stride = all.reg.stride;
    
    if (!vm_file.count("autolink")) 
      {
	std::stringstream ss;
	ss << " --autolink " << data->d << " ";
	all.options_from_file.append(ss.str());
      }

    learner l(data, drive, learn, finish, all.l.sl);
    return l;
  }
}
