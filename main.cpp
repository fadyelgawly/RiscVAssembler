/*
 *
 * The professors R/I implementations are commented out for clarity
 *
 * The file has the B/S/U types implemented
 *
 * comments regarding the execution and not the decoding are put above
 * or between the statements of each type's related code
 *
 */

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <cmath>

using namespace std;

int regs[32]={0};
unsigned int pc = 0x0;

char memory[8*1024];    // only 8KB of memory located at address 0
char dataMemory[8*1024];    // 8KB of memory for allocated for storing data

void emitError(char *s)
{
    cout << s;
    exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW){
    cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}
void instDecExec(unsigned int instWord)
{
    unsigned int rd, rs1, rs2, funct3, funct7, opcode;
    unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
    unsigned int address;
    
    unsigned int instPC = pc - 4;    // should this be removed
    
    opcode = instWord & 0x0000007F;
    rd = (instWord >> 7) & 0x0000001F;
    funct3 = (instWord >> 12) & 0x00000007;
    rs1 = (instWord >> 15) & 0x0000001F;
    rs2 = (instWord >> 20) & 0x0000001F;
    
    //////////////////////////////////////////
    //    * JALR INSTRUCTION IS NOT+ FINISHED.//
    //    * B_imm is not tested                //
    //////////////////////////////////////////
    
    
    // - inst[31] - inst[30:25] inst[24:21] inst[20]
    I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
    B_imm = (((instWord >> 8) & 0xF) | (((instWord >> 25) & 0x1F) << 4) | (((instWord >> 7) & 1) << 9) | ((instWord >> 31) ? 0xFFFFF800 : 0x0)) << 1;
    S_imm = ((instWord >> 7) & 0x1F) | ((instWord >> 25) & 0x3F) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
    U_imm = ((instWord >> 12) & 0xFFFFF);
    
    printPrefix(instPC, instWord);
    
    /*
     if(opcode == 0x33){        // R Instructions
     switch(funct3){
     case 0: if(funct7 == 32) { // rd's place has an immediate in it
     cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
     regs[rd] = regs[rs1] - regs[rs2];
     }
     else {
     cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
     regs[rd] = regs[rs1] + regs[rs2];
     }
     break;
     default:
     cout << "\tUnkown R Instruction \n";
     }
     }
     
     else if(opcode == 0x13){    // I instructions
     switch(funct3){
     case 0:    cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
     regs[rd] = regs[rs1] + (int)I_imm;
     break;
     default:
     cout << "\tUnkown I Instruction \n";
     }
     }
     */
    
    if(opcode == 0x63){    // SB instructions (Branch)
        
        int relAddr = pc + B_imm; // target address
        bool misAlign = (pc % 4) ? 0:1; // check if target address is divisible by 4
        
        switch(funct3){
            case 0:    cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(regs[rs1] == regs[rs2]){
                    if(!misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            case 1:    cout << "\tBNE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(regs[rs1] != regs[rs2]){
                    if(misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            case 4:    cout << "\tBLT\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(regs[rs1] < regs[rs2]){
                    if(misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            case 5:    cout << "\tBGE\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(regs[rs1] >= regs[rs2]){
                    if(misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            case 6:    cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(abs(regs[rs1]) < abs(regs[rs2])){
                    if(misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            case 7:    cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";
                if(abs(regs[rs1]) >= abs(regs[rs2])){
                    if(misAlign){
                        pc = relAddr;
                    }
                    else{
                        // throw misalignment exception, not mentioned
                    }
                }
                else{
                    // pc to next instruction
                }
                break;
                
            default:
                cout << "\tUnkown SB Instruction \n";
        }
    }
    
    
    /*     build realistic-size memory
     beginning of data memory should not be 0
     
     check memory bounds
     throw outOfBounds exception
     
     check for alignment (relAddr % 4)
     throw misalignment exception
     
     values in registers are already sign-extended    */
    
    else if(opcode == 0x23){    // S instructions
        
        int relAddr = regs[rs1] + S_imm;
        
        switch(funct3){
                
                // store the lowest 8 bits
            case 0:    cout << "\tSB\tx" << rs2 << hex << ", 0x" << (int)S_imm << "(x"  << rs2 << ")\n";
                dataMemory[relAddr] = regs[rs2] & 0xFF;
                break;
                
                // store the lowest 16 bits
            case 1:    cout << "\tSH\tx" << rs2 << hex << ", 0x" << (int)S_imm << "(x"  << rs2 << ")\n";
                dataMemory[relAddr] = regs[rs2] & 0xFFFF;
                break;
                
                // store all 32 bits
            case 2:    cout << "\tSW\tx" << rs2 << hex << ", 0x" << (int)S_imm << "(x"  << rs2 << ")\n";
                dataMemory[relAddr] = regs[rs2];
                break;
                
            default:
                cout << "\tUnkown S Instruction \n";
        }
    }
    
    /*    immediates are sign extended in the instrucions
     lowest bits in case of overflow    */
    
    else if(opcode == 0x37){    // U instructions (LUI)
        
        cout << "\tLUI\tx" << rd << hex << ", 0x" << (int)U_imm << endl;
        regs[rd] = (U_imm << 12);    // add 12 zeros to imm's right, then store in rd
    }
    
    else if(opcode == 0x17){    // U instructions (AUIPC)
        
        cout << "\tAUIPC\tx" << rd << hex << ", 0x" << (int)U_imm << endl;
        regs[rd] = pc + (U_imm << 12);     // add 12 zeros to imm's right, add imm to pc, then store in rd
    }
    
    else {
        cout << "\tUnkown Instruction \n";
    }
    
}

int main(int argc, char *argv[]){
    
    unsigned int instWord=0;
    ifstream inFile;
    ofstream outFile;
    
    if(argc<1) emitError("use: rv32i_sim <machine_code_file_name>\n");
    
    inFile.open(argv[1], ios::in | ios::binary | ios::ate);
    
    if(inFile.is_open())
    {
        int fsize = inFile.tellg();
        
        inFile.seekg (0, inFile.beg);
        if(!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");
        
        while(true){
            instWord =     (unsigned char)memory[pc] |
            (((unsigned char)memory[pc+1])<<8) |
            (((unsigned char)memory[pc+2])<<16) |
            (((unsigned char)memory[pc+3])<<24);
            pc += 4;
            // remove the following line once you have a complete simulator
            if(pc==32) break;            // stop when PC reached address 32
            instDecExec(instWord);
        }
        
        // dump the registers
        for(int i=0;i<32;i++)
            cout << "x" << dec << i << ": \t"<< "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";
        
    } else emitError("Cannot access input file\n");
}

