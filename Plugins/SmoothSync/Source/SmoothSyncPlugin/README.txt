Plugin Version: 1.30

# Smooth Sync

## Step 1 - Drag and Drop

1. Put the SmoothSync component onto any networked actor ("Replicates" is checked) that you want to be Smoothly Synced. Make sure "Replicate Movement" is unchecked as Smooth Sync will be replacing it. 
2. To sync a component, use SmoothSync.setSceneComponentToSync() to pass in any Transform that you want to sync. Use one SmoothSync for each Transform.
2. It is now smoothly synced across the network. Move the object on the server if unowned or the owner if owned to have it Smoothly Sync across all systems.

## Step 2 - Tweak to Your Needs

Now that it is on your networked object:

1. Read the tooltips corresponding to the variables to tweak the smoothness to your game's
specific needs. More detailed comments are in the code comments for the variables. 
2. Reduce your bandwidth by only sending position, rotation, and velocity variables that you need.


# How it Works

Smooth Sync stores a list of network States to interpolate between and then goes into extrapolation during latency spikes. 

# Methods you may need

SmoothSync.setSceneComponentToSync() - Used to set the Transform that you want to sync. If none is set up, it will sync the Actor. Must have one SmoothSync for each Transform that you want to sync.
SmoothSync.teleport() - Used to teleport your objects for situations like respawning so that they don't interpolate.
SmoothSync.forceStateSendNextFixedUpdate() - Useful if you have a low send rate but you want to manually send a transform
in between the send rate. 
SmoothSync.clearBuffer() - You will need to manually call this on all of the object's instances if you change the object's network owner during runtime.
SmoothSync.enableSmoothSync() - Used to enable / disable Smooth Sync.


Don't hesitate to contact us with any problems, questions, or comments.
With Love,
Noble Whale Studios