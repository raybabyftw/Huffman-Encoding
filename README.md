# Huffman-Encoding


In this project, I embarked on an exploration of data encoding and decoding, diving deep into the transformation of text into a compressed format using two types of binary trees: binary search trees and Huffman trees. Encoding, the process of converting data into a different format, is a fundamental concept in computer science, crucial for efficient processing, storage, or transmission of information.

My Journey with Binary Search Trees
The adventure began with implementing a Counter Abstract Data Type (ADT) using a binary search tree (BST), but with a twist. Instead of integers, this tree was designed to handle strings, or "tokens," as I learned to call them. Each node in this specially designed BST held not just a token but also a count of how many times that token appeared in the text. This setup was crucial for what was to come next, laying the foundational stone for encoding data efficiently.

Discovering the Huffman Tree
The heart of my project was the Huffman tree, a binary tree engineered for encoding and decoding data. What set the Huffman tree apart was its unique property: only leaf nodes contained tokens, and the path from the root to each leaf—encoded as a series of 0s and 1s—represented the token's unique binary code. This design ensured that each encoded string was distinct and decodable back into its original text, a marvel of data representation.

Encoding and Decoding: The Process
The real magic happened when I started encoding text. After counting the frequency of each token in a text file using my BST-based Counter ADT, I used those frequencies to construct a Huffman tree. This tree enabled me to assign shorter binary codes to more frequent tokens, optimizing the size of the encoded data.

Decoding, the reverse process, was equally fascinating. Given a binary string, I used the Huffman tree to translate it back to the original text, demonstrating the power of Huffman encoding in compressing data without losing information.

Hands-on Application and Testing
Throughout this project, I tackled a series of hands-on tasks with provided text files, guiding me from analyzing token frequencies and constructing Huffman trees to the final encoding and decoding steps. The use of Counter and Huffman ADT implementations, coupled with rigorous testing against both provided and custom test cases, deepened my understanding and skills in data compression algorithms.
