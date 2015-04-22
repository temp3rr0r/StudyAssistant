#include "DbParameters.h"
#include "FileReader.h"

using namespace std;
 
// Interface
class ISensor  {
public:
   // pure virtual function providing interface framework.
   virtual ~ISensor() { };
   virtual vector<string> getValues() = 0;  
};