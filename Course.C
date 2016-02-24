// Course.cxx
#include <iostream>
#include "Course.h"

ClassImp(Course);

Course::Course() {
	;
}

void Course::MyPrint() const {
	std::cout << "This is the Course class" << std::endl;
}