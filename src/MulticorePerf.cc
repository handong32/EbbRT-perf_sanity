#include <ebbrt/Debug.h>
#include <ebbrt/Cpu.h>
#include <ebbrt/EventManager.h>
#include <ebbrt/Future.h>
#include <ebbrt/Runtime.h>
#include <ebbrt/Messenger.h>
#include <ebbrt/UniqueIOBuf.h>

#include "MulticorePerf.h"

void perf::Init(size_t num_cpus) {
  auto mcore_perf_root = new MulticorePerfRoot();
  mcore_perf_root->ebb_ = MulticorePerf::Create(mcore_perf_root, MulticorePerf::global_id);

  //kprintf("%s num_cpus=%d\n", __PRETTY_FUNCTION__, num_cpus);
  for (size_t i = 0; i < num_cpus; i++) {
    //kprintf("setup i=%d\n", i);
    mcore_perf_root->Setup(i);
  }
  
  for (size_t i = 0; i < num_cpus; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	perf::mcoreperf->Init();
	p.SetValue();
      },
      i);
    f.Block();
  }
}

void perf::Clear(size_t num_cpus) {
  //kprintf("%s num_cpus=%d\n", __PRETTY_FUNCTION__, num_cpus);
  
  for (size_t i = 0; i < num_cpus; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	perf::mcoreperf->Clear();	
	p.SetValue();
      },
      i);
    f.Block();
  }
}

void perf::Start(size_t num_cpus) {
  //kprintf("%s num_cpus=%d\n", __PRETTY_FUNCTION__, num_cpus);
  
  for (size_t i = 0; i < num_cpus; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	perf::mcoreperf->Start();
	p.SetValue();
      },
      i);
    f.Block();
  }
}

void perf::Stop(size_t num_cpus) {
  //kprintf("%s num_cpus=%d\n", __PRETTY_FUNCTION__, num_cpus);
  
  for (size_t i = 0; i < num_cpus; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	perf::mcoreperf->Stop();
	p.SetValue();
      },
      i);
    f.Block();
  }
}

void perf::Print(size_t num_cpus) {
  //kprintf("%s num_cpus=%d\n", __PRETTY_FUNCTION__, num_cpus);
  
  for (size_t i = 0; i < num_cpus; i++) {
    ebbrt::Promise<void> p;
    auto f = p.GetFuture();
    ebbrt::event_manager->SpawnRemote(
      [&p]() {
	perf::mcoreperf->Print();
	p.SetValue();
      },
      i);
    f.Block();
  }
}

void perf::MulticorePerf::Init() {
  kprintf("%s\n", __PRETTY_FUNCTION__);
  // perfCycles_ = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::cycles);
  // perfInst_ = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::instructions);
  // perfLLC_miss_ = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::llc_misses);

  // if(core_ == 0 || core_ == 1) {
  //   powerMeter_ = ebbrt::rapl::RaplCounter();
  // }
  // time_start_us = time_end_us = 0.0;
}

void perf::MulticorePerf::Start() {
  kprintf("%s\n", __PRETTY_FUNCTION__);
  pc_.perfInst.Start();
}

void perf::MulticorePerf::Stop() {
  kprintf("%s\n", __PRETTY_FUNCTION__);
  pc_.perfInst.Stop();
}

void perf::MulticorePerf::Print() {
  kprintf("%s\n", __PRETTY_FUNCTION__);
  kprintf("Core %u \t Instructions=%llu\n", core_,
	  static_cast<uint64_t>(pc_.perfInst.Read()));
}

/*
void perf::MulticorePerf::Clear() {
  perfCycles_.Clear();
  perfInst_.Clear();
  perfLLC_miss_.Clear();
  
  if(core_ == 0 || core_ == 1) {
    powerMeter_.Clear();
  }
  
  time_start_us = time_end_us = 0.0;
}



void perf::MulticorePerf::Stop() {
  perfCycles_.Stop();
  perfInst_.Stop();
  perfLLC_miss_.Stop();  
  if(core_ == 0 || core_ == 1) {
    powerMeter_.Stop();
  }
  auto d = ebbrt::clock::Wall::Now().time_since_epoch();
  time_end_us = std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

void perf::MulticorePerf::Print() {
  kprintf("Core %u \t Instructions=%llu\n", core_,
	  static_cast<uint64_t>(perfInst_.Read()));
  kprintf(" \t Cycles=%llu\n", 
	  static_cast<uint64_t>(perfCycles_.Read()));
  kprintf(" \t LLC_miss=%llu\n", 
	  static_cast<uint64_t>(perfLLC_miss_.Read()));

  double total_time = ((double)(time_end_us - time_start_us) / 1000000.0);
  kprintf(" \t Time = %.4lf seconds\n", total_time);

  if(core_ == 0 || core_ == 1) { 
    double nrg = powerMeter_.Read();
    kprintf(" \t Energy = %.2fJ\n", nrg);
    kprintf(" \t Watts = %.2fJ\n", nrg/total_time);
  }
  kprintf("\n");
  
}
*/
