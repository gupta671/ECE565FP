/**
 * Copyright (c) 2018 Inria
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * Declaration of a Bimodal Interval Prediction replacement policy.
 * Has a probability of when placing new entries, placing them as MRU.
 *
 * Although both LRU and LIP can be seen as specific cases of BIP
 * where the bimodal throtle parameter are 1 and 0, respectively, we
 * decide not to inherit from it, and do the other way around (inherit
 * from LRU) for efficiency reasons.
 *
 * In the original paper they use btp = 1/32 ~= 3%.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__

#include "mem/cache/replacement_policies/bip_rp.hh"

struct DIPRPParams;

class DIPRP : public BIPRP
{
  protected:
    /**
     * Set Dueling counter used to keep track of which replacement
     * policy is performing better on the benchmark. 
     * increment PSEL if a hit on set following SRRIP policy,
     * decrement PSEL if a hit on set following BRRIP policy. 
     * Based on PSEL > some constant, decide if the rest of the sets
     * follow BRRIP or SRRIP.
     */
    unsigned num_sd_sets;
    mutable int PSEL;
    unsigned psel_bits;
    
  
  public:
    /** Convenience typedef. */
    typedef DIPRPParams Params;
    /**
     * Construct and initiliaze this replacement policy.
     */
    DIPRP(Params *p);

    /**
     * Destructor.
     */
    ~DIPRP() {}

    /**
     * Reset replacement data for an entry. Used when an entry is inserted.
     * Uses the bimodal throtle parameter to decide whether the new entry
     * should be inserted as MRU, or LRU.
     *
     * @param replacement_data Replacement data to be reset.
     */

    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    // void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
    //                                                                  override;



};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__
