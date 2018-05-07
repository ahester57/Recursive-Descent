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

static int blockCount = 0;
static int tempCount = 0;
static int tempIndex = 0;

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
gen_program(node_t* root)
{
	int result = 0;	
	tos[0] = -1;
	// if (g != 0) { uh oh }
	globalstack = (stack_t*) malloc(2*sizeof(stack_t));


    fprintf(stderr, "[BEGIN]: NOOP\n");
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

	fprintf(stderr, "\tSTOP\n");
	int i;
    for (i = 0; i < globalstack->nvars; i++) {
		fprintf(stderr, "\t%s 0\n", globalstack->varstack[i]->instance);
    }

    for (i = 0; i < tempCount+1; i++) {
		fprintf(stderr, "\tT%d 0\n", i);
    }
	return result;
}

int
gen_block(node_t* root)
{
	int result = 0;
	fprintf(stderr, "[BLOC%d]: NOOP\n", blockCount++);
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
		    for (i = 0; i < localstacks[n]->nvars; i++) {
				fprintf(stderr, "%s 0\n", localstacks[n]->varstack[i]->instance);
		    }

			result = gen_vars(root->children[0]);
			result = gen_stats(root->children[1]);
			return result;
		default:
			return result;
	}
}

int gen_vars(node_t* root)
{
	// ASSIGN VALUES TO VARS

}

int gen_mvars(node_t* root)
{

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
			//fprintf(stderr, "\tLOAD %d\n", t0);
			int t0_i = tempCount++;
			t0_i = t0;
			//fprintf(stderr, "\tSTORE T%d\n", t0_i);
			// cal left 
			int t1 = gen_M(root->children[0]);
			tempCount++;
			//if +
			if (strcmp(root->children[1]->children[0]->token->id,
					   "+TK") == 0) {
				fprintf(stderr, "\tLOAD T%d\n", t1);
				fprintf(stderr, "\tADD T%d\n", t0_i);
				fprintf(stderr, "\tSTORE T%d\n", t0_i);
				result = t1 + t0;
				// "ADD temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "-TK") == 0) {
				//else -
				fprintf(stderr, "\tLOAD T%d\n", t1);
				fprintf(stderr, "\tSUB T%d\n", t0_i);
				fprintf(stderr, "\tSTORE T%d\n", t0_i);
				result = t1 - t0;
				// "SUB temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "*TK") == 0) {
				//else *
				fprintf(stderr, "\tLOAD T%d\n", t1);
				fprintf(stderr, "\tMULT T%d\n", t0_i);
				fprintf(stderr, "\tSTORE T%d\n", t0_i);
				result = t1 * t0;
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "/TK") == 0) {
				//else /
				fprintf(stderr, "\tLOAD T%d\n", t1);
				fprintf(stderr, "\tDIV T%d\n", t0_i);
				fprintf(stderr, "\tSTORE T%d\n", t0_i);
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
	return 0;
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
			return result;
		case 2:
			// negate
			result = -gen_M(root->children[1]);
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
		int res = gen_expr(root->children[0]);
		return res;
	} else if (strcmp(tkid, "idTK") == 0) {
		fprintf(stderr, "\tLOAD %s\n", child->token->instance);
		fprintf(stderr, "\tSTORE T%d\n", tempCount);
		return tempCount;
	} else if (strcmp(tkid, "intTK") == 0) {
		displaytoken(child->token);
		fprintf(stderr, "\tLOAD %d\n", atoi(child->token->instance));
		fprintf(stderr, "\tSTORE T%d\n", tempCount);

		return tempCount;
	}
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
	fprintf(stderr, "\tREAD %s\n", tk->instance);	
	return 0;
}

int gen_out(node_t* root)
{
			displaytoken(root->token);
	int tvar = gen_expr(root->children[0]);
	fprintf(stderr, "\tWRITE T%d\n", tvar);	
	return tvar;
}

int gen_assign(node_t* root)
{

}

int gen_iff(node_t* root)
{


}

int gen_iter(node_t* root)
{

}

int gen_evaluate(node_t* root)
{

}

int gen_RO(node_t* root)
{

}


