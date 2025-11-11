#include <bits/stdc++.h>
using namespace std;

struct MNTEntry {
    string name;
    int mdtIndex;
};

vector<MNTEntry> MNT;
vector<string> MDT;
map<string, int> ALA;
vector<string> intermediate;

void passOne() {
    ifstream fin("source.asm");
    ofstream fMNT("mnt.txt"), fMDT("mdt.txt"), fINT("intermediate.txt");

    string line;
    bool inMacro = false;
    string macroName;
    int mdtIndex = 0;

    while (getline(fin, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "MACRO") {
            inMacro = true;
            continue;
        } else if (word == "MEND") {
            MDT.push_back("MEND");
            inMacro = false;
            continue;
        }

        if (inMacro) {
            if (macroName.empty()) {
                macroName = word; // first word is macro name
                MNT.push_back({macroName, (int)MDT.size()});
                string args;
                getline(ss, args);
                stringstream argsStream(args);
                string arg;
                int index = 1;
                while (getline(argsStream, arg, ',')) {
                    ALA[arg] = index++;
                }
            } else {
                MDT.push_back(line);
            }
        } else {
            intermediate.push_back(line);
        }
    }

    // Write files
    fMNT << "MacroName\tMDTIndex\n";
    for (auto &m : MNT)
        fMNT << m.name << "\t\t" << m.mdtIndex << "\n";

    fMDT << "Index\tDefinition\n";
    for (int i = 0; i < MDT.size(); i++)
        fMDT << i + 1 << "\t" << MDT[i] << "\n";

    for (auto &i : intermediate)
        fINT << i << "\n";

    fin.close(); fMNT.close(); fMDT.close(); fINT.close();
}

void passTwo() {
    ifstream fINT("intermediate.txt");
    ofstream fEXP("expanded.txt");
    string line;

    while (getline(fINT, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        bool isMacro = false;
        for (auto &m : MNT) {
            if (word == m.name) {
                isMacro = true;
                string rest;
                getline(ss, rest);
                stringstream argStream(rest);
                vector<string> params;
                string param;
                while (getline(argStream, param, ',')) {
                    if (param[0] == ' ') param = param.substr(1);
                    params.push_back(param);
                }

                // Expand macro
                for (int i = m.mdtIndex; i < MDT.size(); i++) {
                    if (MDT[i] == "MEND") break;
                    string expanded = MDT[i];
                    for (int j = 0; j < params.size(); j++) {
                        string search = "&" + string(1, 'A' + j);
                        size_t pos = expanded.find(search);
                        if (pos != string::npos)
                            expanded.replace(pos, search.length(), params[j]);
                    }
                    fEXP << expanded << "\n";
                }
                break;
            }
        }

        if (!isMacro)
            fEXP << line << "\n";
    }

    fINT.close();
    fEXP.close();
}

int main() {
    passOne();
    passTwo();
    cout << "Macro Processor executed successfully.\n";
    cout << "Generated files: mnt.txt, mdt.txt, intermediate.txt, expanded.txt\n";
    return 0;
}



// 🧠 Topic: Two Pass Macro Processor
// 🔹 Definition

// A Macro Processor is a program that expands macros in an assembly language program before the assembler runs.
// It replaces each macro call with the set of instructions defined in the macro body.

// ⚙️ What is a Macro?

// A macro is a sequence of assembly instructions that can be used repeatedly just by calling its name.

// 🧩 Why Two Passes?
// Pass-I:

// Identifies and stores all macro definitions.

// Builds:

// MNT (Macro Name Table)

// MDT (Macro Definition Table)

// ALA (Argument List Array)

// Removes macro definitions from the source program.

// Produces intermediate code (with macro calls intact).

// Pass-II:

// Expands macros in the intermediate code using MNT, MDT, and ALA.

// Produces the expanded source program (ready for assembler).

// 📊 Data Structures Used
// Table	Full Form	Purpose
// MNT	Macro Name Table	Stores each macro name and pointer to its definition in MDT
// MDT	Macro Definition Table	Stores actual macro instructions
// ALA	Argument List Array	Maps macro arguments (&A, &B) to actual parameters
// Intermediate Code	Output of Pass-I (main program without macro definitions)



// 🧮 Working of Macro Processor
// Pass-I Steps

// Read each line of source program.

// When MACRO is found:

// Add macro name to MNT.

// Store macro body in MDT until MEND is found.

// Build ALA for arguments.

// Copy all non-macro statements to intermediate code.

// Pass-II Steps

// Read intermediate code line by line.

// When a macro call is found:

// Use MNT to find its definition.

// Use ALA to substitute arguments.

// Expand the macro by fetching lines from MDT.

// Write final expanded code to output file.


// Viva Questions and Answers
// 🔸 Basic

// Q: What is a macro?
// A: A macro is a group of assembly instructions that can be reused by calling its name.

// Q: What is the difference between a macro and a subroutine?
// A:

// Macro → expanded inline (no call/return overhead).

// Subroutine → involves a jump to a different code segment.

// Q: Why do we need a macro processor?
// A: To automate repetitive assembly code, improve readability, and reduce programming effort.

// Q: What is the function of Pass-I?
// A: Identify and store macro definitions in MNT and MDT, and produce intermediate code.

// Q: What is the function of Pass-II?
// A: Expand all macro calls using MNT and MDT to generate the final expanded code.

// 🔸 Technical

// Q: What are MNT and MDT?
// A:

// MNT (Macro Name Table): Stores macro name and pointer to MDT entry.

// MDT (Macro Definition Table): Stores actual macro body instructions.

// Q: What is ALA (Argument List Array)?
// A: It maps macro arguments (like &A, &B) to actual parameters during expansion.

// Q: What is the purpose of intermediate code in Pass-I?
// A: It holds the main program after removing macro definitions, ready for Pass-II.

// Q: What is the difference between assembler and macro processor?
// A:

// Assembler: Converts assembly code to machine code.

// Macro Processor: Expands macros before assembling.

// Q: When is the macro processor executed?
// A: Before the assembler (as a preprocessor phase).

// 🔸 Advanced

// Q: Can macros be nested?
// A: Yes, but it complicates MNT/MDT management. Most simple macro processors don’t support it.

// Q: What is meant by macro expansion?
// A: The process of replacing a macro call with the sequence of instructions defined in the macro body.

// Q: Why are two passes used?
// A: Pass-I stores definitions, and Pass-II expands macros — similar to two-pass assembler logic.