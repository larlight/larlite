// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SuttonAna_EventViewerDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "sample.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *sample_Dictionary();
   static void sample_TClassManip(TClass*);
   static void *new_sample(void *p = 0);
   static void *newArray_sample(Long_t size, void *p);
   static void delete_sample(void *p);
   static void deleteArray_sample(void *p);
   static void destruct_sample(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::sample*)
   {
      ::sample *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::sample));
      static ::ROOT::TGenericClassInfo 
         instance("sample", "sample.h", 22,
                  typeid(::sample), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &sample_Dictionary, isa_proxy, 4,
                  sizeof(::sample) );
      instance.SetNew(&new_sample);
      instance.SetNewArray(&newArray_sample);
      instance.SetDelete(&delete_sample);
      instance.SetDeleteArray(&deleteArray_sample);
      instance.SetDestructor(&destruct_sample);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::sample*)
   {
      return GenerateInitInstanceLocal((::sample*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::sample*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *sample_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::sample*)0x0)->GetClass();
      sample_TClassManip(theClass);
   return theClass;
   }

   static void sample_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_sample(void *p) {
      return  p ? new(p) ::sample : new ::sample;
   }
   static void *newArray_sample(Long_t nElements, void *p) {
      return p ? new(p) ::sample[nElements] : new ::sample[nElements];
   }
   // Wrapper around operator delete
   static void delete_sample(void *p) {
      delete ((::sample*)p);
   }
   static void deleteArray_sample(void *p) {
      delete [] ((::sample*)p);
   }
   static void destruct_sample(void *p) {
      typedef ::sample current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::sample

namespace {
  void TriggerDictionaryInitialization_libSuttonAna_EventViewer_Impl() {
    static const char* headers[] = {
"sample.h",
0
    };
    static const char* includePaths[] = {
"/a/apps/local/root-6.08.06/include",
"/a/home/houston/ksutton/MyLArLite/UserDev/SuttonAna/EventViewer/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSuttonAna_EventViewer dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$sample.h")))  sample;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSuttonAna_EventViewer dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "sample.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"sample", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSuttonAna_EventViewer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSuttonAna_EventViewer_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSuttonAna_EventViewer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSuttonAna_EventViewer() {
  TriggerDictionaryInitialization_libSuttonAna_EventViewer_Impl();
}
