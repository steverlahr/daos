/**
 * (C) Copyright 2016 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */

#include "dsr_internal.h"

/** DSR object class */
struct daos_obj_class {
	/** class name */
	char				*oc_name;
	/** unique class ID */
	daos_oclass_id_t		 oc_id;
	struct daos_oclass_attr		 oc_attr;
};

/** predefined object classes */
static struct daos_obj_class daos_obj_classes[] = {
	{
		.oc_name	= "tiny_rw",
		.oc_id		= DAOS_OC_TINY_RW,
		{
			.ca_schema		= DAOS_OS_SINGLE,
			.ca_resil		= DAOS_RES_REPL,
			.ca_grp_nr		= 1,
			.u.repl			= {
				.r_num		= 1,
			},
		},
	},
	{
		.oc_name	= "small_rw",
		.oc_id		= DAOS_OC_SMALL_RW,
		{
			.ca_schema		= DAOS_OS_STRIPED,
			.ca_resil		= DAOS_RES_REPL,
			.ca_grp_nr		= 4,
			.u.repl			= {
				.r_num		= 1,
			},
		},
	},
	{
		.oc_name	= "large_rw",
		.oc_id		= DAOS_OC_LARGE_RW,
		{
			.ca_schema		= DAOS_OS_STRIPED,
			.ca_resil		= DAOS_RES_REPL,
			.ca_grp_nr		= DAOS_OC_GRP_MAX,
			.u.repl			= {
				.r_num		= 1,
			},
		},
	},
	{
		.oc_name	= "replica_rw",
		.oc_id		= DAOS_OC_REPLICA_RW,
		{
			.ca_schema		= DAOS_OS_STRIPED,
			.ca_resil		= DAOS_RES_REPL,
			.ca_grp_nr		= DAOS_OC_GRP_MAX,
			.u.repl			= {
				.r_num		= 2,
			},
		},
	},
	{
		.oc_name	= NULL,
		.oc_id		= DAOS_OC_UNKNOWN,
	},
};

/** find the object class attributes for the provided @oid */
struct daos_oclass_attr *
daos_oclass_attr_find(daos_obj_id_t oid)
{
	struct daos_obj_class	*oc;
	daos_oclass_id_t	 ocid;

	/* see daos_objid_generate */
	ocid = daos_obj_id2class(oid);
	for (oc = &daos_obj_classes[0]; oc->oc_id != DAOS_OC_UNKNOWN; oc++) {
		if (oc->oc_id == ocid)
			break;
	}

	if (ocid == DAOS_OC_UNKNOWN) {
		D_DEBUG(DF_SR, "Unknown object class %d for "DF_OID"\n",
			ocid, DP_OID(oid));
		return NULL;
	}

	D_DEBUG(DF_SR, "Find class %s for oid "DF_OID"\n",
		oc->oc_name, DP_OID(oid));
	return &oc->oc_attr;
}

/** Return the redundancy group size of @oc_attr */
int
daos_oclass_grp_size(struct daos_oclass_attr *oc_attr)
{
	switch (oc_attr->ca_resil) {
	default:
		return -DER_INVAL;

	case DAOS_RES_REPL:
		return oc_attr->u.repl.r_num;

	case DAOS_RES_EC:
		return oc_attr->u.ec.e_grp_size;
	}
}

int
daos_oclass_register(daos_handle_t coh, daos_oclass_id_t cid,
		    daos_oclass_attr_t *cattr, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
daos_oclass_query(daos_handle_t coh, daos_oclass_id_t cid,
		 daos_oclass_attr_t *cattr, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
daos_oclass_list(daos_handle_t coh, daos_oclass_list_t *clist,
		daos_hash_out_t *anchor, daos_event_t *ev)
{
	return -DER_NOSYS;
}

/**
 * Return the number of redundancy groups for the object class @oc_attr with
 * the provided metadata @md
 */
int
daos_oclass_grp_nr(struct daos_oclass_attr *oc_attr, struct daos_obj_md *md)
{
	/* NB: @md is unsupported for now */
	return oc_attr->ca_grp_nr;
}
