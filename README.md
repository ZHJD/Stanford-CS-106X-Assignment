# Stanford CS 106X Assignment

（Homepage : <https://web.stanford.edu/class/cs106x/index.html>)

## 作业

### [assign-6-huffman](https://web.stanford.edu/class/cs106x/assignments.html) 

### 简介

#### 压缩

* 构建字节(0 - 255)的频率表
* 使用最小堆构建哈夫曼编码树
* 构建变长编码表
* 哈夫曼树序列化
* 压缩文件

#### 解压缩

* 验证文件类型
* 哈夫曼编码树反序列化
* 解压

#### Q & A 

**Q：** 为什么要在编码表中加入***PSEUDO_EOF*** 并写入到压缩文件中？

**A：** 因为压缩后的信息为可能不是8的倍数，在写入硬盘后，系统会添加一些0，使得文件大小是8的倍数

> Since the character encodings have different lengths, often the length of a Huffmanencoded file does not come out to an exact multiple of 8 bits. Files are stored as sequences of whole bytes, so in cases like this the remaining digits of the last byte are filled with zeroes. You do not need to worry about this; it is managed for you as part of the underlying file system.

但是，在解压缩的时候，无法判断文件的真正结束位置，所以需要人为的在文件末尾插入***PSEUDO_EOF*** ，它被定义为256不同于要压缩的任何内容。

**Q：** 为什么可以压缩多种文件，如文本文件、图片?

**A：** 因为所有文件都可以看成是字节流，而变长编码表是基于字节构建的，统计每个字节在文件中出现的频率，并构建编码表，这样做到了和要压缩的文件类型无关。

**Q：** 实现中遇到了哪些困难? 和[**handout**](https://web.stanford.edu/class/cs106x/res/handouts/29-Assignment-6-Huffman.pdf) 相比添加了哪些extra feature?

**A：** 一开始做编码树的序列化和反序列化的时候，忽视了哈夫曼树的一个重要特征：只有度为0和度为2的结点。因此使用了通用的二叉树序列化的算法，导致把中间节点的值也写入了文件当中。通过这个作业，也学会了如BST的序列化和反序列化。另一个问题是当构建完编码表后，忘记了`rewind(input)` ,造成后续压缩文件时函数读到文件末尾而直接退出，写入到压缩文件中的只有`PSEUDO_EOF`的编码。extra feature有:

1. 序列化哈夫曼编码树，不再写入存放频率表的字典
2. 压缩文件类型`.huf` 写入文件，并检查









