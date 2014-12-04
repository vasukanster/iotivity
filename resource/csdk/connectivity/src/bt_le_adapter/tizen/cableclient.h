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
#ifndef _BLE_CLIENT_
#define _BLE_CLIENT_

#include <bluetooth.h>
#include <bluetooth_type.h>
#include <bluetooth_product.h>

#include "cacommon.h"
#include "caadapterutils.h"
#include "cableclientutil.h"
#include "caadapterinterface.h"
#include "logger.h"
#include "uthreadpool.h"


/**
* @fn  CAStartBLEGattClient
* @brief  Used to start CAStartBleGattClientThread for initializing Gatt Client
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CAStartBLEGattClient();

/**
* @fn  CAStartBleGattClientThread
* @brief  This is thread which will be used to initialize the Gatt Client and start device discovery.
*              1. Set scan parameters 2.Setting neccessary callbacks for connection, characteristics changed and discovery and bond creation.
*              3. Start device discovery
*
* @param[in] data - Currently it will be NULL(no parameter)
*
* @return  NULL on success or failure.
*
*/
void *CAStartBleGattClientThread(void *data);

/**
* @fn  CAStopBLEGattClient
* @brief  Used to stop Gatt Client gracefully in turn it will call CATerminateBLEGattClient function.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
void CAStopBLEGattClient();

/**
* @fn  CATerminateBLEGattClient
* @brief  Used to unset all the callbacks and stop service discovery
*
* @return  void
*
*/
void CATerminateBLEGattClient();

/**
* @fn  CAInitGattClientMutexVaraibles
* @brief  Used to initialize all required mutex variable for Gatt Client implementation.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CAInitGattClientMutexVaraibles();

/**
* @fn  CATerminateGattClientMutexVaraibles
* @brief  Used to terminate all required mutex variable for Gatt Client implementation.
*
* @return  void
*
*/
void CATerminateGattClientMutexVariables();

/**
* @fn  CASetBleClientThreadPoolHandle
* @brief  Used to Set the gThreadPool handle which is required for spawning new thread.
*
* @param[in] handle - Thread pool handle which is given by above layer for using thread creation task.
*
* @return  void
*
*/
void CASetBleClientThreadPoolHandle(u_thread_pool_t handle);


/**
* @fn  CABleGATTCreateBond
* @brief  Used to make LE bond with remote device(pairng the device) using bt_device_create_bond api.
*
* @param[in] remoteAddress - mac address of remote device which we want to bond with
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CABleGATTCreateBond(const char *remoteAddress);


/**
* @fn  CABleDiscoverServices
* @brief  Used to discover the services that is advertised by Gatt Server asynchrounously.
*
* @param[in] remoteAddress - mac address of remote device inwhich we want to discover the services.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CABleDiscoverServices(const char *remoteAddress);

/**
* @fn  CABleDiscoverCharacteristics
* @brief  Used to discover the characteristics in specific service in remote device.
*
* @param[in] service -The attribute handle of service. Unique identifier for service.
* @param[in] remoteAddress - mac address of remote device inwhich we want to discover the services.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CABleDiscoverCharacteristics(bt_gatt_attribute_h service, const char *remoteAddress);

/**
* @fn  CARegisterForChangeInCharacterstics
* @brief  Used to register a callback function that will be invoked when a characteristic value is changed.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CARegisterForChangeInCharacterstics();

/**
* @fn  CABleGattPrimaryServiceCb
* @brief  This is the callback which will be called when we get the primary services repeatedly.
*
* @param[in] service -The attribute handle of service. Unique identifier for service.
* @param[in] userData - user data
*
* @return  0 on failure and 1 on success.
*
*/
CABool_t CABleGattPrimaryServiceCb(bt_gatt_attribute_h service, void *userData);

/**
* @fn  CABtGattCharacteristicsDiscoveredCb
* @brief  This is the callback which will be called after the characteristics are discovered by bt_gatt_discover_characteristics()
*
* @param[in]  result  The result of discovering
* @param[in]  inputIndex  The index of characteristics in a service, starts from 0
* @param[in]  total  The total number of characteristics in a service
* @param[in]  characteristic  The attribute handle of characteristic
* @param[in]  userData  The user data passed from the request function
*
* @return  0 on failure and 1 on success.
*
*/
int32_t  CABtGattCharacteristicsDiscoveredCb(int32_t result, int32_t inputIndex, int32_t total,
        bt_gatt_attribute_h characteristic, void *userData);


