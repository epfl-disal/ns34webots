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
 * Authors: Ignacio Llatser <llatser@gmail.com>,
 * David Mansolino <david.mansolino@cyberbotics.com>,
 * Guillaume Jornod <guillaume.jornod@gmail.com>
 * Sebastian Kühlmorgen <sebastian.kuehlmorgen@tud.de>
 */

#include <vector>
#include <numeric>
#include "ns3/webots_radio.h"
#include "ns3/ns3-utils.h"
#include "ns3/log.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/convoy.h"
#include "ns3/internet-stack-helper.h"

#ifdef __cplusplus
extern "C" {
#endif


bool init_was_done;
int numNodes;
double pathLossExp;
double txPower;
double rxSensitivity;
double txFrequency;
int txChannel;
char routingProtocol; // not used for now

static ns3::NodeContainer nodes;
static ns3::NetDeviceContainer devices;
static ns3::Ipv4InterfaceContainer interfaces;


// initialization routine, to be called prior to any function call
void webots_radio_init() {

  std::cout << "webots_radio_init()" << std::endl;

  // Initialize simulation parameters to default values
  init_was_done = false;
  numNodes = 0;
  pathLossExp = 2;
  txPower = 33;        // Value for typical VANET scenarios according to 802.11p
  rxSensitivity = -95; // Value for typical VANET scenarios according to 802.11p
  txFrequency = 5.9e9; // Value for typical VANET scenarios according to 802.11p
  txChannel = 0;

  // Seed for the ns-3 random number generator
  ns3::RngSeedManager::SetSeed (rand());
}

// The actual initialization is done here, once numNodes has its final value
void init_if_necessary() {

  if(!init_was_done) {

      // Enable some logs by default. Comment this if too noisy
      ns3::LogComponentEnable("Ipv4L3Protocol", ns3::LOG_LEVEL_DEBUG); ns3::LogComponentEnable("Ipv4L3Protocol", ns3::LOG_PREFIX_TIME);
      ns3::LogComponentEnable("ArpL3Protocol", ns3::LOG_LEVEL_ALL); ns3::LogComponentEnable("ArpL3Protocol", ns3::LOG_PREFIX_TIME);

      ns3::LogComponentEnable("AdhocWifiMac", ns3::LOG_LEVEL_INFO); ns3::LogComponentEnable("AdhocWifiMac", ns3::LOG_PREFIX_TIME);
      ns3::LogComponentEnable("DcaTxop", ns3::LOG_LEVEL_DEBUG); ns3::LogComponentEnable("DcaTxop", ns3::LOG_PREFIX_TIME);

      ns3::LogComponentEnable("YansWifiChannel", ns3::LOG_LEVEL_DEBUG); ns3::LogComponentEnable("YansWifiChannel", ns3::LOG_PREFIX_TIME);
      ns3::LogComponentEnable("PropagationLossModel", ns3::LOG_LEVEL_DEBUG); ns3::LogComponentEnable("PropagationLossModel", ns3::LOG_PREFIX_TIME);
      ns3::LogComponentEnable("YansWifiPhy", ns3::LOG_LEVEL_DEBUG); ns3::LogComponentEnable("YansWifiPhy", ns3::LOG_PREFIX_TIME);


      // From now on, we cannot add new nodes to the simulation
      nodes = CreateNodes (numNodes);
      std::cout << numNodes << " nodes created" << std::endl;

      devices = ConfigurePhyMacLayers(nodes, pathLossExp, txPower, rxSensitivity, txFrequency, txChannel);
      std::cout << "PHY and MAC configured with following parameters: pathLossExp=" << pathLossExp << ", txPower=" << txPower << " dBm, rxSensitivity=" << rxSensitivity << " dBm, txFrequency= " << txFrequency << " kHz, txChannel=" << txChannel << std::endl;

      SetForwarding(nodes, routingProtocol);
      //std::cout << "Routing set, routingProtocol=" << routingProtocol << std::endl;

      interfaces = CreateInterfaces (devices);
      std::cout << "Interfaces created" << std::endl;

      InstallApplications (nodes, interfaces);
      std::cout << "Applications installed" << std::endl;

      ns3::Packet::EnablePrinting(); // record every operation performed on the packet’s buffer
      ns3::Packet::EnableChecking();

      init_was_done = true;
  }
}

void webots_radio_cleanup() {

  std::cout << "webots_radio_cleanup()" << std::endl;

  // Run for 5 more seconds to let all outstanding transmissions to be received
  ns3::Simulator::Stop (ns3::Seconds(5));
  ns3::Simulator::Run ();

  // Here it is possible to store the simulation results into a file

  ns3::Simulator::Destroy();

  numNodes = 0;
  init_was_done = false;
}

int webots_radio_new() {

  // All nodes must be added before the start of the simulation
  numNodes++;

  std::cout << "webots_radio_new() -> numNodes = " << numNodes << std::endl;

  return numNodes;
}

void webots_radio_set_alpha(double alpha) {

  std::cout << "webots_radio_set_alpha(" << alpha << ")" << std::endl;

  // Set the propagation model to deterministic log-distance with path loss exponent alpha
  pathLossExp = alpha;
}

void webots_radio_set_protocol(int radio,const char *protocol) {

  std::cout << "webots_radio_set_protocol(" << radio << ", " << protocol << ")" << std::endl;

  // to be implemented
}

void webots_radio_set_address(int radio,const char *address) {

  std::cout << "webots_radio_set_address(" << radio << ", " << address << ")" << std::endl;

  // what is this address for?
}

void webots_radio_set_frequency(int radio,double frequency) {

  std::cout << "webots_radio_set_frequency(" << radio << ", " << frequency << ")" << std::endl;

  // The frequency is set for all simulation nodes
  // frequency is in Hz, but ns-3 requires kHz
  txFrequency = frequency/1000;
}

void webots_radio_set_channel(int radio,int channel) {

  std::cout << "webots_radio_set_channel(" << radio << ", " << channel << ")" << std::endl;

  // The channel is set for all simulation nodes
  txChannel = channel;
}

void webots_radio_set_bitrate(int radio,double bitrate) {

  std::cout << "webots_radio_set_bitrate(" << radio << ", " << bitrate << ")" << std::endl;

  // The bitrate is fixed to 6 Mbps by the 802.11p standard
}

void webots_radio_set_rx_sensitivity(int radio,double rx_sensitivity) {

  std::cout << "webots_radio_set_rx_sensitivity(" << radio << ", " << rx_sensitivity << ")" << std::endl;

  // The receive sensitivity (in dBm) is set for all simulation nodes
  // Note: this only has effect if propagation = 'd'
  rxSensitivity = rx_sensitivity;
}

void webots_radio_set_tx_power(int radio,double tx_power) {

  std::cout << "webots_radio_set_tx_power(" << radio << ", " << tx_power << ")" << std::endl;

  // The transmission power (in dBm) is set for all simulation nodes
  // Note: this only has effect if propagation = 'd'
  txPower = tx_power;
}

void webots_radio_set_callback(int radio, void *user_data, void (*func)(const int radio, const struct WebotsRadioEvent *)) {

  std::cout << "webots_radio_set_callback(" << radio << ")" << std::endl;

  init_if_necessary();

  // find the facility layer for this node
  //ns3::Ptr<ns3::DENM> denm = nodes.Get(radio-1)->GetApplication(0)->GetObject<ns3::DENM>();
  ns3::Ptr<ns3::Convoy> convoy = nodes.Get(radio-1)->GetApplication(0)->GetObject<ns3::Convoy>();

  //denm->SetWebotsCallback(func);
  convoy->SetWebotsCallback(func);
}

// move a radio node
void webots_radio_move(int radio,double x,double y,double z) {

 
  init_if_necessary();

  ns3::Ptr<ns3::Node> node = nodes.Get(radio-1);
  ns3::Ptr<ns3::ConstantPositionMobilityModel> mob = node->GetObject<ns3::ConstantPositionMobilityModel>();
  mob->SetPosition(ns3::Vector3D(x,y,z));
}

struct xz webots_radio_pos(int radio) {

  init_if_necessary();

  ns3::Ptr<ns3::Node> node = nodes.Get(radio-1);
  ns3::Ptr<ns3::ConstantPositionMobilityModel> mob = node->GetObject<ns3::ConstantPositionMobilityModel>();
  ns3::Vector pos = mob->GetPosition();

  struct xz pos_xz;
  pos_xz.x = pos.x;
  pos_xz.z = pos.z;

  std::cout << "webots_radio_pos(" << radio << ") = " << pos_xz.x << ", " << pos_xz.z << ";" << std::endl;

  return pos_xz;
}

void webots_radio_send(int radio, const char *dest, const void *data, int size, double delay) {

  std::cout << "webots_radio_send(" << radio << ", " << dest << ", " << data << ", " << size << ", " << delay
    << ") at time "  << (ns3::Simulator::Now() + ns3::Seconds(delay)).GetSeconds() << " s." << std::endl;


  //ns3::Ptr<ns3::DENM> denm = nodes.Get(radio-1)->GetApplication(0)->GetObject<ns3::DENM>();
  //ns3::Simulator::Schedule(ns3::Seconds(delay), &ns3::DENM::SendPackets, denm, ns3::InetSocketAddress(ns3::Ipv4Address(dest), 11), 1, ns3::Seconds(1));

  // Need to copy data otherwise data get lost
  void * data2 = malloc(size);
  memcpy(data2, data, size);

  ns3::Ptr<ns3::Convoy> convoy = nodes.Get(radio-1)->GetApplication(0)->GetObject<ns3::Convoy>();
  ns3::Simulator::Schedule(ns3::Seconds(delay), &ns3::Convoy::Transmit, convoy, ns3::InetSocketAddress(ns3::Ipv4Address(dest), 11), data2, size);
}

void webots_radio_delete(int radio) {

  std::cout << "webots_radio_delete(" << radio << ")" << std::endl;
  // Memory is freed at the end of the simulation
}

// run the network simulation for a given number of seconds
void webots_radio_run(double seconds) {

  //std::cout << "webots_radio_run(" << seconds << ")" << std::endl;

  init_if_necessary();

  ns3::Simulator::Stop (ns3::Seconds(seconds));
  ns3::Simulator::Run ();
}

#ifdef __cplusplus
}
#endif
