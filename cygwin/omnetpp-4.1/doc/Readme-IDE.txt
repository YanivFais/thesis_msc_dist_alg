The OMNeT++ IDE
~~~~~~~~~~~~~~~


Overview
--------
OMNeT++ 4.1 comes with a powerful integrated environment, based on Eclipse.
To get a quick overview of the IDE features, watch the screencast at:

    http://www.omnest.com/webdemo/ide

or read

    doc/IDE-Overview.pdf

in your OMNeT++ 4.1 installation.


Requirements and compilation
----------------------------
The IDE is Java-based, it requires the Java 5 or Java 6 JRE to be installed
on the computer. We recommend the Sun JRE.

The IDE does not need to be built by the user, it is ready to run right after
unpacking the tarball. Native (C++) parts of the IDE have been pre-compiled
for the supported platforms. The source for these libraries can also be found
under src/.

The IDE is supported on the following platforms:
 - Windows 32bit
 - Linux 32/64bit (i386)
 - Mac OS X 10.5/6 (i386)


Note: while the IDE does not need to be built, you'll still need "./configure"
and "make" to build the simulation kernel and runtime user interfaces (Cmdenv,
Tkenv).


Starting the IDE
----------------
The IDE can be started with the following command:

    omnetpp

on all platforms.


Components and features of the IDE
----------------------------------
The IDE presently contains:
    Eclipse Platform
    Eclipse CDT (C/C++ Development Environment)
    OMNeT++ IDE plug-ins

The OMNeT++ IDE plug-ins provide the following components:
    NED editor
    MSG file editor
    Ini file editor
    Makefile generator
    Simulation launcher
    Result analyzer
    Sequence chart and event log visualizer
plus several associated views, wizards and other goodies.

If you wish, you can install further plug-ins (for example Subversion support)
from www.eclipse.org, or plug-in sites like eclipseplugincentral.com or
eclipse-plugins.info.


Feedback and error reporting
----------------------------
The IDE logs errors and other information to the log file at:

    <your-workspace>/.metadata/.log

The workspace directory is by default the samples/ subdirectory of your
OMNeT++ installation.

If you find bugs or issues with the IDE, please report it using the
Help/Report Bug or Enhancement menu item in the IDE, and attach your
.log file as well.

We hope to be able use the logs to fix bugs much faster and to improve
the IDE.

Best regards,
The OMNeT++ team
