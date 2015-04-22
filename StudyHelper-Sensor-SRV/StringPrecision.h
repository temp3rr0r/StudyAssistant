#include <sstream>
#include <iomanip>

using namespace std;

string to_stringFloat(float floatValue, int decimalPoints) {
	std::ostringstream oStream;

	oStream.setf(ios::fixed, ios::floatfield);
	if (!(oStream << setprecision(decimalPoints) << floatValue) && decimalPoints < 1 )
		return "";
	return oStream.str();
}
string to_stringFloat(float floatValue) {
        return to_stringFloat(floatValue, 2);
}
