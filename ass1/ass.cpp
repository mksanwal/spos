#include <bits/stdc++.h>
using namespace std;

struct Symbol {
    string symbol;
    int address;
};

struct Intermediate {
    int address;
    string label;
    string opcode;
    string operand;
};

map<string, int> opcodeTable = {
    {"ADD", 1}, {"SUB", 2}, {"MUL", 3}, {"MOV", 4}, {"READ", 5}, {"PRINT", 6}
};

map<string, int> directiveTable = {
    {"START", 0}, {"END", 0}, {"ORIGIN", 0}, {"EQU", 0}, {"DC", 0}, {"DS", 0}
};

vector<Symbol> symbolTable;
vector<Intermediate> intermediateCode;
vector<string> sourceCode;

int searchSymbol(string label) {
    for (int i = 0; i < symbolTable.size(); i++)
        if (symbolTable[i].symbol == label)
            return i;
    return -1;
}

void readSourceFile() {
    ifstream fin("source.asm");
    if (!fin) {
        cerr << "Error: source.asm not found!\n";
        exit(1);
    }
    string line;
    while (getline(fin, line)) {
        if (!line.empty())
            sourceCode.push_back(line);
    }
    fin.close();
}

void writeSymbolTable() {
    ofstream fout("symtab.txt");
    fout << "Symbol\tAddress\n";
    for (auto &s : symbolTable)
        fout << s.symbol << "\t" << s.address << "\n";
    fout.close();
}

void writeIntermediateCode() {
    ofstream fout("intermediate.txt");
    fout << "Address\tLabel\tOpcode\tOperand\n";
    for (auto &ic : intermediateCode)
        fout << ic.address << "\t" << ic.label << "\t" << ic.opcode << "\t" << ic.operand << "\n";
    fout.close();
}

void writeMachineCode() {
    ofstream fout("output.txt");
    fout << "Address\tMachineCode\n";

    for (auto &ic : intermediateCode) {
        if (ic.opcode == "START" || ic.opcode == "END")
            continue;

        if (opcodeTable.find(ic.opcode) != opcodeTable.end()) {
            int op = opcodeTable[ic.opcode];
            int addr = 0;
            int idx = searchSymbol(ic.operand);
            if (idx != -1)
                addr = symbolTable[idx].address;
            fout << ic.address << "\t(" << op << "," << addr << ")\n";
        }
    }

    fout.close();
}

void passOne() {
    cout << "PASS I processing...\n";
    int LC = 0;

    for (auto line : sourceCode) {
        string label = "", opcode = "", operand = "";
        stringstream ss(line);
        ss >> opcode;

        if (opcode == "START") {
            ss >> LC;
            intermediateCode.push_back({LC, "", "START", ""});
            continue;
        }

        if (opcodeTable.find(opcode) == opcodeTable.end() &&
            directiveTable.find(opcode) == directiveTable.end()) {
            // label found
            label = opcode;
            ss >> opcode >> operand;
        } else {
            ss >> operand;
        }

        if (opcode == "END") {
            intermediateCode.push_back({LC, label, opcode, operand});
            break;
        }

        if (!operand.empty() && operand.back() == ',')
            operand.pop_back();

        intermediateCode.push_back({LC, label, opcode, operand});

        // add label to symbol table
        if (!label.empty()) {
            int index = searchSymbol(label);
            if (index == -1)
                symbolTable.push_back({label, LC});
        }

        // add operand as symbol if not already present
        if (!operand.empty()) {
            // handle comma-separated operands like A,B
            stringstream ops(operand);
            string temp;
            while (getline(ops, temp, ',')) {
                if (searchSymbol(temp) == -1)
                    symbolTable.push_back({temp, LC + 1});
            }
        }

        LC += 1;
    }

    writeSymbolTable();
    writeIntermediateCode();
}

void passTwo() {
    cout << "PASS II processing...\n";
    writeMachineCode();
}

int main() {
    readSourceFile();
    passOne();
    passTwo();

    cout << "\nAssembler execution completed successfully!\n";
    cout << "Generated files:\n";
    cout << "1. symtab.txt\n";
    cout << "2. intermediate.txt\n";
    cout << "3. output.txt\n";
    return 0;
}





// Topic: Two Pass Assembler
// 🔹 Definition

// An Assembler is a program that converts assembly language code (human-readable mnemonics) into machine code (binary or object code) that the processor can execute.

// ⚙️ Need for Assembler

// Assembly language is easier to write than machine code, but computers only understand binary.

// Assemblers translate assembly instructions like MOV A, B into binary equivalents like 0001 0002.

// Assemblers also resolve symbolic names (like LOOP, VAR1) to memory addresses.

