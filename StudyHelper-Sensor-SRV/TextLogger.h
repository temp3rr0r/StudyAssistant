using namespace std;

class TextLogger {
private:
     static bool instanceFlag;
     static TextLogger *single;
     TextLogger() {
         //private constructor
     }
public:
    static TextLogger* getInstance();
	void log(string);
	~TextLogger() {
        instanceFlag = false;
    }
};

bool TextLogger::instanceFlag = false;
TextLogger* TextLogger::single = NULL;
TextLogger* TextLogger::getInstance() {
    if(! instanceFlag) {
        single = new TextLogger();
        instanceFlag = true;
        return single;
    } else {
        return single;
    }
}

void TextLogger::log(string message) {            
	FILE *f;
	f=fopen("/home/madks/StudyHelper/logs/StudyHelper_srv.log", "a");
	std::time_t t = std::time(0);  // t is an integer type
	string out = "%d ";
	out.append(message);
	out.append("\n");
	fprintf(f, out.c_str(), t);
	fflush(f);
	fclose(f);
}
