#include <fstream>

class FileReader  {
public:
	int readIntFromFile(string filePath, int index) {
		return readIntFromFile(filePath, index, 0);
	}
	
	int readIntFromFile(string filePath, int index, int seek) {
		int value = 0;
 		try {
			ifstream inFile;
			
			inFile.open(filePath.c_str(), ios::in|ios::binary|ios::ate);
			char* oData = 0;
			inFile.seekg(seek, ios::beg);
 			oData = new char[7];
 			inFile.read(oData, index);
 			value = std::stoi(oData);
 			inFile.close();
		}  catch (std::ifstream::failure oor) {
			std::cerr << "I/O error: " << oor.what() << endl;
		}
		return value;
	}
};