// 🧩 Types of Assemblers
// Type	Description
// One-Pass Assembler	Generates machine code in a single scan. Used when forward references are not allowed.
// Two-Pass Assembler	Scans the program twice — once to build symbol table and once to generate machine code. Handles forward references.
// 🔁 Why Two Passes?

// Because when we first read the program, some labels or symbols might appear before they are defined.
// Example:

// JMP LOOP
// ...
// LOOP: ADD A


// We can’t know the address of LOOP in the first line until we’ve scanned the whole program once.
// Hence, two passes are used:

// Pass 1: Build the symbol table and intermediate code.

// Pass 2: Use those tables to generate final machine code.

// 🧮 Working of Two Pass Assembler
// 🔹 Pass I:

// Objective: Define all symbols and generate intermediate code.

// Tasks:

// Initialize Location Counter (LC).

// For each line:

// If a label is present → add it to Symbol Table with current LC.

// Generate Intermediate Code (IC) for each instruction.

// Update LC according to instruction size.

// Write intermediate code to file.

// At end, we have:

// Symbol Table

// Intermediate Code

// Output Example:

// Address  Label  Opcode  Operand
// 100             READ    A
// 101             MOV     A,B
// 102             ADD     B
// 103             PRINT   A
// 104             END

// 🔹 Pass II:

// Objective: Generate final machine code using intermediate code and symbol table.

// Tasks:

// Read intermediate code and symbol table.

// Replace symbolic operands with actual addresses.

// Generate machine opcode + operand address pair.

// Output final machine code.

// Output Example:

// Address MachineCode
// 100     (5,101)
// 101     (4,101)
// 102     (1,101)
// 103     (6,101)

// 📊 Assembler Data Structures
// Data Structure	Purpose
// Symbol Table	Stores label names with their memory addresses.
// Opcode Table	Stores mnemonic (e.g., ADD, MOV) and corresponding opcode number.
// Literal Table	Stores constants or literals like =5.
// Pool Table	Used for literal management.
// Intermediate Code (IC)	Stores partially translated code between Pass I and Pass II.
// 🧱 Example (Pseudo Code Flow)

// Input:

// START 100
// READ A
// MOV A, B
// ADD B
// PRINT A
// END


// Pass 1 Output:

// Symbol Table:
// A -> 101
// B -> 101
// Intermediate Code:
// 100  READ A
// 101  MOV  A,B
// 102  ADD  B
// 103  PRINT A
// 104  END


// Pass 2 Output:

// 100 (5,101)
// 101 (4,101)
// 102 (1,101)
// 103 (6,101)

// 📂 Files Used (in your C++ code)
// File	Description
// source.asm	Input Assembly program
// symtab.txt	Symbol Table output
// intermediate.txt	Intermediate code output
// output.txt	Final machine code output
// 🧠 Viva Questions & Answers
// 🔸 Basic Understanding

// Q: What is an assembler?
// A: An assembler converts assembly language code into machine code.

// Q: Why is it called a “two-pass assembler”?
// A: Because it scans the source code twice — once to build symbol tables and once to generate final machine code.

// Q: What happens in Pass-I of an assembler?
// A: Symbol table creation and generation of intermediate code.

// Q: What happens in Pass-II of an assembler?
// A: Final machine code generation using symbol table and intermediate code.

// Q: What is a symbol table?
// A: A data structure that stores labels/symbols with their corresponding addresses.

// Q: What is intermediate code?
// A: A partially translated form of assembly code used between Pass I and Pass II.

// 🔸 Technical & Code-Based

// Q: What is a location counter (LC)?
// A: It keeps track of the address of each instruction in memory during Pass I.

// Q: What are pseudo-ops or assembler directives?
// A: Commands like START, END, ORIGIN, EQU, DC, DS which tell assembler how to process code, not machine instructions themselves.

// Q: Why do we need a two-pass assembler instead of a one-pass assembler?
// A: Because of forward references — when symbols are used before being defined.

// Q: What output files are generated in your program?
// A: symtab.txt, intermediate.txt, and output.txt.

// Q: What data structures did you use in your C++ program?
// A: Arrays/Vectors for Symbol Table and Intermediate Code, and Maps for Opcode Table.

// 🔸 Advanced / Conceptual

// Q: What is the difference between assembler, compiler, and interpreter?
// A:

// Assembler → Translates assembly to machine code.

// Compiler → Translates high-level code to machine code.

// Interpreter → Executes high-level code line-by-line.

// Q: What is the purpose of directives like DC and DS?
// A:

// DC (Define Constant): Reserves memory and assigns value.

// DS (Define Storage): Reserves memory without assigning value.

// Q: What is a forward reference?
// A: A label or symbol used before its definition appears in the program.

// Q: What is the function of an opcode table?
// A: It stores the operation code number corresponding to each mnemonic instruction.