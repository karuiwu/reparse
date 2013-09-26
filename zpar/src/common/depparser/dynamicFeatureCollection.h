/*
 * dynamicFeatureCollection.h
 *
 *  Created on: May 25, 2013
 *      Author: augustofheart
 */

#ifndef DYNAMICFEATURECOLLECTION_H_
#define DYNAMICFEATURECOLLECTION_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "linguistics/conll.h"
#include "vowpalwabbit/vw.h"
#include "vowpalwabbit/parser.h"

enum { ARCFEATURES = 0 };

typedef std::map<int, std::vector<int> > siblings_t;
typedef std::map<int, int> parent_t;

class DynamicFeatureCollection {
public:
	DynamicFeatureCollection();
	virtual ~DynamicFeatureCollection();
	void makeFeatures(std::vector<int> stack, std::vector<int> buffer,
			siblings_t children, std::vector<CCoNLLCPOS> tags);//, parent_t parents);
	void posFeatures();
	void arcFeatures();
	void readInFeature(std::vector<std::string> feature);
	void readToMap(std::string fileName = "map.txt");
	void writeToMap(CCoNLLOutput conllSentenceTrain);
	void writeToMap(std::string fileName = "map.txt");
	void writeClass(std::string result);
	void printFeatures();
	void clear();

	void trainModel();

	std::vector<std::string> classification;
	std::vector<std::vector<char> > namespaces;
	std::vector<std::vector<std::vector<std::string> > > features;
	int ngram;

	std::map<std::string, std::vector<std::string> > leftTags;
	std::map<std::string, std::vector<std::string> > rightTags;
	std::map<std::string, std::vector<std::pair<std::string, int> > >tagCount;

	std::vector<int> m_stack;
	std::vector<int> m_buffer;
	siblings_t m_children;
	parent_t m_parents;
	std::vector<CCoNLLCPOS> m_tags;

	std::ofstream featuresFile;
	int currentSentenceNum;
};

#include <stdio.h>

using namespace std;


typedef uint32_t fid;



struct vw_namespace {
  char namespace_letter;
public: vw_namespace(const char c) : namespace_letter(c) {}
};



class ezexample {
 private:
  vw*vw_ref;
  bool is_multiline;

  char str[2];
  example*ec;
  vector<fid> past_seeds;
  fid current_seed;
  size_t quadratic_features_num;
  float quadratic_features_sqr;
  char current_ns;
  bool ns_exists[256];
  bool example_changed_since_prediction;

  v_array<example*> example_copies;

  ezexample(const ezexample & ex);
  ezexample & operator=(const ezexample & ex);

  example* get_new_example() {
    example* new_ec = VW::new_unused_example(*vw_ref);
    vw_ref->p->lp->default_label(new_ec->ld);
    return new_ec;
  }


 public:


  // REAL FUNCTIONALITY
  ezexample(vw*this_vw, bool multiline=false) {
    vw_ref = this_vw;
    is_multiline = multiline;

    str[0] = 0; str[1] = 0;
    current_seed = 0;
    current_ns = 0;

    ec = get_new_example();

    quadratic_features_num = 0;
    quadratic_features_sqr = 0.;

    for (size_t i=0; i<256; i++) ns_exists[i] = false;

    if (vw_ref->add_constant)
      VW::add_constant_feature(*this_vw, ec);

    example_changed_since_prediction = true;
  }

  ~ezexample() {
    if (ec->in_use)
      VW::finish_example(*vw_ref, ec);
    for (example**ecc=example_copies.begin; ecc!=example_copies.end; ecc++)
      if ((*ecc)->in_use)
        VW::finish_example(*vw_ref, *ecc);
    example_copies.erase();
    free(example_copies.begin);
  }

  bool ensure_ns_exists(char c) {  // returns TRUE iff we should ignore it :)
    if (vw_ref->ignore_some && vw_ref->ignore[c]) return true;
    if (ns_exists[c]) return false;
    ec->indices.push_back((size_t)c);
    ns_exists[c] = true;
    return false;
  }

  void addns(char c) {
    if (ensure_ns_exists(c)) return;

    ec->atomics[c].erase();
    ec->sum_feat_sq[c] = 0;
    past_seeds.push_back(current_seed);
    current_ns = c;
    str[0] = c;
    current_seed = VW::hash_space(*vw_ref, str);
  }

  void remns() {
    if (ec->indices.size() == 0) {
      current_seed = 0;
      current_ns = 0;
    } else {
      if (ns_exists[current_ns]) {
        ec->total_sum_feat_sq -= ec->sum_feat_sq[current_ns];
        ec->sum_feat_sq[current_ns] = 0;
        ec->num_features -= ec->atomics[current_ns].size();
        ec->atomics[current_ns].erase();

        ns_exists[current_ns] = false;
      }

      current_seed = past_seeds.back();
      past_seeds.pop_back();
      ec->indices.pop();
      example_changed_since_prediction = true;
    }
  }


  inline fid addf(char to_ns, fid fint, float v) {
    if (to_ns == 0) return 0;
    if (ensure_ns_exists(to_ns)) return 0;

    feature f = { v, fint * vw_ref->reg.stride };
    ec->atomics[to_ns].push_back(f);
    ec->sum_feat_sq[to_ns] += v * v;
    ec->total_sum_feat_sq += v * v;
    ec->num_features++;
    example_changed_since_prediction = true;
  }

  inline fid addf(fid fint, float v) { return addf(current_ns, fint, v); }

