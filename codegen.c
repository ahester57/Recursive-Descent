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

static stack_t* globalstack;
static stack_t** localstacks;
static int tos = 0;

int
gen_program(node_t* root)
{
	int result = 0;	
    fprintf(stderr, "[BLOC:]BEGIN\n");
	globalstack = (stack_t*) malloc(2*sizeof(stack_t));
	globalstack->varstack = (token_t**) malloc(64*sizeof(token_t*));
	int g = justbuildglobalstack(root->children[0], globalstack);

	// if (g != 0) { uh oh }

    printf("Num Vars = %d \n", globalstack->nvars);
    int i;
    for (i = 0; i < globalstack->nvars; i++) {
        displaytoken(globalstack->varstack[i]);
    } 
	//localstacks = (stack_t**) malloc(16*sizeof(stack_t*));

	switch(root->num_children)
	{
		case 1: ;
			result = gen_block(root->children[0]);

			return result;	

		case 2: ;
			result = gen_vars(root->children[0]);
			result = gen_block(root->children[1]);
			return result;
		default:
			return result;
	}
}

int gen_block(node_t* root)
{
	fprintf(stderr, "[BLOC%d:] NOOP\n", blockCount++);

}

int gen_vars(node_t* root)
{

}

int gen_mvars(node_t* root)
{

}

int
gen_expr(node_t* root)
{
	int result = 0;
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
			fprintf(stderr, "LOAD %d\n", t0);
			int t0_i = tempCount++;
			fprintf(stderr, "STORE T%d\n", t0_i);
			// cal left 
			int t1 = gen_M(root->children[0]);
			//if +
			if (strcmp(root->children[1]->children[0]->token->id,
					   "+TK") == 0) {
				fprintf(stderr, "LOAD %d\n", t1);
				fprintf(stderr, "ADD T%d\n", t0_i);
				fprintf(stderr, "STORE T%d\n", t0_i);
				result = t1 + t0;
				// "ADD temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "-TK") == 0) {
				//else -
				fprintf(stderr, "LOAD %d\n", t1);
				fprintf(stderr, "SUB T%d\n", t0_i);
				fprintf(stderr, "STORE T%d\n", t0_i);
				result = t1 - t0;
				// "SUB temp"
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "*TK") == 0) {
				//else *
				fprintf(stderr, "LOAD %d\n", t1);
				fprintf(stderr, "MULT T%d\n", t0_i);
				fprintf(stderr, "STORE T%d\n", t0_i);
				result = t1 * t0;
			} else if (strcmp(root->children[1]->children[0]->token->id,
					   "/TK") == 0) {
				//else /
				fprintf(stderr, "LOAD %d\n", t1);
				fprintf(stderr, "DIV T%d\n", t0_i);
				fprintf(stderr, "STORE T%d\n", t0_i);
				result = t1 / t0;
			}
			tempCount = 0;
			return result;
		default:
			return result;
	}
}

int gen_xhelp(node_t* root)
{

}

int gen_M(node_t* root)
{
	int result;
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
	char* r_child = root->children[0]->token->id;
	if (strcmp(r_child, "<>") == 0) {

	} else if (strcmp(r_child, "idTK") == 0) {

	} else if (strcmp(r_child, "intTK") == 0) {

	}
}

int gen_stats(node_t* root)
{

}

int gen_mstat(node_t* root)
{

}

int gen_stat(node_t* root)
{

}

int gen_in(node_t* root)
{

}

int gen_out(node_t* root)
{

}

int gen_assign(node_t* root)
{

}

int gen_iff(node_t* root)
{


}

int gen_loop(node_t* root)
{

}

int gen_evaluate(node_t* root)
{

}

int gen_RO(node_t* root)
{

}


