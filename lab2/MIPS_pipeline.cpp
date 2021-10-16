#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<bitset>

using namespace std;

#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

struct IFStruct {
    bitset<32>  PC;
    bool        nop;
};

struct IDStruct {
    bitset<32>  Instr;
    bool        nop;
};

struct EXStruct {
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem;
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu
    bool        wrt_enable;
    bool        nop;
};

struct MEMStruct {
    bitset<32>  ALUresult;
    bitset<32>  Store_data;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem;
    bool        wrt_enable;
    bool        nop;
};

struct WBStruct {
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop;
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};

class RF {
private:
    vector<bitset<32> > Registers;
public: 
    bitset<32> Reg_data;
    RF() {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }

    bitset<32> readRF(bitset<5> Reg_addr) {
        Reg_data = Registers[Reg_addr.to_ulong()];
        return Reg_data;
    }

    void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data) {
        Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
    }

    void outputRF() {
        ofstream rfout;
        rfout.open("RFresult.txt", std::ios_base::app);
        if (rfout.is_open()) {
            rfout << "State of RF:\t" << endl;
            for (int j = 0; j < 32; j ++) {
                rfout << Registers[j] << endl;
            }
        }
        else cout << "Unable to open file";
        rfout.close();
    }
};

class INSMem {  
private:
    vector<bitset<8> > IMem;
public:
    bitset<32> Instruction;
    INSMem() {
        IMem.resize(MemSize);
        ifstream imem;
        string line;
        int i = 0;
        imem.open("imem.txt");
        if (imem.is_open()) {
            while (getline(imem, line)) {
                IMem[i] = bitset<8> (line.substr(0, 8));
                i ++;
            }
        }
        else cout << "Unable to open file";
        imem.close();
	}
              
    bitset<32> readInstr(bitset<32> ReadAddress) {
        string insmem;
        insmem.append(IMem[ReadAddress.to_ulong()].to_string());
        insmem.append(IMem[ReadAddress.to_ulong()+1].to_string());
        insmem.append(IMem[ReadAddress.to_ulong()+2].to_string());
        insmem.append(IMem[ReadAddress.to_ulong()+3].to_string());
        Instruction = bitset<32> (insmem); //read instruction memory
        return Instruction;
	}
};

class DataMem {
private:
    vector<bitset<8> > DMem;
public:
    bitset<32> ReadData;  
    DataMem()
    {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i = 0;
        dmem.open("dmem.txt");
        if (dmem.is_open()) {
            while (getline(dmem, line)) {
                DMem[i] = bitset<8> (line.substr(0, 8));
                i++;
            }
        }
        else cout << "Unable to open file";
        dmem.close();
    }

    bitset<32> readDataMem(bitset<32> Address) {
        string datamem;
        datamem.append(DMem[Address.to_ulong()].to_string());
        datamem.append(DMem[Address.to_ulong()+1].to_string());
        datamem.append(DMem[Address.to_ulong()+2].to_string());
        datamem.append(DMem[Address.to_ulong()+3].to_string());
        ReadData = bitset<32>(datamem); //read data memory
        return ReadData;
    }

    void writeDataMem(bitset<32> Address, bitset<32> WriteData) {
        DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0,8));
        DMem[Address.to_ulong()+1] = bitset<8>(WriteData.to_string().substr(8,8));
        DMem[Address.to_ulong()+2] = bitset<8>(WriteData.to_string().substr(16,8));
        DMem[Address.to_ulong()+3] = bitset<8>(WriteData.to_string().substr(24,8));
    }

    void outputDataMem() {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open()) {
            for (int j = 0; j < 1000; j ++) {
                dmemout << DMem[j] << endl;
            }     
        }
        else cout << "Unable to open file";
        dmemout.close();
    }
};

class ALU {
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation(bool ALUOP, bitset<32> oprand1, bitset<32> oprand2) {
        if (ALUOP == true)
            ALUresult = bitset<32> (oprand1.to_ulong() + oprand2.to_ulong());
        else
            ALUresult = bitset<32> (oprand1.to_ulong() - oprand2.to_ulong());
        return ALUresult;
    }
};

