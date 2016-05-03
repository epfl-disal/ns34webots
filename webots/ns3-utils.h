/*
 * Copyright (c) 2017 EPFL TUD
 * Distributed Intelligent Systems and Algorithms Laboratory,
 * Ecole polytechnique fédérale de Lausanne
 * Technische Universität, Dresden
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors: Ignacio Llatser <llatser@gmail.com>
 */

#ifndef NS3_UTILS_H_
#define NS3_UTILS_H_

#include "ns3/socket.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/config.h"
#include "ns3/double.h"
#include "ns3/net-device-container.h"



ns3::NodeContainer CreateNodes (uint32_t size);
ns3::NetDeviceContainer ConfigurePhyMacLayers(ns3::NodeContainer nodes, double pathLossExp, double txPower, double rxSensitivity, uint32_t frequency, uint32_t channelNumber);
void ReceivePacket (ns3::Ptr<ns3::Socket> socket);
void SetForwarding(ns3::NodeContainer nodes, char routingProtocol);
ns3::Ipv4InterfaceContainer CreateInterfaces(ns3::NetDeviceContainer wifiDevices);
void InstallApplications (ns3::NodeContainer nodes, ns3::Ipv4InterfaceContainer interfaces);

#endif /* NS3_UTILS_H_ */
