---
title: "Lecture17"
keywords: Lecture
tags: [Lecture]
permalink:  lecture17.html
summary: lecture17
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

Release builds
==============

Green Go button Moving beyond the Green button.
![image](build_options){width="\textwidth"}

Runtime-resources ![image](build_folder){width="75%"}

Release Builds Tidy up before you ship

-   Hide CMD window

-   Don't assume resolution

-   Launch windowed

-   Pipe Exceptions to an error window

-   Turn off any developer cheats

This should be done automatically

Runtime-resources What does your game need, and where does it look for
it?

static vs dynamic linking

.5 Static Linking

-   Larger .exe file

-   No .dll / .so's generated

-   Potential for better compiler optimisation

-   Lock the version of libs

.5 Dynamic (Shared) linking

-   Smaller .exe file

-   Libraries compiled to .dll

-   can reuse and share dlls already on the system

-   Modders can hack swap out your .dlls.

Asset Pipeline You may use different assets for release, or bundle them
up into binary files.

Off-line rendering at higher settings may take place.

This should all be automated -- more on this later

Installers
==========

Why You could just deploy your game as a Zip file

Run as Admin The only time your game should need Admin Privileges, and
it should work without it.

How

Un-Installers Make sure they work, and are added to windows correctly.

Continuous Integration
======================

Benefits

-   Ground Truth - no more "WOMM\"

-   Alerts on broken builds - especially useful for multi-platform

-   Signpost to others that your software still works

-   Cool badge

Deployment
==========

TOP TIPS

-   Embed Version/Commit Numbers \... please.

-   Automate Everything -\
    Avoid one-off mistakes, fix it once, it will always work

Test it Test it on new machines, old machine, fresh machines, virtual
machines, JKCC machines.
