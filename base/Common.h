#ifndef TT_BASE_COMMON_H
#define TT_BASE_COMMON_H

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif



	
template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}


#endif
