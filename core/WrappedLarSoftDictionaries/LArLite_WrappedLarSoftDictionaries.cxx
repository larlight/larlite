// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME LArLite_WrappedLarSoftDictionaries

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
#include "classes.h"
#include "dummyWrappedLarSoftDictionaries.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_Dictionary();
   static void larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_TClassManip(TClass*);
   static void *new_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p = 0);
   static void *newArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(Long_t size, void *p);
   static void delete_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p);
   static void deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p);
   static void destruct_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::larlite::wrapper<vector<recob::Hit> >*)
   {
      ::larlite::wrapper<vector<recob::Hit> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::larlite::wrapper<vector<recob::Hit> >));
      static ::ROOT::TGenericClassInfo 
         instance("larlite::wrapper<vector<recob::Hit> >", "DataFormat/wrapper.h", 47,
                  typeid(::larlite::wrapper<vector<recob::Hit> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::larlite::wrapper<vector<recob::Hit> >) );
      instance.SetNew(&new_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR);
      instance.SetNewArray(&newArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR);
      instance.SetDelete(&delete_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR);
      instance.SetDeleteArray(&deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR);
      instance.SetDestructor(&destruct_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR);

      ::ROOT::AddClassAlternate("larlite::wrapper<vector<recob::Hit> >","larlite::wrapper<std::vector<recob::Hit> >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::larlite::wrapper<vector<recob::Hit> >*)
   {
      return GenerateInitInstanceLocal((::larlite::wrapper<vector<recob::Hit> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::larlite::wrapper<vector<recob::Hit> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::larlite::wrapper<vector<recob::Hit> >*)0x0)->GetClass();
      larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_Dictionary();
   static void larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_TClassManip(TClass*);
   static void *new_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p = 0);
   static void *newArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(Long_t size, void *p);
   static void delete_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p);
   static void deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p);
   static void destruct_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::larlite::wrapper<vector<recob::Cluster> >*)
   {
      ::larlite::wrapper<vector<recob::Cluster> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::larlite::wrapper<vector<recob::Cluster> >));
      static ::ROOT::TGenericClassInfo 
         instance("larlite::wrapper<vector<recob::Cluster> >", "DataFormat/wrapper.h", 47,
                  typeid(::larlite::wrapper<vector<recob::Cluster> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::larlite::wrapper<vector<recob::Cluster> >) );
      instance.SetNew(&new_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR);
      instance.SetNewArray(&newArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR);
      instance.SetDelete(&delete_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR);
      instance.SetDeleteArray(&deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR);
      instance.SetDestructor(&destruct_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR);

      ::ROOT::AddClassAlternate("larlite::wrapper<vector<recob::Cluster> >","larlite::wrapper<std::vector<recob::Cluster> >");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::larlite::wrapper<vector<recob::Cluster> >*)
   {
      return GenerateInitInstanceLocal((::larlite::wrapper<vector<recob::Cluster> >*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::larlite::wrapper<vector<recob::Cluster> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::larlite::wrapper<vector<recob::Cluster> >*)0x0)->GetClass();
      larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::larlite::wrapper<vector<recob::Hit> > : new ::larlite::wrapper<vector<recob::Hit> >;
   }
   static void *newArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::larlite::wrapper<vector<recob::Hit> >[nElements] : new ::larlite::wrapper<vector<recob::Hit> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p) {
      delete ((::larlite::wrapper<vector<recob::Hit> >*)p);
   }
   static void deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p) {
      delete [] ((::larlite::wrapper<vector<recob::Hit> >*)p);
   }
   static void destruct_larlitecLcLwrapperlEvectorlErecobcLcLHitgRsPgR(void *p) {
      typedef ::larlite::wrapper<vector<recob::Hit> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::larlite::wrapper<vector<recob::Hit> >

namespace ROOT {
   // Wrappers around operator new
   static void *new_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::larlite::wrapper<vector<recob::Cluster> > : new ::larlite::wrapper<vector<recob::Cluster> >;
   }
   static void *newArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::larlite::wrapper<vector<recob::Cluster> >[nElements] : new ::larlite::wrapper<vector<recob::Cluster> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p) {
      delete ((::larlite::wrapper<vector<recob::Cluster> >*)p);
   }
   static void deleteArray_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p) {
      delete [] ((::larlite::wrapper<vector<recob::Cluster> >*)p);
   }
   static void destruct_larlitecLcLwrapperlEvectorlErecobcLcLClustergRsPgR(void *p) {
      typedef ::larlite::wrapper<vector<recob::Cluster> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::larlite::wrapper<vector<recob::Cluster> >

namespace ROOT {
   static TClass *vectorlErecobcLcLHitgR_Dictionary();
   static void vectorlErecobcLcLHitgR_TClassManip(TClass*);
   static void *new_vectorlErecobcLcLHitgR(void *p = 0);
   static void *newArray_vectorlErecobcLcLHitgR(Long_t size, void *p);
   static void delete_vectorlErecobcLcLHitgR(void *p);
   static void deleteArray_vectorlErecobcLcLHitgR(void *p);
   static void destruct_vectorlErecobcLcLHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<recob::Hit>*)
   {
      vector<recob::Hit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<recob::Hit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<recob::Hit>", -2, "vector", 214,
                  typeid(vector<recob::Hit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlErecobcLcLHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<recob::Hit>) );
      instance.SetNew(&new_vectorlErecobcLcLHitgR);
      instance.SetNewArray(&newArray_vectorlErecobcLcLHitgR);
      instance.SetDelete(&delete_vectorlErecobcLcLHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlErecobcLcLHitgR);
      instance.SetDestructor(&destruct_vectorlErecobcLcLHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<recob::Hit> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<recob::Hit>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlErecobcLcLHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<recob::Hit>*)0x0)->GetClass();
      vectorlErecobcLcLHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlErecobcLcLHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlErecobcLcLHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<recob::Hit> : new vector<recob::Hit>;
   }
   static void *newArray_vectorlErecobcLcLHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<recob::Hit>[nElements] : new vector<recob::Hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlErecobcLcLHitgR(void *p) {
      delete ((vector<recob::Hit>*)p);
   }
   static void deleteArray_vectorlErecobcLcLHitgR(void *p) {
      delete [] ((vector<recob::Hit>*)p);
   }
   static void destruct_vectorlErecobcLcLHitgR(void *p) {
      typedef vector<recob::Hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<recob::Hit>

namespace ROOT {
   static TClass *vectorlErecobcLcLClustergR_Dictionary();
   static void vectorlErecobcLcLClustergR_TClassManip(TClass*);
   static void *new_vectorlErecobcLcLClustergR(void *p = 0);
   static void *newArray_vectorlErecobcLcLClustergR(Long_t size, void *p);
   static void delete_vectorlErecobcLcLClustergR(void *p);
   static void deleteArray_vectorlErecobcLcLClustergR(void *p);
   static void destruct_vectorlErecobcLcLClustergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<recob::Cluster>*)
   {
      vector<recob::Cluster> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<recob::Cluster>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<recob::Cluster>", -2, "vector", 214,
                  typeid(vector<recob::Cluster>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlErecobcLcLClustergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<recob::Cluster>) );
      instance.SetNew(&new_vectorlErecobcLcLClustergR);
      instance.SetNewArray(&newArray_vectorlErecobcLcLClustergR);
      instance.SetDelete(&delete_vectorlErecobcLcLClustergR);
      instance.SetDeleteArray(&deleteArray_vectorlErecobcLcLClustergR);
      instance.SetDestructor(&destruct_vectorlErecobcLcLClustergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<recob::Cluster> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<recob::Cluster>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlErecobcLcLClustergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<recob::Cluster>*)0x0)->GetClass();
      vectorlErecobcLcLClustergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlErecobcLcLClustergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlErecobcLcLClustergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<recob::Cluster> : new vector<recob::Cluster>;
   }
   static void *newArray_vectorlErecobcLcLClustergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<recob::Cluster>[nElements] : new vector<recob::Cluster>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlErecobcLcLClustergR(void *p) {
      delete ((vector<recob::Cluster>*)p);
   }
   static void deleteArray_vectorlErecobcLcLClustergR(void *p) {
      delete [] ((vector<recob::Cluster>*)p);
   }
   static void destruct_vectorlErecobcLcLClustergR(void *p) {
      typedef vector<recob::Cluster> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<recob::Cluster>

namespace {
  void TriggerDictionaryInitialization_libLArLite_WrappedLarSoftDictionaries_Impl() {
    static const char* headers[] = {
"classes.h",
"dummyWrappedLarSoftDictionaries.h",
0
    };
    static const char* includePaths[] = {
"/home/lsimons/scratch/LArLite_LArSoft_Int/larlite/core",
"/products/lardataobj/v1_00_00/include",
"/products/larcoreobj/v1_00_00/include",
"/products/root/v6_06_02/Linux64bit+2.6-2.12-e9-prof/include",
"/scratch/lsimons/LArLite_LArSoft_Int/larlite/core/WrappedLarSoftDictionaries/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libLArLite_WrappedLarSoftDictionaries dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace recob{class __attribute__((annotate("$clingAutoload$classes.h")))  Hit;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace recob{class __attribute__((annotate("$clingAutoload$classes.h")))  Cluster;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libLArLite_WrappedLarSoftDictionaries dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "classes.h"
#include "dummyWrappedLarSoftDictionaries.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"larlite::wrapper<std::vector<recob::Cluster> >", payloadCode, "@",
"larlite::wrapper<std::vector<recob::Hit> >", payloadCode, "@",
"larlite::wrapper<vector<recob::Cluster> >", payloadCode, "@",
"larlite::wrapper<vector<recob::Hit> >", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libLArLite_WrappedLarSoftDictionaries",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libLArLite_WrappedLarSoftDictionaries_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libLArLite_WrappedLarSoftDictionaries_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libLArLite_WrappedLarSoftDictionaries() {
  TriggerDictionaryInitialization_libLArLite_WrappedLarSoftDictionaries_Impl();
}
