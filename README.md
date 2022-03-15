# P1-Sustainable-Internet

![Screenshot 2021-12-17 173822](https://user-images.githubusercontent.com/43958385/146577960-cae66402-d76e-4548-ae63-1ac57582788f.png)

A Discrete-Event-Simulation program written in C, that simulates a given number of users\
and their internet activity on certain websites.\
The users will get websites recommended based on a collaborative filtering recommendation system with sustainability in mind.\
As the simulation progresses the chance of the user taking a more sustainable choice will increase.\
The output will show the different simulation blocks with their respective sustainable choice value.

## About
This project is a part of the P1 project on first semester (Software).

Group: SW B222 

Made by: Ali, Viktor, Frederik, Thorbjørn, Jonas, Alexander

# Table of Contents
1. [Prerequisites](#prerequisites)
2. [Getting started](#getting-started)
    - [Installation](#installation)
    - [Configuring the input file](#configuring-the-input-file)
    - [Building and running the simulation](#building-and-running-the-simulation)
    - [Simulation input explanation](#simulation-input-explanation)
3. [Tests](#tests)
    - [Building](#building)

# Prerequisites
- [git](https://git-scm.com/)
- [gcc](https://gcc.gnu.org/)
- [make](https://www.gnu.org/software/make/)

# Getting started

## Installation

### Using git
```
git clone https://github.com/FredTheNoob/P1-Sustainable-Internet
```

## Configuring the input file
- Navigate to the input directory

A boilerplate file (websites_data.csv) is provided for use.\
If you wish to use your own make sure you follow the csv's format (Avg. visit duration \[s],Pages per visit,Weight,Category).

- Open the simulation_input.txt file
- Configure the parameters to your liking (check out the [Simulation input explanation](#simulation-input-explanation))

## Building and running the simulation
To build the simulation with the input files ensure you are in the root directory, then run:
```
make
```

You can then run the simulation in your command line of choice by writing:
```
./a.out
```

Or on Windows machines:
```
./a.exe
```

---
**NOTE**:
The simulation may take several minutes depending on your input configuration.
---

## Simulation input explanation
### NUM_USERS
- How many active internet users to have in the simulation.
### NUM_WEBSITES
- The amount of websites in the input csv file.
### NUM_CATEGORIES
- The amount of categories in the input csv file.
### AVG_USER_TIME
- How much time (in seconds) the users have on average each day of the simulation.
### SIM_DURATION_DAYS
- The amount of days the simulation should run.
### NUM_SIMULATIONS
- The amount of simulations to run.
### SUSTAINABLE_CHOICE_INCREMENT
- How much the sustainable choice of the users should increment by each block of simulation.\
(where a block would be referred to as running NUM_SIMULATIONS times)

# Tests
Certain functions have been handpicked for a CUTest. This test has been used for black box testing,\
and is split into two suites (InsertWebsiteNodeTest.c and WebsiteTest.c)

## Building
In the root directory run:

```
make -C Tests
```

## Running
In the root directory run:

```
./Tests.out
```
