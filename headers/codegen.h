#ifndef CODEGEN_H_
#define CODEGEN_H_
#include "node.h"

int gen_program(node_t* root);

int gen_block(node_t* root);

int gen_vars(node_t* root);

int gen_mvars(node_t* root);

int gen_expr(node_t* root);

int gen_xhelp(node_t* root);

int gen_M(node_t* root);

int gen_R(node_t* root);

int gen_stats(node_t* root);

int gen_mstat(node_t* root);

int gen_stat(node_t* root);

int gen_in(node_t* root);

int gen_out(node_t* root);

int gen_assign(node_t* root);

int gen_iff(node_t* root);

int gen_iter(node_t* root);

int gen_evaluate(node_t* root, int ifC);

int gen_RO(node_t* root);

#endif
