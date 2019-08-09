/*-------------------------------------------------------------------------
 *
 * nbtree.c
 *	  Implementation of Lehman and Yao's btree management algorithm for
 *	  Postgres.
 *
 * NOTES
 *	  This file contains only the public interface routines.
 *
 *
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/access/nbtree/nbtree.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/genam.h"
#include "access/noop.h"
#include "access/reloptions.h"
#include "access/relscan.h"

/*
 *	noopbuild() -- build a new noop index.
 */
Datum
noopbuild(PG_FUNCTION_ARGS)
{
	Relation	index = (Relation) PG_GETARG_POINTER(1);
	IndexBuildResult *result;

	/*
	 * Does it make sense for us to be called more than once for any index
	 * relation?
	 * Error on the cautious side for now.
	 */
	if (RelationGetNumberOfBlocks(index) != 0)
		elog(ERROR, "index \"%s\" already contains data",
			 RelationGetRelationName(index));

	/*
	 * Return statistics everything should be 0
	 */
	result = (IndexBuildResult *) palloc0(sizeof(IndexBuildResult));

	PG_RETURN_POINTER(result);
}

/*
 *	noopinsert() -- returns always true
 */
Datum
noopinsert(PG_FUNCTION_ARGS)
{
	PG_RETURN_BOOL(true);
}

/*
 *	noopbeginscan() -- provide an am for the dummy index.
 */
Datum
noopbeginscan(PG_FUNCTION_ARGS)
{
	Relation	rel = (Relation) PG_GETARG_POINTER(0);
	int			nkeys = 0;
	int			norderbys = 0;
	IndexScanDesc scan;

	/* Initialize the descriptor that the rest of the am need. */
	scan = RelationGetIndexScan(rel, nkeys, norderbys);

	PG_RETURN_POINTER(scan);
}

/*
 *	noopgettuple() -- provide an am the dummy index.
 */
Datum
noopgettuple(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);

	/* noop indexes are never lossy, how can they be? */
	scan->xs_recheck = false;

	PG_RETURN_BOOL(false);
}

/*
 * noopgetbitmap() -- construct a TIDBitmap.
 */
Datum
noopgetbitmap(PG_FUNCTION_ARGS)
{
	Node	   *n = (Node *) PG_GETARG_POINTER(1);
	TIDBitmap  *tbm;

	if (n == NULL)
		tbm = tbm_create(work_mem * 1024L);
	else
		tbm = (TIDBitmap *)n;

	PG_RETURN_POINTER(tbm);
}

/*
 *	nooprescan() -- rescan an index relation
 */
Datum
nooprescan(PG_FUNCTION_ARGS)
{
	PG_RETURN_VOID();
}

/*
 *	noopendscan() -- close down a scan
 */
Datum
noopendscan(PG_FUNCTION_ARGS)
{
	PG_RETURN_VOID();
}

/*
 *	noopmarkpos() -- save current scan position
 */
Datum
noopmarkpos(PG_FUNCTION_ARGS)
{
	PG_RETURN_VOID();
}

/*
 *	nooprestrpos() -- restore scan to last saved position
 */
Datum
nooprestrpos(PG_FUNCTION_ARGS)
{
	PG_RETURN_VOID();
}

/*
 * noopbulkdelete() -- bulk deletion am for the dummy index.
 */
Datum
noopbulkdelete(PG_FUNCTION_ARGS)
{
	IndexBulkDeleteResult *volatile stats = (IndexBulkDeleteResult *) PG_GETARG_POINTER(1);

	Assert(stats == NULL);

	PG_RETURN_POINTER(stats);
}

/*
 * noopvacuumcleanup() -- post-VACUUM cleanup.
 *
 * Should always return a null pointer
 */
Datum
noopvacuumcleanup(PG_FUNCTION_ARGS)
{
	IndexBulkDeleteResult *stats = (IndexBulkDeleteResult *) PG_GETARG_POINTER(1);

	Assert(stats == NULL);

	PG_RETURN_POINTER(stats);
}

/*
 * noopcanreturn() -- check whether noop indexes support index-only scans.
 *
 * noop always do not
 */
Datum
noopcanreturn(PG_FUNCTION_ARGS)
{
	PG_RETURN_BOOL(false);
}

/*
 * noopcostestimate() -- provide a cost estimation to the planner
 *
 * XXX: should that be in selfuncs.c?
 */
Datum
noopcostestimate(PG_FUNCTION_ARGS)
{
	elog(ERROR, "this function should not be called");

	PG_RETURN_VOID();
}

/*
 * noopoptions() -- provide the options for the method
 */
Datum
noopoptions(PG_FUNCTION_ARGS)
{
	Datum		reloptions = PG_GETARG_DATUM(0);
	bool		validate = PG_GETARG_BOOL(1);
	bytea	   *result;

	result = default_reloptions(reloptions, validate, RELOPT_KIND_NOOP);
	if (result)
		PG_RETURN_BYTEA_P(result);
	PG_RETURN_NULL();
}