/**
* @fn  CAUpdateCharacteristicsToGattServer
* @brief  Sets the value of characteristic and update the value to GATTServer(unicast).
*
* @param[in]  remoteAddress  The address of the remote device
* @param[in]  data  The value of characteristic (byte array)
* @param[in]  dataLen  The length of value
* @param[in]  TRANSFER_TYPE (UNICAST/MULTICAST)
* @param[in]  position  The unique index of each ble server. Used for multicast feature.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*/
CAResult_t  CAUpdateCharacteristicsToGattServer(const char *remoteAddress, const char  *data,
        int32_t dataLen, TRANSFER_TYPE type, int32_t position);

/**
 * @fn  CAUpdateCharacteristicsToGattServer
 * @brief  Sets the value of characteristic and update the value to All registered GATTServer -> Multicast
 *
 * @param[in]  data  The value of characteristic (byte array)
 * @param[in]  dataLen  The length of value
 *
 * @return  0 on success otherwise a positive error value.
 * @retval  CA_STATUS_OK  Successful
 * @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
 * @retval  CA_STATUS_FAILED Operation failed
 */
CAResult_t  CAUpdateCharacteristicsToAllGattServers(const char  *data, int32_t dataLen);

/**
 * @fn  CASetBLEReqRespClientCallback
 * @brief  used to store upper layer callback locally which will be used to send the data to application
 *
 * @param[in]  CANetworkPacketReceivedCallback  -  upper layer callback function to pass the data to CA layer.
 *
 * @return  void
 *
 */
void CASetBLEReqRespClientCallback(CANetworkPacketReceivedCallback callback);

/**
* @fn  CALEReadDataFromLEClient
* @brief  synchronous function for reading characteristic value.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CALEReadDataFromLEClient();

/**
* @fn  CABleClientSenderQueueEnqueueMessage
* @brief  Used to enqueue the message into sender queue using CAAdapterEnqueueMessage and make signal to the thread to process.
*
* @param[in]  remoteEndpoint  Remote device information
* @param[in]  data  data which we wants to send to remote device
* @param[in]  dataLen  Length of data.
*
* @return  0 on success otherwise a positive error value.
* @retval  CA_STATUS_OK  Successful
* @retval  CA_STATUS_INVALID_PARAM  Invalid input argumets
* @retval  CA_STATUS_FAILED Operation failed
*
*/
CAResult_t CABleClientSenderQueueEnqueueMessage(const CARemoteEndpoint_t *remoteEndpoint,
        void *data, uint32_t dataLen);

/**
* @fn  CABleGattCharacteristicWriteCb
* @brief  This is the callback which will be called after the characteristics changed.
*
* @param[in]  handle  The attribute handle of characteristic
*
* @return  void
*
*/
void CABleGattCharacteristicWriteCb(int result, void *user_data);

/**
* @fn  CABleClientSenderQueueProcessor
* @brief  This is the thread which will be used for processing sender queue.
*
* @return  NULL on success or failure.
*
*/
void *CABleClientSenderQueueProcessor();

/**
* @fn  CABleGattDescriptorDiscoveredCb
* @brief  This is the callback which will be called when descriptor of characteristics is found.
*
* @param[in]  result  The result of discovering
* @param[in]  format  format of descriptor.
* @param[in]  total  The total number of descriptor in a characteristic
* @param[in]  descriptor  The attribute handle of descriptor
* @param[in]  characteristic  The attribute handle of characteristic
* @param[in]  userData  The user data passed from the request function
*
* @return  void
*
*/
void CABleGattDescriptorDiscoveredCb(int32_t result, unsigned char format, int32_t total,
                                     bt_gatt_attribute_h descriptor, bt_gatt_attribute_h characteristic, void *userData);
/**
* @fn  CADiscoverDescriptorThread
* @brief  This is the thread which will be used for finding descriptor of characteristic.
*
* @param[in]  stServiceInfo  Service Information which contains the remote address and service handle, characteristic handle.
*
* @return  NULL on success or failure.
*
*/
void *CADiscoverDescriptorThread(void *stServiceInfo);

/**
* @fn  CADiscoverCharThread
* @brief  This is the thread which will be used for finding characteristic of a service.
*
* @param[in]  stServiceInfo  Service Information which contains the remote address and service handle, characteristic handle.
*
* @return  NULL on success or failure.
*
*/
void *CADiscoverCharThread(void *stServiceInfo);

