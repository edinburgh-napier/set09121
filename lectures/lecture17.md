---
title: "Lecture17"
keywords: Lecture
tags: [Lecture]
permalink:  lecture17.html
summary: lecture17
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---

<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 17 - Releasing your game
### SET09121 - Games Engineering

<br><br>
Kevin Chalmers and Sam Serrels

School of Computing. Edinburgh Napier University


---

# Release builds

---

# Green Go button 
Moving beyond the Green button.

![image](assets/images/build_options.png)


---

# Runtime-resources 

![image](assets/images/build_folder.png)


---

# Release Builds 

Tidy up before you ship

- Hide CMD window
- Don't assume resolution
- Launch windowed
- Pipe Exceptions to an error window
- Turn off any developer cheats

This should be done automatically


---

# Runtime-resources 

What does your game need, and where does it look for it?


---

# static vs dynamic linking

**Static Linking**
- Larger .exe file
- No .dll / .so's generated
- Potential for better compiler optimisation
- Lock the version of libs

**Dynamic (Shared) linking**
- Smaller .exe file
- Libraries compiled to .dll
- can reuse and share dlls already on the system
- Modders can hack swap out your .dlls.


---

# Asset Pipeline 

You may use different assets for release, or bundle them up into binary files.

Off-line rendering at higher settings may take place.

This should all be automated -- more on this later


---

#Installers

Why?  You could just deploy your game as a Zip file

1.  Contain all your game files inside a single runnable .exe file - typically compressed.
2.  Show a dialogue wizard of some kind that let's the user select an installation folder.
3.  Extract your game files to the selected folder.
4.  Optionally create desktop/Start Menu Shortcuts.

---

# Run as Admin 

The only time your game should need Admin Privileges, and it should work without it.


---

# How

![image](assets/images/nullsoft.jpg)


---

# Un-Installers 

Make sure they work, and are added to windows correctly.


---

## Continuous Integration


---

# Continuous Integration

Benefits
- Ground Truth - no more "WOMM"
- Alerts on broken builds - especially useful for multi-platform
- Signpost to others that your software still works
- Cool badge

---

## Deployment Summary


---

# Summary

1. Make sure your game works in Release
2. Understand everything your game needs to run.
3. Make an Installer UnInstaller
4. Test your build automatically in the Cloud


---

# TOP TIPS

- Embed Version/Commit Numbers.
- Automate Everything.
 - Avoid one-off mistakes, fix it once, it will always work
- **Test it** 
 - Test it on new machines, old machines, fresh machines, 
 - virtual machines, JKCC machines, your friends machines
