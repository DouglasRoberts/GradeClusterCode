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
#include "SectionInfo.h"
#include "GradeCluster.h"
#include "MyFunctions.h"

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
void SectionInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class SectionInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      _crsName.Streamer(R__b);
      _secNum.Streamer(R__b);
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
      _secNum.Streamer(R__b);
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

namespace {
  void TriggerDictionaryInitialization_LrnAnaDict_Impl() {
    static const char* headers[] = {
"SectionInfo.h",
"GradeCluster.h",
"MyFunctions.h",
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
class __attribute__((annotate("$clingAutoload$SectionInfo.h")))  SectionInfo;
class __attribute__((annotate("$clingAutoload$GradeCluster.h")))  GradeCluster;
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "SectionInfo.h"
#include "GradeCluster.h"
#include "MyFunctions.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"GradeCluster", payloadCode, "@",
"SectionInfo", payloadCode, "@",
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
