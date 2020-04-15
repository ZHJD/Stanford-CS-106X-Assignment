// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "filelib.h"
#include "vector.h"
#include "pqueue.h"

using namespace std;

static void inline writeMagic(obitstream& output);
static bool inline isHuf(ibitstream& input);
static void pre_order(HuffmanNode* encodingTree, Vector<int>& data, Vector<int>& nodes);
static void serializ(HuffmanNode* encodingTree, obitstream& output);
static HuffmanNode* constructTree(ibitstream& input, Vector<int>& data);
static HuffmanNode* deserializ(ibitstream& input);
static void dfs(const HuffmanNode* encodingTree, Map<int, string>& encodingMap, string s);
static void deleteTree(HuffmanNode* &node);

static const string magic = "huf";  // magic number

/*
 * write marks to encoding file
 */
static void inline writeMagic(obitstream& output) {
    for(char byte : magic) {
        output.put(byte);
    }
}

/*
 * check this file whether is a huffman encoding file
 */
static bool inline isHuf(ibitstream& input) {
    for(char byte : magic) {
        if(input.get() != byte) {
            return false;
        }
    }
    return true;
}

/*
 * traverse the encodingTree using preorder to get characters and pointers
 */
static void pre_order(HuffmanNode* encodingTree, Vector<int>& data, Vector<int>& nodes) {
    if(!encodingTree) {
        return;
    }
    if(encodingTree->isLeaf()) {
        nodes.push_back(0);
        data.push_back(encodingTree->character); // just store the leaf characters
        return;
    }
    nodes.push_back(1);
    pre_order(encodingTree->zero, data, nodes);
    pre_order(encodingTree->one, data, nodes);
}

/*
 * write tree structure and character to compressed file as a header for decompression
 */
static void serializ(HuffmanNode* encodingTree, obitstream& output) {
    Vector<int> data;
    Vector<int> nodes;
    pre_order(encodingTree, data, nodes);
    output << data; // first write data
    //cout << data.size() << endl;
    for(int elem : nodes) {
        output.writeBit(elem); // second write info about tree structure from preorder
    }
}

/*
 * according to the header in compressed file to reconstruct the tree
 */
static HuffmanNode* constructTree(ibitstream& input, Vector<int>& data) {
    int bit = input.readBit();
    if(bit == 0) {
        HuffmanNode* root = new HuffmanNode(data.pop_front());
        return root;
    }
    else {
        /*
         * because huffman tree only has the node with degree zero and degree two,
         * not including degree one node,so we are confident to construct zero and
         * one without any judgement.Otherwise, we have to using outside node.
         */
        HuffmanNode* root = new HuffmanNode();
        root->zero = constructTree(input, data);
        root->one  = constructTree(input, data);
        return root;
    }
}

/*
 * reconstruct the encoding tree for decompression
 */
static HuffmanNode* deserializ(ibitstream& input) {
    Vector<int> data;
    input >> data; // read vector data from input file.
    return constructTree(input, data);
}

/*
 * buildFrequencyTable - scan every byte in input stream, and to
 * compute the frequency of every bit,in other words, we get the
 * frequency of 0-255(one byte),not just ASCII code,by doing this,
 * not only can we encode txt file, but also can encode image,audio..
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    for(int ch = input.get(); !input.fail(); ch = input.get()) {
        freqTable[ch]++;
    }
    /*
     * PSEUDO_EOF: this is a macro defined in another file,it's value
     * is 256,as we all know,it is a two byte value,so PSEUDO_EOF is
     * different from any byte we need to encode.we need to write PSEUDO_EOF's
     * encode to compressed file,because the sum of all bits may not be
     * divided by 8,but the final file will write another some bits to
     * be divided by 8. so if no PSEUDO_EOF,we actually don't know the
     * truly end.
     */
    freqTable[PSEUDO_EOF] = 1;
    rewindStream(input); // rewind input stream to file start
    return freqTable;
}

/*
 * buildEncodingTree - by using min-heap to build huffman tree
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // lower priority numbers correspond to higher effective priorities
    auto pq = PriorityQueue<HuffmanNode*>();
    HuffmanNode *node = nullptr;
    for(const auto& key : freqTable.keys()) {
        node = new HuffmanNode(key, freqTable.get(key));
        pq.enqueue(node, node->count);
    }

    node = nullptr;

    HuffmanNode *zero = nullptr;
    HuffmanNode *one  = nullptr;
    while(pq.size() > 1) {
        zero = pq.dequeue();
        one  = pq.dequeue();
        node = new HuffmanNode(NOT_A_CHAR, zero->count + one->count,
                               zero,
                               one);
        pq.enqueue(node, node->count);
    }
    return pq.dequeue();
}

/*
 * to traverse encoding tree to get encoding map
 */
static void dfs(const HuffmanNode* encodingTree, Map<int, string>& encodingMap, string s) {
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
        node = encodingTree; // return to root for another byte
    }
}

void compress(istream& input, obitstream& output) {
    const auto freqTable = buildFrequencyTable(input);
    const auto encodingTree = buildEncodingTree(freqTable);
    const auto encodingMap = buildEncodingMap(encodingTree);
    writeMagic(output);
    serializ(encodingTree, output);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {
    if(!isHuf(input)) {
        cout << "This is not a Huffman encoding file!\n";
        return;
    }
    const auto encodingTree = deserializ(input);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
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
