#ifndef Course_h
#define Course_h
#include "TObject.h"

class Course : public TObject {

public:
	Course();
	void MyPrint() const;
	
	ClassDef(Course, 1)

};
#endif
