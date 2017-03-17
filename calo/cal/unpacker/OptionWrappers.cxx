#include <OptionWrapper.h>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/view/iterator_range.hpp>
#include <boost/fusion/include/iterator_range.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/include/fold.hpp>
#include <iostream>
#include <tuple>
#include <assert.h>

#include <functional>

#include <FairLogger.h>

#include <FairLmdSource.h>
#include <FairRunOnline.h>
#include <R3BCaloUnpack.h>
#include <R3BCaloRawAna.h>
#include <R3BCaloCalibParFinder.h>


#ifdef TIME_DEBUG
#include <x86intrin.h>

auto start=__rdtsc();
inline void debugTime(const char* step)
{
  auto now=__rdtsc();
  std::cerr << step <<": "<< (now-start)/1e6 << "M cycles\n";
  start=now;
}
#else
#define debugTime(...)
#endif

namespace bf=boost::fusion;

//name/varname, type,                   default,     help text            short
ARG(help,     bool,                     0,           "Display this help", "h");
ARG(in,       std::vector<std::string>, {},          "Input files",       "i");
ARG(out,      std::string,              "/dev/null", "Output file",       "o");
ARG(nevents,  int,                      -1,          "Number of Events",  "n");
ARG(loglevel, int,                      4,           "Log level",         "l");


template <class Base, class ... SubWrappers >
class SubOptionWrapper: public Base
{
protected:
  std::tuple<SubWrappers...> w;
public:

  struct {
  template<class T>
  bpo::options_description* operator()(bpo::options_description* mother, T& x)
  {
    auto o=x.createOptions(); 
    if (o) 
      {
	mother->add(*o); 
	delete o;
      }
    return mother;
  }} callCreateOptions;

  struct {
  template<class T>
  bpo::variables_map* operator()( bpo::variables_map* vm, T& x )
  {
    x.setAll(*vm);
    return vm;
  } } callSetAll;

  SubOptionWrapper() : Base() {};
  SubOptionWrapper(std::string name) : Base(name) {};

  bpo::options_description* createOptions()
  {
    auto descr = Base::createOptions();
    //    boost::fusion::for_each(w, [descr](auto& x){auto o=x.createOptions(); if (o) {descr->add(*o); delete o;}});
    boost::fusion::fold(w, descr, callCreateOptions);
    return descr;
  }
  void setAll(bpo::variables_map& vm)
  {
    Base::setAll(vm);
    //boost::fusion::for_each(w, [&vm](auto& x){x.setAll(vm);});
    boost::fusion::fold(w, &vm, callSetAll);
  }
};


template <class wrapped, class... stuff>
struct Wrapper {};

struct Global 
{
  int ret;
};

template <class Runable, class... Wrappers >
class Main: public SubOptionWrapper<OptionWrapper<Global, arg_help, arg_nevents, arg_loglevel>, Runable, Wrappers...>
{
protected:
  const bpo::options_description *descr;
public:
  Main(): descr(nullptr){};
  //virtual
  Global* create()
  {
    assert(descr);
    if (this->help)
      {
	std::cout << *descr << "\n";
	Global* ret=new Global;
	ret->ret=1;
	return ret;
      }
    this->loglevel=std::min((int)(sizeof(LogLevelString)/sizeof(LogLevelString[0]))-1, std::max(this->loglevel, 0)),
    FairLogger::GetLogger()->SetLogScreenLevel(LogLevelString[this->loglevel]);
    return nullptr;
  }

  int main(int argc, char** argv)
  {
    debugTime("created main object.");
    this->descr=this->createOptions();
    debugTime("created options.");
    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, *this->descr), vm);
    debugTime("parsed options.");
    this->setAll(vm);
    debugTime("set options.");
    auto ret=create();
    if (!ret)
    // Wrappers[0] better be runable.
      std::get<0>(this->w).create()->Run(this->nevents, 0);
    debugTime("ran program");
    return ret?ret->ret:0;
  }
};