/**
* @fn  CABtGattBondCreatedCb
* @brief  This is the callback which will be called when bond created with remote device.
*
* @param[in]  result  The result of bond creation.
* @param[in]  device_info  remote device information
* @param[in]  userData  The user data passed from the request function
*
* @return  void
*
*/
void CABtGattBondCreatedCb(int32_t result, bt_device_info_s *device_info, void *user_data);

/**
* @fn  CAGATTCreateBondThread
* @brief  This is the thread  which will be used for creating bond with remote device.
*
* @param[in]  stServiceInfo  Service Information which contains the remote address and service handle, characteristic handle.
*
* @return  NULL on success or failure.
*
*/
void *CAGATTCreateBondThread(void *stServiceInfo);

/**
* @fn  CABleGattCharacteristicChangedCb
* @brief  This is the callback which will be called after the characteristic value changes happen.
*
* @param[in]  characteristic  The attribute handle of characteristic
* @param[in]  value  Value of the characteristics of a service.
* @param[in]  valueLen  length of data.
* @param[in]  userData  The user data passed from the request function
*
* @return  0 on failure and 1 on success.
*
*/
void CABleGattCharacteristicChangedCb(bt_gatt_attribute_h characteristic, unsigned char *value,
                                      int32_t valueLen, void *userData);

/**
* @fn  CABleGattCharacteristicsDiscoveredCb
* @brief  This is the callback which will be called after the characteristics are discovered by bt_gatt_discover_characteristics()
*
* @param[in]  result  The result of discovering
* @param[in]  inputIndex  The index of characteristics in a service, starts from 0
* @param[in]  total  The total number of characteristics in a service
* @param[in]  characteristic  The attribute handle of characteristic
* @param[in]  userData  The user data passed from the request function
*
* @return  0 on failure and 1 on success.
*
*/
CABool_t CABleGattCharacteristicsDiscoveredCb(int32_t result, int32_t inputIndex, int32_t total,
        bt_gatt_attribute_h characteristic, void *userData);

/**
* @fn  CAGATTCreateBondThread
* @brief  This is the thread  which will be used for making gatt connection with remote devices
*
* @param[in]  remoteAddress  Mac address of remote device which wants to connect with.
*
* @return  NULL on success or failure.
*
*/
void *CAGattConnectThread (void *remoteAddress);
/**
* @fn  CAPrintDiscoveryInformation
* @brief  Used to print device information(Util method)
*
* @param[in]  discoveryInfo  Device information structure.
*
* @return  void
*
*/
void CAPrintDiscoveryInformation(bt_adapter_le_device_discovery_info_s *discoveryInfo);
/**
* @fn  CABtAdapterLeDeviceDiscoveryStateChangedCb
* @brief  This is the callback which will be called when the device discovery state changes.
*
* @param[in]  result  The result of discovering
* @param[in]  discoveryState  State of the discovery(FOUND/STARTED/ FINISHED)
* @param[in]  discoveryInfo  Remote Device information.
* @param[in]  userData  The user data passed from the request function
*
* @return  void
*
*/
void CABtAdapterLeDeviceDiscoveryStateChangedCb(int32_t result,
        bt_adapter_le_device_discovery_state_e discoveryState,
        bt_adapter_le_device_discovery_info_s *discoveryInfo,
        void *userData);
/**
* @fn  CADiscoverBLEServicesThread
* @brief  This is thread which will be spawned for discovering ble services. Once called discover api, then it will be terminated.
*
* @param[in]  remoteAddress  Mac address of the remote device in which we want to search services.
*
* @return  NULL for success or failure.
*
*/
void *CADiscoverBLEServicesThread (void *remoteAddress);

/**
* @fn  CABleGattConnectionStateChangedCb
* @brief  This is the callback which will be called whenever there is change in gatt connection with server(Connected/Disconnected)
*
* @param[in]  result  The result of discovering
* @param[in]  connected  State of connection
* @param[in]  remoteAddress  Mac address of the remote device in which we made connection.
* @param[in]  userData  The user data passed from the request function
*
* @return  void
*
*/
void CABleGattConnectionStateChangedCb(int32_t result, bool connected, const char *remoteAddress,
                                       void *userData);



#endif //#ifndef _BLE_CLIENT_
