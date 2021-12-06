/*
    Cache Simulator
    Level one L1 and level two L2 cache parameters are read from file
    (block size, line per set and set per cache).
    The 32 bit address is divided into:
    - tag bits (t)
    - set index bits (i)
    - block offset bits (b)

    o = log2(block size); i = log2(#sets) t = 32 - s - b
*/

#include <cmath>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <iostream>

using namespace std;
//access state
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss

struct config {
    int L1blocksize;
    int L1setsize;
    int L1size;
    int L2blocksize;
    int L2setsize;
    int L2size;
};

/* you can define the cache class here, or design your own data structure for L1 and L2 cache */
/**
 * Write Back
 * Write No-Allocate
 * Inclusive
 **/

class cache {
public:
    int size;
    int setsize;
    int blocksize;

    int set_nu; // number of sets

    int off_sz; // offset bits
    int ind_sz; // index bits
    int tag_sz; // tag bits

    int evc_id = 0;

    vector<vector<string> > tag_array;

    cache (int size, int setsize, int blocksize) {
        this->size = size;
        this->setsize = setsize;
        this->blocksize = blocksize;
        off_sz = log2(blocksize);
        set_nu = size * 1024 / setsize / blocksize;
        ind_sz = log2(set_nu);
        tag_sz = 32 - ind_sz - off_sz;

        tag_array.assign(set_nu, vector<string> (setsize, ""));
    }

    bool Read(bitset<32> address) {
        string sa = address.to_string();
        string tag = sa.substr(0, tag_sz);
        string ind = sa.substr(tag_sz, ind_sz);

        int index = stoi(ind, nullptr, 2);

        int i;
        for (i = 0; i < setsize; i ++) {
            if (tag_array[index][i] == tag) {
                return true;
            }
        }
        return false;
    }

    bool Write(bitset<32> address) {
        string sa = address.to_string();
        string tag = sa.substr(0, tag_sz);
        string ind = sa.substr(tag_sz, ind_sz);

        int index = stoi(ind, nullptr, 2);

        // Empty way exists
        for (int i = 0; i < setsize; i ++) {
            if (tag_array[index][i].empty()) {
                tag_array[index][i] = tag;
                return false;
            }
        }
        // Eviction
        tag_array[index][evc_id] = tag;
        evc_id += 1;
        evc_id %= setsize;
        return true;
    }

    bool Eviction(bitset<32> address) {
        string sa = address.to_string();
        string tag = sa.substr(0, tag_sz);
        string ind = sa.substr(tag_sz, ind_sz);

        int index = stoi(ind, nullptr, 2);

        // Empty way exists
        for (int i = 0; i < setsize; i ++) {
            if (tag_array[index][i].empty()) {
                tag_array[index][i] = tag;
                return false;
            }
        }
        // Eviction
        return true;
    }

    friend ostream& operator<<(ostream& os, const cache& ca) {
        os << ca.off_sz << '/' << ca.ind_sz << '/' << ca.tag_sz;
        return os;
    }
};

int main(int argc, char* argv[]){

    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);
    
    // read config file
    while (!cache_params.eof()) {
        cache_params >> dummyLine;
        cache_params >> cacheconfig.L1blocksize;
        cache_params >> cacheconfig.L1setsize;              
        cache_params >> cacheconfig.L1size;
        cache_params >> dummyLine;              
        cache_params >> cacheconfig.L2blocksize;           
        cache_params >> cacheconfig.L2setsize;        
        cache_params >> cacheconfig.L2size;
    }

    // Implement by you: 
    // initialize the hirearch cache system with those configs
    // probably you may define a Cache class for L1 and L2, or any data structure you like

    cache L1 = cache(cacheconfig.L1size, cacheconfig.L1setsize, cacheconfig.L1blocksize);
    cache L2 = cache(cacheconfig.L2size, cacheconfig.L2setsize, cacheconfig.L2blocksize);

    // cout << L1 << endl;
    // cout << L1.tag_array.size() << endl;
    // cout << L1.tag_array[0].size() << endl;
    // cout << L2 << endl;
    // cout << L2.tag_array.size() << endl;
    // cout << L2.tag_array[0].size() << endl;

    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";

    traces.open(argv[2]);
    tracesout.open(outname.c_str());

    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;

    if (traces.is_open() && tracesout.is_open()) {    
        while (getline (traces,line)) {   // read mem access file and access Cache

            int L1AcceState = 0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
            int L2AcceState = 0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;

            istringstream iss(line); 
            if (!(iss >> accesstype >> xaddr)) { break; }
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);

            // access the L1 and L2 Cache according to the trace;
            if (accesstype.compare("R") == 0) {
                //Implement by you:
                // read access to the L1 Cache,
                //  and then L2 (if required),
                //  update the L1 and L2 access state variable;

                // Read L1 hit
                if (L1.Read(accessaddr)) {
                    L1AcceState = RH;
                    L2AcceState = NA;
                } 
                // Read L2 hit
                else if (L2.Read(accessaddr)) {
                    L1AcceState = RM;
                    L2AcceState = RH;
                    // Load to L1
                    L1.Write(accessaddr);
                }
                // Both miss
                else {
                    L1AcceState = RM;
                    L2AcceState = RM;

                    if (L2.Eviction(accessaddr)) { // Eviction
                        string sa = accessaddr.to_string();
                        string tag_2 = sa.substr(0, L2.tag_sz);
                        string ind_2 = sa.substr(L2.tag_sz, L2.ind_sz);

                        int index_2 = stoi(ind_2, nullptr, 2);

                        // Inclusive
                        string evc_tag_2 = L2.tag_array[index_2][L2.evc_id];
                        string tmp = evc_tag_2 + ind_2;
                        /**
                         *  L2 block is larger than L1 block
                         *  Invalidate all L1 blocks
                         **/
                        int num = L1.tag_sz + L1.ind_sz - tmp.size();
                        for (int k = 0; k < pow(2, num); k ++) {
                            tmp += string(num - to_string(k).size(), '0') + to_string(k);                            
                            string evc_tag_1 = tmp.substr(0, L1.tag_sz);
                            string evc_ind_1 = tmp.substr(L1.tag_sz, L1.ind_sz);
                            int evc_index_1 = stoi(evc_ind_1, nullptr, 2);
                            int i;
                            for (i = 0; i < L1.setsize; i ++) {
                                if (L1.tag_array[evc_index_1][i] == evc_tag_1) {
                                    L1.tag_array[evc_index_1][i]  = "";
                                    break;
                                }
                            }
                        }
                        L2.tag_array[index_2][L2.evc_id] = tag_2;
                        L2.evc_id += 1;
                        L2.evc_id %= L2.setsize;
                    }
                    
                    L1.Write(accessaddr);
                }

            } else {
                //Implement by you:
                // write access to the L1 Cache,
                // and then L2 (if required),
                // update the L1 and L2 access state variable;

                if (L1.Read(accessaddr)) {
                    L1AcceState = WH;
                    L2AcceState = NA;
                } else if (L2.Read(accessaddr)) {
                    L1AcceState = WM;
                    L2AcceState = WH;
                } else {
                    L1AcceState = WM;
                    L2AcceState = WM;
                }
            }

            // Output hit/miss results for L1 and L2 to the output file
            tracesout << L1AcceState << " " << L2AcceState << endl;
        }
        traces.close();
        tracesout.close(); 
    }
    else cout << "Unable to open trace or traceout file ";

    return 0;
}

