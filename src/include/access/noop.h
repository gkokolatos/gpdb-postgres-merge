/*-------------------------------------------------------------------------
 *
 * noop.h
 *	  header file for greenplum noop access method implementation.
 *
 * src/include/access/noop.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef NOOP_H
#define NOOP_H

/*
 * prototypes for functions in noop.c (external entry points for noop)
 */
extern Datum noopbuild(PG_FUNCTION_ARGS);
extern Datum noopinsert(PG_FUNCTION_ARGS);
extern Datum noopbeginscan(PG_FUNCTION_ARGS);
extern Datum noopgettuple(PG_FUNCTION_ARGS);
extern Datum noopgetbitmap(PG_FUNCTION_ARGS);
extern Datum nooprescan(PG_FUNCTION_ARGS);
extern Datum noopendscan(PG_FUNCTION_ARGS);
extern Datum noopmarkpos(PG_FUNCTION_ARGS);
extern Datum nooprestrpos(PG_FUNCTION_ARGS);
extern Datum noopbulkdelete(PG_FUNCTION_ARGS);
extern Datum noopvacuumcleanup(PG_FUNCTION_ARGS);
extern Datum noopcanreturn(PG_FUNCTION_ARGS);
extern Datum noopcostestimate(PG_FUNCTION_ARGS);
extern Datum noopoptions(PG_FUNCTION_ARGS);


#endif /* NOOP_H */
