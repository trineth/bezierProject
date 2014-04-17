#ifndef ADD_H
#define ADD_H
 
class Parser {
public:
	Parser();
	Parser(int argc, char *argv[]);

	void parse (int argc, char *argv[]);
	void parseFile(char file[]);
	void parseLine(std::ifstream &inputfile, std::string &line, Patch &patch);
	std::vector<std::string> getStringLine(std::ifstream &inputfile, std::string &line);
};

 
#endif