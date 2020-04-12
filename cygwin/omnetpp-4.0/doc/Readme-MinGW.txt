Installing OMNeT++ on Windows/MinGW
===================================

1. Installing OMNeT++

Download the omnetpp source code from the omnetpp site.
Make sure you select to download the windows specific archive because it 
contains additional files required for compiling OMNeT++ on Windows.
The ZIP archive contains a complete MSYS and MINGW installation and all
additional 3rd party components (perl, tcl/tk, libxml2 etc). The only
external dependency for OMNeT++ is an installed Java Runtime Environment 
(ver>=1.5). 

Copy the omnetpp archive to the directory where you want to install it. Be sure
that the path to the directory does NOT contain any space.
Extract the archive using the command:

    unzip omnetpp-<version>-src-windows.zip

A sub-directory called omnetpp-<version> will be created which will contain the simulator
files. The directory will contain a full MSYS and MINGW installation customized for
the OMNeT++ environment.

Start mingwenv.cmd which will bring up a bash shell where the the path is already set
to include the omnetpp-<version>/bin directory. If you want to start omnetpp simulations
from the windows explorer please add the omnetpp-<version>/bin directory to your system path.

All software needed to compile the OMNeT++ libraries with MINGW compiler is included in
the archive file.

First you should check configure.user to make sure it contains the settings
you need however in most cases you don't need to change anything:
    notepad configure.user

Then the usual GNU-like stuff:
    ./configure
    make

This will create both debug and release binaries.

You should now test all samples and check they run correctly. As an example,
the dyna example is started by entering the following commands:

    cd samples/dyna
    ./dyna

By default the samples will run using the Tcl/Tk environment. You should see
nice gui windows and dialogs.

2. Starting the IDE

OMNeT++ 4.0 comes with a brand new IDE (based on Eclipse). You will need at least
Java Runtime Environment 5.0 installed on your machine.  You should be able 
to start the IDE by typing:

    omnetpp

NOTE: The IDE is supported ONLY on the following platforms:
 - Windows 32bit
 - Linux 32/64bit (i386)
 - Mac OS X 10.4/5 (i386/ppc)

3. Reconfiguring the libraries

If you need to recompile the OMNeT++ components with different flags (e.g.
different optimization), then cd to the top-level omnetpp
directory, edit configure.user accordingly, then say:

    ./configure
    make clean
    make

If you want to recompile just a single library, then cd to the directory
of the library and type:

    make clean
    make

If you want to build ONLY release or debug builds use: 
(if MODE is not specified both debug and release will be created)
   
    make MODE=release
    
If you want to create static libraries use: 
(by default shared libraries will be created)
    
    make SHARED_LIBS=no    

NOTE: The built libraries and programs are immediately copied 
to the lib/ and bin/ subdirs.


4. Portability issues

OMNeT++ has been tested with the MinGW gcc compiler. The current distribution
contains gcc 4.2.1 version.

Microsoft Visual C++ is not supported in the Academic Edition.

