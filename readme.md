# Fake scene Presenter unreal plugin

# Overview

The FSP offers gamemode (just to setup correct pawn), pawn with cinecamera and attached components (trackrider and recorder). 

# Setup

## Level setup
Set the GM_FSP as a the game mode in the level. Alternatively create your own Game Mode which inherits from FSPGameMode (c++ class) and edit your variables.
Open FSP mode (Modes -> FSP) and Initialize FSP.

### Engine setup
To allow object tracing and data collection, you need to add a FSPObject trace in settings. 

### Objects
Each object to be tracked needs to have the FSPObject component. This will allow the object to be tracked as well as be moved etc. To prevent problems with occlusion, each FPS object then needs to set its collision response to overlap FSPObject traces. This will allow tracing objects which only "peak" behind corners without necessity to setup multiple collision boxes within them. 

# Use

The most straighforward way is to use the FSPGame mode. it uses the BP_FSPPawn as the main pawn. If you desire to have the pawn tracking a specific path, use the BP_CameraTrack (see Tracks). The input is being parsed in the BP_Experiment manager

## Tracks

Setup the BP_CameraTrack as you see fit

Add BP_FPSTrackRider and attach whatever you see fit onto it. The rider follows the track when enabled with given duration to cover the entire track.

The 

## Recording
Recording of positions happens in the viewport. The speed of the recording shouldn't make much difference, but try keeping it around 30 s. Too fast recordings can lead to skewed results. 
- in playmode press "E" in editor or esc in build
- press record
- let it run

### Video Capture

Atm recording is kinda hacky, as other solutions would require c++ classes which will come later.

Simple recording can be achieved through screen recording, but that might have some issues with timing. The best and most controlled way is with hacky sequencer

Option 1: use a regular screen capture
Option 2: Use a sequencer
- Create a sequence
- set the sequence length to desired video length. It NEEDS to be the same as the length of the rider duration
// NOT implemented :D 
- set the sequence to the track

### Screenshots capture:
Option 1: Screenshots from hooks
-  
Option 2: Screenshots from trail
- in the FSP_recorder set the desired number of screenshots in the N Screenshot field
- in playmode press "E" or esc in build
- press record screenshots
- screenshots are consecutively created and logs objects position are recorded as well

Prepare the track 

Recording is done


### Changes
Lighting baking

There is the issue of any object, which is moved int he scene, will, by necessity, affect the scene lighting. Therefore it is necessary to bake the lights before each individual run, while hiding the original objects. This can be circumvented in case you are using multiple scenes, 

# Developer Overview
The game mode sets the default pawn to be FSPPawn. Pawn

Track is defined by FSPTrack. 


# FAQ
Q: I created a scene, added few unreal brushes as temporary objects, added FSPObject to them, but the log does not record these objects?
A: This has to do with Unreal brushes and how they handle collisions. See the issue [here](https://answers.unrealengine.com/questions/4917/bug-event-hit-not-called-on-brush-hits.html). The solution is to use static meshes and properly setup collisions or to add explicitely add a collider on the brush to generate hit events.