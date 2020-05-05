#include <unordered_map>
#include <ebbrt/native/Acpi.h>
#include <ebbrt/native/Perf.h>
#include <ebbrt/native/Cpu.h>
#include <ebbrt/native/Clock.h>
#include <ebbrt/EventManager.h>

#include "MulticorePerf.h"
#define NIL_ITERATIONS  1000000

void AppMain() {
  size_t ncpus = ebbrt::Cpu::Count();
  perf::Init(ncpus);
  perf::Start(ncpus);

  
  for (size_t i = 0; i < 2; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	for( int j = 1; j<NIL_ITERATIONS; j++){
	  asm volatile(""); 
	}
	p.SetValue();
      },
      i);
    f.Block();
  }

  /*for (size_t i = 0; i < 2; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
      for( int j = 1; j<NIL_ITERATIONS; j++){
	  asm volatile(""); 
	}
	p.SetValue();
      },
      i);
    f.Block();
    }*/
  
  perf::Stop(ncpus);
  perf::Print(ncpus);
  
  /*perf::Init(ncpus);      
  perf::Start(ncpus);

  perf::Stop(ncpus);
  perf::Print(ncpus);*/
  
}
