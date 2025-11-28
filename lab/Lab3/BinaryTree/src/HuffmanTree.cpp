#include "../include/HuffmanTree.h"

// 构造函数（TODO：简化了下冗余代码）
HuffmanTreeNode::HuffmanTreeNode(char ch, int freq) : character(ch), frequency(freq) {}

// 重写 getValue 方法，返回字符
string HuffmanTreeNode::getValue() {
    return string(1, character);  // 返回字符
}

// 获取节点频率
int HuffmanTreeNode::getFrequency() const {
    return frequency;
}

// 构建哈夫曼树
HuffmanTreeNode* HuffmanTreeNode::buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    // 创建最小堆
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, freq_cmp> minHeap;

    // 将所有字符和频率加入堆中
    // TODO
    // for(声明变量 : 容器)，“:” 类似于 in
    for (const pair<const char, int>& pair : freqMap)
        minHeap.push(new HuffmanTreeNode(pair.first, pair.second));
    
    // 合并最小的两个节点，直到堆中只剩下一个节点
    // TODO
    while (minHeap.size() > 1)
    {
        // 1. 取最小俩节点
        HuffmanTreeNode* left = minHeap.top();
        minHeap.pop();
        HuffmanTreeNode* right = minHeap.top();
        minHeap.pop();

        // 2. 创造新节点
        int freq = left->getFrequency() + right->getFrequency();
        HuffmanTreeNode* parent = new HuffmanTreeNode('\0', freq);
        parent->left = left;
        parent->right = right;

        // 3. 放回最小堆中
        minHeap.push(parent);
    }

    // 返回哈夫曼树的根节点（TODO：访问 top 前，必要的是否为空判断）
    return minHeap.empty()? nullptr : minHeap.top();
}

// 打印哈夫曼树的编码（先序遍历）
void HuffmanTreeNode::printHuffmanCodes(HuffmanTreeNode* root, string code) {
    // TODO
    // 1. 递归终止条件
    if (!root) return;

    // 2. 叶子节点为字符
    if (!root->left && !root->right)
        cout << root->getValue() << ": " << code << endl;

    // 3. 递归遍历
    printHuffmanCodes(static_cast<HuffmanTreeNode*>(root->left), code + "0");
    printHuffmanCodes(static_cast<HuffmanTreeNode*>(root->right), code + "1");
}
