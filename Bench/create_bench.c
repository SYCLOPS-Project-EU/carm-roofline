#include "config_test.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					CREATE FP TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_benchmark_flops(char * op, char * precision, int long long fp, int Vlen, int LMUL){
	
	int flops, check;
	char * assembly_op_flops_1, * assembly_op_flops_2, * registr;
	
	if(strcmp(op,"mad") == 0){
		select_ISA_flops(&flops, &assembly_op_flops_1, "mul", precision);	//Select FP operation based on the ISA
		select_ISA_flops(&flops, &assembly_op_flops_2, "add", precision);	//Select FP operation based on the ISA
	}else{
		select_ISA_flops(&flops, &assembly_op_flops_1,op, precision);	//Select FP operation based on the ISA
	}

	select_ISA_flops_register(&registr, precision);

	write_asm_fp (fp, op, flops, registr, assembly_op_flops_1, assembly_op_flops_2, precision, Vlen, LMUL); 	//Write Assembly Code
	
	//Free auxiliary variables
	free(assembly_op_flops_1);
	if(strcmp(op,"mad") == 0) free(assembly_op_flops_2);
	
	check = system("make -f Test/Makefile_Benchmark");
	if (check != 0){
		printf("There was a problem making the benchmark");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					CREATE MEM TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_benchmark_mem(int long long num_rep, int num_ld, int num_st, char * precision, int Vlen, int LMUL){
	
	char * assembly_op, * assembly_op_2, * registr;
	int align, ops, check;
	
	select_ISA_mem(&align, &ops, &assembly_op, "load", precision);   //Select memory operation based on the ISA
	select_ISA_mem(&align, &ops, &assembly_op_2, "store", precision);

	select_ISA_mem_register(&registr, precision);

	write_asm_mem (num_rep, align, ops, num_ld, num_st, registr, assembly_op, assembly_op_2, precision, Vlen, LMUL); //Write ASM code
		
	//Free auxiliary variables
	free(assembly_op);
	
	check = system("make -f Test/Makefile_Benchmark");
	if (check != 0){
		printf("There was a problem making the benchmark");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					CREATE MIXED TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_benchmark_mixed(char * op, int long long num_rep, int num_ld, int num_st, int num_fp, char * precision, int Vlen, int LMUL){
	
	char * assembly_op, * assembly_op_2, * registr;
	int align, ops, check;

	int flops;//, check;
	char * assembly_op_flops_1, * assembly_op_flops_2, * registr_flops;
	
	if(strcmp(op,"mad") == 0){
		select_ISA_flops(&flops, &assembly_op_flops_1, "mul", precision);	//Select FP operation based on the ISA
		select_ISA_flops(&flops, &assembly_op_flops_2, "add", precision);	//Select FP operation based on the ISA
	}else{
		select_ISA_flops(&flops, &assembly_op_flops_1, op, precision);	//Select FP operation based on the ISA
	}

	select_ISA_flops_register(&registr_flops, precision);
	
	select_ISA_mem(&align, &ops, &assembly_op, "load", precision);   //Select memory operation based on the ISA
	select_ISA_mem(&align, &ops, &assembly_op_2, "store", precision);

	select_ISA_mem_register(&registr, precision);

	write_asm_mixed (num_rep, align, op, ops, num_ld, num_st, num_fp, registr, registr_flops, assembly_op, assembly_op_2, assembly_op_flops_1, assembly_op_flops_2, precision, Vlen, LMUL); //Write ASM code
		
	//Free auxiliary variables
	free(assembly_op);
	
	check = system("make -f Test/Makefile_Benchmark");
	if (check != 0){
		printf("There was a problem making the benchmark");
	}
}