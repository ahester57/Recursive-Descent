/* Austin Hester
CS 4280 sp18
C.Z. Janikow */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"
#include "token.h"
#include "tree.h"
#include "treehelper.h"
#include "node.h"
#include "stack.h"

static FILE* outfile;
static int blockCount = 0;
static int tempCount = 0;
static int tempIndex = 0;
static int iffCount = 0;
static int iterCount = 0;

static stack_t* globalstack;
static stack_t** localstacks;
static int tos[17];
static int localStackCount = 0;

int*
isinoneofthesestacks(token_t* tk, int which_stacks[], int num_stacks)
{
	int result[2];
	int i;
	for (i = 0; i < num_stacks; i++) {
		int r = isinstack(tk, localstacks[which_stacks[i]]);
		if (r >= 0) {
			result[0] = which_stacks[i];
			result[1] = r;
			return result;	
		}
	}
	return NULL;
}

int
gen_program(node_t* root, FILE* fp)
{
	if (fp == (FILE*)NULL) {
		fprintf(stderr, "FILE NOT OPEN\n");
		return -1;
	}
	outfile = fp;

	int result = 0;	
	tos[0] = -1;
	// if (g != 0) { uh oh }
	globalstack = (stack_t*) malloc(2*sizeof(stack_t));


    fprintf(outfile, "BEGIN: NOOP\n");
	localstacks = (stack_t**) malloc(16*sizeof(stack_t*));

	switch(root->num_children)
	{
		case 1: ;
			result = gen_block(root->children[0]);

			break;
		case 2: ;
			globalstack->varstack = (token_t**) malloc(64*sizeof(token_t*));
			globalstack->varvalues = (int*) malloc(64*sizeof(int));
			int g = justbuildglobalstack(root->children[0], globalstack);
			tos[0] = globalstack->nvars-1;	

			result = gen_vars(root->children[0]);
			result = gen_block(root->children[1]);
			break;
		default:
			break;
	}

	fprintf(outfile, "\tSTOP\n");
	int i;
    for (i = 0; i < globalstack->nvars; i++) {
		fprintf(outfile, "\t%s 0\n", globalstack->varstack[i]->instance);
    }

    for (i = 0; i < tempCount+1; i++) {
		fprintf(outfile, "\tT%d 0\n", i);
    }
	return result;
}

int
gen_block(node_t* root)
{
	int result = 0;
	fprintf(outfile, "BLOC%d: NOOP\n", blockCount++);
			displaytoken(root->token);
	switch(root->num_children)
	{
		case 1: ;
			// deal with stats
			result = gen_stats(root->children[0]);

			return result;	

		case 2: ;
			// build the local stack	
			int n = localStackCount++;
			localstacks[n] = (stack_t*) malloc(2*sizeof(stack_t));
			localstacks[n]->varstack = (token_t**) malloc(64*sizeof(token_t*));
			localstacks[n]->varvalues = (int*) malloc(64*sizeof(int));

			// build local stack
			int l = justbuildlocalstack(root->children[0], localstacks[n]);
			int i;

			result = gen_vars(root->children[0]);
			result = gen_stats(root->children[1]);
		    for (i = 0; i < localstacks[n]->nvars; i++) {
				fprintf(outfile, "%s 0\n", localstacks[n]->varstack[i]->instance);
		    }
			return result;
		default:
			return result;
	}
}

int gen_vars(node_t* root)
{
	int result = 0;
	// ASSIGN VALUES TO VARS
	switch(root->num_children)
	{
		case 1:
			// there is no assign
			return result;
		case 3: ; 
			// only one assign
			result = gen_assign(root);
			return result;
		case 4: ;
			result = gen_assign(root);
			result = gen_mvars(root->children[3]);
			return result;
		default:
			return 0;
	}
}

int gen_mvars(node_t* root)
{
	int result = 0;
	// ASSIGN VALUES TO VARS
	switch(root->num_children)
	{
		case 1: ;
			node_t* child = root->children[0];
			if (strcmp(child->token->id, "<>") == 0) {
				result = gen_vars(child);
				return result;
			} else {
				// there is no assign
				return result;
			}
		case 3: ; 
			// only one assign
			result = gen_assign(root);
			return result;
		case 4: ;
			result = gen_assign(root);
			result = gen_mvars(root->children[3]);
			return result;
		default:
			return 0;
	}
}

int
gen_assign(node_t* root)
{

			displaytoken(root->token);
	node_t* id = root->children[0];
	node_t* val = root->children[2];
	int result = gen_expr(val);
	fprintf(outfile, "\tLOAD T%d\n", result);		
	fprintf(outfile, "\tSTORE %s\n", id->token->instance);
	return result;
}

