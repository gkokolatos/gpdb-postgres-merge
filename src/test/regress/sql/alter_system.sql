-- Set up, create a wrapper around pg_file_settings that will execute on all
-- segments and the QD node
CREATE FUNCTION gp_file_segment_settings()
RETURNS TABLE (
	gp_segment_id int,
	sourcefile text,
	sourceline int,
	seqno int,
	name text,
	setting text,
	applied boolean,
	error text
) AS
$$
    SELECT pg_catalog.gp_execution_segment() AS gp_segment_id,
		substring(a.sourcefile, 'demoDataDir.*$') as sourcefile,
		a.sourceline,
		a.seqno,
		a.name,
		a.setting,
		a.applied,
		a.error
    FROM pg_catalog.pg_file_settings a
$$
LANGUAGE SQL EXECUTE ON ALL SEGMENTS;

CREATE VIEW gp_file_settings AS
	SELECT * FROM gp_file_segment_settings()
	UNION ALL
	SELECT
		pg_catalog.gp_execution_segment() AS gp_segment_id,
		substring(sourcefile, 'demoDataDir.*$') as sourcefile,
		sourceline,
		seqno,
		name,
		setting,
		applied,
		error
	FROM pg_file_settings;

CREATE FUNCTION gp_segment_reload_conf()
RETURNS TABLE (
	gp_segment_id int,
	reload_conf boolean
) AS
$$
	SELECT pg_catalog.gp_execution_segment() AS gp_segment_id,
	pg_reload_conf() as reload_conf
$$
LANGUAGE SQL EXECUTE ON ALL SEGMENTS;

CREATE VIEW gp_reload_conf AS
 	SELECT * FROM gp_segment_reload_conf()
	UNION ALL
	SELECT pg_catalog.gp_execution_segment() AS gp_segment_id,
		pg_reload_conf() AS reload_conf;

-- Vanilla command runs only on master node
ALTER SYSTEM SET gp_segment_connect_timeout = '30s';
SELECT * FROM gp_file_settings WHERE name = 'gp_segment_connect_timeout';
SELECT * FROM gp_reload_conf;
ALTER SYSTEM RESET gp_segment_connect_timeout;
SELECT * FROM gp_reload_conf;
SELECT * FROM gp_file_settings WHERE name = 'gp_segment_connect_timeout';

-- With DISPATCH it should apply to all nodes
ALTER SYSTEM SET gp_segment_connect_timeout = '100s' DISPATCH;
SELECT * FROM gp_reload_conf;
SELECT * FROM gp_file_settings WHERE name = 'gp_segment_connect_timeout';
ALTER SYSTEM RESET gp_segment_connect_timeout DISPATCH;
SELECT * FROM gp_reload_conf;
SELECT * FROM gp_file_settings WHERE name = 'gp_segment_connect_timeout';

-- Clean up
DROP VIEW IF EXISTS gp_file_settings;
DROP VIEW IF EXISTS gp_reload_conf;
DROP FUNCTION IF EXISTS gp_file_segment_settings();
DROP FUNCTION IF EXISTS gp_segment_reload_conf();
