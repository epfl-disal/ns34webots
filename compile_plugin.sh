# Copyright (c) 2017 EPFL
# Distributed Intelligent Systems and Algorithms Laboratory,
# Ecole polytechnique fédérale de Lausanne
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Authors: Guillaume Jornod <guillaume.jornod@gmail.com>,
# David Mansolino <david.mansolino@cyberbotics.com>


#/bin/bash
echo 'Deleting ns-3-allinone and plugins folder'
rm -rf ns-3-allinone
rm -rf plugins
echo 'Cloning ns3 repo'
#Compilation optimised for ns3.25
hg clone http://code.nsnam.org/ns-3-allinone
echo 'Entering ns3 repo'
cd ns-3-allinone
echo 'Downloading sources'
./download.py -n ns-3-dev
echo 'Copying webots module'
cp -r ../webots ns-3-dev/src
echo 'Copying facilities module'
cp -r ../facilities ns-3-dev/src
cd ns-3-dev
echo 'Compiling sources'
./waf --build-profile=optimized --disable-examples --disable-tests --disable-python --enable-modules=webots configure
./waf
echo 'If the compilation fails, get back to ns3.25'
echo 'Copying libraries to ../../plugins/radio/omnet/dependencies'
mkdir -p ../../plugins/radio/omnet/dependencies
dependecies=(libns3-dev-bridge-optimized.so libns3-dev-core-optimized.so libns3-dev-energy-optimized.so libns3-dev-facilities-optimized.so libns3-dev-facilities-optimized.so libns3-dev-internet-optimized.so libns3-dev-mobility-optimized.so libns3-dev-mpi-optimized.so libns3-dev-network-optimized.so libns3-dev-propagation-optimized.so libns3-dev-stats-optimized.so libns3-dev-traffic-control-optimized.so libns3-dev-wave-optimized.so libns3-dev-wifi-optimized.so libns3-dev-spectrum-optimized.so libns3-dev-antenna-optimized.so)
for i in ${dependecies[@]}; do
	cp build/$i ../../plugins/radio/omnet/dependencies/$i
	echo "Copying $i to ../../plugins/radio/omnet/dependencies"
done
echo 'Creating libomnet.so for webots'
cp build/libns3-dev-webots-optimized.so ../../plugins/radio/omnet/libomnet.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../plugins/radio/omnet/dependencies #make sure dependencies are found by Webots/plugin
echo
echo 'There are multiple ways to make webots use the plugin, all with their pros and cons'
echo '> Adding the plugin and its dependencies in the LD_LIBRARY_PATH environment variable'
echo '	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../plugins/radio/omnet/dependencies'
echo '	This command is actually executed at the end of the script'
echo '	Pro: Webots can be run directly after the compilation'
echo '	Cons: the path is relative and the export command is valid only for the current shell. Moreover, need to keep the libraries in this folder and keep track of its location.'
echo ' The latter can be worked around by adding the export command in the .bash_rc with the absolute path. Note that webots should be called from the command line otherwise this command is not executed before loading webots'
echo '> Placing the omnet directory and its subdirectories in [webots directory]/resources/projects/plugins/radio/omnet'
echo '	Pros: common file for all projects and no need to worry how webots is launched'
echo ' 	Cons: if webots not installed from cyberbotics package repository (i.e. copying the binaries in a user defined folder, generally to have multiple version on the same system), the plugin will not be ported from one version to another. Moreover, this method does not allow to have the real omnet plugin working on the same version'
echo '> Placing the omnet directory and its subdirectories in the plugin/radio directory of the project that requires the plugin'
echo ' 	Pros: no questions regarding webots version or installation directory, possible to work with the original omnet plugin within another project.'
echo '	Cons: need to copy the files for each project (and therefore having binaries on git or other RCS). Multiple versions of the same file on the system. '
