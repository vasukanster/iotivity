//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __EASY_CONNECT_COMMON_H_
#define __EASY_CONNECT_COMMON_H_

#include "ocstack.h"
#include "octypes.h"

// Defines
#define OIC_STRING_MAX_VALUE 100
#define IPV4_ADDR_SIZE 16
#define IP_PORT 6298
#define NET_WIFI_SSID_SIZE 16
#define NET_WIFI_PWD_SIZE 16

/**
 * @brief Mac address length for BT port
 */
#define NET_MACADDR_SIZE 18

//The following variable determines the interface (wifi, ethernet etc.)
//to be used for sending unicast messages. Default set to Ethernet.
static OCConnectivityType OC_CONNTYPE = OC_IPV4;

static const char * UNICAST_PROVISIONING_QUERY = "coap://%s:%d/oic/res?rt=oic.prov";
static const char * OIC_PROVISIONING_URI = "/oic/prov";

/**
 * Provisioning Device Status
 */
typedef struct {
    /// Address of remote server
    OCDevAddr * addr;
    /// Indicates adaptor type on which the response was received
    OCConnectivityType connType;
} ProvDeviceInfo;

/**
 * Provosioning Status
 */
typedef enum {
    DEVICE_PROVISIONED = 0, DEVICE_NOT_PROVISIONED
} ProvStatus;

/**
 * Response from queries to remote servers. Queries are made by calling the @ref OCDoResource API.
 */
typedef struct {
    // Provisioning Status
    ProvStatus provStatus;
    // Provisioning Device Info
    ProvDeviceInfo provDeviceInfo;
} ProvisioningInfo;

/**
 * @brief  Network information of the Enroller
 */
typedef union
{
    /**
     * @brief BT Mac Information
     */
    struct
    {
        char btMacAddress[NET_MACADDR_SIZE];   /**< BT mac address **/
    } BT;

    /**
     * @brief LE MAC Information
     */
    struct
    {
        char leMacAddress[NET_MACADDR_SIZE];   /**< BLE mac address **/
    } LE;

    /**
     * @brief IP Information
     */
    struct
    {
        char ipAddress[IPV4_ADDR_SIZE]; /**< IP Address of the Enroller**/
        char ssid[NET_WIFI_SSID_SIZE]; /**< ssid of the Enroller**/
        char pwd[NET_WIFI_PWD_SIZE]; /**< pwd of the Enroller**/
    } WIFI;
} EnrolleeInfo_t;


/**
 * @brief Network Information
 */
typedef struct
{
    EnrolleeInfo_t netAddressInfo;          /**< Enroller Network Info**/
    OCConnectivityType connType;            /**< Connectivity Type**/
    bool isSecured;                         /**< Secure connection**/
} EnrolleeNWProvInfo_t;

/**
 * Client applications implement this callback to consume responses received from Servers.
 */
typedef void (*OCProvisioningStatusCB)(ProvisioningInfo provInfo);

#endif

