//File for functions focused on decompression/decoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Read and return a single int from the file
int readIntFromFile(FILE * in_file){
        //TODO: write your implementation here
        int num;
        fread(&num, 4, 1, in_file);
        return num;
}


// Read and return a single char from the file
char readCharFromFile(FILE * in_file){
        return (char)fgetc(in_file);
}

// Read and return the number of unique characters in the file
// Obtain all character frequencies of the encoded file and store them in ascii_freq
int loadFrequenciesFromFile(FILE * in_file, int* ascii_freq){
        //TODO: write your implementation here
        int nChars;
        //nChars = readCharFromFile(in_file);                           //Reads the frequency and 3 0 bytes
        int n;
        nChars = readIntFromFile(in_file);
        //printf("%d %d", nChars, readCharFromFile(in_file));
        //readCharFromFile(in_file);
        //readCharFromFile(in_file);
        for(int i = 0; i < nChars; i++) {
                /*int n = readCharFromFile(in_file);                    //Reads the value
                int p = readCharFromFile(in_file);                      //Reads the frequency
                if(p < 0) {
                        p+=256;                                         //Adds 256 in case it is negative (signed)
                }
                ascii_freq[n] = p;                                      //Sets the int array ascii value index to the frequency
                numFreq += p;
                readCharFromFile(in_file);
                readCharFromFile(in_file);
                readCharFromFile(in_file);*/
                int n = readCharFromFile(in_file);
                int p = readIntFromFile(in_file);
                if(n < 0){
                        n+=256;
                }
                ascii_freq[n] = p;
                numFreq += p;
        }
        return nChars;
}


// Allocates, initializes, and returns a node array from the file
// (1) Load all frequency information from the header of the specified file
// (2) Builds a node array from the file frequencies
NodeArray * buildNodeArrayFromFile(FILE* in_file){
        //TODO: write your implementation here
}

// Decodes the input file using the huffman tree
// Write the decoded file into the output file
// Assumes that the next byte to read is the first byte to decode (just past the header)
void writeUncompressedFile(FILE* in_file, FILE* out_file, NodeArray* huffman_tree){
        //TODO: write your implementation here
        int c = readCharFromFile(in_file);
        char * entireString = (char*)malloc(sizeof(char) * 100000);
        int count = 0;
        while (count < numFreq) {                                       //Loop through the entire file
                char * code = (char*)malloc(sizeof(char) * 8);
                for(int i = 0; i < 8; i++) {
                        int pow = 1;
                        for(int j = 0; j < 7 - i; j++) {
                                pow *= 2;
                        }

                        if((c / pow) == 1){
                                code[i] = '1';                          //Convert to dec to string
                                c -= pow;
                        }
                        else{
                                code[i] = '0';
                        }

                }
                strcat(entireString, code);                             //Creates an entire string for the entire file
                c = readCharFromFile(in_file);
                if(c < 0) {
                        c+=256;
                }
                count++;

        }
        int i = 0;
        int j = 0;
        while(i < numFreq){

                Node * n = getRootHuffmanTree(huffman_tree);
                while(isLeafNode(n) == 0) {
                        char num = entireString[j];
                        if(num == '1') {
                                n = n->left;                            //Traverses the tree to get the char value
                        }
                        else{
                                n = n->right;

                        }
                        j++;

                }
                fprintf(out_file,"%c", n->val);
                i++;
        }
        fclose(out_file);
        fclose(in_file);

}

// Primary decoding function:
// (1) Open the input file and load all frequency information
// (2) Build the Huffman Tree for those frequencies
// (3) Traverse the Huffman Tree bit-by-bit and write each decoded
// character to the output file
// (4) Free the node arrays and close the files
void uncompress(char * input_file, char* output_file, int print){
        //TODO: write your implementation here
        FILE * input;
        FILE * output;
        FILE * org;
        output = fopen(output_file, "w+");
        if(!(input = fopen(input_file, "r"))){
                printf("Could not open file %s\n", input_file);
                exit(1);
        }
        int * ascii_freq;
        int nChars;
        NodeArray * nArr;
        ascii_freq = (int*)malloc(sizeof(int) * 256);
        nChars = loadFrequenciesFromFile(input, ascii_freq);
        nArr = initializeNodeArray(ascii_freq, nChars);
        int j = 0;
        for(int i = 0; i < 256; i++){
                if(ascii_freq[i] != 0) {
                        Node * n = (Node*)malloc(sizeof(Node));         //Creates the node array
                        n->val = (char)i;
                        n->frequency = ascii_freq[i];
                        nArr->nodes[j] = n;
                        j++;
                }
        }
        sortNodeArray(nArr);
        nArr = buildHuffmanTree(nArr);
        if(print == 1) {
                printHuffmanTree(nArr);                                 //If we are printing the array and writing to a file
        }
        if(print == 2) {
                printHuffmanTree(nArr);                                 //If we are printing but not writing to a file
                return;
        }
        writeUncompressedFile(input, output, nArr);

}
