/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2024-11-19T23:30:43+0800
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
  0x3f168a333ee7edb5U, 0x3f12583fbe6c2d8cU, 0x3ec060183ed8ec05U, 0x3d7dae0ebdd415e0U,
  0x3d669779beba1c47U, 0xc004a1aabfee5a1cU, 0x3ffabaf600000000U, 0xc0015e583fb0476fU,
  0x3fc70b4f00000000U, 0xc00b8ad200000000U, 0xbeb30710be65a766U, 0xbe740fc03d5708b0U,
  0xbc3ccb803ec2933cU, 0x3ea390e4be8ac8edU, 0x3e89b9143db12bc0U, 0x3ebb0aa13e1ad304U,
  0x3e42651c3e109988U, 0xbe486dcabe6fb4f3U, 0xbf2c26f0bea895c5U, 0x40a8108dbea252a8U,
  0x3eefd2663efd074bU, 0xbef65b22beb26bd4U, 0x3e132f8a3e7e4537U, 0xbf1063e83eba9b82U,
  0x40286658be5e3997U, 0xbe8bc8a23c3f8a2bU, 0x3e9cbfbb3cd87fc0U, 0x3dc79b96be9f2512U,
  0x3ebd58e8be2b1cbeU, 0xbe994a47bebb2c14U, 0xbedc897bbdb7579cU, 0x3dbc21903b9b0380U,
  0xbeafd0713ec24478U, 0xbe1890963e8bd3fcU, 0xbcd03e203dbd8f18U, 0x3eae806f3efd0c8cU,
  0xbeaa97923d161eb0U, 0x3f004e21bece73d8U, 0xbf392d9fbe2f0648U, 0x406b38e13ed14a6cU,
  0xbe813fd6be572848U, 0x3f0a04d4be49b450U, 0xbf0f4e3a3e9c8bceU, 0xbd2da9f3bec4ebf6U,
  0xc08778ddbe7ce536U, 0xbb21adc83e768926U, 0x3e60113a3e9a3beaU, 0x3e514130bdeee3b7U,
  0xbef47abcbead430bU, 0x404aad8f3e433b04U, 0x3ec5090b3e9d9e11U, 0xbebf81efbea4b529U,
  0x3eb6eab1bdec6552U, 0xbded9a743eac2df2U, 0x4079a1fbbe9398f6U, 0xbe7e461f3cdd6000U,
  0x3d1f9d50bd313e50U, 0x3ec97b90be88a2afU, 0xbea7e9cc3ebab75eU, 0xbe48789c3e1e3de4U,
  0xbc3318563eadc5daU, 0xbe1189dfbeb5ec71U, 0x3ea86cbbbea824fcU, 0x3f3536d7bcadf860U,
  0xc000ef1c3c63cfc0U, 0xbe80cdc4be9e29e8U, 0x3d418dc03e0328c8U, 0xbe93de8e3e8d030aU,
  0x3eba429abe9e299aU, 0x3ee4a926be46c17cU, 0xbd5269983d873910U, 0xbe4db0ea3e5dc844U,
  0xbd908c84be6e2826U, 0x3dece320be7091e7U, 0xbec648843e832318U, 0x3ecd3a1e3e29921bU,
  0xbe04b8f1bd89a0f4U, 0xbd9cd6303eb048a5U, 0x3f31e5b0bee6f7bfU, 0xc029449c3da76ef8U,
  0x3e5603373e8dbc63U, 0xbdaea0153ed01e50U, 0xbbbac9fb3d77fd08U, 0xbe979cf5bdc5f6d0U,
  0x400caec73e6d0a40U, 0xbec0cb6dbdce818cU, 0x3d358660bce14590U, 0x3e30cbac3eb0ba9aU,
  0xbe91e70e3e305b88U, 0xbed2a269bc996bd0U, 0xbe599aa7bd8ee60fU, 0xbd04b7ee3eab44a4U,
  0x3e200c4c3e8ebe86U, 0xbeeccc50beaf7383U, 0x3e013dd0bee91160U, 0xbeb25429be2d45e4U,
  0xbe51cbd6bedaf663U, 0xbe9635debcb126d0U, 0xbd37c3d0be727ba2U, 0xbecdfa46bc9987d0U,
  0xc01c135600000000U, 0xbca91e41bfb73d40U, 0xbfe7b9bb00000000U, 0xbfca79b43ff7f204U,
  0xbfe0a7e3U, 0x3fb1ff7cU, 0x3fb0014400000000U, 0xbfbd8439U,
  0xbbc4c5ddU, 0xbf30db683efca8b2U, 0x3ca9a517bf565c70U, 0xbf71da42bdaf58b0U,
  0xbed11cf2402acfb0U, 0x3e2887ecbf197dfcU, 0xbdf9b6c03eaddd6cU, 0x3f1f87893ed0392cU,
  0xbe0da056bef8b115U, 0xbeaabb64be54088eU, 0x3fad2ce8U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

