/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "fsl_ele_crypto.h"
#include "fsl_s3mu.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/
#define RESPONSE_SUCCESS (0x0u)

#define LOAD_FW                  (0x17020406u)
#define LOAD_FW_SIZE             (0x4u)
#define LOAD_FW_RESPONSE_HDR     (0xe1020206u)
#define LOAD_FW_RESPONSE_SUCCESS (0x000000d6u)

#define OPEN_SESSION              (0x17100307u)
#define OPEN_SESSION_SIZE         (0x3u)
#define OPEN_SESSION_RESPONSE_HDR (0xe1100307u)

#define CLOSE_SESSION              (0x17110207u)
#define CLOSE_SESSION_SIZE         (0x2u)
#define CLOSE_SESSION_RESPONSE_HDR (0xe1110207u)

#define OPEN_HASH_SESSION              (0x17900607u)
#define OPEN_HASH_SESSION_SIZE         (0x6u)
#define OPEN_HASH_SESSION_RESPONSE_HDR (0xe1900307u)

#define CLOSE_HASH_SESSION              (0x17910207u)
#define CLOSE_HASH_SESSION_SIZE         (0x2u)
#define CLOSE_HASH_SESSION_RESPONSE_HDR (0xe1910207u)

#define HASH_ONE_GO              (0x17920807u)
#define HASH_ONE_GO_SIZE         (0x8u)
#define HASH_ONE_GO_RESPONSE_HDR (0xe1920207u)

#define OPEN_KEY_STORE              (0x17300607u)
#define OPEN_KEY_STORE_SIZE         (0x6u)
#define OPEN_KEY_STORE_RESPONSE_HDR (0xe1300307u)
#define MIN_MAC_LEN_SHIFT           (24u)
#define MIN_MAC_FLAG_SHIFT          (19u)
#define KEYSTORE_CREATE_SHIFT       (16u)

#define CLOSE_KEY_STORE              (0x17310307u)
#define CLOSE_KEY_STORE_SIZE         (0x3u)
#define CLOSE_KEY_STORE_RESPONSE_HDR (0xe1310207u)

#define KEY_MNG_OPEN              (0x17400607u)
#define KEY_MNG_OPEN_SIZE         (0x6u)
#define KEY_MNG_OPEN_RESPONSE_HDR (0xe1400307u)

#define KEY_MNG_CLOSE              (0x17410207u)
#define KEY_MNG_CLOSE_SIZE         (0x2u)
#define KEY_MNG_CLOSE_RESPONSE_HDR (0xe1410207u)

#define KEY_GEN                 (0x17420707u)
#define KEY_GEN_SIZE            (0x7u)
#define KEY_GEN_RESPONSE_HDR    (0xe1420307u)
#define KEY_GEN_UPDATE_SHIFT    (16u)
#define KEY_GEN_CREATE_SHIFT    (17u)
#define KEY_GEN_TYPE_SHIFT      (24u)
#define KEY_GEN_PERMANENT_SHIFT (16u)
#define KEY_GEN_TRANSINT_SHIFT  (17u)
#define KEY_GEN_MASTER_SHIFT    (18u)
#define KEY_GEN_KEK_SHIFT       (19u)

#define SIGN_OPEN              (0x17700607u)
#define SIGN_OPEN_SIZE         (0x6u)
#define SIGN_OPEN_RESPONSE_HDR (0xe1700307u)

#define SIGN_CLOSE              (0x17710207u)
#define SIGN_CLOSE_SIZE         (0x2u)
#define SIGN_CLOSE_RESPONSE_HDR (0xe1710207u)

#define SIGN_CMD          (0x17720807u)
#define SIGN_SIZE         (0x8u)
#define SIGN_RESPONSE_HDR (0xe1720207u)
#define SIG_SCHEME_SHIFT  (16u)
#define SIG_FLAGS_SHIFT   (24u)

