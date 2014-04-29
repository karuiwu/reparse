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
	os << node.id << "\t" << node.word << "\t" << node.lemma << "\t"
			<< node.ctag << "\t" << node.tag << "\t" << node.feats;

//	os << "id: " << node.id << "\t| word: " << node.word << "\t| lemma: "
//			<< node.lemma << "\t| ctag: " << node.ctag << "\t| tag: "
//			<< node.tag << "\t| feats: " << node.feats;
	return os;
}

std::istream & operator >>(std::istream &is, LinkOutputNode &node) {
	std::string line;
	is >> static_cast<LinkInputNode&>(node);

	// Read in the head
	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");

	int head = 0;
	int prevIndex = 0;
	std::size_t found = line.find(",");
	while(found != std::string::npos){
		std::istringstream iss_head(line.substr(prevIndex, found-prevIndex));
		if(!(iss_head >> head)){
			head = -1;
		}
		node.heads.push_back(head);

		prevIndex = found+1;
		found = line.find(",", found+1);
		head = 0;
	}
	if(found == std::string::npos){
		found = line.length();
	}


	std::istringstream iss_head(line.substr(prevIndex, found-prevIndex));
	if(!(iss_head >> head)){
		head = -1;
	}
	node.heads.push_back(head);

//	std::cout << "line: " << line << "\n";
//	std::cout << "heads: ";
//	for (int i = 0; i < node.heads.size(); i++){
//		std::cout << node.heads[i] << " ";
//	}
//	std::cout << "\n";

	getline(is, line, '\t');
	ASSERT(is && !line.empty(), "Not well formatted Link data");

//	std::cout << "line: " << line << "\n";

	prevIndex = 0;
	found = line.find(",");


	std::string label;
	while(found != std::string::npos){
		label = line.substr(prevIndex, found-prevIndex);
		node.labels.push_back(label);

		prevIndex = found+1;
		found = line.find(",", found+1);
	}

	if(found == std::string::npos){
		found = line.length();
	}
	label = line.substr(prevIndex, found-prevIndex);
	node.labels.push_back(label);

//	std::cout << "label: ";
//	for(int i = 0; i < node.labels.size(); i++){
//		std::cout << node.labels[i] << " ";
//	}
//	std::cout << "\n\n";


//	std::string line2;
//	while (getline(is, line, '\t') && getline(is, line2, '\t')) {
//		std::istringstream iss_head(line);
//
//		if (line.compare("-") != 0 && line.compare("_") != 0
//				&& line2.compare("-") != 0 && line2.compare("_")) {
//			head = 0;
//			if (!(iss_head >> head)) {
//				head = -1;
//			}
//			node.heads.push_back(head);
//
//			label = line2;
//			node.labels.push_back(label);
//		}
//	}

	return is;
}

std::ostream & operator <<(std::ostream &os, const LinkOutputNode &node) {
	os << static_cast<const LinkInputNode&>(node);

	std::ostringstream oss;
	os << "\t";
	if (node.heads.empty())
		oss << '_';
	else {
		for (unsigned i = 0; i < node.heads.size(); i++) {
			os << node.heads[i];
			if(i != node.heads.size()-1){
				os << ',';
			}
		}
	}
//	os << "]";

	os << "\t";
	if (node.labels.empty())
		os << "_";
	else {
		for (unsigned i = 0; i < node.labels.size(); i++) {
			os << node.labels[i];
			if(i != node.labels.size()-1){
				os << ',';
			}
		}
	}
//	os << "]";
	os << '\t' << oss.str() << '\t';

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

//	os << "aaa" << std::endl;
	return os;
}

std::istream & operator >>(std::istream &is, LinkOutput &sent) {
	sent.clear();
	std::string line;
	getline(is, line);

	while (is && !lstrip(line).empty()) {
		LinkOutputNode node;
		std::istringstream iss(rstrip(line));
		iss >> node;
		sent.push_back(node);
		getline(is, line);
	}
	return is;
}

std::ostream & operator <<(std::ostream &os, const LinkOutput &sent) {
	for (unsigned i = 1; i < sent.size(); ++i)
		os << sent.at(i) << std::endl;

	os << std::endl;
	return os;
}
