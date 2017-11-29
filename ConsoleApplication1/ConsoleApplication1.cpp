/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdint.h>

#define MAXLINELENGTH 1000
////
using namespace std;
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
/////////////////////////////////////////////////////
int
convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return(num);
}
int
main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
			argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse to read a line from
	inFilePtr */
	if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		/* reached end of file */
	}

	/* this is how to rewind the file ptr so that you start reading from the
	beginning of the file */
	rewind(inFilePtr);

	/* after doing a readAndParse, you may want to do the following to test the
	opcode */
	if (!strcmp(opcode, "add")) {
		/* do whatever you need to do for opcode "add" */
		//int destReg=atoi(arg0)+atoi(arg1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ofstream ofs;
	ofs.open(outFileString);
	vector<string> oc;
	vector<string> labels;
	vector<int> reg(100); reg.at(0) = 0;
	vector< vector<string> > args;
	vector<string> innerArg;

	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) == 1) {
		oc.push_back(string(opcode));
		innerArg.push_back(string(arg0)); innerArg.push_back(string(arg1)); innerArg.push_back(string(arg2));
		args.push_back(innerArg); innerArg.erase(innerArg.begin(), innerArg.end());
		if (label != NULL)
		{
			labels.push_back(string(label));
		}
		else
		{
			labels.push_back("null");
		}
	}
	int offSet;

	for (int i = 0; i<int(oc.size()); i++)
	{
		if ((oc.at(i)).compare("add") == 0)
		{
			int bOC = 0, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), bdR = atoi(args.at(i).at(2).c_str());
			ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (bdR)) << "\n";
		}
		else if ((oc.at(i)).compare("nor") == 0)
		{
			int bOC = 0, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), bdR = atoi(args.at(i).at(2).c_str());
			ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (bdR)) << "\n";
		}
		else if ((oc.at(i)).compare("lw") == 0)
		{
			offSet = atoi((args.at(i).at(2)).c_str()); //OFFSETFIELD IN MACHINE LANGUAGE
			int memAdd;//memory address=contents of regA+offsetField
			if (isNumber(&(args.at(i).at(2))[0u]) == 1) //NUMERIC ADDRESS
			{
			}
			else //SYMBOLIC ADDRESS
			{
				for (int j = 0; j<int(labels.size()); j++)
				{
					if (labels.at(j) == (args.at(i).at(2))) { offSet = j; }
				}
			}
			int bOC = 2, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), boS = offSet;
			if (boS<0) { boS = convertNum(boS); }
			ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (boS << (15) >> (15))) << "\n";
		}
		else if ((oc.at(i)).compare("sw") == 0)
		{
			offSet = atoi((args.at(i).at(2)).c_str()); //OFFSETFIELD IN MACHINE LANGUAGE
			if (isNumber(&(args.at(i).at(2))[0u]) == 1) //NUMERIC ADDRESS
			{
			}
			else //SYMBOLIC ADDRESS
			{
				for (int j = 0; j<int(labels.size()); j++)
				{
					if (labels.at(j) == (args.at(i).at(2))) { offSet = j; }
				}
			}
			int bOC = 2, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), boS = offSet;
			if (boS<0) { boS = convertNum(boS); }
			ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (boS << (15) >> (15))) << "\n";
		}
		else if ((oc.at(i)).compare("beq") == 0)
		{
			offSet = atoi(args.at(i).at(2).c_str()); //OFFSETFIELD IN MACHINE LANGUAGE
			if (reg.at(atoi(args.at(i).at(0).c_str())) == reg.at(atoi(args.at(i).at(1).c_str()))) //regA and regB have same contents
			{
				if (isNumber(&(args.at(i).at(2))[0u]) == 1)
				{
					int bOC = 4, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), boS = offSet;
					if (boS<0) { boS = convertNum(boS); }
					ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (boS << (15) >> (15))) << "\n";
				}
				else
				{
					int thisIndex = i, offsetIndex;
					for (int j = 0; j<int(labels.size()); j++)
					{
						if (labels.at(j) == args.at(i).at(2)) { offsetIndex = j; } //FINDING ADDRESS OF OFFSET
					}
					offSet = offsetIndex - thisIndex - 1; //difference between addresses
					int bOC = 4, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), boS = offSet;
					if (boS<0) { boS = convertNum(boS); }
					ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (boS << (15) >> (15))) << "\n";
				}
			}
			else
			{
				int bOC = 4, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str()), boS = offSet;
				if (boS<0) { boS = convertNum(boS); }
				ofs << ((bOC << 22) | (brA << 19) | (brB << 16) | (boS << (15) >> (15))) << "\n";

			}
		}
		else if ((oc.at(i)).compare("jalr") == 0)
		{
			int bOC = 2, brA = atoi(args.at(i).at(0).c_str()), brB = atoi(args.at(i).at(1).c_str());
			ofs << ((bOC << 22) | (brA << 19) | (brB << 16)) << "\n";
		}
		else if ((oc.at(i)).compare("halt") == 0)
		{
			int bOC = 6;
			ofs << (bOC << 22) << "\n";
		}
		else if ((oc.at(i)).compare("noop") == 0)
		{
			int bOC = 7;
			ofs << (bOC << 22) << "\n";
		}
		else if ((oc.at(i)).compare(".fill") == 0)
		{
			int fillNum = atoi(args.at(i).at(0).c_str());
			if (isNumber(&(args.at(i).at(0))[0u]) != 1)
			{
				for (int j = 0; j<int(labels.size()); j++)
				{
					if (labels.at(j) == args.at(i).at(0)) { fillNum = j; }
				}
			}
			ofs << fillNum << "\n";
		}
	}

	return(0);
}

/*
* Read and parse a line of the assembly-language file.  Fields are returned
* in label, opcode, arg0, arg1, arg2 (these strings must have memory already
* allocated to them).
*
* Return values:
*     0 if reached end of file
*     1 if all went well
*
* exit(1) if line is too long.
*/
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
	char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	* Parse the rest of the line.  Would be nice to have real regular
	* expressions, but scanf will suffice.
	*/
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		opcode, arg0, arg1, arg2);
	return(1);

}

int
isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return((sscanf(string, "%d", &i)) == 1);
}
