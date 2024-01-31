# Make & CMake Installation Guide
## Windows

To install Make and CMake on Windows, follow these steps:

### Installing Chocolatey Windows package manager
In order to download and install Make and CMake on Windows, you will need a package manager. To install the Chocolatey Windows package manager, follow these steps:
1. Open Windows PowerShell with administrative privileges.
2. Run `Get-ExecutionPolicy`. If it returns `Restricted`, then run `Set-ExecutionPolicy AllSigned` or `Set-ExecutionPolicy Bypass -Scope Process`.
3. Run the following command:
    ```shell
    Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
4. Wait a few seconds for the command to complete.
5. If you don't see any errors, you are ready to use Chocolatey! Type `choco list` to view a list of your installed packages.

### Installing Make & Cmake
To install Make & Cmake, simply run these two commands inside a terminal (or the Windows CMD) with administrative privileges:

    choco install make
    choco install cmake

**Adding CMake to your system PATH**

After installing CMake, you may also need to add it to your system PATH. Here's how to do this:

1. Access Environment Variables:
    * Press the Windows key.
    * Type "Edit the system environment variables" and press Enter.
2. Edit System Variables:
    * Click the "Environment Variables" button.
3. Edit the PATH Variable:
    * Under "User variables for ${YOUR_USER}," find "Path" and click "Edit".
4. Add CMake Path:
    * Click "New".
    * Add the following path: **C:\Program Files\CMake\bin**

After completing these steps, Make & CMake should be ready to use.

## Linux
### Debian-based
To install the Make and CMake on any debian-based distro (Ubuntu, Mint, PopOS, etc.), simply execute the following command:

    sudo apt install make cmake

### Arch-based
To install Make and CMake on any arch-based distro (Manjaro, Garuda, Endeavour, etc.), simply execute the following commands:

    sudo pacman -S make cmake