#define VERIFY_OPEN              (0x17800607u)
#define VERIFY_OPEN_SIZE         (0x6u)
#define VERIFY_OPEN_RESPONSE_HDR (0xe1800307u)

#define VERIFY_CLOSE              (0x17810207u)
#define VERIFY_CLOSE_SIZE         (0x2u)
#define VERIFY_CLOSE_RESPONSE_HDR (0xe1810207u)

#define VERIFY_CMD                 (0x17820907u)
#define VERIFY_SIZE                (0x9)
#define VERIFY_RESPONSE_HDR        (0xe1820307)
#define VERIFY_RESPONSE_SUCCESS    (0x5a3cc3a5)
#define VERIFY_SIG_SIZE_SHIFT      (16u)
#define VERIFY_FLAG_INPUT_SHIFT    (8u)
#define VERIFY_FLAG_COMP_SHIFT     (9u)
#define VERIFY_FLAG_INTERNAL_SHIFT (10u)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * brief Load ELE FW
 *
 * This function Loads firmware into EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param fw pointer in system memory where FW can be found
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_LoadFw(S3MU_Type *mu, const uint8_t *fw)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[LOAD_FW_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    /****************** Load EdgeLock FW message ***********************/
    tmsg[0] = LOAD_FW;      // LOAD_FW Command Header
    tmsg[1] = (uint32_t)fw; // EdgeLock FW address
    tmsg[2] = 0x0u;         // Reserved
    tmsg[3] = (uint32_t)fw; // EdgeLock FW address
    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, LOAD_FW_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == LOAD_FW_RESPONSE_HDR && rmsg[1] == LOAD_FW_RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Session
 *
 * This function opens Session for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param sessionID pointer to output unique session ID word
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenSession(S3MU_Type *mu, uint32_t *sessionID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[OPEN_SESSION_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = OPEN_SESSION; // OPEN_SESSION Command Header
    tmsg[1] = 0x02000001u;  // EdgeLock ID
    tmsg[2] = 0x00000000u;  // Operating mode | Priority
    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, OPEN_SESSION_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Check that response corresponds to the sent command */
    if (rmsg[0] == OPEN_SESSION_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Session ID data */
        *sessionID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close ELE Session
 *
 * This function closes the Session for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param sessionID unique session ID obtained by calling ELE_OpenSession()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseSession(S3MU_Type *mu, uint32_t sessionID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[CLOSE_SESSION_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = CLOSE_SESSION; // CLOSE_SESSION Command Header
    tmsg[1] = sessionID;     // Session ID

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, CLOSE_SESSION_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Check that response corresponds to the sent command */
    if (rmsg[0] == CLOSE_SESSION_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE HASH Service
 *
 * This function opens HASH Service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param sessionID unique session ID obtained by calling ELE_OpenSession()
 * param hashHandleID pointer to output unique HASH session handle ID word
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenHashService(S3MU_Type *mu, uint32_t sessionID, uint32_t *hashHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[OPEN_HASH_SESSION_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = OPEN_HASH_SESSION; // OPEN_SESSION Command Header
    tmsg[1] = sessionID;         // Session handle ID
    tmsg[2] = 0x00000000u;       // User Input address extension (UIA) - not used
    tmsg[3] = 0x00000000u;       // User Output address extension (UOA) - not used
    tmsg[4] = 0x00000000u;       // Flags (reserved)
    tmsg[5] = S3MU_ComputeMsgCrc(tmsg, OPEN_HASH_SESSION_SIZE - 1u);
    // CRC sum of all the words of the message (excluding the CRC itself)

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, OPEN_HASH_SESSION_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Check that response corresponds to the sent command */
    if (rmsg[0] == OPEN_HASH_SESSION_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Service ID data */
        *hashHandleID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close ELE HASH Service
 *
 * This function closes the HASH Service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param hashHandleID unique HASH handle ID obtained by calling ELE_OpenHashService()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseHashService(S3MU_Type *mu, uint32_t hashHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[CLOSE_HASH_SESSION_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = CLOSE_HASH_SESSION; // CLOSE_HASH_SESSION Command Header
    tmsg[1] = hashHandleID;       // Hash handle ID

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, CLOSE_HASH_SESSION_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Check that response corresponds to the sent command */
    if (rmsg[0] == CLOSE_HASH_SESSION_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief ELE HASH
 *
 * This function computes HASH of a given message using EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param hashHandleID unique HASH handle ID obtained by calling ELE_OpenHashService()
 * param msg pointer where data to be hashed can be found
 * param out pointer to output HASH data (size is defined by used algorithm), user must ensure this memory is available
 * to ELE param size size of data in byte param alg define used hash algorithm, see hash_algo_t
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Hash(S3MU_Type *mu, uint32_t hashHandleID, const uint8_t *msg, uint8_t *out, size_t size, hash_algo_t alg)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[HASH_ONE_GO_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];
    uint32_t outSize = 0u;

    switch (alg)
    {
        case kELE_Sha224:
            outSize = 224u / 8u;
            break;
        case kELE_Sha256:
            outSize = 256u / 8u;
            break;
        case kELE_Sha384:
            outSize = 384u / 8u;
            break;
        case kELE_Sha512:
            outSize = 512u / 8u;
            break;
        case kELE_SM3256:
            outSize = 256u / 8u;
            break;

        default:
            return kStatus_InvalidArgument;
    }

    tmsg[0] = HASH_ONE_GO;   // HASH_ONE_GO Command Header
    tmsg[1] = hashHandleID;  // Session handle ID
    tmsg[2] = (uint32_t)msg; // Input Hash address
    tmsg[3] = (uint32_t)out; // Output Hash address
    tmsg[4] = size;          // Input size
    tmsg[5] = outSize;       // Output size
    tmsg[6] = alg;           // Flag (reserved) | Alg = SHA256
    tmsg[7] = S3MU_ComputeMsgCrc(tmsg, HASH_ONE_GO_SIZE - 1u);
    // CRC sum of all the words of the message (excluding the CRC itself)

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, HASH_ONE_GO_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    if (S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT) != kStatus_Success)
    {
        return status;
    }
    /* Check that response corresponds to the sent command */
    if (rmsg[0] == HASH_ONE_GO_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open and Create Key Store
 *
 * This function create key store and open key store services in the EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param sessionID unique session ID obtained by calling ELE_OpenSession()
 * param conf pointer where keystore configuration structure can be found
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenKeystore(S3MU_Type *mu, uint32_t sessionID, ele_keystore_t *conf, uint32_t *keystoreHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[OPEN_KEY_STORE_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    /* Create Flag word */

    /*   +----------------+----------+------------------------+
     *   | Min MAC length |  Flags   |      Max updates       |
     *   +----------------+----------+------------------------+
     *   | bit 31-24      | Bit 23-16|       Bit 15-0         |
     *   +----------------+----------+------------------------+
     */
    uint32_t flags = (conf->min_mac_len << MIN_MAC_LEN_SHIFT) | (conf->min_mac_check << MIN_MAC_FLAG_SHIFT) |
                     (conf->create << KEYSTORE_CREATE_SHIFT) | (conf->max_updates);

    tmsg[0] = OPEN_KEY_STORE; // OPEN_KEY_STORE Command Header
    tmsg[1] = sessionID;      // Session handle ID
    tmsg[2] = conf->id;       // Key Store ID (User defined)
    tmsg[3] = conf->nonce;    // Authentication nonce
    tmsg[4] = flags;          // MAC Min lenght (bit 31-24) | Flag (bit 23-16) | MAX update: (bit 15-0)
    tmsg[5] = S3MU_ComputeMsgCrc(tmsg, OPEN_KEY_STORE_SIZE - 1u);

    /* Send message Security Sub-System */
    if (S3MU_SendMessage(mu, tmsg, OPEN_KEY_STORE_SIZE) != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    if (S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT) != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == OPEN_KEY_STORE_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Key Store ID data */
        *keystoreHandleID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close Key Store
 *
 * This function closes key store and its services in the EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseKeystore(S3MU_Type *mu, uint32_t keystoreHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[OPEN_KEY_STORE_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = CLOSE_KEY_STORE;  // CLOSE_KEY_STORE Command Header
    tmsg[1] = keystoreHandleID; // Key Store HandleID
    tmsg[2] = 0x0;              // Flags (reserved)

    /* Send message Security Sub-System */
    if (S3MU_SendMessage(mu, tmsg, CLOSE_KEY_STORE_SIZE) != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    if (S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT) != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == CLOSE_KEY_STORE_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Key Management Service
 *
 * This function opens Key Management Service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 * param keyHandleID pointer where unique key management handle ID word will be stored
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenKeyService(S3MU_Type *mu, uint32_t keystoreHandleID, uint32_t *keyHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[KEY_MNG_OPEN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = KEY_MNG_OPEN;     // KEY_MNG_OPEN Command Header
    tmsg[1] = keystoreHandleID; // Session handle ID
    tmsg[2] = 0x00000000u;      // User Input address extension (UIA) - not used
    tmsg[3] = 0x00000000u;      // User Output address extension (UOA) - not used
    tmsg[4] = 0x00000000u;      // Flag (reserved)
    tmsg[5] = S3MU_ComputeMsgCrc(tmsg, KEY_MNG_OPEN_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, KEY_MNG_OPEN_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == KEY_MNG_OPEN_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Key Management Handle ID data */
        *keyHandleID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close ELE Key Management Service
 *
 * This function closes the Key Management Service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param keyHandleID unique key management handle ID obtained by calling ELE_OpenKeyService()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseKeyService(S3MU_Type *mu, uint32_t keyHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[KEY_MNG_OPEN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = KEY_MNG_CLOSE; // KEY_MNG_CLOSE Command Header
    tmsg[1] = keyHandleID;   // Key Management handle ID

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, KEY_MNG_CLOSE_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == KEY_MNG_CLOSE_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief ELE Generate Key
 *
 * This function generates a key inside given keys store in the EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param keyHandleID unique key management handle ID obtained by calling ELE_OpenKeyService()
 * param conf pointer where key generate configuration structure can be found
 * param keyID pointer where unique Key ID word will be stored
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_GenerateKey(S3MU_Type *mu, uint32_t keyHandleID, ele_gen_key_t *conf, uint32_t *keyID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[KEY_GEN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    if (conf->key_group > 1023u)
    {
        return kStatus_OutOfRange;
    }

    /*   +--------------+------------+------------------------+
     *   | Key Type     |    Flags   |      Public key size   |
     *   +--------------+------------+------------------------+
     *   | bit 31-24    |   Bit 23-16|       Bit 15-0         |
     *   +--------------+------------+------------------------+
     */
    uint32_t flags_size_type = (conf->key_type << KEY_GEN_TYPE_SHIFT) | (conf->create << KEY_GEN_CREATE_SHIFT) |
                               (conf->update << KEY_GEN_UPDATE_SHIFT) | (conf->out_size);

    /*   +-------------------------+--------------------------+
     *   |        Key Info         |        Key Group         |
     *   +-------------------------+--------------------------+
     *   |        bit 31-16        |         Bit 15-0         |
     *   +-------------------------+--------------------------+
     */
    uint32_t info_group = (conf->kek << KEY_GEN_KEK_SHIFT) | (conf->master << KEY_GEN_MASTER_SHIFT) |
                          (conf->transient << KEY_GEN_TRANSINT_SHIFT) | (conf->permanent << KEY_GEN_PERMANENT_SHIFT) |
                          (conf->key_group);

    tmsg[0] = KEY_GEN;            // KEY_GEN Command Header
    tmsg[1] = keyHandleID;        // Session handle ID
    tmsg[2] = conf->key_id;       // User defined Key Identifier
    tmsg[3] = flags_size_type;    // Key Type | Flags | Out Size
    tmsg[4] = info_group;         // Key info | Key Group
    tmsg[5] = conf->out_key_addr; // Output buffer addres for public key
    tmsg[6] = S3MU_ComputeMsgCrc(tmsg, KEY_GEN_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, KEY_GEN_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == KEY_GEN_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Key pair ID data */
        *keyID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Sign Service
 *
 * This function opens Sign generation service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param keystoreHandleID unique session ID obtained by calling ELE_OpenKeystore()
 * param signHandleID pointer where unique sign handle ID word will be stored
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenSignService(S3MU_Type *mu, uint32_t keystoreHandleID, uint32_t *signHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[SIGN_OPEN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = SIGN_OPEN;        // SIGN_OPEN Command Header
    tmsg[1] = keystoreHandleID; // Key Store handle ID
    tmsg[2] = 0x00000000u;      // User Input address extension (UIA) - not used
    tmsg[3] = 0x00000000u;      // User Output address extension (UOA) - not used
    tmsg[4] = 0x00000000u;      // Flag (reserved)
    tmsg[5] = S3MU_ComputeMsgCrc(tmsg, SIGN_OPEN_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, SIGN_OPEN_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == SIGN_OPEN_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Key pair ID data */
        *signHandleID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close ELE Sign Service
 *
 * This function closes the Sign generation service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param signHandleID unique session ID obtained by calling ELE_OpenSignService()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseSignService(S3MU_Type *mu, uint32_t signHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[SIGN_CLOSE_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = SIGN_CLOSE;   // SIGN_CLOSE Command Header
    tmsg[1] = signHandleID; // Sign handle ID

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, SIGN_CLOSE_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == SIGN_CLOSE_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Sign
 *
 * This function generates signiture based on provided data and keys using EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param signHandleID unique session ID obtained by calling ELE_OpenSignService()
 * param conf pointer where sign generate configuration structure can be found
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Sign(S3MU_Type *mu, uint32_t signHandleID, ele_sign_t *conf)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[SIGN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    /*   +--------------+------------+------------------------+
     *   |   Flags      | Sig Scheme |      Signature size    |
     *   +--------------+------------+------------------------+
     *   | bit 31-24    | Bit 23-16  |       Bit 15-0         |
     *   +--------------+------------+------------------------+
     */
    uint32_t flag_scheme_size =
        (conf->input_flag << SIG_FLAGS_SHIFT) | (conf->scheme << SIG_SCHEME_SHIFT) | (conf->sig_size);

    tmsg[0] = SIGN_CMD;                  // SIGN_CMD Command Header
    tmsg[1] = signHandleID;              // Sign service handle ID
    tmsg[2] = conf->key_id;              // Key ID to be used for signing
    tmsg[3] = (uint32_t)conf->msg;       // Address where the message digest to be signed can be found.
    tmsg[4] = (uint32_t)conf->signature; // Address where the signature will be written
    tmsg[5] = conf->msg_size;            // Size of message
    tmsg[6] = flag_scheme_size;          // Flag | Signatrure scheme | size of signature + 8 bit for Ry
    tmsg[7] = S3MU_ComputeMsgCrc(tmsg, SIGN_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, SIGN_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == SIGN_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Verify Service
 *
 * This function opens Verify service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param sessionID unique session ID obtained by calling ELE_OpenSession()
 * param verifyHandleID pointer where unique verify handle ID word will be stored
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_OpenVerifyService(S3MU_Type *mu, uint32_t sessionID, uint32_t *verifyHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[VERIFY_OPEN_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = VERIFY_OPEN; // VERIFY_OPEN Command Header
    tmsg[1] = sessionID;   // Key Store handle ID
    tmsg[2] = 0x00000000u; // User Input address extension (UIA) - not used
    tmsg[3] = 0x00000000u; // User Output address extension (UOA) - not used
    tmsg[4] = 0x00000000u; // Flag (reserved)
    tmsg[5] = S3MU_ComputeMsgCrc(tmsg, VERIFY_OPEN_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, VERIFY_OPEN_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == VERIFY_OPEN_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* read Key pair ID data */
        *verifyHandleID = rmsg[2];
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Close ELE Verify Service
 *
 * This function closes Verify service for EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param verifyHandleID unique session ID obtained by calling ELE_OpenVerifyService()
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 */
status_t ELE_CloseVerifyService(S3MU_Type *mu, uint32_t verifyHandleID)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[VERIFY_CLOSE_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    tmsg[0] = VERIFY_CLOSE;   // VERIFY_OPEN Command Header
    tmsg[1] = verifyHandleID; // Verify handle ID

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, VERIFY_CLOSE_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == VERIFY_CLOSE_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

/*!
 * brief Open ELE Verify
 *
 * This function is used to proceed with a signature verification operation using EdgeLock Enclave.
 *
 * param base MU peripheral base address
 * param verifyHandleID unique session ID obtained by calling ELE_OpenVerifyService()
 * param conf pointer where verification configuration structure can be found
 * param result true if verification success, false otherwise
 *
 * return Status kStatus_Success if success, kStatus_Fail if fail
 * Possible errors: kStatus_S3MU_InvalidArgument, kStatus_S3MU_AgumentOutOfRange
 */
status_t ELE_Verify(S3MU_Type *mu, uint32_t verifyHandleID, ele_verify_t *conf, bool *result)
{
    status_t status = kStatus_Fail;
    uint32_t tmsg[VERIFY_SIZE];
    uint32_t rmsg[S3MU_RR_COUNT];

    /*   +-------------------------+--------------------------+
     *   |     Signature size      |      Public Key size     |
     *   +-------------------------+--------------------------+
     *   |        bit 31-16        |         Bit 15-0         |
     *   +-------------------------+--------------------------+
     */
    uint32_t sig_key_size = (conf->sig_size << VERIFY_SIG_SIZE_SHIFT) | (conf->key_size);

    /*   +------------------------+------------+--------------+
     *   |      Reserved          |  Flags     | Sign  Scheme |
     *   +------------------------+------------+--------------+
     *   |       Bit 31-16        | bit 15-8   |    Bit 7-0   |
     *   +------------------------+------------+--------------+
     */
    uint32_t flags_scheme = (conf->internal << VERIFY_FLAG_INTERNAL_SHIFT) |
                            (conf->compresed << VERIFY_FLAG_COMP_SHIFT) |
                            (conf->input_flag << VERIFY_FLAG_INPUT_SHIFT) | (conf->scheme);

    tmsg[0] = VERIFY_CMD;                // VERIFY_CMD Command Header
    tmsg[1] = verifyHandleID;            // Verify service handle ID
    tmsg[2] = (uint32_t)conf->pub_key;   // Public Key address to be used
    tmsg[3] = (uint32_t)conf->msg;       // Address where the message to be verified can be found
    tmsg[4] = (uint32_t)conf->signature; // Address where the signature can be found
    tmsg[5] = sig_key_size;              // Signature size | Public key size
    tmsg[6] = conf->msg_size;            // Size of message
    tmsg[7] = flags_scheme;              // Flags | Signatrure Scheme
    tmsg[8] = S3MU_ComputeMsgCrc(tmsg, VERIFY_SIZE - 1u);

    /* Send message Security Sub-System */
    status = S3MU_SendMessage(mu, tmsg, VERIFY_SIZE);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Wait for response from Security Sub-System */
    status = S3MU_GetResponse(mu, rmsg, S3MU_RR_COUNT);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Check that response corresponds to the sent command */
    if (rmsg[0] == VERIFY_RESPONSE_HDR && rmsg[1] == RESPONSE_SUCCESS)
    {
        /* If success, test result */
        if (rmsg[2] == VERIFY_RESPONSE_SUCCESS)
        {
            *result = true;
        }
        else
        {
            *result = false;
        }
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}
