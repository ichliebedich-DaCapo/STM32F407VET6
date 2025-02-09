/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2024-11-20T13:59:46+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[119] = {
  0xbdd848d83d9bc539U, 0xbc182917bd2ee8f0U, 0x3ea2cf8bbcf80340U, 0xbec4063bbbed3c80U,
  0xbf02fb8e3f33ed03U, 0x40302143U, 0x401722b400000000U, 0xc014c36e00000000U,
  0x0U, 0xc02be65bU, 0xbec30d8fbe86de82U, 0x3eafcb8abe778cffU,
  0xbe43d3323c38fb40U, 0x3edc1582be825f7fU, 0x3e1e268c3d831630U, 0xbe553f20be0d7b03U,
  0xbf8929a83ca983c0U, 0x3d86224fbe5012a0U, 0x3df4a9d0be50c098U, 0x3e9d510c3f01bdacU,
  0x3e939e9ebec401a4U, 0xbee1d4d33e92ffd6U, 0xbe31b785beb61478U, 0x3e832bf23ed0b230U,
  0x3e1978143e8d2562U, 0xbedb95f2bf157371U, 0xbf46ee7abe3be2e0U, 0x3e03ca84beb4a8f0U,
  0xbee4db6ebe35cc0eU, 0xbe0dd5da3f1afb65U, 0x3ead4018bef39058U, 0xbf2ad422becac3ccU,
  0x3e4af061be05bbcaU, 0xbe71e56bbebb66b6U, 0x3e87e1563ed21696U, 0xbe9e994ebee013c3U,
  0x3e9bd4a8be525034U, 0x3ebe2bda3e6dcc2cU, 0x3ed7661ebe9bc3c0U, 0xbeda3efdbda321e4U,
  0xbd1240803ef3c603U, 0x3f5029533e6f25b4U, 0xbf3ffd23bcc045b0U, 0x3edc13d4bdb96e70U,
  0x3e3943bc3de4c504U, 0x3e9c6e56bd2e30ddU, 0xbf00e71a3ec7fb40U, 0x3d00f184bd66d438U,
  0xbeb3c3563e0c9b88U, 0xbed2b3913eb0f232U, 0xbd4fe3603ee341d5U, 0x3f2df9963ea1e39aU,
  0xbed23eb0bec51c22U, 0xbd5de160be295692U, 0xbea5f5d4be1952c3U, 0xbc8af2b0beaf92ccU,
  0xbf3d96123c4e3e20U, 0x3f338446bced6f90U, 0x3ed7e658be04da94U, 0xbe57ce863e9e844aU,
  0x3ec46430bec9afceU, 0xbe31ec74bec52e7aU, 0x3e8945883e6be150U, 0x3d4341d03ea8dd56U,
  0xbe0e80de3f2140a2U, 0x3e530cf4beec2438U, 0x3e050d873e47865cU, 0x3dc42db7be004150U,
  0x3dd7ddc0beb44feaU, 0x3e067e903d7052c8U, 0xbe7297523f16c9e6U, 0x3ee59f163de57250U,
  0xbf2877d1be93e618U, 0xbe87e221bec48562U, 0x3d0aa2283e28d96dU, 0xbdc9f3203f3f2ebfU,
  0x3e8ebab53dcdaf50U, 0xbed96f4cbe6009a1U, 0xbe6d0df23e7d0ee8U, 0xbe4da50ebdeaccc9U,
  0xbdbcd5103f9b0691U, 0x3f75e3083e47eda8U, 0xbef833d9be5914d0U, 0xbe80b970be9e14c8U,
  0xbccd0c20bf1a12bdU, 0xbd542120bce8c0e0U, 0xbe0d50d4bed3b136U, 0xbedc715a3e881e62U,
  0x3e34c8d83d6b98d0U, 0xbdb01428bde0b470U, 0x3db10eb83f7f3863U, 0x3f5b5627be30a556U,
  0xbf14add03e971ed4U, 0x3df244883ecd690cU, 0x3d4b7a40bec5be24U, 0x3daedd60bf094304U,
  0xbf8666c5bedcc04aU, 0x3f31e4b2be663c1cU, 0x3ed03ffebe186176U, 0xbec54ac93ea93754U,
  0xc00953c400000000U, 0xc019c5493ea3a2bdU, 0xbd19c56abfeb86c6U, 0xbfed97903fdb2a70U,
  0xbfeaeff53fe1c3e7U, 0x3e9d8f3cbff317ebU, 0x3fd8523b3fd98133U, 0x402f6096U,
  0xbff8eeee40332c9dU, 0xbf9237ab3ee81afeU, 0xbfb593003cd5a260U, 0x3e2f37c8c00edf2eU,
  0xbf427ead3f777650U, 0xbfad02bb4037805cU, 0xba9bf9b3bf0e0378U, 0x3f8828553f484baeU,
  0x3d3cab90413dbb96U, 0xc02868a74127658fU, 0x3fdf9c15U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

