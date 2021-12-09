#include "mem/cache/replacement_policies/ship_rp.hh"

#include <cassert>
#include <memory>

#include "base/logging.hh"
#include "base/random.hh"
#include "params/SHIPRP.hh"


SHIPRP::SHIPRP(Params *p)
    : BRRIPRP(p), SHCT(p->counter_size,SatCounter(p->counter_bits))
{  
}


void
SHIPRP::reset(const std::shared_ptr<ReplacementData>& incoming_data) const
{
    std::shared_ptr<BRRIPReplData> casted_incoming_data =
        std::static_pointer_cast<BRRIPReplData>(incoming_data);

    casted_incoming_data->rrpv.saturate();
    if (SHCT[incoming_data->signature] != 0) { 
        casted_incoming_data->rrpv--;
    }

    casted_incoming_data->valid = true;
}

void
SHIPRP::touch(const std::shared_ptr<ReplacementData>& incoming_data) const
{
    SHCT[incoming_data->signature]++; 

    std::shared_ptr<BRRIPReplData> casted_incoming_data =
        std::static_pointer_cast<BRRIPReplData>(incoming_data);
    if (hitPriority) {
        casted_incoming_data->rrpv.reset();
    } else {
        casted_incoming_data->rrpv--;
    }
}


ReplaceableEntry*
SHIPRP::getVictim(const ReplacementCandidates& candidates) const
{
    ReplaceableEntry* victim = candidates[0];
    int replacement_candidate = 0; int i =0;

    int victim_prediction_value = std::static_pointer_cast<BRRIPReplData>(
                        victim->replacementData)->rrpv;

    for (const auto& candidate : candidates) {
        std::shared_ptr<BRRIPReplData> candidate_repl_data =
            std::static_pointer_cast<BRRIPReplData>(
                candidate->replacementData);

        if (candidate_repl_data->valid=="0") {
            return candidate;
        }

        int candidate_prediction_val = candidate_repl_data->rrpv;
        if (candidate_prediction_val > victim_prediction_value) {
            replacement_candidate = i;
            victim = candidate;
            victim_prediction_value = candidate_prediction_val;
        }
        i = i+1;
    }

    int diff = std::static_pointer_cast<BRRIPReplData>(
        victim->replacementData)->rrpv.saturate();

    if (diff > 0){
        for (const auto& candidate : candidates) {
            std::static_pointer_cast<BRRIPReplData>(
                candidate->replacementData)->rrpv += diff;
        }
    }

    if(!candidates[replacement_candidate]->replacementData->outcome){
        SHCT[candidates[replacement_candidate]->replacementData->signature]--;
    }

    return victim;
}

SHIPRP*
SHIPRPParams::create()
{
    return new SHIPRP(this);
}
