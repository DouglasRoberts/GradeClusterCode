#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ namespace MyFunctions;
#pragma link C++ class SectionInfo;
#pragma link C++ class Student;
#pragma link C++ class Student::Grade;
#pragma link C++ class Student::Enrollment;
#pragma link C++ class GradeCluster;
#pragma link C++ class std::map<TString, TString>;
#pragma link C++ class std::vector<SectionInfo>;
#pragma link C++ class std::vector<Student::Enrollment>;
#pragma link C++ class std::vector<Student::Grade>;
//#pragma link C++ class std::vector<std::reference_wrapper<Student::Grade>>;

#endif
