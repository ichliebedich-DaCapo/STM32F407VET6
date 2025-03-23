/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-03-23T09:25:29+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
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
const ai_u64 s_network_weights_array_u64[89] = {
  0xbf9b19d53f2ebcd9U, 0xbf272cdcbfa0ed88U, 0x3f32b791bf33415dU, 0xbf66abffbf4f43baU,
  0xbfa5a075bdaadf27U, 0x3f50f0073f211622U, 0xbf4554a43f32439dU, 0xbf5e33e9bf68a3daU,
  0xbead8bbd3e629314U, 0xbeee92fbbd46403fU, 0x3f398f2abe96c052U, 0x3fb7f842bee1a310U,
  0xbdda6ba1bf4134cdU, 0x3f58eb593eb3f34cU, 0xbec23c383f4f8a10U, 0xbecea34ebf07285bU,
  0xbf1efc203e143986U, 0xbfa60a4dbf2b74b8U, 0x3eb31f74bf956599U, 0x3ebcac13bf61fcdaU,
  0xbdb749babf1fa244U, 0x3f961222bc859ab1U, 0xbf95bf593f8d74abU, 0xbf928012bf3095d1U,
  0x3fc058f2be93e076U, 0x3fca87a73f7c0b29U, 0xbfce6e5b3fc43e3aU, 0x3f4a14dc402f6e9eU,
  0x3f5b62d9be55530dU, 0xbfd47824bf1865b1U, 0x402ae4ebbf8de858U, 0x3fb37c9e40090597U,
  0xbec15ce53eac38c1U, 0xbfc00b0bbf1da782U, 0x3eb0beb0bf50747aU, 0x3f018d58bf46ba07U,
  0xbf0b21c0bf134e8eU, 0x3f8191403d9db40bU, 0xbf18cab93f371871U, 0xbf3f50e9bf17200aU,
  0xbf852a563d93a6d2U, 0xbfe04d34be7c86b0U, 0x3fabfbdfbf8fb55bU, 0x3efa61eebfdd16ccU,
  0xbee829a0bf5c3630U, 0x3f5bb5c53e7baab6U, 0xbfc20a993f426258U, 0xbfccdbb0bfd22970U,
  0xbf7d73bfbe80927aU, 0xbf912ee5be889312U, 0x3f7688d5bf6c5d9fU, 0x3f10c899bfac4da8U,
  0xbf29f67fbf5e4990U, 0x3f4f7cd93d546537U, 0xbfa746f83f4dd7d8U, 0xbed930cfbf7a16a6U,
  0x3f6b1ff3bf3d1972U, 0x3f9c29ac3f763973U, 0xbf0a549e3fd1dae8U, 0x3f378b4a3f9fbc75U,
  0x3f824f1fbe2d929aU, 0xbed99589bf251996U, 0x3fbc57dfbedf3de6U, 0x3f725cce3f3b09e8U,
  0xbf40d7003e7ee416U, 0xbf8b7981be243aeeU, 0x3e66f20abf5f5b64U, 0x3eedc7dcbf6f9d99U,
  0xbea8e089be366023U, 0x3ecc853b3f276ff5U, 0xbf8d876d3f311a8bU, 0xbf78443dbf8ab63aU,
  0xbed363d43dde7566U, 0xbf50a6b5befe800eU, 0x3f9a78cfbf6a1f7eU, 0x3cd780d7bf9d99afU,
  0xbd12622bbf04fde1U, 0x3f085d123e8c7d31U, 0xbf3291663efef74eU, 0xbfa827e7bfb20be4U,
  0x3e1ec9da3f3364feU, 0x3f48a27e3f24c597U, 0x3d2c35073f136382U, 0x3f5beec03f22ff3bU,
  0x3fa4ccaebfa656b8U, 0xbfa12475bfb3d36bU, 0x3fe33de1bfc9ffe3U, 0xbfb5a790bfddeccdU,
  0xbea3fa04U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

