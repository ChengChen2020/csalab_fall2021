#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize 65536


class RF
{
private:
    vector<bitset<32> > Registers;
public:
    bitset<32> ReadData1, ReadData2;
    RF() {
        Registers.resize(32);
        Registers[0] = bitset<32> (0);
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable) {
        // '00000' == NULL
        ReadData1 = Registers[RdReg1.to_ulong()];
        ReadData2 = Registers[RdReg2.to_ulong()];
        if (WrtEnable.to_ulong() == 1) {
            Registers[WrtReg.to_ulong()] = WrtData;
        }
    }

    void OutputRF() {
        ofstream rfout;
        // rfout.open("RFresult.txt", std::ios_base::app);
        rfout.open("RFresult.txt");
        if (rfout.is_open()) {
            rfout << "A state of RF:" << endl;
            for (int j = 0; j < 32; j ++) {
                rfout << Registers[j] << endl;
            }
        }
        else cout << "Unable to open file";
        rfout.close();
    }
};


class ALU
{
public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2) {
        switch(ALUOP.to_ulong()) {
            case ADDU:
                ALUresult = bitset<32> (oprand1.to_ulong() + oprand2.to_ulong());
                break;
            case SUBU:
                ALUresult = bitset<32> (oprand1.to_ulong() - oprand2.to_ulong());
                break;
            case AND:
                ALUresult = oprand1 & oprand2;
                break;
            case OR:
                ALUresult = oprand1 | oprand2;
                break;
            case NOR:
                ALUresult = ~ (oprand1 | oprand2);
                break;
            default:
                cout << "ERROR" << endl;
        }
        return ALUresult;
    }
};


class INSMem
{
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
                IMem[i] = bitset<8> (line);
                i ++;
            }
        }
        else cout << "Unable to open file";
        imem.close();
    }

    bitset<32> ReadMemory (bitset<32> ReadAddress) {
        // Read the byte at the ReadAddress and the following three byte.
        int addr = ReadAddress.to_ulong();
        string st = "";
        for (int i = 0; i < 4; i ++) {
            st += IMem[addr + i].to_string();
        }
        Instruction = bitset<32> (st);
        return Instruction;
    }
};

class DataMem
{
private:
    vector<bitset<8> > DMem;
public:
    bitset<32> readdata;
    DataMem() {
        DMem.resize(MemSize);
        ifstream dmem;
        string line;
        int i = 0;
        dmem.open("dmem.txt");
        if (dmem.is_open()) {
            while (getline(dmem, line)) {
                DMem[i] = bitset<8> (line);
                i ++;
            }
        }
        else cout << "Unable to open file";
        dmem.close();

    }  
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) {
        int addr = Address.to_ulong();
        if (readmem.to_ulong() == 1) {
            string st = "";
            for (int i = 0; i < 4; i ++) {
                st += DMem[addr + i].to_string();
            }
            readdata = bitset<32> (st);
        } else if (writemem.to_ulong() == 1) {
            for (int i = 0; i < 4; i ++) {
                DMem[addr + i] = bitset<8> (WriteData.to_string().substr(i * 8, 8));
            }
        }
        return readdata;
    }

    void OutputDataMem() {
        ofstream dmemout;
        dmemout.open("dmemresult.txt");
        if (dmemout.is_open()) {
            for (int j = 0; j< 1000; j++) {     
              dmemout << DMem[j] << endl;
            }
        }
        else cout << "Unable to open file";
        dmemout.close();
    }

};  



int main() {
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    int PC = 0;

    while (1) {
        // Fetch
        bitset<32>ins = myInsMem.ReadMemory(PC);
        cout << "Instruction:" << ins << endl;

        // If current insturciton is "11111111111111111111111111111111", then break;
        if (ins == bitset<32> ("11111111111111111111111111111111"))
            break;

        // Decode
        bitset<6> opcode = bitset<6> (ins.to_string().substr(0, 6));

        bool isLoad = opcode == bitset<6> ("100011");
        bool isStore = opcode == bitset<6> ("101011");
        bool isRtype = opcode == bitset<6> ("000000");
        bool isJtype = opcode == bitset<6> ("000010") or opcode == bitset<6> ("000011"); // j && jal
        bool isItype = !isRtype && !isJtype;
        bool isBranch = opcode == bitset<6> ("000100"); // beq

        bitset<3> op;
        if (isLoad or isStore) {
            op = bitset<3> (ADDU);
        } else if (isRtype) {
            op = bitset<3> (ins.to_string().substr(29, 3));
        } else {
            op = bitset<3> (ins.to_string().substr(3, 3));
        }

        bitset<1> WrtEnable;
        if (isStore or isBranch or isJtype) {
            WrtEnable = bitset<1> (0);
        } else {
            WrtEnable = bitset<1> (1);
        }


        // Execute
        PC = PC + 4;

        if (isJtype) {
            string jaddr = (bitset<32> (PC)).to_string().substr(0, 4) + ins.to_string().substr(6, 26) + "00";
            PC = (bitset<32> (jaddr)).to_ulong();
        } else {
            bitset<5> rs = bitset<5> (ins.to_string().substr(6, 5));
            bitset<5> rt = bitset<5> (ins.to_string().substr(11, 5));
            myRF.ReadWrite(rs, rt, NULL, NULL, NULL);
            bitset<32> result;
            if (isItype) {
                bitset<16> imm = bitset<16> (ins.to_string().substr(16, 16));
                bitset<32> extimm = bitset<32> (string(16, imm.to_string()[0]) + imm.to_string());
                bitset<32> result = myALU.ALUOperation(op, myRF.ReadData1, extimm);
                if (isLoad) {
                    bitset<32> data = myDataMem.MemoryAccess(result, NULL, isLoad, NULL);
                    myRF.ReadWrite(NULL, NULL, rt, data, WrtEnable);
                } else if (isStore) {
                    myDataMem.MemoryAccess(result, myRF.ReadData2, NULL, isStore);
                } else if (isBranch) {
                    string baddr = string(14, imm.to_string()[0]) + imm.to_string() + "00";
                    if (myRF.ReadData1 == myRF.ReadData2) {
                        PC = PC + (bitset<32> (baddr)).to_ulong();
                    }
                } else {
                    myRF.ReadWrite(NULL, NULL, rt, result, WrtEnable);
                }
            } else {
                bitset<5> rd = bitset<5> (ins.to_string().substr(16, 5));
                result = myALU.ALUOperation(op, myRF.ReadData1, myRF.ReadData2);
                myRF.ReadWrite(NULL, NULL, rd, result, WrtEnable);
            }
        }

        myRF.OutputRF(); // dump RF;
        cout << endl;
    }

    myDataMem.OutputDataMem(); // dump data mem

    return 0;
}
