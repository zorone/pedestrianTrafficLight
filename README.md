# PedestrianTrafficLight Project

You're currently on `WIP_fallback` branch

This project is the 4th year final project of the Civil Engineer Department, Faculty of Engineering.  

The project is making use of Arduino Uno rev3, to control and deploy on real Traffic Light System, which deploy inside the university, so we could measure the impact of installing these Signal Controlling System on pedestrian crossing as if it causes any positive or negative impact in general or not.  

## Structure of This Repository

This repository have taken used of the branch system of git. the main one can be described below.  

PedestrianTrafficLight  
├ master  
├ WIP  
├ **WIP_fallback**  
├ WIP_fallback2  
├ ino_release  
└ ino_singlefile  

On this `WIP_fallback` branch, it contains all the commit, which include both in usable state and which that doesn't. To put simply, this branch contains all the history that related to controlling the arduino using basic imperative style. Which design so that the user could read easily from top-to-bottom. As this repository is dedicate to Civil Engineer colleague, I need to dumb down the code as much as possible, so the reader can follow through what's going on in the code.  

For historical record, `WIP_fallback` is a fallback for `WIP` branch, which use interrupt, but it doesn't work as well as I hope for, and might further add complexity to the project itself. So I decide to change the paradigm, and stick with blocking-delay method instead.  

This `WIP_fallback` has been push to branch `master` since tag `v0.5` and later on.  

## Structure of This branch

`WIP_fallback` branch will contain files like ordinary Platformio Project. The important parts is as followed:  

**WIP_fallback**
├ include/  
│ ├ common.h  
│ ├ control.h  
│ └ pin.h  
├ pedestrianTrafficLight_test/  
├ src/  
│ ├ common.cpp  
│ ├ control.cpp  
│ └ main.cpp  
├ test/  
│ ├ allPin_test.ino  
│ ├ countdownDisplay_test.ino  
│ ├ time_delta_test.c  
│ ├ time_delta_test_2.c
│ ├ time_delta_test.py  
│ ├ time_delta_test_result.txt  
│ └ time_delta_test_result_failed.txt  
├ .gitignore  
├ LICENSE  
├ platformio.ini  
└ README.md (This README)

`include/` folder is where I put all the header files (.h) that the program required.  
  `common.h` is definition header for common function that use throughout the project.  
  `control.h` is definition header for function that have responsibility for controlling timing and signal of the lights.  
  `pin.h` contained the pin definition of the used pin on the board itself. You can change the responsibility of each pin here.  

`pedestrianTrafficLight_test/` folder is unfinished work. It intends to be receiver board to test if the controlling board (which is this Platformio project) is working correctly and the timing is within the constraint time.  

`src/` folder is where I put all the core of the codes. Almost all important stuffs will reside here.  
  `main.cpp` is where the program first executed. Take a look here to see how the program will perform in general.  
  `common.cpp` is where the implementation of common function reside.  
  `control.cpp` is where the implementation of signal control and timing comparison reside.  

`test/` folder is where I write the tests to test against the logic of the code. Although this `test/` folder also appear in normal Platformio project. I didn't make use of the Platformio Unit Test Capability. I wrote it as another regular program. So Platformio's testing feature will not work here. You need to compile and run manually, which we will discuss later on.  

`.gitignore` file, is the file to tell git which files would you not like to track, so you won't accidentally add it into the repo.  

`LICENSE` file, it is LICENSE of this entire repository. Which is **MIT License**.

`README.md` file, it is the file that you're reading right now. It contains all useful information for this repository. The `README.md` file will differ for each branch, so it could further explain the reason behind each branch.  

## Usage

To use this repository, You will need 3 main components.  
1. [Visual Studio Code](https://code.visualstudio.com/download) or its derivative  
2. [Platformio IDE](https://docs.platformio.org/en/latest/integration/ide/vscode.html)  
3. [Atmel AVR platform](https://docs.platformio.org/en/latest/platforms/atmelavr.html?utm_source=platformio&utm_medium=piohome)  

And lastly, we'll need hardware to run on.  

We'll need:  
1. Arduino Uno  
2. Wiring  
3. LED light (If you can get traffic Light Form Factor, I'd recommend getting that instead.)  
4. (optional) Latching Switch
