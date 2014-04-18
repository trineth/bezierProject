#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "parser.h"
#include "patch.h"
#include "point.h"

class Parser {

	std::vector<Patch> patches;
	int patchNumber;
	float subd;
	bool adaptive;
	bool uniform;
public:
	Parser() {
		patchNumber = 0;
		adaptive = uniform = false;
	}

	Parser(int argc, char *argv[]) {
		parse(argc, argv);
		patchNumber = 0;
		adaptive = uniform = false;
	}

	void parse(int argc, char *argv[]) {
		if (argc > 2) { // at least 2 arguments
			parseFile(argv[1]);
			subd = ::atof(arv[2]);
		}


		if (argc > 3) { // third optional parameter
			std::string str = std::string(argv[3]);
			if (str.compare("-a") != 0) { // if the argument is -a
				adapative = true;
			} else if (str.compare("-u") != 0) { // if the argument is -u
				uniform = true;
			}
		}
	}

	void parseFile(char file[]) {
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

	void parseLine(std::ifstream &inputfile, std::string &line, Patch &patch) {
		std::vector<std::string> array = getStringLine(inputfile, line);
		if (array.size() != 12) {
			return;
		}
		float values[12];
		int i = 0;
		while (i < array.size()) {
			values[i] = ::atof(array[i].c_str());
		}

		Point point1(values[0], values[1], values[2]);
		Point point2(values[3], values[4], values[5]);
		Point point3(values[6], values[7], values[8]);
		Point point4(values[9], values[10], values[11]);

		patch.addCurve(point1, point2, point3, point4);
	}

	std::vector<std::string> getStringLine(std::ifstream &inputfile, std::string &line) {
		std::getline(inputfile, line);
		std::istringstream iss(line);
		std::istream_iterator<std::string> begin(iss), end;
		return std::vector<std::string> array(begin, end);
	}

	std::vector<Patch> getPatches() {
		return patches;
	}

	int getPatchNumber() {
		return patchNumber;
	}

	float getSubdivision() {
		return subd;
	}

	bool isAdaptive() {
		return adaptive;
	}

	bool isUniform() {
		return uniform;
	}
};