  inline ezexample& set_label(string label) {
    VW::parse_example_label(*vw_ref, *ec, label);
    ec->global_weight = vw_ref->p->lp->get_weight(ec->ld);
    example_changed_since_prediction = true;
    return *this;
  }

  void mini_setup_example() {
    ec->partial_prediction = 0.;
    vw_ref->sd->t += ec->global_weight;
    ec->example_t = vw_ref->sd->t;

    ec->num_features      -= quadratic_features_num;
    ec->total_sum_feat_sq -= quadratic_features_sqr;

    quadratic_features_num = 0;
    quadratic_features_sqr = 0.;

    for (vector<string>::iterator i = vw_ref->pairs.begin(); i != vw_ref->pairs.end(); i++) {
      quadratic_features_num
        += (ec->atomics[(int)(*i)[0]].end - ec->atomics[(int)(*i)[0]].begin)
        *  (ec->atomics[(int)(*i)[1]].end - ec->atomics[(int)(*i)[1]].begin);
      quadratic_features_sqr
        += ec->sum_feat_sq[(int)(*i)[0]]
        *  ec->sum_feat_sq[(int)(*i)[1]];
    }
    ec->num_features      += quadratic_features_num;
    ec->total_sum_feat_sq += quadratic_features_sqr;
  }

  float predict() {
    static example* empty_example = is_multiline ? VW::read_example(*vw_ref, (char*)"") : NULL;
    if (example_changed_since_prediction) {
      mini_setup_example();
      vw_ref->learn(ec);
      if (is_multiline) vw_ref->learn(empty_example);
      example_changed_since_prediction = false;
    }
    return ec->final_prediction;
  }

  void train() {  // if multiline, add to stack; otherwise, actually train
    if (example_changed_since_prediction) {
      mini_setup_example();
      example_changed_since_prediction = false;
    }

    if (!is_multiline) {
      vw_ref->learn(ec);
    } else {   // is multiline
      // we need to make a copy
      example* copy = get_new_example();
      assert(ec->in_use);
      VW::copy_example_data(copy, ec, vw_ref->p->lp->label_size, vw_ref->p->lp->copy_label);
      assert(copy->in_use);
      vw_ref->learn(copy);
      example_copies.push_back(copy);
    }
  }

  void clear_features() {
    for (size_t i=0; i<256; i++) {
      if (current_ns == 0) break;
      remns();
    }
  }

  void finish() {
    static example* empty_example = is_multiline ? VW::read_example(*vw_ref, (char*)"") : NULL;
    if (is_multiline) {
      vw_ref->learn(empty_example);
      for (example**ecc=example_copies.begin; ecc!=example_copies.end; ecc++)
        VW::finish_example(*vw_ref, *ecc);
      example_copies.erase();
    }
  }


  // HELPER FUNCTIONALITY

  inline fid hash(string fstr)         { return VW::hash_feature(*vw_ref, fstr, current_seed); }
  inline fid hash(char*  fstr)         { return VW::hash_feature_cstr(*vw_ref, fstr, current_seed); }
  inline fid hash(char c, string fstr) { str[0] = c; return VW::hash_feature(*vw_ref, fstr, VW::hash_space(*vw_ref, str)); }
  inline fid hash(char c, char*  fstr) { str[0] = c; return VW::hash_feature_cstr(*vw_ref, fstr, VW::hash_space(*vw_ref, str)); }

  inline fid addf(fid    fint           ) { return addf(fint      , 1.0); }
  inline fid addf(string fstr, float val) { return addf(hash(fstr), val); }
  inline fid addf(string fstr           ) { return addf(hash(fstr), 1.0); }

  inline fid addf(char ns, fid    fint           ) { return addf(ns, fint          , 1.0); }
  inline fid addf(char ns, string fstr, float val) { return addf(ns, hash(ns, fstr), val); }
  inline fid addf(char ns, string fstr           ) { return addf(ns, hash(ns, fstr), 1.0); }

  inline ezexample& operator()(fid         fint           ) { addf(fint, 1.0); return *this; }
  inline ezexample& operator()(string      fstr           ) { addf(fstr, 1.0); return *this; }
  inline ezexample& operator()(const char* fstr           ) { addf(fstr, 1.0); return *this; }
  inline ezexample& operator()(fid         fint, float val) { addf(fint, val); return *this; }
  inline ezexample& operator()(string      fstr, float val) { addf(fstr, val); return *this; }
  inline ezexample& operator()(const char* fstr, float val) { addf(fstr, val); return *this; }
  inline ezexample& operator()(const vw_namespace&n) { addns(n.namespace_letter); return *this; }

  inline ezexample& operator()(char ns, fid         fint           ) { addf(ns, fint, 1.0); return *this; }
  inline ezexample& operator()(char ns, string      fstr           ) { addf(ns, fstr, 1.0); return *this; }
  inline ezexample& operator()(char ns, const char* fstr           ) { addf(ns, fstr, 1.0); return *this; }
  inline ezexample& operator()(char ns, fid         fint, float val) { addf(ns, fint, val); return *this; }
  inline ezexample& operator()(char ns, string      fstr, float val) { addf(ns, fstr, val); return *this; }
  inline ezexample& operator()(char ns, const char* fstr, float val) { addf(ns, fstr, val); return *this; }


  inline ezexample& operator--() { remns(); return *this; }

  inline float      operator()() { return predict(); }


};




#endif /* DYNAMICFEATURECOLLECTION_H_ */
