#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

class PHT {
private:
    int size;
    vector<vector<int> > counter;

public:
    PHT(int m) {
        size = pow(2, m);
        counter.assign(size, vector<int> (2, 1));
    }

    int predict(bitset<32> pc) {
        return counter[pc.to_ulong() % size][0];
    }

    bool correct_update(bitset<32> pc, bool taken) {
        int index = pc.to_ulong() % size;
        if (taken) {
            if (counter[index][0] == 1) {
                counter[index][1] = 1;
                return true;
            } else {
                if (counter[index][1] == 1)
                    counter[index].assign({1, 0});
                else
                    counter[index].assign({0, 1});
                return false;
            }
        } else {
            if (counter[index][0] == 0) {
                counter[index][1] = 0;
                return true;
            } else {
                if (counter[index][1] == 0)
                    counter[index].assign({0, 1});
                else
                    counter[index].assign({1, 0});
                return false;
            }
        }
    }
};

int main (int argc, char** argv) {
    ifstream config;
    config.open(argv[1]);

    int m;
    config >> m;

    PHT pht(m);

    config.close();

    ofstream out;
    string out_file_name = string(argv[2]) + ".out";
    out.open(out_file_name.c_str());

    ifstream trace;
    trace.open(argv[2]);

    int total_wrong = 0;
    int total = 0;

    unsigned long addr;
    bool taken;

    while (trace >> std::hex >> addr >> taken) {
        bitset<32> pc = bitset<32> (addr);
        int prediction = pht.predict(pc);
        if (!pht.correct_update(addr, taken)) {
            total_wrong += 1;
        }
        total += 1;
        out << prediction << endl;
    }

    float miss_rate = 0;
    miss_rate = (float) total_wrong / (float) total;
    cout << "m = " << m << endl;
    cout << "miss_rate = " << miss_rate << endl;
    
    trace.close();
    out.close();
}
