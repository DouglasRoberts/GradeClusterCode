// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME LrnAnaDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Course.h"
#include "MakeCourseSections.h"
#include "SectionInfo.h"
#include "GradeCluster.h"
#include "MyFunctions.h"
#include "EnglishCodes.h"
#include "CourseRules.h"
#include "StudentCoursesByTerm.h"
#include "StudentDegreesByTerm.h"
#include "StudentMajorsByTerm.h"
#include "StudentTransferCoursesByTerm.h"
#include "StudentEnrollmentsByTerm.h"

// Header files passed via #pragma extra_include

namespace MyFunctions {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *MyFunctions_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("MyFunctions", 0 /*version*/, "MyFunctions.h", 8,
                     ::ROOT::DefineBehavior((void*)0,(void*)0),
                     &MyFunctions_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *MyFunctions_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_Course(void *p = 0);
   static void *newArray_Course(Long_t size, void *p);
   static void delete_Course(void *p);
   static void deleteArray_Course(void *p);
   static void destruct_Course(void *p);
   static void streamer_Course(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Course*)
   {
      ::Course *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Course >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Course", ::Course::Class_Version(), "Course.h", 5,
                  typeid(::Course), DefineBehavior(ptr, ptr),
                  &::Course::Dictionary, isa_proxy, 16,
                  sizeof(::Course) );
      instance.SetNew(&new_Course);
      instance.SetNewArray(&newArray_Course);
      instance.SetDelete(&delete_Course);
      instance.SetDeleteArray(&deleteArray_Course);
      instance.SetDestructor(&destruct_Course);
      instance.SetStreamerFunc(&streamer_Course);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Course*)
   {
      return GenerateInitInstanceLocal((::Course*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Course*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *EnglishCodes_Dictionary();
   static void EnglishCodes_TClassManip(TClass*);
   static void *new_EnglishCodes(void *p = 0);
   static void *newArray_EnglishCodes(Long_t size, void *p);
   static void delete_EnglishCodes(void *p);
   static void deleteArray_EnglishCodes(void *p);
   static void destruct_EnglishCodes(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnglishCodes*)
   {
      ::EnglishCodes *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::EnglishCodes));
      static ::ROOT::TGenericClassInfo 
         instance("EnglishCodes", "EnglishCodes.h", 18,
                  typeid(::EnglishCodes), DefineBehavior(ptr, ptr),
                  &EnglishCodes_Dictionary, isa_proxy, 0,
                  sizeof(::EnglishCodes) );
      instance.SetNew(&new_EnglishCodes);
      instance.SetNewArray(&newArray_EnglishCodes);
      instance.SetDelete(&delete_EnglishCodes);
      instance.SetDeleteArray(&deleteArray_EnglishCodes);
      instance.SetDestructor(&destruct_EnglishCodes);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnglishCodes*)
   {
      return GenerateInitInstanceLocal((::EnglishCodes*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EnglishCodes*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *EnglishCodes_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::EnglishCodes*)0x0)->GetClass();
      EnglishCodes_TClassManip(theClass);
   return theClass;
   }

   static void EnglishCodes_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StudentCoursesByTerm_Dictionary();
   static void StudentCoursesByTerm_TClassManip(TClass*);
   static void *new_StudentCoursesByTerm(void *p = 0);
   static void *newArray_StudentCoursesByTerm(Long_t size, void *p);
   static void delete_StudentCoursesByTerm(void *p);
   static void deleteArray_StudentCoursesByTerm(void *p);
   static void destruct_StudentCoursesByTerm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StudentCoursesByTerm*)
   {
      ::StudentCoursesByTerm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StudentCoursesByTerm));
      static ::ROOT::TGenericClassInfo 
         instance("StudentCoursesByTerm", "StudentCoursesByTerm.h", 18,
                  typeid(::StudentCoursesByTerm), DefineBehavior(ptr, ptr),
                  &StudentCoursesByTerm_Dictionary, isa_proxy, 0,
                  sizeof(::StudentCoursesByTerm) );
      instance.SetNew(&new_StudentCoursesByTerm);
      instance.SetNewArray(&newArray_StudentCoursesByTerm);
      instance.SetDelete(&delete_StudentCoursesByTerm);
      instance.SetDeleteArray(&deleteArray_StudentCoursesByTerm);
      instance.SetDestructor(&destruct_StudentCoursesByTerm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StudentCoursesByTerm*)
   {
      return GenerateInitInstanceLocal((::StudentCoursesByTerm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StudentCoursesByTerm*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StudentCoursesByTerm_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StudentCoursesByTerm*)0x0)->GetClass();
      StudentCoursesByTerm_TClassManip(theClass);
   return theClass;
   }

   static void StudentCoursesByTerm_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StudentTransferCoursesByTerm_Dictionary();
   static void StudentTransferCoursesByTerm_TClassManip(TClass*);
   static void *new_StudentTransferCoursesByTerm(void *p = 0);
   static void *newArray_StudentTransferCoursesByTerm(Long_t size, void *p);
   static void delete_StudentTransferCoursesByTerm(void *p);
   static void deleteArray_StudentTransferCoursesByTerm(void *p);
   static void destruct_StudentTransferCoursesByTerm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StudentTransferCoursesByTerm*)
   {
      ::StudentTransferCoursesByTerm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StudentTransferCoursesByTerm));
      static ::ROOT::TGenericClassInfo 
         instance("StudentTransferCoursesByTerm", "StudentTransferCoursesByTerm.h", 18,
                  typeid(::StudentTransferCoursesByTerm), DefineBehavior(ptr, ptr),
                  &StudentTransferCoursesByTerm_Dictionary, isa_proxy, 0,
                  sizeof(::StudentTransferCoursesByTerm) );
      instance.SetNew(&new_StudentTransferCoursesByTerm);
      instance.SetNewArray(&newArray_StudentTransferCoursesByTerm);
      instance.SetDelete(&delete_StudentTransferCoursesByTerm);
      instance.SetDeleteArray(&deleteArray_StudentTransferCoursesByTerm);
      instance.SetDestructor(&destruct_StudentTransferCoursesByTerm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StudentTransferCoursesByTerm*)
   {
      return GenerateInitInstanceLocal((::StudentTransferCoursesByTerm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StudentTransferCoursesByTerm*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StudentTransferCoursesByTerm_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StudentTransferCoursesByTerm*)0x0)->GetClass();
      StudentTransferCoursesByTerm_TClassManip(theClass);
   return theClass;
   }

   static void StudentTransferCoursesByTerm_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StudentDegreesByTerm_Dictionary();
   static void StudentDegreesByTerm_TClassManip(TClass*);
   static void *new_StudentDegreesByTerm(void *p = 0);
   static void *newArray_StudentDegreesByTerm(Long_t size, void *p);
   static void delete_StudentDegreesByTerm(void *p);
   static void deleteArray_StudentDegreesByTerm(void *p);
   static void destruct_StudentDegreesByTerm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StudentDegreesByTerm*)
   {
      ::StudentDegreesByTerm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StudentDegreesByTerm));
      static ::ROOT::TGenericClassInfo 
         instance("StudentDegreesByTerm", "StudentDegreesByTerm.h", 18,
                  typeid(::StudentDegreesByTerm), DefineBehavior(ptr, ptr),
                  &StudentDegreesByTerm_Dictionary, isa_proxy, 0,
                  sizeof(::StudentDegreesByTerm) );
      instance.SetNew(&new_StudentDegreesByTerm);
      instance.SetNewArray(&newArray_StudentDegreesByTerm);
      instance.SetDelete(&delete_StudentDegreesByTerm);
      instance.SetDeleteArray(&deleteArray_StudentDegreesByTerm);
      instance.SetDestructor(&destruct_StudentDegreesByTerm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StudentDegreesByTerm*)
   {
      return GenerateInitInstanceLocal((::StudentDegreesByTerm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StudentDegreesByTerm*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StudentDegreesByTerm_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StudentDegreesByTerm*)0x0)->GetClass();
      StudentDegreesByTerm_TClassManip(theClass);
   return theClass;
   }

   static void StudentDegreesByTerm_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StudentMajorsByTerm_Dictionary();
   static void StudentMajorsByTerm_TClassManip(TClass*);
   static void *new_StudentMajorsByTerm(void *p = 0);
   static void *newArray_StudentMajorsByTerm(Long_t size, void *p);
   static void delete_StudentMajorsByTerm(void *p);
   static void deleteArray_StudentMajorsByTerm(void *p);
   static void destruct_StudentMajorsByTerm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StudentMajorsByTerm*)
   {
      ::StudentMajorsByTerm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StudentMajorsByTerm));
      static ::ROOT::TGenericClassInfo 
         instance("StudentMajorsByTerm", "StudentMajorsByTerm.h", 18,
                  typeid(::StudentMajorsByTerm), DefineBehavior(ptr, ptr),
                  &StudentMajorsByTerm_Dictionary, isa_proxy, 0,
                  sizeof(::StudentMajorsByTerm) );
      instance.SetNew(&new_StudentMajorsByTerm);
      instance.SetNewArray(&newArray_StudentMajorsByTerm);
      instance.SetDelete(&delete_StudentMajorsByTerm);
      instance.SetDeleteArray(&deleteArray_StudentMajorsByTerm);
      instance.SetDestructor(&destruct_StudentMajorsByTerm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StudentMajorsByTerm*)
   {
      return GenerateInitInstanceLocal((::StudentMajorsByTerm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StudentMajorsByTerm*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StudentMajorsByTerm_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StudentMajorsByTerm*)0x0)->GetClass();
      StudentMajorsByTerm_TClassManip(theClass);
   return theClass;
   }

   static void StudentMajorsByTerm_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CourseRules_Dictionary();
   static void CourseRules_TClassManip(TClass*);
   static void *new_CourseRules(void *p = 0);
   static void *newArray_CourseRules(Long_t size, void *p);
   static void delete_CourseRules(void *p);
   static void deleteArray_CourseRules(void *p);
   static void destruct_CourseRules(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CourseRules*)
   {
      ::CourseRules *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CourseRules));
      static ::ROOT::TGenericClassInfo 
         instance("CourseRules", "CourseRules.h", 18,
                  typeid(::CourseRules), DefineBehavior(ptr, ptr),
                  &CourseRules_Dictionary, isa_proxy, 0,
                  sizeof(::CourseRules) );
      instance.SetNew(&new_CourseRules);
      instance.SetNewArray(&newArray_CourseRules);
      instance.SetDelete(&delete_CourseRules);
      instance.SetDeleteArray(&deleteArray_CourseRules);
      instance.SetDestructor(&destruct_CourseRules);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CourseRules*)
   {
      return GenerateInitInstanceLocal((::CourseRules*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CourseRules*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CourseRules_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CourseRules*)0x0)->GetClass();
      CourseRules_TClassManip(theClass);
   return theClass;
   }

   static void CourseRules_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StudentEnrollmentsByTerm_Dictionary();
   static void StudentEnrollmentsByTerm_TClassManip(TClass*);
   static void *new_StudentEnrollmentsByTerm(void *p = 0);
   static void *newArray_StudentEnrollmentsByTerm(Long_t size, void *p);
   static void delete_StudentEnrollmentsByTerm(void *p);
   static void deleteArray_StudentEnrollmentsByTerm(void *p);
   static void destruct_StudentEnrollmentsByTerm(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StudentEnrollmentsByTerm*)
   {
      ::StudentEnrollmentsByTerm *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StudentEnrollmentsByTerm));
      static ::ROOT::TGenericClassInfo 
         instance("StudentEnrollmentsByTerm", "StudentEnrollmentsByTerm.h", 18,
                  typeid(::StudentEnrollmentsByTerm), DefineBehavior(ptr, ptr),
                  &StudentEnrollmentsByTerm_Dictionary, isa_proxy, 0,
                  sizeof(::StudentEnrollmentsByTerm) );
      instance.SetNew(&new_StudentEnrollmentsByTerm);
      instance.SetNewArray(&newArray_StudentEnrollmentsByTerm);
      instance.SetDelete(&delete_StudentEnrollmentsByTerm);
      instance.SetDeleteArray(&deleteArray_StudentEnrollmentsByTerm);
      instance.SetDestructor(&destruct_StudentEnrollmentsByTerm);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StudentEnrollmentsByTerm*)
   {
      return GenerateInitInstanceLocal((::StudentEnrollmentsByTerm*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StudentEnrollmentsByTerm*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StudentEnrollmentsByTerm_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StudentEnrollmentsByTerm*)0x0)->GetClass();
      StudentEnrollmentsByTerm_TClassManip(theClass);
   return theClass;
   }

   static void StudentEnrollmentsByTerm_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MakeCourseSections_Dictionary();
   static void MakeCourseSections_TClassManip(TClass*);
   static void *new_MakeCourseSections(void *p = 0);
   static void *newArray_MakeCourseSections(Long_t size, void *p);
   static void delete_MakeCourseSections(void *p);
   static void deleteArray_MakeCourseSections(void *p);
   static void destruct_MakeCourseSections(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MakeCourseSections*)
   {
      ::MakeCourseSections *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MakeCourseSections));
      static ::ROOT::TGenericClassInfo 
         instance("MakeCourseSections", "MakeCourseSections.h", 8,
                  typeid(::MakeCourseSections), DefineBehavior(ptr, ptr),
                  &MakeCourseSections_Dictionary, isa_proxy, 0,
                  sizeof(::MakeCourseSections) );
      instance.SetNew(&new_MakeCourseSections);
      instance.SetNewArray(&newArray_MakeCourseSections);
      instance.SetDelete(&delete_MakeCourseSections);
      instance.SetDeleteArray(&deleteArray_MakeCourseSections);
      instance.SetDestructor(&destruct_MakeCourseSections);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MakeCourseSections*)
   {
      return GenerateInitInstanceLocal((::MakeCourseSections*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MakeCourseSections*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MakeCourseSections_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MakeCourseSections*)0x0)->GetClass();
      MakeCourseSections_TClassManip(theClass);
   return theClass;
   }

   static void MakeCourseSections_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_SectionInfo(void *p = 0);
   static void *newArray_SectionInfo(Long_t size, void *p);
   static void delete_SectionInfo(void *p);
   static void deleteArray_SectionInfo(void *p);
   static void destruct_SectionInfo(void *p);
   static void streamer_SectionInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SectionInfo*)
   {
      ::SectionInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SectionInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SectionInfo", ::SectionInfo::Class_Version(), "SectionInfo.h", 10,
                  typeid(::SectionInfo), DefineBehavior(ptr, ptr),
                  &::SectionInfo::Dictionary, isa_proxy, 16,
                  sizeof(::SectionInfo) );
      instance.SetNew(&new_SectionInfo);
      instance.SetNewArray(&newArray_SectionInfo);
      instance.SetDelete(&delete_SectionInfo);
      instance.SetDeleteArray(&deleteArray_SectionInfo);
      instance.SetDestructor(&destruct_SectionInfo);
      instance.SetStreamerFunc(&streamer_SectionInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SectionInfo*)
   {
      return GenerateInitInstanceLocal((::SectionInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SectionInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GradeCluster(void *p = 0);
   static void *newArray_GradeCluster(Long_t size, void *p);
   static void delete_GradeCluster(void *p);
   static void deleteArray_GradeCluster(void *p);
   static void destruct_GradeCluster(void *p);
   static void streamer_GradeCluster(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GradeCluster*)
   {
      ::GradeCluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GradeCluster >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GradeCluster", ::GradeCluster::Class_Version(), "GradeCluster.h", 10,
                  typeid(::GradeCluster), DefineBehavior(ptr, ptr),
                  &::GradeCluster::Dictionary, isa_proxy, 16,
                  sizeof(::GradeCluster) );
      instance.SetNew(&new_GradeCluster);
      instance.SetNewArray(&newArray_GradeCluster);
      instance.SetDelete(&delete_GradeCluster);
      instance.SetDeleteArray(&deleteArray_GradeCluster);
      instance.SetDestructor(&destruct_GradeCluster);
      instance.SetStreamerFunc(&streamer_GradeCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GradeCluster*)
   {
      return GenerateInitInstanceLocal((::GradeCluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GradeCluster*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Course::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Course::Class_Name()
{
   return "Course";
}

//______________________________________________________________________________
const char *Course::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Course*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Course::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Course*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Course::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Course*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Course::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Course*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SectionInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SectionInfo::Class_Name()
{
   return "SectionInfo";
}

//______________________________________________________________________________
const char *SectionInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SectionInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SectionInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SectionInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SectionInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SectionInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SectionInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SectionInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GradeCluster::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GradeCluster::Class_Name()
{
   return "GradeCluster";
}

//______________________________________________________________________________
const char *GradeCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GradeCluster*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GradeCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GradeCluster*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GradeCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GradeCluster*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GradeCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GradeCluster*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Course::Streamer(TBuffer &R__b)
{
   // Stream an object of class Course.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, Course::IsA());
   } else {
      R__c = R__b.WriteVersion(Course::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Course(void *p) {
      return  p ? new(p) ::Course : new ::Course;
   }
   static void *newArray_Course(Long_t nElements, void *p) {
      return p ? new(p) ::Course[nElements] : new ::Course[nElements];
   }
   // Wrapper around operator delete
   static void delete_Course(void *p) {
      delete ((::Course*)p);
   }
   static void deleteArray_Course(void *p) {
      delete [] ((::Course*)p);
   }
   static void destruct_Course(void *p) {
      typedef ::Course current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Course(TBuffer &buf, void *obj) {
      ((::Course*)obj)->::Course::Streamer(buf);
   }
} // end of namespace ROOT for class ::Course

namespace ROOT {
   // Wrappers around operator new
   static void *new_EnglishCodes(void *p) {
      return  p ? new(p) ::EnglishCodes : new ::EnglishCodes;
   }
   static void *newArray_EnglishCodes(Long_t nElements, void *p) {
      return p ? new(p) ::EnglishCodes[nElements] : new ::EnglishCodes[nElements];
   }
   // Wrapper around operator delete
   static void delete_EnglishCodes(void *p) {
      delete ((::EnglishCodes*)p);
   }
   static void deleteArray_EnglishCodes(void *p) {
      delete [] ((::EnglishCodes*)p);
   }
   static void destruct_EnglishCodes(void *p) {
      typedef ::EnglishCodes current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EnglishCodes

namespace ROOT {
   // Wrappers around operator new
   static void *new_StudentCoursesByTerm(void *p) {
      return  p ? new(p) ::StudentCoursesByTerm : new ::StudentCoursesByTerm;
   }
   static void *newArray_StudentCoursesByTerm(Long_t nElements, void *p) {
      return p ? new(p) ::StudentCoursesByTerm[nElements] : new ::StudentCoursesByTerm[nElements];
   }
   // Wrapper around operator delete
   static void delete_StudentCoursesByTerm(void *p) {
      delete ((::StudentCoursesByTerm*)p);
   }
   static void deleteArray_StudentCoursesByTerm(void *p) {
      delete [] ((::StudentCoursesByTerm*)p);
   }
   static void destruct_StudentCoursesByTerm(void *p) {
      typedef ::StudentCoursesByTerm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StudentCoursesByTerm

namespace ROOT {
   // Wrappers around operator new
   static void *new_StudentTransferCoursesByTerm(void *p) {
      return  p ? new(p) ::StudentTransferCoursesByTerm : new ::StudentTransferCoursesByTerm;
   }
   static void *newArray_StudentTransferCoursesByTerm(Long_t nElements, void *p) {
      return p ? new(p) ::StudentTransferCoursesByTerm[nElements] : new ::StudentTransferCoursesByTerm[nElements];
   }
   // Wrapper around operator delete
   static void delete_StudentTransferCoursesByTerm(void *p) {
      delete ((::StudentTransferCoursesByTerm*)p);
   }
   static void deleteArray_StudentTransferCoursesByTerm(void *p) {
      delete [] ((::StudentTransferCoursesByTerm*)p);
   }
   static void destruct_StudentTransferCoursesByTerm(void *p) {
      typedef ::StudentTransferCoursesByTerm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StudentTransferCoursesByTerm

namespace ROOT {
   // Wrappers around operator new
   static void *new_StudentDegreesByTerm(void *p) {
      return  p ? new(p) ::StudentDegreesByTerm : new ::StudentDegreesByTerm;
   }
   static void *newArray_StudentDegreesByTerm(Long_t nElements, void *p) {
      return p ? new(p) ::StudentDegreesByTerm[nElements] : new ::StudentDegreesByTerm[nElements];
   }
   // Wrapper around operator delete
   static void delete_StudentDegreesByTerm(void *p) {
      delete ((::StudentDegreesByTerm*)p);
   }
   static void deleteArray_StudentDegreesByTerm(void *p) {
      delete [] ((::StudentDegreesByTerm*)p);
   }
   static void destruct_StudentDegreesByTerm(void *p) {
      typedef ::StudentDegreesByTerm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StudentDegreesByTerm

namespace ROOT {
   // Wrappers around operator new
   static void *new_StudentMajorsByTerm(void *p) {
      return  p ? new(p) ::StudentMajorsByTerm : new ::StudentMajorsByTerm;
   }
   static void *newArray_StudentMajorsByTerm(Long_t nElements, void *p) {
      return p ? new(p) ::StudentMajorsByTerm[nElements] : new ::StudentMajorsByTerm[nElements];
   }
   // Wrapper around operator delete
   static void delete_StudentMajorsByTerm(void *p) {
      delete ((::StudentMajorsByTerm*)p);
   }
   static void deleteArray_StudentMajorsByTerm(void *p) {
      delete [] ((::StudentMajorsByTerm*)p);
   }
   static void destruct_StudentMajorsByTerm(void *p) {
      typedef ::StudentMajorsByTerm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StudentMajorsByTerm

namespace ROOT {
   // Wrappers around operator new
   static void *new_CourseRules(void *p) {
      return  p ? new(p) ::CourseRules : new ::CourseRules;
   }
   static void *newArray_CourseRules(Long_t nElements, void *p) {
      return p ? new(p) ::CourseRules[nElements] : new ::CourseRules[nElements];
   }
   // Wrapper around operator delete
   static void delete_CourseRules(void *p) {
      delete ((::CourseRules*)p);
   }
   static void deleteArray_CourseRules(void *p) {
      delete [] ((::CourseRules*)p);
   }
   static void destruct_CourseRules(void *p) {
      typedef ::CourseRules current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CourseRules

namespace ROOT {
   // Wrappers around operator new
   static void *new_StudentEnrollmentsByTerm(void *p) {
      return  p ? new(p) ::StudentEnrollmentsByTerm : new ::StudentEnrollmentsByTerm;
   }
   static void *newArray_StudentEnrollmentsByTerm(Long_t nElements, void *p) {
      return p ? new(p) ::StudentEnrollmentsByTerm[nElements] : new ::StudentEnrollmentsByTerm[nElements];
   }
   // Wrapper around operator delete
   static void delete_StudentEnrollmentsByTerm(void *p) {
      delete ((::StudentEnrollmentsByTerm*)p);
   }
   static void deleteArray_StudentEnrollmentsByTerm(void *p) {
      delete [] ((::StudentEnrollmentsByTerm*)p);
   }
   static void destruct_StudentEnrollmentsByTerm(void *p) {
      typedef ::StudentEnrollmentsByTerm current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StudentEnrollmentsByTerm

namespace ROOT {
   // Wrappers around operator new
   static void *new_MakeCourseSections(void *p) {
      return  p ? new(p) ::MakeCourseSections : new ::MakeCourseSections;
   }
   static void *newArray_MakeCourseSections(Long_t nElements, void *p) {
      return p ? new(p) ::MakeCourseSections[nElements] : new ::MakeCourseSections[nElements];
   }
   // Wrapper around operator delete
   static void delete_MakeCourseSections(void *p) {
      delete ((::MakeCourseSections*)p);
   }
   static void deleteArray_MakeCourseSections(void *p) {
      delete [] ((::MakeCourseSections*)p);
   }
   static void destruct_MakeCourseSections(void *p) {
      typedef ::MakeCourseSections current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MakeCourseSections

//______________________________________________________________________________
void SectionInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class SectionInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      _crsName.Streamer(R__b);
      R__b >> _secNum;
      R__b >> _term;
      {
         vector<SectionInfo::GrdRecord> &R__stl =  _grades;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct SectionInfo::GrdRecord));
         if (R__tcl1==0) {
            Error("_grades streamer","Missing the TClass object for struct SectionInfo::GrdRecord!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            SectionInfo::GrdRecord R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> _gradeHist;
      _collegeSD.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, SectionInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(SectionInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      _crsName.Streamer(R__b);
      R__b << _secNum;
      R__b << _term;
      {
         vector<SectionInfo::GrdRecord> &R__stl =  _grades;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct SectionInfo::GrdRecord));
         if (R__tcl1==0) {
            Error("_grades streamer","Missing the TClass object for struct SectionInfo::GrdRecord!");
            return;
         }
            vector<SectionInfo::GrdRecord>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((SectionInfo::GrdRecord*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << (TObject*)_gradeHist;
      _collegeSD.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SectionInfo(void *p) {
      return  p ? new(p) ::SectionInfo : new ::SectionInfo;
   }
   static void *newArray_SectionInfo(Long_t nElements, void *p) {
      return p ? new(p) ::SectionInfo[nElements] : new ::SectionInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_SectionInfo(void *p) {
      delete ((::SectionInfo*)p);
   }
   static void deleteArray_SectionInfo(void *p) {
      delete [] ((::SectionInfo*)p);
   }
   static void destruct_SectionInfo(void *p) {
      typedef ::SectionInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SectionInfo(TBuffer &buf, void *obj) {
      ((::SectionInfo*)obj)->::SectionInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::SectionInfo

//______________________________________________________________________________
void GradeCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class GradeCluster.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<SectionInfo> &R__stl =  _cluster;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            SectionInfo R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b >> _hist;
      R__b.CheckByteCount(R__s, R__c, GradeCluster::IsA());
   } else {
      R__c = R__b.WriteVersion(GradeCluster::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<SectionInfo> &R__stl =  _cluster;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<SectionInfo>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((SectionInfo&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b << (TObject*)_hist;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GradeCluster(void *p) {
      return  p ? new(p) ::GradeCluster : new ::GradeCluster;
   }
   static void *newArray_GradeCluster(Long_t nElements, void *p) {
      return p ? new(p) ::GradeCluster[nElements] : new ::GradeCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_GradeCluster(void *p) {
      delete ((::GradeCluster*)p);
   }
   static void deleteArray_GradeCluster(void *p) {
      delete [] ((::GradeCluster*)p);
   }
   static void destruct_GradeCluster(void *p) {
      typedef ::GradeCluster current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GradeCluster(TBuffer &buf, void *obj) {
      ((::GradeCluster*)obj)->::GradeCluster::Streamer(buf);
   }
} // end of namespace ROOT for class ::GradeCluster

namespace ROOT {
   static TClass *vectorlESectionInfogR_Dictionary();
   static void vectorlESectionInfogR_TClassManip(TClass*);
   static void *new_vectorlESectionInfogR(void *p = 0);
   static void *newArray_vectorlESectionInfogR(Long_t size, void *p);
   static void delete_vectorlESectionInfogR(void *p);
   static void deleteArray_vectorlESectionInfogR(void *p);
   static void destruct_vectorlESectionInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<SectionInfo>*)
   {
      vector<SectionInfo> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<SectionInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<SectionInfo>", -2, "vector", 477,
                  typeid(vector<SectionInfo>), DefineBehavior(ptr, ptr),
                  &vectorlESectionInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<SectionInfo>) );
      instance.SetNew(&new_vectorlESectionInfogR);
      instance.SetNewArray(&newArray_vectorlESectionInfogR);
      instance.SetDelete(&delete_vectorlESectionInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlESectionInfogR);
      instance.SetDestructor(&destruct_vectorlESectionInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<SectionInfo> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<SectionInfo>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlESectionInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<SectionInfo>*)0x0)->GetClass();
      vectorlESectionInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlESectionInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlESectionInfogR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<SectionInfo> : new vector<SectionInfo>;
   }
   static void *newArray_vectorlESectionInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<SectionInfo>[nElements] : new vector<SectionInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlESectionInfogR(void *p) {
      delete ((vector<SectionInfo>*)p);
   }
   static void deleteArray_vectorlESectionInfogR(void *p) {
      delete [] ((vector<SectionInfo>*)p);
   }
   static void destruct_vectorlESectionInfogR(void *p) {
      typedef vector<SectionInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<SectionInfo>

namespace ROOT {
   static TClass *vectorlESectionInfocLcLGrdRecordgR_Dictionary();
   static void vectorlESectionInfocLcLGrdRecordgR_TClassManip(TClass*);
   static void *new_vectorlESectionInfocLcLGrdRecordgR(void *p = 0);
   static void *newArray_vectorlESectionInfocLcLGrdRecordgR(Long_t size, void *p);
   static void delete_vectorlESectionInfocLcLGrdRecordgR(void *p);
   static void deleteArray_vectorlESectionInfocLcLGrdRecordgR(void *p);
   static void destruct_vectorlESectionInfocLcLGrdRecordgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<SectionInfo::GrdRecord>*)
   {
      vector<SectionInfo::GrdRecord> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<SectionInfo::GrdRecord>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<SectionInfo::GrdRecord>", -2, "vector", 477,
                  typeid(vector<SectionInfo::GrdRecord>), DefineBehavior(ptr, ptr),
                  &vectorlESectionInfocLcLGrdRecordgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<SectionInfo::GrdRecord>) );
      instance.SetNew(&new_vectorlESectionInfocLcLGrdRecordgR);
      instance.SetNewArray(&newArray_vectorlESectionInfocLcLGrdRecordgR);
      instance.SetDelete(&delete_vectorlESectionInfocLcLGrdRecordgR);
      instance.SetDeleteArray(&deleteArray_vectorlESectionInfocLcLGrdRecordgR);
      instance.SetDestructor(&destruct_vectorlESectionInfocLcLGrdRecordgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<SectionInfo::GrdRecord> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<SectionInfo::GrdRecord>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlESectionInfocLcLGrdRecordgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<SectionInfo::GrdRecord>*)0x0)->GetClass();
      vectorlESectionInfocLcLGrdRecordgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlESectionInfocLcLGrdRecordgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlESectionInfocLcLGrdRecordgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<SectionInfo::GrdRecord> : new vector<SectionInfo::GrdRecord>;
   }
   static void *newArray_vectorlESectionInfocLcLGrdRecordgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<SectionInfo::GrdRecord>[nElements] : new vector<SectionInfo::GrdRecord>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlESectionInfocLcLGrdRecordgR(void *p) {
      delete ((vector<SectionInfo::GrdRecord>*)p);
   }
   static void deleteArray_vectorlESectionInfocLcLGrdRecordgR(void *p) {
      delete [] ((vector<SectionInfo::GrdRecord>*)p);
   }
   static void destruct_vectorlESectionInfocLcLGrdRecordgR(void *p) {
      typedef vector<SectionInfo::GrdRecord> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<SectionInfo::GrdRecord>

namespace ROOT {
   static TClass *maplETStringcOTStringgR_Dictionary();
   static void maplETStringcOTStringgR_TClassManip(TClass*);
   static void *new_maplETStringcOTStringgR(void *p = 0);
   static void *newArray_maplETStringcOTStringgR(Long_t size, void *p);
   static void delete_maplETStringcOTStringgR(void *p);
   static void deleteArray_maplETStringcOTStringgR(void *p);
   static void destruct_maplETStringcOTStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TString>*)
   {
      map<TString,TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TString>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TString>", -2, "map", 783,
                  typeid(map<TString,TString>), DefineBehavior(ptr, ptr),
                  &maplETStringcOTStringgR_Dictionary, isa_proxy, 4,
                  sizeof(map<TString,TString>) );
      instance.SetNew(&new_maplETStringcOTStringgR);
      instance.SetNewArray(&newArray_maplETStringcOTStringgR);
      instance.SetDelete(&delete_maplETStringcOTStringgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTStringgR);
      instance.SetDestructor(&destruct_maplETStringcOTStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TString> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<TString,TString>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TString>*)0x0)->GetClass();
      maplETStringcOTStringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTStringgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<TString,TString> : new map<TString,TString>;
   }
   static void *newArray_maplETStringcOTStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<TString,TString>[nElements] : new map<TString,TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTStringgR(void *p) {
      delete ((map<TString,TString>*)p);
   }
   static void deleteArray_maplETStringcOTStringgR(void *p) {
      delete [] ((map<TString,TString>*)p);
   }
   static void destruct_maplETStringcOTStringgR(void *p) {
      typedef map<TString,TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TString>

namespace {
  void TriggerDictionaryInitialization_LrnAnaDict_Impl() {
    static const char* headers[] = {
"Course.h",
"MakeCourseSections.h",
"SectionInfo.h",
"GradeCluster.h",
"MyFunctions.h",
"EnglishCodes.h",
"CourseRules.h",
"StudentCoursesByTerm.h",
"StudentDegreesByTerm.h",
"StudentMajorsByTerm.h",
"StudentTransferCoursesByTerm.h",
"StudentEnrollmentsByTerm.h",
0
    };
    static const char* includePaths[] = {
"/Applications/root_v6.04.14/include",
"/Volumes/Macintosh_HD_2/LearningAnalytics/GradeClusterCode/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Course.h")))  Course;
class __attribute__((annotate("$clingAutoload$EnglishCodes.h")))  EnglishCodes;
class __attribute__((annotate("$clingAutoload$MakeCourseSections.h")))  StudentCoursesByTerm;
class __attribute__((annotate("$clingAutoload$StudentTransferCoursesByTerm.h")))  StudentTransferCoursesByTerm;
class __attribute__((annotate("$clingAutoload$StudentDegreesByTerm.h")))  StudentDegreesByTerm;
class __attribute__((annotate("$clingAutoload$StudentMajorsByTerm.h")))  StudentMajorsByTerm;
class __attribute__((annotate("$clingAutoload$CourseRules.h")))  CourseRules;
class __attribute__((annotate("$clingAutoload$StudentEnrollmentsByTerm.h")))  StudentEnrollmentsByTerm;
class __attribute__((annotate("$clingAutoload$MakeCourseSections.h")))  MakeCourseSections;
class __attribute__((annotate("$clingAutoload$MakeCourseSections.h")))  SectionInfo;
class __attribute__((annotate("$clingAutoload$MakeCourseSections.h")))  GradeCluster;
class __attribute__((annotate("$clingAutoload$MakeCourseSections.h")))  TString;
namespace std{inline namespace __1{template <class _Tp> struct __attribute__((annotate("$clingAutoload$string")))  less;
}}
namespace std{inline namespace __1{template <class _T1, class _T2> struct __attribute__((annotate("$clingAutoload$string")))  pair;
}}
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Course.h"
#include "MakeCourseSections.h"
#include "SectionInfo.h"
#include "GradeCluster.h"
#include "MyFunctions.h"
#include "EnglishCodes.h"
#include "CourseRules.h"
#include "StudentCoursesByTerm.h"
#include "StudentDegreesByTerm.h"
#include "StudentMajorsByTerm.h"
#include "StudentTransferCoursesByTerm.h"
#include "StudentEnrollmentsByTerm.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Course", payloadCode, "@",
"CourseRules", payloadCode, "@",
"EnglishCodes", payloadCode, "@",
"GradeCluster", payloadCode, "@",
"MakeCourseSections", payloadCode, "@",
"SectionInfo", payloadCode, "@",
"StudentCoursesByTerm", payloadCode, "@",
"StudentDegreesByTerm", payloadCode, "@",
"StudentEnrollmentsByTerm", payloadCode, "@",
"StudentMajorsByTerm", payloadCode, "@",
"StudentTransferCoursesByTerm", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("LrnAnaDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_LrnAnaDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_LrnAnaDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_LrnAnaDict() {
  TriggerDictionaryInitialization_LrnAnaDict_Impl();
}
