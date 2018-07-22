#ifndef QTUMX86_H
#define QTUMX86_H

#include "qtumstate.h"
#include "qtumtransaction.h"

#include <x86lib.h>

class ContractVM;

class x86ContractVM : public ContractVM{
public:
    x86ContractVM(DeltaDBWrapper &db, const ContractEnvironment &env, uint64_t remainingGasLimit)
            : ContractVM(db, env, remainingGasLimit)
    {}
    virtual bool execute(ContractOutput &output, ContractExecutionResult &result, bool commit);
private:
    const ContractEnvironment &getEnv();
    const std::vector<uint8_t> buildAdditionalData(ContractOutput &output);

    friend class QtumHypervisor;
};

class QtumHypervisor : public x86Lib::InterruptHypervisor{
    QtumHypervisor(x86ContractVM &vm, const ContractOutput& out, DeltaDBWrapper& db_) : contractVM(vm), output(out), db(db_){
    }
    virtual void HandleInt(int number, x86Lib::x86CPU &vm);
private:
    x86ContractVM &contractVM;
    ContractOutput output;
    DeltaDBWrapper &db;

    friend x86ContractVM;
};


static const int QTUM_SYSTEM_ERROR_INT = 0xFF;

//interrupt 0x40
enum QtumSystemCall{
    PreviousBlockTime = 0,
    BlockGasLimit = 1,
    BlockCreator = 2,
    BlockDifficulty = 3,
    BlockHeight = 4,
    GetBlockHash = 5,
    IsCreate = 6,


    SelfAddress = 7,
    ReadStorage = 8,
    WriteStorage = 9
};

enum QtumEndpoint{
    QtumSystem = 0x40,
    QtumTrustedLibrary = 0x41,
    InteralUI = 0x50
};


#endif
