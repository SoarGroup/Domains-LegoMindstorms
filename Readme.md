## Soar LEGO® Mindstorms

Welcome to the Soar LEGO® Mindstorms repository! This repository contains code to run Soar on LEGO® Mindstorms EV3 robots.  It also includes a simulator for the normal robot we use (treads, two color sensors, and a touch sensor).  A copy of the tutorial using this repository can be found [here](https://docs.google.com/document/d/1o9cWNXP0AgLJzQmlCPAdEuyOAd7NEor-ECX9C724lwE/).

# Directory Structure

* agents/
  * This folder contains example agents including the agents for the Soar Workshop tutorials.
* Robot-Code/
  * This folder contains the code for running Soar on a LEGO® Mindstorms EV3 Robot.
* Robot-Simulator/
  * This folder contains a simulator for the default configuration of our LEGO® Mindstorms EV3 robots (two color sensors in the center facing down, a touch sensor facing forward, and two treads for moving).

## Motivation

We created this project as a means to teach people Soar in an interative manner.  We decided to use LEGO® Mindstorms EV3 because who doesn't like playing with legos? ;)

## Installation

Before you can use any of the code you should download a copy of Soar.  This can be found [here]() in binary form and [here]() in repository form.

### Robot Code

TODO: Documentation for the Robot Code part

### Robot Simulator

To use the robot simulator, you need to complete the following steps:

1. Import "Robot-Simulator" as an eclipse project.
2. Setup the sml.jar library in Java Build Path -> Libraries to point to your Soar sml.jar (found in bin/java/ or out/java/)
3. Edit the launch configuration environment variable SOAR_HOME to point to your Soar Home directory (bin/ or out/).
4. Add the library path for your platform (DYLD_LIBRARY_PATH for OS X, LD_LIBRARY_PATH for Linux, PATH for Windows) to point to your SOAR_HOME directory.
5. Launch it!

## API Reference

Link to API reference including interface

## Contributors

Feel free to fork this project and submit pull requests for us! We welcome any help on this project.  Also feel free to submit issues to inform us of any issues you encounter with the code.  Please do not use the issues as a help resource, we will ignore those.  If you need help with this, please contact [Soar Group](http://sourceforge.net/p/soar/mailman/soar-group/) instead.

## License

Soar LEGO® Mindstorms is distributed under the BSD License

Copyright © 2014. The Regents of University of Michigan. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE SOAR CONSORTIUM OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. The views and conclusions contained in the software and documentation are those of the authors and should not be interpreted as representing official policies, either expressed or implied, of The University of Michigan.
