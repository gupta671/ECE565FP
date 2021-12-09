#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_SHIP_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_SHIP_RP_HH__

#include "base/sat_counter.hh"
#include "mem/cache/replacement_policies/brrip_rp.hh"

struct SHIPRPParams;
class SHIPRP : public BRRIPRP
{

  public:
    mutable std::vector<SatCounter> SHCT;
    typedef SHIPRPParams Params;
    SHIPRP(Params *p);

    ~SHIPRP() {}

    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const override;

    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const override;

    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const override;

};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_SHIP_RP_HH__
