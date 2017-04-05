#include <vector>
#include <string>
#include <boost/program_options.hpp>
namespace bpo=boost::program_options;

template<class T>
const std::basic_string<char> getName()
{
  std::string s=std::string(__PRETTY_FUNCTION__);
  int start=s.find("T =");
  int end=s.find("]");
  if (start!=std::string::npos && end!=std::string::npos)
    {
      start+=4; //"T = "
      return s.substr(start, end-start);
    }
  else
    {
      return "unknown type";
    }
}

template <class T>
class OptionBase
{
protected:
  typedef T base_t;
  typedef decltype(  bpo::value<T>() ) BPOV_t;
  BPOV_t bpov;
  template <class E>
  void set_default (std::vector<E> def)
  {
    this->bpov->default_value(def, "");
  }

  void set_default (bool def)
  {
    this->bpov->default_value(def);
    this->bpov->implicit_value(!def);
  }

  template <class T2>
  void set_default (T2 def)
  {
    this->bpov->default_value(def);
  }


  OptionBase(base_t defaultval)
  {
    this->bpov=boost::program_options::value<T>();
    this->set_default(defaultval);
  }
  //virtual BPO_t createBPOValue();
//  virtual void set(boost::program_options::variables_map);
};



#define ARG(NAME, TYPE, DEFAULT, DESCRIPTION, SHORTCUT)			\
class arg_## NAME: public OptionBase<TYPE >				\
{									\
protected:								\
 arg_## NAME (): OptionBase<TYPE>(DEFAULT) {};				\
 TYPE NAME;								\
public:  								\
 inline void set(bpo::variables_map& vm)				\
 {									\
  this->NAME=vm[#NAME].as<TYPE>();					\
 }									\
 inline void addOption(bpo::options_description& descr)			\
 {									\
  descr.add_options()((std::string(#NAME)+(strlen(SHORTCUT)?std::string(",")\
					   +SHORTCUT:"")).c_str(),	\
		      this->bpov, DESCRIPTION);				\
 };									\
};  


// special case: no options
struct OptionWrapperBase
{
  bpo::options_description* createOptions() { return nullptr; };
  void setAll(bpo::variables_map& vm) {};
};

template <class T, class... Args>
  class OptionWrapper: public Args...
{
 protected:
  std::string optname;
 public:
  typedef T wrapped_t;
  OptionWrapper(): optname(getName<T>()+" Options" ) {};
  OptionWrapper(const std::string optname): optname(optname) {};
 void setAll(bpo::variables_map& vm)
  {
    setAllRec(vm, static_cast<Args*>(this)...);
  }
 

 bpo::options_description* createOptions()
 {
   auto descr= new bpo::options_description(optname);
   return createOptionsRec(descr, static_cast<Args*>(this)...);
 }

 //create and run, the wrapped class, possibly feeding it MoreWrappers
 // template<class... MoreWrappers>
 
 protected: //template recursions /////////////////////////////////////////

 void setAllRec(bpo::variables_map& vm)
 {
   return;
 }

 template<class Arg1, class... RestArgs>
   void setAllRec(bpo::variables_map& vm, Arg1 a1, RestArgs... rest)
 {
   a1->set(vm);
   this->setAllRec(vm, rest...);
   return;
 }
 
 bpo::options_description* createOptionsRec(bpo::options_description* descr)
 {
   return descr;
 }
 template<class Arg1, class... RestArgs>
   bpo::options_description* createOptionsRec(bpo::options_description* descr,
					      Arg1 a1, RestArgs... rest)
 {
   a1->addOption(*descr);
   return createOptionsRec(descr, rest...);
 }

};


template<class T, class ...Args>
  T* callConstructor(Args... args)
{
  return new T(args...);
}

 template<class T, class... Args>
struct SimpleWrapOuter
{
  template<Args... args> 
  struct SimpleWrap: public OptionWrapperBase
  {
    const std::function<T*() > create;
    SimpleWrap(): create(std::bind(callConstructor<T, Args...>, args...)) {}
  };
};

  //using TrivialWrap=SimpleWrapOuter<T>::SimpleWrap<>;
template<class T>
using TrivialWrap = typename SimpleWrapOuter<T>::template SimpleWrap<>;
#define BOOST_PP_VARIADICS 1
#include <boost/preprocessor/config/config.hpp>
//#undef BOOST_PP_VARIADICS

#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/preprocessor/list/transform.hpp>
#include <boost/preprocessor/list/to_tuple.hpp> 
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <type_traits>

template<class T, class Enable=void>
struct capture_t;

template<class T>
  struct capture_t<T*>
{
  using t=T*&;
};

template<class T> 
struct capture_t< T, typename std::enable_if<std::is_integral<T>::value>::type >
{
  using t=T;
};

/*#if 0
 template<>
 struct capture_t<int>
 {
   using t=int;
 };
 #endif*/
template struct capture_t<int>;

#define MAKETYPE_ELEM(r, data, elem) capture_t<decltype(elem)>::t
#define MAKETYPE(...)  BOOST_PP_TUPLE_ENUM(BOOST_PP_LIST_TO_TUPLE(BOOST_PP_LIST_TRANSFORM(MAKETYPE_ELEM, _ , BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) )))
#define SIMPLEWRAP(T, ...) SimpleWrapOuter<T, MAKETYPE(__VA_ARGS__) >::SimpleWrap<__VA_ARGS__>