int
gen_expr(node_t* root)
{
	int result = 0;
			displaytoken(root->token);
	switch(root->num_children)
	{
		case 1:
			// call child()
			result = gen_M(root->children[0]);
			return result;
		case 2: ; // I got 'a label can only be part of a statment.... ;'
			// cal right
			int t0 = gen_xhelp(root->children[1]);
			//
			// store in T0
			//fprintf(outfile, "\tLOAD %d\n", t0);
			int t0_i = tempCount++;
			t0_i = t0;
			//fprintf(outfile, "\tSTORE T%d\n", t0_i);
			// cal left 
			int t1 = gen_M(root->children[0]);
			tempCount++;
			//if +
			if (strcmp(root->children[1]->children[0]->token->id,
					   "+TK") == 0) {
				fprintf(outfile, "\tLOAD T%d\n", t1);
				fprintf(outfile, "\tADD T%d\n", t0_i);
				fprintf(outfile, "\tSTORE T%d\n", t0_i);
				result = t1 + t0;
				// "ADD temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "-TK") == 0) {
				//else -
				fprintf(outfile, "\tLOAD T%d\n", t1);
				fprintf(outfile, "\tSUB T%d\n", t0_i);
				fprintf(outfile, "\tSTORE T%d\n", t0_i);
				result = t1 - t0;
				// "SUB temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "*TK") == 0) {
				//else *
				fprintf(outfile, "\tLOAD T%d\n", t1);
				fprintf(outfile, "\tMULT T%d\n", t0_i);
				fprintf(outfile, "\tSTORE T%d\n", t0_i);
				result = t1 * t0;
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "/TK") == 0) {
				//else /
				fprintf(outfile, "\tLOAD T%d\n", t1);
				fprintf(outfile, "\tDIV T%d\n", t0_i);
				fprintf(outfile, "\tSTORE T%d\n", t0_i);
				//result = (int)(t1 / t0);
				result = 1;
			}
			return t0_i;
		default:
			return t0_i;
	}
}

int gen_xhelp(node_t* root)
{
	// THIS FIRSTSTSTSTSTSTSTST
			displaytoken(root->token);
	int result = gen_expr(root->children[1]);
	return result;
}

int
gen_M(node_t* root)
{
	int result;
			displaytoken(root->token);
	switch(root->num_children)
	{
		case 1:
			// call child()
			result = gen_R(root->children[0]);
			tempCount++;
			return result;
		case 2:
			// negate
			result = gen_M(root->children[1]);
			fprintf(outfile, "\tLOAD T%d\n", result);
			fprintf(outfile, "\tMULT %d\n", -1);
			fprintf(outfile, "\tSTORE T%d\n", result);
			return result;
		default:
			return 0;
	}
}

int gen_R(node_t* root)
{
	node_t* child = root->children[0];
			displaytoken(root->token);
	char* tkid = child->token->id;
	if (strcmp(tkid, "<>") == 0) {
		return gen_expr(root->children[0]);
	} else if (strcmp(tkid, "idTK") == 0) {
		fprintf(outfile, "\tLOAD %s\n", child->token->instance);
		fprintf(outfile, "\tSTORE T%d\n", tempCount);
		return tempCount;
	} else if (strcmp(tkid, "intTK") == 0) {
		displaytoken(child->token);
		fprintf(outfile, "\tLOAD %d\n", atoi(child->token->instance));
		fprintf(outfile, "\tSTORE T%d\n", tempCount);

		return tempCount;
	}
	return 0;
}

int gen_stats(node_t* root)
{
	int result;
			//displaytoken(root->token);
	switch(root->num_children)
	{
		case 1:
			// call child()
			result = gen_stat(root->children[0]);
			return result;
		case 2:
			// negate
			result = gen_stat(root->children[0]);
			result = gen_mstat(root->children[1]);
			return result;
		default:
			return 0;
	}
}

int gen_mstat(node_t* root)
{
	int result;
			displaytoken(root->token);
	switch(root->num_children)
	{
		case 1:
			// call child()
			result = gen_stat(root->children[0]);
			return result;
		case 2:
			// negate
			result = gen_stat(root->children[0]);
			result = gen_mstat(root->children[1]);
			return result;
		default:
			return 0;
	}
}

