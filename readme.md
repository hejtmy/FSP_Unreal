# Fake scene Presenter unreal plugin

## Overview

The FSP offers gamemode (just to setup correct pawn), pawn with cinecamera and attached components (trackrider and recorder). 

## Setup
The package logging is build on top of BrainvrUnrealFramework. Need to include the plugin in the project and add "BrainvrUnrealFramework" to the FSP.build.cs file as a dependency.

## Use

The most straighforward way is to use the FSPGame mode. it uses the BP_FSPPawn as the main pawn.

If you desire to have the pawn tracking a specific path, use the BP_CameraTrack (see Tracks)

The input is being parsed in the BP_Experiment manager

### Tracks

Setup the BP_CameraTrack as you see fit

Add BP_FPSTrackRider and attach whatever you see fit onto it. The rider follows the track when enabled with given duration to cover the entire track.

The 

### Recording
REcording of positions happens in the viewport 
- in playmode press "E" in editor or esc in build
- press record
- let it run

Recording of video
Option 1: use a regular screen capture
Option 2: Use a sequencer
- Create a sequence
- set the sequence length to desired video length. It NEEDS to be the same as the length of the rider duration
// NOT implemented :D 
- set the sequence to the track


Recording of screenshots:
Option 1: Screenshots from hooks
-  
Option 2:
- Screenshots from trail

Atm recording is kinda hacky, as other solutions would require c++ classes which will come later.

Simple recording can be achieved through screen recording, but that might have some issues with timing. The best and most controlled way is with hacky sequencer.

Prepare the track 

Recording is done