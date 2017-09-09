
![MEL Logo](https://raw.githubusercontent.com/epezent/MEL/master/logo.png)

# MEL - MAHI Exoskeleton Library

## Developers

- [Evan Pezent](http://evanpezent.com) (epezent@rice.edu)
- [Craig McDonald](http://craiggmcdonald.com/) (craig.g.mcdonald@gmail.com)

## Table of Contents

1. [Git / GitHub Setup](#git--github-installation)
2. [Installation (Windows 64-bit)](#installation-windows-64-bit)
    - [Visual Studio 2017 - Visual C++](#visual-studio-2017---visual-c)
    - [Required C++ Libraries](#required-c-libraries-boost-eigen-quarc)
    - [Python 2.7 (Optional)](#python-27--numpy--pyqt4--pyqtgraph)
    - [Sublime Text 3 (Optional)](#sublime-text-3-optional)
3. [Building MEL (Windows 64-bit)](#building-mel-windows-64-bit)
4. [Using MEL (Windows 64-bit)](#using-mel-in-your-projects-windows-64-bit)

##   Git / GitHub Setup

1. Download and install [GitHub Desktop](https://desktop.github.com/). Login using your GitHub account once installation is complete.

2. Choose **Clone a repository**

3. Enter the MEL URL: **https://github.com/epezent/MEL**

4. Set the local path to **C:\whatever\you\like\MEL**

3. Clone the repository. Navigate to the local path in Windows Explorer and verify that the repository has been cloned.

## Installation (Windows 64-bit)

**IMPORTANT:** It is important that you install all items **exactly** where specified. If you choose to install items in locations other than **C:\dev**, you will break dependency location references in the MEL Visual Studio Solution and will have to update them yourself.

###  Visual Studio 2017 - Visual C++

1. Download [Visual Studio 2017 Community](https://www.visualstudio.com/)

2. Run the VS2017 installer and check the following options:

    Under the **Workloads** tab:

    - Desktop development with C++

    Under the **Individual components** tab:

    - Git for Windows
    - GitHub extension for Visual Studio
    - Python language support (if you plan to use Python for GUIs, scopes, or machine learning)
    - Visual Studio Tools for Unity (if you plan to use Unity Engine for games and visualizations)

3. Start and complete the VS2017 installation

###   Required C++ Libraries (Boost, Eigen, Quarc)

1. Download and run [Boost 1.64.0 - MSVC 14.1 (64-bit)](https://sourceforge.net/projects/boost/files/boost-binaries/1.64.0/boost_1_64_0-msvc-14.1-64.exe/download)

    - Set the installation directory to **C:\dev\boost\_1\_64\_0**
    - Alternatively, you can choose to build the [Boost](http://www.boost.org/) yourself

2. Download the latest stable release of [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

    - Extract the folder labeled **eigen-eigen-5a10...** to **C:\dev** and change the folder name to just **eigen**
    - If you are in **C:\dev\eigen** you should see folders *bench*, *blas*, *cmake*, *debug*, etc.

3. Navigate to **C:\Program Files\Quanser\QUARC** (you must have Quarc 2.5 or greater installed)

    - Copy the *include* and *lib* folders to a new directory *C:\dev\quarc*
    - From *C:\dev\quarc\lib* you can delete all folders EXCEPT *windows* and *win64*
    - If you don't have Quarc installed, you can get the folders from another MAHI lab member who does

###   Python 2.7 + NumPy + PyQt4 + PyQtGraph (Optional)

**Note:** The MELScope application is written entirely in Python using PyQt4 for GUIs and PyQtGraph for plotting widgets. If you plan to use MELScope or want to develop your own GUIs for MEL programs, follow theses steps to install the Python environment.

1. Download [Python 2.7.13 (64-bit) Installer](https://www.python.org/downloads/release/python-2713/)

2. Run the installer and set the installation location to **C:\dev\Python27**. Ensure that the "Add python.exe to path" option is set to "Will be installed on local hard drive"

3. Download and install the [Microsoft Visual C++ Compiler for Python 2.7](https://www.microsoft.com/en-us/download/details.aspx?id=44266)

4. Go to the [Unofficial Windows Binaries for Python Extension Packages](http://www.lfd.uci.edu/~gohlke/pythonlibs/) page. Download the latest binaries for **NumPy**, **SciPy**, and **MatPlotLib**. Make sure you select the correct build (i.e. listings ending with **-cp27-cp27m-win\_amd64.whl**). At the time of writing, the latest versions were:

    - numpy-1.13.1+mkl-cp27-cp27m-win_amd64.whl
    - scipy-0.19.1-cp27-cp27m-win_amd64
    - matplotlib-2.0.2-cp27-cp27m-win_amd64

5. Add the .whl files to a new directory **C:\dev\Python27\wheels**

6. While in this directory in Windows Explorer, Shift+Right-Click in an empty space and select "Open command (or PowerShell) window here". Execute the following console commands **IN THIS ORDER**:

   ```
   pip install --upgrade pip
   pip install numpy-1.13.1+mkl-cp27-cp27m-win_amd64.whl
   pip install scipy-0.19.1-cp27-cp27m-win_amd64.whl
   pip install matplotlib-2.0.2-cp27-cp27m-win_amd64.whl
   pip install PyOpenGL
   pip install PyOpenGL_accelerate
   pip install PyYAML
   pip install qdarkstyle
   pip install pyflakes
   pip install iPython
   ```

   **NOTE:** PyOpenGL_accelerate may fail to install if you skipped step 2. This is because it must be built from source and requires the legacy MSVC compiler be installed.

7. Download and the [PyQt4 binaries for Python 2.7 (64-bit)](https://sourceforge.net/projects/pyqt/files/PyQt4/PyQt-4.11.4/PyQt4-4.11.4-gpl-Py2.7-Qt4.8.7-x64.exe/download?use_mirror=managedway&r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fpyqt%2Ffiles%2FPyQt4%2FPyQt-4.11.4%2F&use_mirror=managedway). Run the installer and ensure that the Python installation directory is correctly identified as **C:\dev\Python27**

8. Download the [PyQtGraph (x86-64) Installer](http://www.pyqtgraph.org/). Run the installer and ensure that the Python installation directory is correctly identified as **C:\dev\Python27**

9. Create a new Python script called **test.py** and paste the following code. Run the script to test PyQt4 + PyQtGraph. Play around with each test example, particularly the benchmarks, to ensure everything is working properly.

    ```python
    import pyqtgraph.examples
    pyqtgraph.examples.run()
    ```

**NOTE:** The newer Python 3.6 and PyQt5 have been tested and were found to be less stable and slower than the more mature Python 2.7 + PyQt4 pairing when used with PyQtGraph. You may choose to install Python 3.6 and PyQt5 (or any combination of the two, really) but note that the installation procedure will differ slightly (specifically when downloading wheels/binaries and building PyOpenGl_accelerate).


### Sublime Text 3 (Optional)

**Note:** You can optionally install Sublime Text 3 to write Python. Following these directions will turn Sublime into a very awesome, lightweight Python IDE!

1. Download [Sublime Text 3](https://www.sublimetext.com/)

2. Install [Package Control](https://packagecontrol.io/installation) for Sublime Text 3

3. Once Package Control is installed, press Ctrl+Shft+P in Sublime to open the Command Palette. Begin typing **"install"** and select **"Package Control: Install Package"** when it appears, then type the names of the following packages and press Enter to install them.

    - [Anaconda](https://packagecontrol.io/packages/Anaconda) (**required**)
    - [SublimeLinter](https://packagecontrol.io/packages/SublimeLinter) (**required**)
    - [SublimeLinter-pyflakes](https://packagecontrol.io/packages/SublimeLinter-pyflakes) (**required**)
    - [Terminal](https://packagecontrol.io/packages/Terminal) (recommended)
    - [SideBarEnhancements](https://packagecontrol.io/packages/SideBarEnhancements) (recommended)
    - [SyncedSideBar](https://packagecontrol.io/packages/SyncedSideBar) (recommended)
    - [BracketHighlighter](https://packagecontrol.io/packages/BracketHighlighter) (recommended)
    - [Git](https://packagecontrol.io/packages/Git) (recommended)
    - [GitGutter](https://packagecontrol.io/packages/GitGutter) (recommended)

4. Once Anaconda, SublimeLinter, and SublimeLinter-pyflakes are installed, navitage to **Preferences >> Package Settings >> Anaconda >> Settings - User** and add the following settings:

    ```json
    {
      "anaconda_linting": false,
      "swallow_startup_errors": true,
      "hide_snippets_on_completion": true,
      "complete_parameters": true,
      "auto_formatting_timeout": 5
    }
    ```

5. Open any *.py Python file in Sublime. Navigate to **Preferences >> Settings - Syntax Specific** and add the following into Python.sublime-settings:

    ```json
    {
      "auto_indent": true,
      "rulers": [79],
      "smart_indent": true,
      "trim_automatic_white_space": true,
      "use_tab_stops": true,
      "word_wrap": false,
      "wrap_width": 80
    }
    ```

6. Right-click in the Python file and choose **SublimeLiner >> Toggle Linter...** from the context menu. Make sure **pyflakes** is enabled.

7. Optionally choose a new Sublime theme from [Package Control](https://packagecontrol.io/). Evan's personal favorites are [Material Theme](https://packagecontrol.io/packages/Material%20Theme) and [Spacegray](https://packagecontrol.io/packages/Theme%20-%20Spacegray) with [A File Icon](https://packagecontrol.io/packages/A%20File%20Icon) installed on top.

## Building MEL (Windows 64-bit)

If you are on Windows and using Visual Studio, building MEL is fairly straight-forward using the provided Solution file.

1. Navigate to your cloned MEL repository, and open the MEL solution (*MEL.sln*)

2. The MEL solution has 3 projects:

    1. **MEL** - This is the MEL library, itself. It outputs **MEL.lib** to *\bin*.

    2. **MELShareDLL** - This is an optionally compiled dynamically linked library that exposes an interface to MEL's shared memory system, **MELShare**, for developing apps in Unity, Python, etc. It outputs **MELShare.dll** to *\bin*.

    3. **Examples** - This is an optionally compiled executable that demonstrates basic MEL use. It outputs **Examples.exe** to *\bin*.

3. Make sure the Visual Studio build configuration is set to **Release x64**. You can build all three projects at once by selecting **Build >> Build Solution** from the menu. If you only want to build one project, right-click the project in the Solution Explorer and select **Build**. The resulting binaries (.lib, .dll, or. exe) will output to *\bin*.

## Using MEL In Your Projects (Windows 64-bit)

1. If you are starting a new project, first create a new Visual Studio solution. Go to **File >> New >> Project**. Choose **Win32 Console Application**  and give your Project and Solution a name. In the following wizard, uncheck **Precompiled Headers** (unless you know how these work and want to use them). Note that you could optionally add a new Project to your MEL solution instead of creating a new one, but this is not recommended.

2. Right-click your project in the Solution Explorer and select **Properties**. Make the following changes/additions:

    - **C/C++ >> General >> Additional Include Directories**
        - add: *path-to-your-MEL-repository\MEL;C:\dev\boost_1_64_0\;C:\dev\eigen;*
    - **Linker >> General >> Additional Library Directories**
        - add: *path-to-your-MEL-repository\bin;C:\dev\boost_1_64_0\lib64-msvc-14.1;*
    - **Linker >> Input >> Additional Dependencies**
        - add: *MEL.lib;*

3. Your project should now be configured to use MEL! For usage examples, check out the [Examples project](https://github.com/epezent/MEL/tree/master/Examples), the official MEL documentation, or the [OpenWrist](https://github.com/epezent/OpenWrist) and MAHI Exo-II repositories.