template<class source, class ... tasks>
class Wrapper<FairRunOnline, source, tasks...> 
  : public SubOptionWrapper<OptionWrapper<FairRunOnline, arg_out > ,
			    source, tasks... >
{
  using wrapped_t= FairRunOnline; //why can't you figure this out?
  wrapped_t* fro;
  // with polymorphic lambdas (gcc 4.9), this would not be neccesary. 
  struct add_task
  {
    wrapped_t *fro;
    add_task(wrapped_t* fro): fro(fro) {};
    template<class T>
    void operator()(T t) const
    {
      using wrapped_t=typename T::wrapped_t;
      LOG(INFO) << "Adding Task " << getName<wrapped_t>() << FairLogger::endl;
  
      this->fro->AddTask(t.create());
    }
  };
public:
  wrapped_t* create()
  {
    auto s=std::get<0>(this->w).create();
    this->fro=new wrapped_t(s);
    this->fro->SetOutputFile(this->out.c_str()); 
    auto start=bf::advance< boost::mpl::int_<1> > (bf::begin(this->w));
    auto end  =bf::end(this->w);
    bf::for_each(bf::iterator_range<decltype(start), decltype(end)>(start, end), add_task(this->fro));
    this->fro->Init();
    return this->fro;
  }
};

template <class ... unpackers>
class Wrapper<FairLmdSource, unpackers...> 
  : public SubOptionWrapper<OptionWrapper<FairLmdSource, arg_in>, unpackers...>
{
  typedef FairLmdSource wrapped_t;
  wrapped_t* source;
public:
  //virtual
  wrapped_t* create()
  {
    source=new wrapped_t();
    boost::fusion::for_each(this->w, *this);
    if (this->in.empty())
      {
	// we don't want a coredump, so let's just exit, 
	//  not throw stuff or LOG(FATAL).
	std::cout <<"FairLmdSource will segfault without input files,"
	  " please give at least one.\n";
	exit(-1);
      }
    for(const auto i: this->in)
      source->AddFile(i.c_str());
    return source;
  }

private: // \begin{hack}
  template <class T>
  void operator()(T t) const
  {
    using wrapped_t=typename T::wrapped_t;

    LOG(INFO) << "Adding unpacker " << getName<wrapped_t>() << FairLogger::endl;
    this->source->AddUnpacker(t.create());
  }
  template<int N> friend struct boost::fusion::detail::for_each_unrolled;
};

// wrapper for a class without cli options which needs constructor vars
char* R3BCaloUnpack_str=const_cast<char*>("R3BCaloUnpack");
template <>
struct Wrapper<R3BCaloUnpack> : public 
 SIMPLEWRAP(R3BCaloUnpack, R3BCaloUnpack_str, 100, 10000, 2, -1, 9) {};

// same, but default constructable
template <>
struct Wrapper<R3BCaloRawAna>: public TrivialWrap<R3BCaloRawAna> {};


template <>
struct Wrapper<R3BCaloCalibParFinder>: public TrivialWrap<R3BCaloCalibParFinder> {};


template<class, class Enable=void>
class RecursiveWrap;

template<class WrappedType, class... WrapperArgs>
struct RecursiveWrap<WrappedType(WrapperArgs...)> 
  : public Wrapper<WrappedType, RecursiveWrap<WrapperArgs>... > {};

template<class WrappedType, class... WrapperArgs>
struct RecursiveWrap<WrappedType(*)(WrapperArgs...)> 
  : public Wrapper<WrappedType, RecursiveWrap<WrapperArgs>... > 
{using wrapped_t=WrappedType;};

template<class T>
struct RecursiveWrap<T, typename std::enable_if<std::is_class<T>::value>::type>
  : public Wrapper<T>
{using wrapped_t=T;};

#ifndef MAIN
#error "Please define MAIN to the classes you want to compile, e.g. -DMAIN=FairRunOnline(FairLmdSource(R3BCaloUnpack),R3BCaloRawAna)"
#endif
//using M=Main<RecursiveWrap<FairRunOnline(FairLmdSource(R3BCaloUnpack),R3BCaloRawAna)> >;
using M=Main<RecursiveWrap<MAIN> >;

int main(int argc, char** argv)
{
  return (new M)->main(argc, argv);
}
