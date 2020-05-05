#ifndef MULTICORE_PERF_H
#define MULTICORE_PERF_H

#include <ebbrt/Clock.h>
#include <ebbrt/Debug.h>
#include <ebbrt/Future.h>
#include <ebbrt/MulticoreEbb.h>
#include <ebbrt/SpinLock.h>
#include <ebbrt/UniqueIOBuf.h>
#include <ebbrt/native/Multiboot.h>
#include <ebbrt/native/Perf.h>
#include <ebbrt/native/Rapl.h>
#include <ebbrt/native/Msr.h>

#define kprintf ebbrt::kprintf_force

namespace perf {
  void Init(size_t);
  void Clear(size_t);
  void Start(size_t);
  void Stop(size_t);
  void Print(size_t);
  
  class MulticorePerf;
  
  class MulticorePerfRoot {
  public:
    class pc {      
    public:
    pc(uint32_t idx) : idx_(idx) {}
      
      uint32_t idx_;
      ebbrt::perf::PerfCounter perfCycles;
      ebbrt::perf::PerfCounter perfInst;
      ebbrt::perf::PerfCounter perfLLC_ref;
      ebbrt::perf::PerfCounter perfLLC_miss;
      ebbrt::perf::PerfCounter perfTLB_store_miss;
      ebbrt::perf::PerfCounter perfTLB_load_miss;
      ebbrt::rapl::RaplCounter powerMeter;    
    };

	
    MulticorePerfRoot() {
      pcs.resize(ebbrt::Cpu::Count());
      //kprintf("MulticorePerfRoot() pcs.size == %d\n", pcs.size());
    }

    void Setup(size_t i) {
      pcs[i].reset(new pc(static_cast<uint32_t>(i)));
    }
    
    ebbrt::EbbRef<MulticorePerf> ebb_;
        
  private:
    pc& GetPC(uint32_t index) const { return *pcs[index]; }
    std::vector<std::unique_ptr<pc>> pcs;
    
    friend class MulticorePerf;
  }; // end class MulticorePerfRoot
  

  class MulticorePerf : public ebbrt::MulticoreEbb<MulticorePerf, MulticorePerfRoot> {
  public:
    static const ebbrt::EbbId global_id = ebbrt::GenerateStaticEbbId("MulticorePerf");

    explicit MulticorePerf(const MulticorePerfRoot &root)
      : root_(const_cast<MulticorePerfRoot &>(root)), core_(ebbrt::Cpu::GetMine()),
      pc_(root.GetPC(ebbrt::Cpu::GetMine())) {
      
      pc_.perfCycles = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::cycles);
      pc_.perfInst = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::instructions);
      pc_.perfLLC_miss = ebbrt::perf::PerfCounter(ebbrt::perf::PerfEvent::llc_misses);

      
      //kprintf("MulticorePerf() core=%d\n", core_);
    }

    void Init();
    void Start();
    void Stop();
    void Print();
    void Clear();
    
  private:
    MulticorePerfRoot &root_;
    size_t core_;
    MulticorePerfRoot::pc& pc_;
  }; // end class MulticorePerf

  constexpr auto mcoreperf = ebbrt::EbbRef<MulticorePerf>(MulticorePerf::global_id);
  
} //namespace perf

#endif
