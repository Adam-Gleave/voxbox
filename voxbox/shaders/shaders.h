#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Shaders 
{
public:
	Shaders() {
		ifstream vertFile(vertLoc);
		string line;

		if (vertFile.is_open()) {
			while (getline(vertFile, line)) {
				vert += line;
				vert += "\n";
			}
			vertFile.close();
		}
		else {
			cout << "Unable to open vertex shader.";
		}

		ifstream fragFile(fragLoc);
		
		if (fragFile.is_open()) {
			while (getline(fragFile, line)) {
				frag += line;
				frag += "\n";
			}
			fragFile.close();
		}
		else {
			cout << "Unable to open fragment shader.";
		}
	}

	const char *getFrag() {
		return frag.c_str();
	}

	const char *getVert() {
		return vert.c_str();
	}

private:
	std::string vert = "";
	std::string frag = "";
	const std::string vertLoc = "shaders/test.vert";
	const std::string fragLoc = "shaders/test.frag";
};