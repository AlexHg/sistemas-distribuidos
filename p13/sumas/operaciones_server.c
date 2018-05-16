/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "operaciones.h"

struct Operandos *
suma_1_svc(struct Operandos *argp, struct svc_req *rqstp)
{
	static struct Operandos  result;

	/*
	 * insert server code here
	 */
	printf("%s\n", "suma_1_svc");
	result.res = argp->i + argp->j;

	return &result;
}

struct Operandos *
producto_1_svc(struct Operandos *argp, struct svc_req *rqstp)
{
	static struct Operandos  result;

	/*
	 * insert server code here
	 */
	printf("%s\n", "producto_1_svc");

	result.res = argp->i * argp->j;

	return &result;
}

struct Operandos *
cociente_1_svc(struct Operandos *argp, struct svc_req *rqstp)
{
	static struct Operandos  result;

	/*
	 * insert server code here
	 */
	printf("%s\n", "cociente_1_svc");
	result.res = argp->i /(double) argp->j;

	return &result;
}