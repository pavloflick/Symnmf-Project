# ifndef SYM_NMF
# define SYM_NMF


double **sym_c(double **pointsMatrix, int N, int dim); 
double **ddg_c(double **pointsMatrix, int N, int dim);
double **norm_c(double **pointsMatrix, int N, int dim);
double **symnmf_c(double **initialH, double **W, int N, int k);

void ptintMatrix(double **mtrx, int rows, int cols);        /*only to check the code, printing in this project is from s-f.py or s-f.c*/
void freeMatrix(double **mtrx, int rows);


# endif
