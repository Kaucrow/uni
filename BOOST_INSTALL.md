# Boost C++ Libraries Installation Guide
## Windows

To install the Boost C++ Libraries on Windows, follow these steps:

1. Visit the [Boost Libs download page](https://www.boost.org/users/download/) by clicking the link.
2. Download any of the two zip files labeled "Windows".

Once the download is complete, proceed with the installation:

3. Uncompress the downloaded zip file.
4. Open a terminal or the Windows CMD.
5. Navigate to the uncompressed folder using the 'cd' command.

Execute the following two commands to build and install Boost:

    ./bootstrap gcc
    ./b2 link=static runtime-link=static --prefix=C:\Boost install

The first command prepares the Boost.Build system for use. The second command invokes Boost.Build to build the separately-compiled Boost libraries, and installs them in the `C:\Boost` directory.

After completing these steps, the Boost C++ Libraries should be ready to use. If you haven't already done so, remember to add them to your include path for your development environment.