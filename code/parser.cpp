#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include "parser.h"
#include "patch.h"
#include "point.h"

// class Parser {
// 	std::vector<Patch> patches;
// 	int patchNumber;
// 	float subd;
// 	bool adaptive;
// 	bool uniform;
// public:
	Parser::Parser() {
		patchNumber = 0;
		adaptive = uniform = false;
	}

	Parser::Parser(int argc, char *argv[]) {
		parse(argc, argv);
		patchNumber = 0;
		adaptive = uniform = false;
	}

	void Parser::parse(int argc, char *argv[]) {

		if (argc > 2) { // at least 2 arguments
			parseFile(argv[1]);
			subd = ::atof(argv[2]);
			if (argc > 3) {
				std::string str = std::string(argv[3]);
				if (str.compare("-a") != 0) { // at least 3 arguments&if the argument is -a 
					adaptive = true;
				} else { // if the argument is -u
					uniform = true;
				}
			} else {
				uniform = true;
			}
		} else {
			std::string input = "./main[filename] [subdivision] -[a/u]";
			std::cout << "Function main accepts input " << input << "\n";
			exit(0);
		}
	}

	void Parser::parseFile(char file[]) {
		std::ifstream inputfile;
		inputfile.open(file);

		std::string line;
		std::vector<std::string> array = getStringLine(inputfile, line);
		if (array.size() != 1) {
			exit(0);
		}

		int i = 0;
		patchNumber =  ::atoi(array[0].c_str());
		while (i < patchNumber) { //One patch per loop
			Patch patch;
			parseLine(inputfile, line, patch); //Curve 1
			parseLine(inputfile, line, patch); //Curve 2
			parseLine(inputfile, line, patch); //Curve 3
			parseLine(inputfile, line, patch); //Curve 4
			parseLine(inputfile, line, patch); //Empty line
			patches.push_back(patch);
			i++;
		}
	}

	void Parser::parseLine(std::ifstream &inputfile, std::string &line, Patch &patch) {
		std::vector<std::string> array = getStringLine(inputfile, line);
		if (array.size() < 1) {
			return;
		}
		
		float values[12];
		int i = 0;
		while (i < array.size()) {
			values[i] = ::atof(array[i].c_str());
			i++;
		}

		Point point1(values[0], values[1], values[2]);
		Point point2(values[3], values[4], values[5]);
		Point point3(values[6], values[7], values[8]);
		Point point4(values[9], values[10], values[11]);

		patch.addCurve(point1, point2, point3, point4);
	}

	std::vector<std::string> Parser::getStringLine(std::ifstream &inputfile, std::string &line) {
		std::getline(inputfile, line);
		std::istringstream iss(line);
		std::istream_iterator<std::string> begin(iss), end;
		std::vector<std::string> array(begin, end);
		return array;
	}

	std::vector<Patch> Parser::getPatches() {
		return patches;
	}

	int Parser::getPatchNumber() {
		return patchNumber;
	}

	float Parser::getSubdivision() {
		return subd;
	}

	bool Parser::isAdaptive() {
		return adaptive;
	}

	bool Parser::isUniform() {
		return uniform;
	}
// };