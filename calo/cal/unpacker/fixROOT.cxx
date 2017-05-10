/*
 ROOT likes to mess with signal handlers, showing it's pitiable excuse for a stack trace instead of generating a core dump just like unix programs should. 

 Luckily, we can simply hook sigaction to intercept the offending calls. 

 Yes, this is messy. Clean, ROOT, working. Pick two. 
*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <signal.h>

int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact)
{
  if (signum != SIGUSR1 && signum != SIGUSR2)
    return 0;

  //bloody C++ forbids void* to function pointer casts. 
  //thus, we use this double pointer workaround. 
  //now in a C file, still keeping compatibility
  int (** _sigaction)(int, const struct sigaction *,
		      struct sigaction *);
  *((void**)_sigaction)=dlsym(RTLD_NEXT, "sigaction");
  return (*_sigaction)(signum, act, oldact);
}
