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

#include "mem/cache/replacement_policies/dip_rp.hh"
#include <string>
#include <memory>
#include "debug/CacheRepl.hh"
#include "base/random.hh"
#include "params/DIPRP.hh"  

DIPRP::DIPRP(Params *p)
    : BIPRP(p), num_sd_sets(p->num_sd_sets),psel_bits(p->psel_bits)
{
    PSEL = pow(2, p->psel_bits-1);
}

void
DIPRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    int setNum = replacement_data->setnumber;
    std::shared_ptr<LRUReplData> casted_replacement_data =
        std::static_pointer_cast<LRUReplData>(replacement_data);
    
    uint32_t constituency = floor(setNum/num_sd_sets);
    uint32_t offset = setNum % num_sd_sets; 
    
    bool LRUSetPresent = constituency == offset;
    bool BIPSetPresent = (constituency+1)*(num_sd_sets-1) == setNum;

    if (LRUSetPresent){
        PSEL++;
        if (PSEL > pow(2,psel_bits)){PSEL--;}
        casted_replacement_data->lastTouchTick = 1;
        
    } else if(BIPSetPresent){
	if (random_mt.random<unsigned>(1, 100) <= btp) {
        casted_replacement_data->lastTouchTick = curTick();
    } else {
        // Make their timestamps as old as possible, so that they become LRU
        casted_replacement_data->lastTouchTick = 1;
    }
        PSEL--;
        if (PSEL < 0){PSEL++;}
        
        

    } else {
        if (PSEL > pow(2, psel_bits-1)) { //BIP Has more hits
            if (random_mt.random<unsigned>(1, 100) <= btp) {
        casted_replacement_data->lastTouchTick = curTick();
    } else {
        // Make their timestamps as old as possible, so that they become LRU
        casted_replacement_data->lastTouchTick = 1;
    }
        }
        else {
        casted_replacement_data->lastTouchTick = 1;            
        }
    }
    // printf("PSEL: %d \n",PSEL);
}


DIPRP*
DIPRPParams::create()
{
    return new DIPRP(this);
}