void printState(stateStruct state, int cycle) {

    ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    
    if (printstate.is_open()) {
        printstate<<"State after executing cycle:\t"<<cycle<<endl;

        printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;
        printstate<<"IF.nop:\t"<<state.IF.nop<<endl;

        printstate<<"ID.Instr:\t"<<state.ID.Instr<<endl;
        printstate<<"ID.nop:\t"<<state.ID.nop<<endl;

        printstate<<"EX.Read_data1:\t"<<state.EX.Read_data1<<endl;
        printstate<<"EX.Read_data2:\t"<<state.EX.Read_data2<<endl;
        printstate<<"EX.Imm:\t"<<state.EX.Imm<<endl;
        printstate<<"EX.Rs:\t"<<state.EX.Rs<<endl;
        printstate<<"EX.Rt:\t"<<state.EX.Rt<<endl;
        printstate<<"EX.Wrt_reg_addr:\t"<<state.EX.Wrt_reg_addr<<endl;
        printstate<<"EX.is_I_type:\t"<<state.EX.is_I_type<<endl;
        printstate<<"EX.rd_mem:\t"<<state.EX.rd_mem<<endl;
        printstate<<"EX.wrt_mem:\t"<<state.EX.wrt_mem<<endl;
        printstate<<"EX.alu_op:\t"<<state.EX.alu_op<<endl;
        printstate<<"EX.wrt_enable:\t"<<state.EX.wrt_enable<<endl;
        printstate<<"EX.nop:\t"<<state.EX.nop<<endl;

        printstate<<"MEM.ALUresult:\t"<<state.MEM.ALUresult<<endl;
        printstate<<"MEM.Store_data:\t"<<state.MEM.Store_data<<endl;
        printstate<<"MEM.Rs:\t"<<state.MEM.Rs<<endl;
        printstate<<"MEM.Rt:\t"<<state.MEM.Rt<<endl;
        printstate<<"MEM.Wrt_reg_addr:\t"<<state.MEM.Wrt_reg_addr<<endl;
        printstate<<"MEM.rd_mem:\t"<<state.MEM.rd_mem<<endl;
        printstate<<"MEM.wrt_mem:\t"<<state.MEM.wrt_mem<<endl;
        printstate<<"MEM.wrt_enable:\t"<<state.MEM.wrt_enable<<endl;
        printstate<<"MEM.nop:\t"<<state.MEM.nop<<endl;

        printstate<<"WB.Wrt_data:\t"<<state.WB.Wrt_data<<endl;
        printstate<<"WB.Rs:\t"<<state.WB.Rs<<endl;
        printstate<<"WB.Rt:\t"<<state.WB.Rt<<endl;
        printstate<<"WB.Wrt_reg_addr:\t"<<state.WB.Wrt_reg_addr<<endl;
        printstate<<"WB.wrt_enable:\t"<<state.WB.wrt_enable<<endl;
        printstate<<"WB.nop:\t"<<state.WB.nop<<endl;
    }
    else cout<<"Unable to open file";
    printstate.close();
}
 

