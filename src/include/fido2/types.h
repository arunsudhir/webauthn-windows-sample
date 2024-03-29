/*
 * Copyright (c) 2018 Yubico AB. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef _TYPES_H
#define _TYPES_H

#include "packed.h"
#include "blob.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* COSE ES256 (ECDSA over P-256 with SHA-256) public key */
typedef struct es256_pk {
	unsigned char	x[32];
	unsigned char	y[32];
} es256_pk_t;

/* COSE ES256 (ECDSA over P-256 with SHA-256) (secret) key */
typedef struct es256_sk {
	unsigned char	d[32];
} es256_sk_t;

/* COSE RS256 (2048-bit RSA with PKCS1 padding and SHA-256) public key */
typedef struct rs256_pk {
	unsigned char n[256];
	unsigned char e[3];
} rs256_pk_t;

/* COSE EDDSA (ED25519) */
typedef struct eddsa_pk {
	unsigned char x[32];
} eddsa_pk_t;

PACKED_TYPE(fido_authdata_t,
struct fido_authdata {
	unsigned char rp_id_hash[32]; /* sha256 of fido_rp.id */
	uint8_t       flags;          /* user present/verified */
	uint32_t      sigcount;       /* signature counter */
	/* actually longer */
})

PACKED_TYPE(fido_attcred_raw_t,
struct fido_attcred_raw {
	unsigned char aaguid[16]; /* credential's aaguid */
	uint16_t      id_len;     /* credential id length */
	uint8_t       body[];     /* credential id + pubkey */
})

typedef struct fido_attcred {
	unsigned char aaguid[16]; /* credential's aaguid */
	fido_blob_t   id;         /* credential id */
	int           type;       /* credential's cose algorithm */
	union {                   /* credential's public key */
		es256_pk_t es256;
		rs256_pk_t rs256;
		eddsa_pk_t eddsa;
	} pubkey;
} fido_attcred_t;

typedef struct fido_attstmt {
	fido_blob_t x5c; /* attestation certificate */
	fido_blob_t sig; /* attestation signature */
} fido_attstmt_t;

typedef struct fido_rp {
	char *id;   /* relying party id */
	char *name; /* relying party name */
} fido_rp_t;

typedef struct fido_user {
	fido_blob_t  id;           /* required */
	char        *icon;         /* optional */
	char        *name;         /* optional */
	char        *display_name; /* required */
} fido_user_t;

typedef struct fido_cred {
	fido_blob_t       cdh;           /* client data hash */
	fido_rp_t         rp;            /* relying party */
	fido_user_t       user;          /* user entity */
	fido_blob_array_t excl;          /* list of credential ids to exclude */
	fido_opt_t        rk;            /* resident key */
	fido_opt_t        uv;            /* user verification */
	int               ext;           /* enabled extensions */
	int               type;          /* cose algorithm */
	char             *fmt;           /* credential format */
	int               authdata_ext;  /* decoded extensions */
	fido_blob_t       authdata_cbor; /* raw cbor payload */
	fido_authdata_t   authdata;      /* decoded authdata payload */
	fido_attcred_t    attcred;       /* returned credential (key + id) */
	fido_attstmt_t    attstmt;       /* attestation statement (x509 + sig) */
} fido_cred_t;

typedef struct _fido_assert_stmt {
	fido_blob_t     id;              /* credential id */
	fido_user_t     user;            /* user attributes */
	fido_blob_t     hmac_secret_enc; /* hmac secret, encrypted */
	fido_blob_t     hmac_secret;     /* hmac secret */
	int             authdata_ext;    /* decoded extensions */
	fido_blob_t     authdata_cbor;   /* raw cbor payload */
	fido_authdata_t authdata;        /* decoded authdata payload */
	fido_blob_t     sig;             /* signature of cdh + authdata */
} fido_assert_stmt;

typedef struct fido_assert {
	char              *rp_id;        /* relying party id */
	fido_blob_t        cdh;          /* client data hash */
	fido_blob_t        hmac_salt;    /* optional hmac-secret salt */
	fido_blob_array_t  allow_list;   /* list of allowed credentials */
	fido_opt_t         up;           /* user presence */
	fido_opt_t         uv;           /* user verification */
	int                ext;          /* enabled extensions */
	fido_assert_stmt  *stmt;         /* array of expected assertions */
	size_t             stmt_cnt;     /* number of allocated assertions */
	size_t             stmt_len;     /* number of received assertions */
} fido_assert_t;

typedef struct fido_opt_array {
	char **name;
	bool *value;
	size_t len;
} fido_opt_array_t;

typedef struct fido_str_array {
	char **ptr;
	size_t len;
} fido_str_array_t;

typedef struct fido_byte_array {
	uint8_t *ptr;
	size_t len;
} fido_byte_array_t;

typedef struct fido_cbor_info {
	fido_str_array_t  versions;   /* supported versions: fido2|u2f */
	fido_str_array_t  extensions; /* list of supported extensions */
	unsigned char     aaguid[16]; /* aaguid */
	fido_opt_array_t  options;    /* list of supported options */
	uint64_t          maxmsgsiz;  /* maximum message size */
	fido_byte_array_t protocols;  /* supported pin protocols */
} fido_cbor_info_t;

typedef struct fido_dev_info {
	char    *path;         /* device path */
	int16_t  vendor_id;    /* 2-byte vendor id */
	int16_t  product_id;   /* 2-byte product id */
	char    *manufacturer; /* manufacturer string */
	char    *product;      /* product string */
} fido_dev_info_t;

PACKED_TYPE(fido_ctap_info_t,
/* defined in section 8.1.9.1.3 (CTAPHID_INIT) of the fido2 ctap spec */
struct fido_ctap_info {
	uint64_t nonce;    /* echoed nonce */
	uint32_t cid;      /* channel id */
	uint8_t  protocol; /* ctaphid protocol id */
	uint8_t  major;    /* major version number */
	uint8_t  minor;    /* minor version number */
	uint8_t  build;    /* build version number */
	uint8_t  flags;    /* capabilities flags; see FIDO_CAP_* */
})

typedef struct fido_dev {
	uint64_t          nonce;     /* issued nonce */
	fido_ctap_info_t  attr;      /* device attributes */
	uint32_t          cid;       /* assigned channel id */
	void		 *io_handle; /* abstract i/o handle */
	fido_dev_io_t	  io;        /* i/o functions & data */
} fido_dev_t;

#ifdef __cplusplus
}
#endif

#endif /* !_TYPES_H */
