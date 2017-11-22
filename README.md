# Bidirectional coupling of vehicle and networks simulators: A ns-3 plugin for webots

## Notice
```
  Copyright (c) 2017 EPFL TUD
  Distributed Intelligent Systems and Algorithms Laboratory,
  Ecole polytechnique fédérale de Lausanne
  Technische Universität, Dresden
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 
  Authors: Ignacio Llatser <llatser@gmail.com>,
  David Mansolino <david.mansolino@cyberbotics.com>,
  Guillaume Jornod <guillaume.jornod@gmail.com>,
  Sebastian Kühlmorgen <sebastian.kuehlmorgen@tud.de>
```
## Introduction notes
This page introduces a plugin allowing to use ns-3 as a VANET simulator within webots.
It has been developed jointly at the Distributed Intelligent Systems and Algorithm Laboratory (DISAL) at Ecole polytechnique fédérale de Lausanne (EPFL) and Techische Universität Dresden (TUD).

Please cite as Llatser, I., Jornod, G., Festag, A., Mansolino, D., Navarro, I., Martinoli, A. "Simulation of Cooperative Automated Driving by Bidirectional Coupling of Vehicle and Network Simulators." In Intelligent Vehicles Symposium, 2017, IEEE
```
@inproceedings{llatser2017simulation,
  title={Simulation of Cooperative Automated Driving by Bidirectional Coupling of Vehicle and Network Simulators},
  author={Llatser, Ignacio and Jornod, Guillaume and Festag, Andreas and Mansolino, David and Navarro, Inaki and Martinoli, Alcherio},
  booktitle={Intelligent Vehicles Symposium (IV), 2017 IEEE},
  year={2017},
  pages={1881--1886},
  organization={IEEE}
}
```

## Contents
This repository contains the sources and compilation procedure for ns-3 webots plugin.
The `webots` and `facilities` directories contain the sources of the necessary modules.

## Workflow
The script downloads ns-3 sources, then adds the two developed modules and compiles the required libraries.
These libraries are copied in `plugins/radio/omnet/dependencies` and create a `libomnet.so` at the root of this repository.

## Installation procedure
First, run the script: `./compile_plugin.sh`.

Then, there are multiple ways to make webots use the plugin, all with their pros and cons:
- Adding the plugin and its dependencies in the `LD_LIBRARY_PATH` environment variable

	`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../plugins/radio/omnet/dependencies`

	This command is actually executed at the end of the script

	**Pro**: Webots can be run directly after the compilation

	**Cons**: the path is relative and the export command is valid only for the current shell. Moreover, need to keep the libraries in this folder and keep track of its location.

 	The latter can be worked around by adding the export command in the .bash_rc with the absolute path. Note that webots should be called from the command line otherwise this command is not executed before loading webots
- Placing the omnet directory and its subdirectories in `[webots directory]/resources/projects/plugins/radio/omnet`

	**Pros**: common file for all projects and no need to worry how webots is launched

 	**Cons**: if webots not installed from cyberbotics package repository (i.e. copying the binaries in a user defined folder, generally to have multiple version on the same system), the plugin will not be ported from one version to another. Moreover, this method does not allow to have the real omnet plugin working on the same version

- Placing the omnet directory and its subdirectories in the plugin/radio directory of the project that requires the plugin

 	**Pros**: no questions regarding webots version or installation directory, possible to work with the original omnet plugin within another project.

	**Cons**: need to copy the files for each project (and therefore having binaries on git or other RCS). Multiple versions of the same file on the system.

## Usage within webots
To be used within a webots world, the radio should be added in the `sensorsSlotCenter` as
```
  RadioProto {
    protocol "802.11p"
    txPowerMin 30
    txPowerMax 30
    address "webotsreceiver.epfl.ch"
    txPower 30
    frequency 5.9e+09
    bitrate 6000000
  }
```
The radio is then accessed, having first included `radio.h`, with the following commands:
```
 radio = wb_robot_get_device("radio");
  wb_radio_enable(radio, SIM_TIMESTEP);
  wb_radio_set_callback(radio, CALLBACK);
```
where `SIM_TIMESTEP` is the size of the timestep of the control loop in milliseconds and `CALLBACK` is the function of the form

```
  void function CALLBACK(WbRadioEvent event)
  {
  	const DATA_TYPE* msg = reinterpret_cast<DATA_TYPE*>(wb_radio_event_get_data(event));
  }
```

Finally, use:
```
   WbRadioMessage msg_radio = wb_radio_message_new((int)sizeof(*DATA),(const char *)DATA, "255.255.255.255");
   // Generate a random delay before each message transmission
   wb_radio_send(radio, msg_radio, DELAY);
```
to send a message, where DATA is the data to be sent and DELAY is a delay used to avoid collisions due to the fact that all agents are exactly synchronized in simulation.
