/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __ELE_CRYPTO_DRIVER_H__
#define __ELE_CRYPTO_DRIVER_H__

#include "fsl_common.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup ele_crypto
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief Defines ELE Crypto driver version 2.0.0.
 *
 * Change log:
 * - Version 2.0.0
 *   - initial version
 */
#define FSL_ELE_CRYPTO_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/


/*******************************************************************************
 * HASH Definitions
 ******************************************************************************/
/*!
 * @addtogroup ele_driver_hash
 * @{
 */

/*! @brief Supported cryptographic block cipher functions for HASH creation */
typedef enum _hash_algo_t
{
    kELE_Sha224 = 0x00,      /*!< SHA_224  */
    kELE_Sha256 = 0x01,      /*!< SHA_256  */
    kELE_Sha384 = 0x02,      /*!< SHA_384  */
    kELE_Sha512 = 0x03,      /*!< SHA_512  */
    kELE_SM3256 = 0x11,      /*!< SM3_256  */
} hash_algo_t;

/*!
 *@}
 */ /* end of ele_driver_hash */

/*******************************************************************************
 * KEYSTORE Definitions
 ******************************************************************************/
/*!
 * @addtogroup ele_driver_keystore
 * @{
 */


/*! @brief ELE KeyStore structure. */
typedef struct _ele_keystore
{
    uint32_t id;          /*!< User defined word identifying the key store */
    uint32_t nonce;       /*!< Nonce used as authentication proof for accessing the key store */
    uint16_t max_updates; /*!< Maximum number of updates authorized on this storage. */
    bool create;          /*!< If set TRUE, keystore is created, if FALSE load operation is requested */
    bool min_mac_check;   /*!< If set TRUE, minimal MAC length check is used */
    uint8_t min_mac_len;  /*!< The minimum MAC length (in bits) accepted by the EdgeLock Enclave to perform MAC verification operations */
} ele_keystore_t;

/*!
 *@}
 */ /* end of ele_driver_keystore */

/*******************************************************************************
 * KEY GENERATE Definitions
 ******************************************************************************/
/*!
 * @addtogroup ele_driver_key_gen
 * @{
 */

/*! @brief Key types */
typedef enum _key_get_type_t
{
    kKey_ECDSA_NIST_P256          = 0x02u,
    kKey_ECDSA_NIST_P384          = 0x03u,
    kKey_ECDSA_NIST_P521          = 0x04u,
    kKey_ECDSA_BRAINPOOL_R1_256   = 0x13u,
    kKey_ECDSA_BRAINPOOL_R1_320   = 0x14u,
    kKey_ECDSA_BRAINPOOL_R1_384   = 0x15u,
    kKey_ECDSA_BRAINPOOL_R1_512   = 0x16u,
    kKey_ECDSA_BRAINPOOL_T1_256   = 0x23u,
    kKey_ECDSA_BRAINPOOL_T1_320   = 0x24u,
    kKey_ECDSA_BRAINPOOL_T1_384   = 0x25u,
    kKey_ECDSA_BRAINPOOL_T1_512   = 0x26u,
    kKey_AES_128                  = 0x30u,
    kKey_AES_192                  = 0x31u,
    kKey_AES_256                  = 0x32u,
    kKey_DSA_SM2_FP_256           = 0x42u,
    kKey_SM4_128                  = 0x50u,
    kKey_HMAC_224                 = 0x60u,
    kKey_HMAC_256                 = 0x61u,
    kKey_HMAC_384                 = 0x62u,
    kKey_HMAC_512                 = 0x63u,
} key_get_type_t;

/*! @brief Key generation structure */
typedef struct _ele_gen_key
{
    uint32_t key_id;           /*!< Key ID. In case of “create” operation a new key identifier is created */
    key_get_type_t key_type;   /*!< Key Type */
    uint16_t key_group;        /*!< Key Group It must be a value in the range 0-1023 */
    bool create;               /*!< When set create a new key */
    bool update;               /*!< When set update an existing key */
    bool transient;            /*!< When set the key is transient, persistent otherwise */
    bool permanent;            /*!< When set, the key is permanent TODO: Not available now */
    bool master;               /*!< When set the key is Master. Only master keys can be used as input of key derivation functions */
    bool kek;                  /*!< When set, the key is considered as a KEK. It can only be used to import keys in the key store */
    uint32_t out_key_addr;     /*!< Address where to store the public key */
    uint16_t out_size;         /*!< Size in bytes of the output where to store the generated public key. */
                               /*!< Ignored if a symmetric key is generated.*/
} ele_gen_key_t;

