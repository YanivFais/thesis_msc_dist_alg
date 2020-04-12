Installing OMNeT++ on Mac OS X
==============================

1. Installing OMNeT++

Download the omnetpp source code and documentation from the omnetpp site.
Make sure you select to download the generic archive omnetpp-version-src.tgz.

Copy the omnetpp archive to the directory where you want to install it
(usually your home directory). Extract the archive using the command:

    tar zxvf omnetpp-version-src.tgz

A sub-directory called omnetpp-version will be created which will contain the simulator
files. You should now add the following lines to your startup file (.bashrc
or .bash_profile if you're using bash; .profile if you're using some other
sh-like shell):

    export PATH=$PATH:~/omnetpp/bin

You will need to restart the shell before proceeding (logout and login again).

You should check configure.user to make sure it contains the settings
you need. (In most cases you don't need to change anything.)

NOTE: If you want to build 64bit binaries, you should add the '-m64' option to
      the CFLAGS_RELEASE, CFLAGS_DEBUG and LDFLAGS variable in the configure.user
      file. Beacuse Tcl/Tk does ot have a 64 bit version you will be able to use
      only Cmdenv (the command line environment). Set the NO_TCL=yes in configure.user 
      too.
      
For Mac OS X 10.5:
 
    1. Install XCode 3 on your machine 
    2. ./configure 
    3. make 
 
For Mac OS X 10.4:

    1. Install XCode 2.5 on your machine 
    2. Install fink on your machine (install required packages: at least  
       bison 2.x must be present on your system) (XCode 2.x comes with bison 1.x) 
    3. The default gcc 4.0.1 compiler will not work. Either upgrade to 4.2, or 
       switch back to the 3.3 version (sudo gcc_select 3.3)
    4. ./configure 
    5. make
      
This will create both debug and release binaries.

You should now test all samples and check they run correctly. As an example,
the dyna example is started by entering the following commands:

    cd samples/dyna
    ./dyna

By default the samples will run using the Tcl/Tk environment. You should see
nice gui windows and dialogs.


2. Starting the IDE

OMNeT++ 4.0 comes with a brand new IDE (based on Eclipse). Mac OS X already has Java 5
installed so you should be ok. You should be able to start the IDE by typing:

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


4. Notes

The Tcl/Tk environment is using the native Aqua version of Tcl/Tk, so you 
will see native widgets. However, due to problems in the Tk/Aqua port,
you may experience focus problems and other UI quirks. We are aware
of these problems, but have not found a workaround yet.

