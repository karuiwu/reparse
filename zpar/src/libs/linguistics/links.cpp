#include "linguistics/links.h"





std::istream & operator >>(std::istream &is, LinkInputNode &node) {
	std::string line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data (id not found)");
	std::istringstream iss_id(line);
	iss_id >> node.id;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(),
			"Not well formatted Link data (word not found);");
	node.word = line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(),
			"Not well formatted Link data (lemma not found)");
	node.lemma = line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data (cpos not found)");
	node.ctag = line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data (pos not found(");
	node.tag = line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(),
			"Not well formatted Link data (features not found)");
	node.feats = line;

	/**
	 * Edited by JK
	 */
//   int where = is.tellg();
//   getline(is, line, '\t');
//   ASSERT(is && !line.empty(), "Not well formatted Link data (head not found)");
//   std::istringstream iss_head(line);
//   iss_head >> node.head;
//   is.seekg(where);
//end
	return is;
}



std::ostream & operator <<(std::ostream &os, const LinkInputNode &node) {
	os << node.id << '\t' << node.word << '\t' << node.lemma << "\t"
			<< node.ctag << '\t' << node.tag << '\t' << node.feats;
	return os;
}

std::istream & operator >>(std::istream &is, LinkOutputNode &node) {
	std::string line;

	is >> static_cast<CCoNLLInputNode&>(node);

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");
	std::istringstream iss_head(line);
//iss_head >> node.head;

//Read in all the heads into the heads vector
	while (iss_head) {
		std::string head;
		if (!getline(iss_head, head, ','))
			break;
		node.heads.push_back((atoi(head.c_str())));
	}

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");

	std::istringstream iss_label(line);
	while (iss_label) {
		std::string label;
		if (!getline(iss_label, label, ','))
			break;
		node.labels.push_back((label.c_str()));
	}
//node.label = line;

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");
	if (line == "_") {
		// Dont add anything to pheads, just check if its empty.
		//	node.pheads.push_back(DEPENDENCY_LINK_NO_HEAD);

	} else {
		std::istringstream iss_phead(line);
//      iss_phead >> node.phead;

//Read in all the pheads into the pheads vector
		while (iss_phead) {
			std::string phead;
			if (!getline(iss_phead, phead, ','))
				break;
			node.pheads.push_back((atoi(phead.c_str())));
		}

	}

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");

	std::istringstream iss_plabel(line);
	while (iss_plabel) {
		std::string plabel;
		if (!getline(iss_plabel, plabel, ','))
			break;
		node.plabels.push_back((plabel.c_str()));
	}
//node.plabel = line;
	return is;
}

std::ostream & operator <<(std::ostream &os,
		const LinkOutputNode &node) {
	// get phead formatted correct by inserting underscore
	std::ostringstream oss;
	//if (node.phead == DEPENDENCY_LINK_NO_HEAD)
	if (node.pheads.empty())
		oss << '_';
	else {
		os << "[";
		for (unsigned i = 0; i < node.pheads.size(); i++) {
			os << node.pheads[i] << ',';
		}
		os << "]";
//		oss << node.pheads;
	}

	os << static_cast<const LinkInputNode&>(node) << "\t" << node.head << '\t';
	if (node.labels.empty())
		os << "_";
	else {
		os << "[";
		for (unsigned i = 0; i < node.labels.size(); i++) {
			os << node.labels[i] << ',';
		}
//		os << node.labels;
	}
	os << '\t' << oss.str() << '\t';

	if (node.plabels.empty())
		os << "_";
	else {
		os << "[";
		for (unsigned i = 0; i < node.plabels.size(); i++) {
			os << node.plabels[i] << ',';
		}
//		os << node.plabels;
	}
	return os;
}

std::istream & operator >>(std::istream &is, LinkInput &sent) {
	sent.clear();
	std::string line;
	getline(is, line);

	while (is && !lstrip(line).empty()) {
		LinkInputNode node;
		std::istringstream iss(rstrip(line));
		iss >> node;
		sent.push_back(node);
		getline(is, line);
	}
	return is;
}

std::ostream & operator <<(std::ostream &os, const LinkInput &sent) {
	for (unsigned i = 1; i < sent.size(); ++i)
		os << sent.at(i) << std::endl;
	os << std::endl;
	return os;
}





std::istream & operator >> (std::istream &is, LinkOutput &sent) {
   sent.clear();
   std::string line;
   getline(is, line);

   while(is && !lstrip(line).empty())
   {
      LinkOutputNode node;
      std::istringstream iss(rstrip(line));
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

std::ostream & operator << (std::ostream &os, const LinkOutput &sent) {
   for (unsigned i=1; i<sent.size(); ++i)
      os << sent.at(i) << std::endl ;
   os << std::endl ;
   return os ;
}