int gen_stat(node_t* root)
{
	int result = 0;
			//displaytoken(root->token);
	node_t* child = root->children[0];		
	token_t* tk = child->token;
	if (strcmp(tk->instance, "<in>") == 0) {
        result = gen_in(child);
        return result;
    } else if (strcmp(tk->instance, "<out>") == 0) {
        result = gen_out(child);
        return result;
    } else if (strcmp(tk->instance, "<block>") == 0) {
        result = gen_block(child);
        return result;
    } else if (strcmp(tk->instance, "<iff>") == 0) {
        result = gen_iff(child);
        return result;
    } else if (strcmp(tk->instance, "<iter>") == 0) {
        result = gen_iter(child);
        return result;
    } else if (strcmp(tk->instance, "<assign>") == 0) {
        result = gen_assign(child);
        return result;
    } else {
        return result;
    }	
}

int gen_in(node_t* root)
{
	token_t* tk = root->children[0]->token;
	// check where and what token is
	// need to redesign stack really
	fprintf(outfile, "\tREAD %s\n", tk->instance);	
	return 0;
}

int gen_out(node_t* root)
{
			displaytoken(root->token);
	int tvar = gen_expr(root->children[0]);
	fprintf(outfile, "\tWRITE T%d\n", tvar);	
	return tvar;
}


int gen_iff(node_t* root)
{
	int result = 0;
	switch(root->num_children)
	{
		case 1:
			// wrong
			return result;
		case 2: ;
			int ifC = iffCount++;
			result = gen_evaluate(root->children[0], "FOUT", ifC);
/*			fprintf(outfile, "\tLOAD T%d\n", result);
			fprintf(outfile, "\tMULT %d\n", -1);
			fprintf(outfile, "\tSTORE T%d\n", result);
*/
			result = gen_stat(root->children[1]);
			fprintf(outfile, "FOUT%d: NOOP\n", ifC);
			return result;
		default:
			return 0;
	}
	return 0;
}

int gen_iter(node_t* root)
{
	int result = 0;
	switch(root->num_children)
	{
		case 1:
			// wrong
			return result;
		case 2: ;
			int itC = iterCount++;
			fprintf(outfile, "LIN%d: NOOP\n", itC);
			result = gen_evaluate(root->children[0], "LOUT", itC);
/*			fprintf(outfile, "\tLOAD T%d\n", result);
			fprintf(outfile, "\tMULT %d\n", -1);
			fprintf(outfile, "\tSTORE T%d\n", result);
*/
			result = gen_stat(root->children[1]);
			fprintf(outfile, "\tBR LIN%d\n", itC);
			fprintf(outfile, "LOUT%d: NOOP\n", itC);
			return result;
		default:
			return 0;
	}
	return 0;
}

int gen_evaluate(node_t* root, char* label, int ifC)
{
	int result = 0;
	switch(root->num_children)
	{
		case 1:
			// wrong
			return result;
		case 2:
			// wrong
			return result;
		case 3: ;
			//int ifC = iffCount++;
			int right = gen_expr(root->children[2]);
/*			fprintf(outfile, "\tLOAD T%d\n", result);
			fprintf(outfile, "\tMULT %d\n", -1);
			fprintf(outfile, "\tSTORE T%d\n", result);
*/
			int left = gen_expr(root->children[0]);
			fprintf(outfile, "\tLOAD T%d\n", left);
			fprintf(outfile, "\tSUB T%d\n", right);

			int result = gen_RO(root->children[1], label, ifC);

			return result;
		default:
			return 0;
	}

}

int gen_RO(node_t* root, char* label, int ifC)
{
	token_t* tk = root->children[0]->token;
	if (strcmp(tk->id, "<TK") == 0) {
		fprintf(outfile, "\tBRZPOS %s%d\n", label, ifC);
        return 0;
    } else if (strcmp(tk->id, "<<TK") == 0) {
		fprintf(outfile, "\tBRPOS FOUT%d\n", ifC);
        return 0;
    } else if (strcmp(tk->id, ">TK") == 0) {
		fprintf(outfile, "\tBRZNEG %s%d\n", label, ifC);
        return 0;
    } else if (strcmp(tk->id, ">>TK") == 0) {
		fprintf(outfile, "\tBRNEG FOUT%d\n", ifC);
        return 0;
    } else if (strcmp(tk->id, "=TK") == 0) {
		fprintf(outfile, "\tBRZERO FOUT%d\n", ifC);
        return 0;
    } else if (strcmp(tk->id, "==TK") == 0) {
		fprintf(outfile, "\tBRNEG FOUT%d\n", ifC);
		fprintf(outfile, "\tBRPOS FOUT%d\n", ifC);
        return 0;
    } else {
        return 0;
    }
}


