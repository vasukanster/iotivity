/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#ifndef __CA_INTERFACE_CONTROLLER_H_
#define __CA_INTERFACE_CONTROLLER_H_

#include "caadapterinterface.h"
#include "uthreadpool.h" /* for thread pool */

#ifdef __cplusplus
extern "C"
{
#endif

void CAInitializeAdapters(u_thread_pool_t handle);

void CASetPacketReceivedCallback(CANetworkPacketReceivedCallback callback);

void CASetNetworkChangeCallback(CANetworkChangeCallback callback);

void CAStartAdapter(CAConnectivityType_t connectivity);

void CAStopAdapter(CAConnectivityType_t connectivity);

CAResult_t CAGetNetworkInfo(CALocalConnectivity_t **info, uint32_t *size);

CAResult_t CASendUnicastData(CARemoteEndpoint_t* endpoint, void* data, uint32_t length);

CAResult_t CASendMulticastData(void *data, uint32_t length);

CAResult_t CAStartListeningServerAdapters();

CAResult_t CAStartDiscoveryServerAdapters();

void CATerminateAdapters();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //#ifndef __CA_INTERFACE_CONTROLLER_H_
