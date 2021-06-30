/*
 * Copyright (c) 2019, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <vector>

#include <keymaster/cppcose/cppcose.h>

namespace aidl::android::hardware::security::keymint::remote_prov {

using bytevec = std::vector<uint8_t>;
using namespace cppcose;

extern bytevec kTestMacKey;

// The Google root key for the Endpoint Encryption Key chain, encoded as COSE_Sign1
inline constexpr uint8_t kCoseEncodedRootCert[] = {
        0x84, 0x43, 0xa1, 0x01, 0x27, 0xa0, 0x58, 0x2a, 0xa4, 0x01, 0x01, 0x03, 0x27, 0x20, 0x06,
        0x21, 0x58, 0x20, 0x99, 0xb9, 0xee, 0xdd, 0x5e, 0xe4, 0x52, 0xf6, 0x85, 0xc6, 0x4c, 0x62,
        0xdc, 0x3e, 0x61, 0xab, 0x57, 0x48, 0x7d, 0x75, 0x37, 0x29, 0xad, 0x76, 0x80, 0x32, 0xd2,
        0xb3, 0xcb, 0x63, 0x58, 0xd9, 0x58, 0x40, 0x1e, 0x22, 0x08, 0x4b, 0xa4, 0xb7, 0xa4, 0xc8,
        0xd7, 0x4e, 0x03, 0x0e, 0xfe, 0xb8, 0xaf, 0x14, 0x4c, 0xa7, 0x3b, 0x6f, 0xa5, 0xcd, 0xdc,
        0xda, 0x79, 0xc6, 0x2b, 0x64, 0xfe, 0x99, 0x39, 0xaf, 0x76, 0xe7, 0x80, 0xfa, 0x66, 0x00,
        0x85, 0x0d, 0x07, 0x98, 0x2a, 0xac, 0x91, 0x5c, 0xa7, 0x25, 0x14, 0x49, 0x06, 0x34, 0x75,
        0xca, 0x8a, 0x27, 0x7a, 0xd9, 0xe3, 0x5a, 0x49, 0xeb, 0x02, 0x03};

// The Google Endpoint Encryption Key certificate, encoded as COSE_Sign1
inline constexpr uint8_t kCoseEncodedGeekCert[] = {
        0x84, 0x43, 0xa1, 0x01, 0x27, 0xa0, 0x58, 0x4e, 0xa5, 0x01, 0x01, 0x02, 0x58, 0x20,
        0xd0, 0xae, 0xc1, 0x15, 0xca, 0x2a, 0xcf, 0x73, 0xae, 0x6b, 0xcc, 0xcb, 0xd1, 0x96,
        0x1d, 0x65, 0xe8, 0xb1, 0xdd, 0xd7, 0x4a, 0x1a, 0x37, 0xb9, 0x43, 0x3a, 0x97, 0xd5,
        0x99, 0xdf, 0x98, 0x08, 0x03, 0x38, 0x18, 0x20, 0x04, 0x21, 0x58, 0x20, 0xbe, 0x85,
        0xe7, 0x46, 0xc4, 0xa3, 0x42, 0x5a, 0x40, 0xd9, 0x36, 0x3a, 0xa6, 0x15, 0xd0, 0x2c,
        0x58, 0x7e, 0x3d, 0xdc, 0x33, 0x02, 0x32, 0xd2, 0xfc, 0x5e, 0x1e, 0x87, 0x25, 0x5f,
        0x72, 0x60, 0x58, 0x40, 0x9b, 0xcf, 0x90, 0xe2, 0x2e, 0x4b, 0xab, 0xd1, 0x18, 0xb1,
        0x0e, 0x8e, 0x5d, 0x20, 0x27, 0x4b, 0x84, 0x58, 0xfe, 0xfc, 0x32, 0x90, 0x7e, 0x72,
        0x05, 0x83, 0xbc, 0xd7, 0x82, 0xbe, 0xfa, 0x64, 0x78, 0x2d, 0x54, 0x10, 0x4b, 0xc0,
        0x31, 0xbf, 0x6b, 0xe8, 0x1e, 0x35, 0xe2, 0xf0, 0x2d, 0xce, 0x6c, 0x2f, 0x4f, 0xf2,
        0xf5, 0x4f, 0xa5, 0xd4, 0x83, 0xad, 0x96, 0xa2, 0xf1, 0x87, 0x58, 0x04};

/**
 * Generates random bytes.
 */
bytevec randomBytes(size_t numBytes);

struct EekChain {
    bytevec chain;
    bytevec last_pubkey;
    bytevec last_privkey;
};

/**
 * Generates an X25518 EEK with the specified eekId and an Ed25519 chain of the
 * specified length. All keys are generated randomly.
 */
ErrMsgOr<EekChain> generateEekChain(size_t length, const bytevec& eekId);

/**
 * Returns the CBOR-encoded, production Google Endpoint Encryption Key chain.
 */
bytevec getProdEekChain();

struct BccEntryData {
    bytevec pubKey;
};

/**
 * Validates the provided CBOR-encoded BCC, returning a vector of BccEntryData
 * structs containing the BCC entry contents.  If an entry contains no firmware
 * digest, the corresponding BccEntryData.firmwareDigest will have length zero
 * (there's no way to distinguish between an empty and missing firmware digest,
 * which seems fine).
 */
ErrMsgOr<std::vector<BccEntryData>> validateBcc(const cppbor::Array* bcc);

}  // namespace aidl::android::hardware::security::keymint::remote_prov