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

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

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

    int set_nu;

    int off_sz;
    int ind_sz;
    int tag_sz;

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

            string sa = accessaddr.to_string();
            string tag_1 = sa.substr(0, L1.tag_sz);
            string ind_1 = sa.substr(L1.tag_sz, L1.ind_sz);
            string off_1 = sa.substr(L1.tag_sz + L1.ind_sz, L1.off_sz);
            string tag_2 = sa.substr(0, L2.tag_sz);
            string ind_2 = sa.substr(L2.tag_sz, L2.ind_sz);
            string off_2 = sa.substr(L2.tag_sz + L2.ind_sz, L2.off_sz);
            
            // cout << sa << endl;
            // cout << tag_1 << ' ' << ind_1 << ' ' << off_1 << endl;
            // cout << tag_2 << ' ' << ind_2 << ' ' << off_2 << endl;

            int index_1 = stoi(ind_1, nullptr, 2);
            int index_2 = stoi(ind_2, nullptr, 2);

            // cout << accesstype << endl;
            // access the L1 and L2 Cache according to the trace;
            if (accesstype.compare("R") == 0) {
                //Implement by you:
                // read access to the L1 Cache,
                //  and then L2 (if required),
                //  update the L1 and L2 access state variable;

                // Read L1 hit
                for (string tag : L1.tag_array[index_1]) {
                    if (tag.compare(tag_1) == 0) {
                        L1AcceState = RH;
                        break;
                    }
                }
                // Read L1 miss
                if (L1AcceState != RH) {
                    L1AcceState  = RM;
                    // Read L2 hit
                    for (string tag : L2.tag_array[index_2]) {
                        if (tag.compare(tag_2) == 0) {
                            L2AcceState = RH;
                            // Empty way exists
                            int i;
                            for (i = 0; i < L1.setsize; i ++) {
                                if (L1.tag_array[index_1][i].empty()) {
                                    L1.tag_array[index_1][i] = tag_1;
                                    break;
                                }
                            }
                            // Eviction
                            if (i == L1.setsize) {
                                L1.tag_array[index_1][L1.evc_id] = tag_1;
                                L1.evc_id += 1;
                                L1.evc_id %= L1.setsize;
                            }
                            break;
                        }
                    }
                    // Read L2 miss
                    if (L2AcceState != RH) {
                        L2AcceState  = RM;
                        // Empty way exists
                        int i;
                        for (i = 0; i < L2.setsize; i ++) {
                            if (L2.tag_array[index_2][i].empty()) {
                                L2.tag_array[index_2][i] = tag_2;
                                break;
                            }
                        }
                        // Eviction
                        if (i == L2.setsize) {
                            L2.tag_array[index_2][L2.evc_id] = tag_2;
                            L2.evc_id += 1;
                            L2.evc_id %= L2.setsize;
                        }
                    }
                }

            } else {
                //Implement by you:
                // write access to the L1 Cache,
                // and then L2 (if required),
                // update the L1 and L2 access state variable;

                // Write L1 hit
                for (string tag : L1.tag_array[index_1]) {
                    if (tag.compare(tag_1) == 0) {
                        L1AcceState = WH;
                        break;
                    }
                }
                // Write L1 miss
                if (L1AcceState != WH) {
                    L1AcceState  = WM;
                    // Write L2 hit
                    for (string tag : L2.tag_array[index_2]) {
                        if (tag.compare(tag_2) == 0) {
                            L2AcceState = WH;
                            break;
                        }
                    }
                    // Write L2 miss
                    if (L2AcceState != WH) {
                        L2AcceState  = WM;
                    }
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