/*!
 *@}
 */ /* end of ele_driver_key_gen */

/*******************************************************************************
 * SIGN and VERIFY Definitions
 ******************************************************************************/
/*!
 * @addtogroup ele_driver_sign_verify
 * @{
 */

/*! @brief Signature scheme  */
typedef enum _key_sig_scheme_t
{
    kECDSA_NIST_224_SHA224 =	0x00u,
    kECDSA_NIST_256_SHA256 =	0x02u,
    kECDSA_NIST_384_SHA384 =	0x03u,
    kECDSA_NIST_521_SHA512 =	0x04u,
    kECDSA_BRAINPOOL_R1_256_SHA256 =	0x13u,
    kECDSA_BRAINPOOL_R1_384_SHA384 =	0x15u,
    kECDSA_BRAINPPOL_T1_256_SHA256 =	0x23u,
    kECDSA_BRAINPOOL_T1_384_SHA384 =	0x25u,
    kRSA_SSA_PKCS1_V1_5_SHA1 =	0x70u,
    kRSA_SSA_PKCS1_V1_5_SHA224 =	0x71u,
    kRSA_SSA_PKCS1_V1_5_SHA256 =	0x72u,
    kRSA_SSA_PKCS1_V1_5_SHA384 =	0x73u,
    kRSA_SSA_PKCS1_V1_5_SHA512 =	0x74u,
    kRSA_SSA_PKCS1_PSS_MGF1_SHA1 =	0x75u,
    kRSA_SSA_PKCS1_PSS_MGF1_SHA224 =	0x76u,
    kRSA_SSA_PKCS1_PSS_MGF1_SHA256 =	0x77u,
    kRSA_SSA_PKCS1_PSS_MGF1_SHA384 =	0x78u,
    kRSA_SSA_PKCS1_PSS_MGF1_SHA512 =	0x79u,
} key_sig_scheme_t;

/*! @brief Sign generation structure */
typedef struct _ele_sign
{
    uint32_t key_id;           /*!< Key ID to be used for signing */
    uint8_t const *msg;        /*!< Address where the data to be signed can be found */
    uint32_t msg_size;         /*!< Size of message */
    uint8_t *signature;        /*!< Address where the signature will be written */
    uint16_t sig_size;         /*!< Size of signature */
    key_sig_scheme_t scheme;   /*!< Used signature scheme */
    bool input_flag;           /*!< When set input is the actual message, if not message digest */
} ele_sign_t;

/*! @brief Verify generation structure */
typedef struct _ele_verify
{
    uint8_t const *pub_key;    /*!< Public Key to be used for verification */
    uint16_t key_size;         /*!< Size of public key */
    uint8_t const *msg;        /*!< Address where the data to be signed can be found */
    uint32_t msg_size;         /*!< Size of message */
    uint8_t *signature;        /*!< Address where the signature will be written */
    uint16_t sig_size;         /*!< Size of signature */
    key_sig_scheme_t scheme;   /*!< Used signature scheme */
    bool input_flag;           /*!< When set input is the actual message, if not message digest */
    bool compresed;            /*!< When set signature is compressed */
    bool internal;             /*!< When set key is an internal reference (See import Public key) */
} ele_verify_t;

/*!
 *@}
 */ /* end of ele_driver_sign_verify */
