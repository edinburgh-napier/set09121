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
Babis Koniaris/Tobias Grubenmann
<br>


School of Computing. Edinburgh Napier University



---

## Release builds

![image](assets/images/gold-master.jpg) <!-- .element width="60%"  -->


---

# Green Go button 
Moving beyond the Green button.

![image](assets/images/build_options.png) <!-- .element width="95%"  -->


---

# Runtime Resources 

![image](assets/images/build_folder.png) <!-- .element width="70%"  -->


---

# Release Builds 

Tidy up before you ship

- Hide CMD window <!-- .element: class="fragment" -->
- Don't assume resolution <!-- .element: class="fragment" -->
- Pipe Exceptions to an error popup window <!-- .element: class="fragment" -->
- Turn off any developer cheats <!-- .element: class="fragment" -->

This should ideally be done automatically <!-- .element: class="fragment" -->


---

# Runtime Resources 

What does your game need, and where does it look for it?


---

# Static vs Dynamic Linking

**Static Linking**
- Larger .exe file  <!-- .element: class="fragment" -->
- No .dll / .so's generated <!-- .element: class="fragment" -->
- Potential for better compiler optimisation <!-- .element: class="fragment" -->
- Lock the version of libs <!-- .element: class="fragment" -->

**Dynamic (Shared) linking**
- Smaller .exe file <!-- .element: class="fragment" -->
- Libraries compiled to .dll <!-- .element: class="fragment" -->
- can reuse and share dlls already on the system <!-- .element: class="fragment" -->
- Modders can hack/swap out your .dlls. <!-- .element: class="fragment" -->


---

# Asset Pipeline During Development Vs Release

* During **development**: 
	* Assets need to be frequently iterated on/edited
	* Assets need to be in an artist-friendly form
	* Heavy/slow compression hurts iteration times

* At **release**:
	* Assets are not modified anymore
	* Assets need to be in optimal form for game execution
	* Heavy compression is fine, as long as decompression does not hurt load times
	
* Examples: 
	* texture/mesh/sound compression
	* ... or conversion	to game-ready formats
---

# Asset Pipeline Out There

* AAA studios do complicated things here. 

* Even UE4/Unity have complicated final "cooking" steps. 

* This should all be automated - more on this later 

* You probably do not need a custom asset pipeline

---

# Installers

Why?  You could just deploy your game as a Zip file

1.  Contain all your game files inside a single runnable .exe file - typically compressed. <!-- .element: class="fragment" -->
2.  Show a dialogue wizard of some kind that let's the user select an installation folder. <!-- .element: class="fragment" -->
3.  Extract your game files to the selected folder. <!-- .element: class="fragment" -->
4.  Optionally create desktop/Start Menu Shortcuts. <!-- .element: class="fragment" -->
4.  Install dependencies / registry values. <!-- .element: class="fragment" -->

---

# Running as Admin 

### HINT - This has been a recurring issue.  

Windows requires elevated permissions to touch `C:\Program Files` <!-- .element: class="fragment" -->

The only time your game should need Admin Privileges is the installer, and it should work without it if a user installs to a non-protected space. <!-- .element: class="fragment" -->

Consider where your save game files will go. <!-- .element: class="fragment" -->

Perhaps look at where loads of other games save stuff? <!-- .element: class="fragment" -->

---

# How

![image](assets/images/nullsoft.jpg) <!-- .element width="80%"  -->


---

# Un-Installers 

Make sure they work, and are added to Windows correctly.


---

## Continuous Integration


---

# Continuous Integration Benefits

- Ground Truth - no more "works on my machine"
- Alerts on broken builds - especially useful for multi-platform
- Signpost to others that your software still works
- Links in well with Unit Testing

---

# Continuous Integration Disadvantages

- Might cost you! You're using someone else's computer to do work
- Can be a bit of a faff to set up...

but...
- It is a *good idea* that most companies of a certain size and above use.
- Check out AppVeyor or Github Actions for a way to integrate (heh) this into your current workflow

---

## Deployment Summary


---

# Summary

1. Make sure your game works in Release
2. Understand everything your game needs to run.
3. Make an Installer & Uninstaller
4. Test your build automatically in the Cloud


---

# TOP TIPS

- Embed Version/Commit Numbers.
- Automate Everything.
 - Avoid one-off mistakes, fix it once, it will always work
- **Test it** 
 - Test it on new machines, old machines, fresh machines, 
 - Virtual machines, JKCC/VDS machines, your friends' machines
 
---

---

## IF YOUR CODE DOES NOT RUN ON MY MACHINE WHEN I TEST IT, YOU WILL LOSE MARKS!

---

# TOP TIPS

Test your code on other machines

Check you have all the files included in the installer

Check you have the right .dlls
