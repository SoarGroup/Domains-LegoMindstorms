## Soar Lego Mindstorms

Welcome to the Soar Lego Mindstorms repository! This repository contains code to run Soar on Lego Mindstorms EV3 robots.  It also includes a simulator for the normal robot we use (treads, two color sensors, and a touch sensor).

# Directory Structure

* agents/
  * This folder contains example agents including the agents for the Soar Workshop tutorials.
* Robot-Code/
  * This folder contains the code for running Soar on a Lego Mindstorms EV3 Robot.
* Robot-Simulator/
  * This folder contains a simulator for the default configuration of our Lego Mindstorms EV3 robots (two color sensors in the center facing down, a touch sensor facing forward, and two treads for moving).

## Motivation

We created this project as a means to teach people Soar in an interative manner.  We decided to use Lego Mindstorms EV3 because who doesn't like playing with legos? ;)

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

TODO: Which license are we using for this? Public Domain? BSD? UM Generic (ie. Liar's Dice license?).