/*******************************************************************************
 * API
 *******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Load ELE FW
 *
 * This function Loads firmware into EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param fw pointer in system memory where FW can be found
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_LoadFw(S3MU_Type *mu, const uint8_t *fw);

/*!
 * @brief Open ELE Session
 *
 * This function opens Session for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param sessionID pointer where store unique session ID word
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenSession(S3MU_Type *mu, uint32_t *sessionID);

/*!
 * @brief Close ELE Session
 *
 * This function closes the Session for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param sessionID unique session ID obtained by calling ELE_OpenSession()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseSession(S3MU_Type *mu, uint32_t sessionID);

/*!
 * @brief Open ELE HASH Service
 *
 * This function opens HASH Service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param sessionID unique session ID obtained by calling ELE_OpenSession()
 * @param hashHandleID pointer where unique HASH handle ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenHashService(S3MU_Type *mu, uint32_t sessionID, uint32_t *hashHandleID);

/*!
 * @brief Close ELE HASH Service
 *
 * This function closes the HASH Service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param hashHandleID unique HASH handle ID obtained by calling ELE_OpenHashService()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseHashService(S3MU_Type *mu, uint32_t hashHandleID);

/*!
 * @brief ELE HASH
 *
 * This function computes HASH of a given message using EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param hashHandleID unique HASH handle ID obtained by calling ELE_OpenHashService()
 * @param msg pointer where data to be hashed can be found
 * @param out pointer to output HASH data (size is defined by used algorithm), user must ensure this memory is available to ELE
 * @param size size of data in byte
 * @param alg define used hash algorithm, see hash_algo_t
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Hash(S3MU_Type *mu, uint32_t hashHandleID, const uint8_t *msg, uint8_t *out, size_t size, hash_algo_t alg);

/*!
 * @brief Open and Create Key Store
 *
 * This function create key store and open key store services in the EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param sessionID unique session ID obtained by calling ELE_OpenSession()
 * @param conf pointer where keystore configuration structure can be found
 * @param keystoreHandleID pointer where unique Keystore handle ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenKeystore(S3MU_Type *mu, uint32_t sessionID, ele_keystore_t *conf, uint32_t *keystoreHandleID);

/*!
 * @brief Close Key Store
 *
 * This function closes key store and its services in the EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseKeystore(S3MU_Type *mu, uint32_t keystoreHandleID);

/*!
 * @brief Open ELE Key Management Service
 *
 * This function opens Key Management Service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 * @param keyHandleID pointer where unique key management handle ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenKeyService(S3MU_Type *mu, uint32_t keystoreHandleID, uint32_t *keyHandleID);

/*!
 * @brief Close ELE Key Management Service
 *
 * This function closes the Key Management Service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param keyHandleID unique key management handle ID obtained by calling ELE_OpenKeyService()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseKeyService(S3MU_Type *mu, uint32_t keyHandleID);

/*!
 * @brief ELE Generate Key
 *
 * This function generates a key inside given keys store in the EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param keyHandleID unique key management handle ID obtained by calling ELE_OpenKeyService()
 * @param conf pointer where key generate configuration structure can be found
 * @param keyID pointer where unique Key ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_GenerateKey(S3MU_Type *mu, uint32_t keyHandleID, ele_gen_key_t *conf, uint32_t *keyID);


/*!
 * @brief Open ELE Sign Service
 *
 * This function opens Sign generation service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 * @param signHandleID pointer where unique sign handle ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenSignService(S3MU_Type *mu, uint32_t keystoreHandleID, uint32_t *signHandleID);

/*!
 * @brief Close ELE Sign Service
 *
 * This function closes the Sign generation service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param signHandleID unique session ID obtained by calling ELE_OpenSignService()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseSignService(S3MU_Type *mu, uint32_t signHandleID);

/*!
 * @brief Open ELE Sign
 *
 * This function generates signiture based on provided data and keys using EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param signHandleID unique session ID obtained by calling ELE_OpenSignService()
 * @param conf pointer where sign generate configuration structure can be found
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Sign(S3MU_Type *mu, uint32_t signHandleID, ele_sign_t *conf);

/*!
 * @brief Open ELE Verify Service
 *
 * This function opens Verify service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param sessionID unique session ID obtained by calling ELE_OpenSession()
 * @param verifyHandleID pointer where unique verify handle ID word will be stored
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenVerifyService(S3MU_Type *mu, uint32_t sessionID, uint32_t *verifyHandleID);

/*!
 * @brief Close ELE Verify Service
 *
 * This function closes Verify service for EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param verifyHandleID unique session ID obtained by calling ELE_OpenVerifyService()
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseVerifyService(S3MU_Type *mu, uint32_t verifyHandleID);

/*!
 * @brief Open ELE Verify
 *
 * This function is used to proceed with a signature verification operation using EdgeLock Enclave.
 *
 * @param base MU peripheral base address
 * @param signHandleID unique session ID obtained by calling ELE_OpenVerifyService()
 * @param conf pointer where verification configuration structure can be found
 * @param result true if verification success, false otherwise
 *
 * @return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Verify(S3MU_Type *mu, uint32_t verifyHandleID, ele_verify_t *conf, bool *result);

#if defined(__cplusplus)
}
#endif

/*! @} */ /* end of group s3mu */

#endif /* __ELE_CRYPTO_H__ */
