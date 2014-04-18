#ifndef PARSER_H
#define PARSER_H

#include "patch.h"

class Parser {
public:
	Parser();
	Parser(int argc, char *argv[]);

	void parse (int argc, char *argv[]);
	void parseFile(char file[]);
	void parseLine(std::ifstream &inputfile, std::string &line, Patch &patch);
	std::vector<std::string> getStringLine(std::ifstream &inputfile, std::string &line);
	std::vector<Patch> getPatches();
	int getPatchNumber();
	float getSubdivision();
	bool isAdaptive();
	bool isUniform();
};

 
#endif