int main() {
    
    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;
    ALU myALU;

    int cycle = 0;
    stateStruct state;

    state.WB.nop  = true;
    state.MEM.nop = true;
    state.EX.nop  = true;
    state.ID.nop  = true;
    state.IF.nop  = false;

    while (1) {
        stateStruct newState = state;

        newState.WB.nop  = state.MEM.nop;
        newState.MEM.nop = state.EX.nop;
        newState.EX.nop  = state.ID.nop;
        newState.ID.nop  = state.IF.nop;

        /* --------------------- WB stage --------------------- */
        if (!state.WB.nop && state.WB.wrt_enable) {
            myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
        }

        /* --------------------- MEM stage --------------------- */
        if (!state.MEM.nop) {

            newState.WB.Wrt_data     = state.MEM.ALUresult;
            newState.WB.Rs           = state.MEM.Rs;
            newState.WB.Rt           = state.MEM.Rt;
            newState.WB.wrt_enable   = state.MEM.wrt_enable;
            newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;

            if (state.MEM.rd_mem) {
                newState.WB.Wrt_data = myDataMem.readDataMem(state.MEM.ALUresult);
            }
            else if (state.MEM.wrt_mem) {
                myDataMem.writeDataMem(state.MEM.ALUresult, state.MEM.Store_data);
            }
        
        }

        /* --------------------- EX stage --------------------- */
        if (!state.EX.nop) {
            
            string imm = state.EX.Imm.to_string();
            bitset<32> extimm(string(16, imm[0]) + imm);

            myALU.ALUOperation(state.EX.alu_op, state.EX.Read_data1, state.EX.is_I_type ? extimm : state.EX.Read_data2);            
            
            newState.MEM.ALUresult    = myALU.ALUresult;
            newState.MEM.rd_mem       = state.EX.rd_mem;
            newState.MEM.Rs           = state.EX.Rs;
            newState.MEM.Rt           = state.EX.Rt;
            newState.MEM.Store_data   = state.EX.Read_data2;
            newState.MEM.wrt_enable   = state.EX.wrt_enable;
            newState.MEM.wrt_mem      = state.EX.wrt_mem;
            newState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr;
        
        }


        /* --------------------- ID stage --------------------- */
        bool isTaken = false;

        if (!state.ID.nop) {

            string ins = state.ID.Instr.to_string();

            bool isBranch           = ins.substr(0, 6) == string("000100");

            newState.EX.is_I_type   = ins.substr(0, 6) != string("000000");

            newState.EX.rd_mem      = ins.substr(0, 6) == string("100011"); // lw
            newState.EX.wrt_mem     = ins.substr(0, 6) == string("101011"); // sw

            newState.EX.Rs          = bitset<5> (ins.substr(6, 5));
            newState.EX.Rt          = bitset<5> (ins.substr(11, 5));
            newState.EX.Imm         = bitset<16>(ins.substr(16, 16));

            newState.EX.Read_data1  = myRF.readRF(newState.EX.Rs);
            newState.EX.Read_data2  = myRF.readRF(newState.EX.Rt);

            newState.EX.Wrt_reg_addr= newState.EX.is_I_type ? newState.EX.Rt : bitset<5> (ins.substr(16, 5));

            newState.EX.alu_op      = (ins.substr(0, 2) == string("10")) || 
                                      (!newState.EX.is_I_type && (ins.substr(26, 6) == string("100001")));

            newState.EX.wrt_enable  = !newState.EX.wrt_mem && !isBranch;

            if (isBranch) {
                /** 
                 * we will assume that the beq (branch-if-qual) instruction
                 * operates like a bne (branch-if-not-equal) instruction
                 **/
                if (myRF.readRF(newState.EX.Rs) != myRF.readRF(newState.EX.Rt)) {
                    bitset<32> branchAddr(string(14, newState.EX.Imm[0]) + newState.EX.Imm.to_string() + "00");
                    newState.IF.PC = bitset<32> (branchAddr.to_ulong() + state.IF.PC.to_ulong() + 4);
                    newState.ID.nop = true;
                    isTaken = true;
                }
            }
        
        }


        /* --------------------- IF stage --------------------- */
        if (!state.IF.nop) {
            
            newState.ID.Instr = myInsMem.readInstr(state.IF.PC);
            cout << newState.ID.Instr << endl;
            if (newState.ID.Instr.to_ulong() == 0xffffffff) {
                newState.ID.nop = true;
                newState.IF.nop = true;
            }
            else if (!isTaken)
                newState.IF.PC = bitset<32> (state.IF.PC.to_ulong() + 4);
        
        }



        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;

        // printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ...

        cycle += 1;
        state = newState; /* The end of the cycle and updates the current state with the values calculated in this cycle */

    }
    
    myRF.outputRF(); // dump RF;
    myDataMem.outputDataMem(); // dump data mem

    return 0;
}