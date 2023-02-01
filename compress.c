//File for functions focused on compression/encoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>
// Writes the number of unique characters to the file as a single byte
// Then, for each character in the node array, write the character (as a byte)
// and the frequency of that character (size = sizeof(unsigned int))
//Format: [nChars] [char1 ascii code(char)] [char1 frequency(int)] [char2 ascii code] [char2 frequency] ...
void writeCharFrequenciesinFile(FILE * out_file, NodeArray * node_array){
        //TODO: write your implementation her
        //fprintf(out_file, "%d", node_array->nChars);                          //Writes the frequency and 3 0 bytes
        //fprintf(out_file, "%c", 0);
        //fprintf(out_file, "%c", 0);
        //fprintf(out_file, "%c", 0);
        fwrite(&node_array->nChars, 4, 1, out_file);
        for(int i =0; i < node_array->nChars; i++) {
                fprintf(out_file, "%c", node_array->nodes[i]->val);             //Writes the val to the file
                fprintf(out_file, "%c",node_array->nodes[i]->frequency);        //Writes the frequency after
                fprintf(out_file, "%c", 0);
                fprintf(out_file, "%c", 0);
                fprintf(out_file, "%c", 0);

        }
}

// Adds 0 bits to the end of the final byte if the encoded document
// ends with a partial byte (cannot write single bits to the file!)
void padBuffer(FILE * out_file){
        //TODO: write your implementation here
        for(int i = nBits; i < 8; i++) {
                strcat(bit_buffer, "0");                                        //Adds 0s if needed
        }
        writeBitBuffer(bit_buffer, out_file);
}

// Add the next bit to the bit buffer. If a full byte is buffered,
// write it to the file (cannot write individual bits to a file!)
// Reset nBits and the bitBuffer if the buffer is written to the file
void writeBitBuffer(char * code, FILE * out_file){
        //TODO: write your implementation here
        int dec = 0;
        for(int i = 0; i < 8; i++) {
                if(code[i] == '1') {
                        int num = 1;
                        for(int j = 7; j > i; j--) {                            //Rewrites the buffer as a decimal
                                num *= 2;
                        }
                        dec += num;
                }

        }
        fprintf(out_file, "%c", dec);                                           //Prints the value to the file
}


// Given a code string, it takes each "bit" representing character and
// writes it to the bit buffer for the output file
void writeCodeToFile (char * code, FILE * out_file){
        //TODO: write your implementation here
        strcat(bit_buffer, code);
        nBits += strlen(code);
        char * temp = (char*)malloc(sizeof(char) * 256);
        if(nBits >= 9) {
                strncpy(temp, bit_buffer, 8);
                int j = 0;                                                      //Writes bits to file if nBits > 8
                for(int i = 8; i < 256; i++) {                                  //Adds the remaining bits to the beginning of the buffer
                        bit_buffer[j] = bit_buffer[i];
                        j++;
                }
                nBits = nBits - 8;
                writeBitBuffer(temp, out_file);
        }

        else if(nBits == 8) {
                strncpy(temp, bit_buffer, 8);                                   //Writes bits to file if exactly 8 bits are given
                nBits = 0;
                memset(bit_buffer, 0, 256);
                writeBitBuffer(temp, out_file);
        }
}


// Primary compression method
// (1) stores frequencies in the output file
// (2) rewrites the input file using the Huffman Code in the huffman_tree
void createCompressedFile(char* input_file, char * output_file, NodeArray * node_array, NodeArray * huffman_tree){
        //TODO: write your implementation here
        FILE * f;
        FILE * input;
        f = fopen(output_file, "w+");
        input = fopen(input_file, "r");
        writeCharFrequenciesinFile(f, node_array);
        unsigned char c = (unsigned char)fgetc(input);
        while(!feof(input)) {
                writeCodeToFile(char_to_code[c], f);
                c = (unsigned char)fgetc(input);
        }

        if(bit_buffer[0] != '\0') {
                padBuffer(f);                                                   //Pads buffer with 0s if buffer is not empty
        }

        fclose(f);
        fclose(input);



}
