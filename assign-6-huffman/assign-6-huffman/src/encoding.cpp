// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "filelib.h"
#include <queue>
#include "vector.h"

using namespace std;
// TODO: include any other headers you need

#define SHORT_INT_SIZE sizeof(short int)

struct compare {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) {
        return a->count > b->count;
    }
};



static void pre_order(HuffmanNode* encodingTree, Vector<int>& data, Vector<int>& nodes) {
    if(!encodingTree) {
        return;
    }
    if(encodingTree->isLeaf()) {
        nodes.push_back(0);
        data.push_back(encodingTree->character);
        return;
    }
    nodes.push_back(1);
    pre_order(encodingTree->zero, data, nodes);
    pre_order(encodingTree->one, data, nodes);
}

static void serializ(HuffmanNode* encodingTree, obitstream& output) {
    Vector<int> data;
    Vector<int> nodes;
    pre_order(encodingTree, data, nodes);
    //output << data;
    cout << data.size() << endl;
    for(int elem : nodes) {
        output.writeBit(elem);
    }
}

static HuffmanNode* constructTree(ibitstream& input, const Vector<int>& data, int& pos) {
    if(pos >= data.size()) {
        return nullptr;
    }
    int bit = input.readBit();
    if(bit == 0) {
        HuffmanNode* root = new HuffmanNode(data[pos]);
        pos += 1;
        return root;
    }
    else {
        HuffmanNode* root = new HuffmanNode();
        root->zero = constructTree(input, data, pos);
        root->one  = constructTree(input, data, pos);
        return root;
    }
}

static HuffmanNode* deserializ(ibitstream& input) {
    Vector<int> data;
    input >> data;
    //cout << data.size() << endl;
    int pos = 0;
    return constructTree(input, data, pos);
}

Map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    Map<int, int> freqTable;   // this is just a placeholder so it will compile
    for(int ch = input.get(); !input.fail(); ch = input.get()) {
        freqTable[ch]++;
    }
    freqTable[PSEUDO_EOF] = 1;
    rewindStream(input);       // tells the stream to seek back to the beginning
    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare>pq;
    HuffmanNode *node = nullptr;
    for(const auto& key : freqTable.keys()) {
        node = new HuffmanNode(key, freqTable.get(key));
        pq.push(node);
    }

    node = nullptr;

    HuffmanNode *zero = nullptr;
    HuffmanNode *one  = nullptr;
    while(pq.size() > 1) {
        zero = pq.top();
        pq.pop();
        one  = pq.top();
        pq.pop();
        node = new HuffmanNode(NOT_A_CHAR, zero->count + one->count,
                               zero,
                               one);
        pq.push(node);

    }
    return pq.top();   // this is just a placeholder so it will compile
}

static void dfs(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string s) {
    if(!encodingTree) {
        return;
    }
    if(encodingTree->isLeaf()) {
        encodingMap[encodingTree->character] = s;
        return;
    }
    s += "0";
    dfs(encodingTree->zero, encodingMap, s);
    s[s.size() - 1] = '1';
    dfs(encodingTree->one, encodingMap, s);
}


Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    Map<int, string> encodingMap;
    dfs(encodingTree, encodingMap, "");
    return encodingMap;
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    for(int ch = input.get(); !input.fail(); ch = input.get()) {
        for(const auto bit : encodingMap[ch]) {
            output.writeBit(bit - '0');
        }
    }
    for(const auto bit : encodingMap[PSEUDO_EOF]) {
        output.writeBit(bit - '0');
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int bit = input.readBit();
    HuffmanNode* node = encodingTree;
    while(!input.fail()) {
        while(!node->isLeaf()) {
            if(bit == 1) {
                node = node->one;
            }
            else {
                node = node->zero;
            }
            bit = input.readBit();
        }
        if(node->character != PSEUDO_EOF) {
            output.put(node->character);
        }
        else {
            break;
        }
        node = encodingTree;
    }
}

void compress(istream& input, obitstream& output) {
    const auto freqTable = buildFrequencyTable(input);
    const auto encodingTree = buildEncodingTree(freqTable);
    const auto encodingMap = buildEncodingMap(encodingTree);
    serializ(encodingTree, output);
    encodeData(input, encodingMap, output);
}

void decompress(ibitstream& input, ostream& output) {
    const auto encodingTree = deserializ(input);
    decodeData(input, encodingTree, output);
}

static void deleteTree(HuffmanNode* &node) {
    if(!node) {
        return;
    }
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
    node = nullptr;
}

void freeTree(HuffmanNode* node) {
    if(!node) {
        return;
    }
    deleteTree(node);
}
