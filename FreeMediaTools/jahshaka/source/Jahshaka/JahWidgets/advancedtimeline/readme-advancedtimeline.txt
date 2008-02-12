advancedtimeline 
================

advancedtimeline contains a fork of the (unfinished!) v3 timeline project. NB in v3, the timeline editor and keyframe editor both use the same 'timeline' widget set, since the only difference is their concept of 'events' on each 'track'. For keyframer the events are sets of keyframe curves, for the editor they are video/audio tracks.  The track headers also change.

* Initially we use it only for a keyframer widget *

The editor events are more complex to manage due to the need to check the off-screen sections, and their thumbnail cache. keyframe events are simpler in that sense.

The ruler also behaves slightly differently for editor versus keyframer.