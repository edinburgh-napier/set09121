---
title: "Repo Setup"
keywords: repo_setup
tags: [repo_setup]
permalink:  repo_setup.html
summary: GIT gud
sidebar: home_sidebar
---

You will need Git installed. You can use a Git GUI (e.g sourcetree). However I will work with and show the cmd-line commands.

### Create a repo
When get onto move advanced topics, we will provide some code for you. For now you should create a blank Git repo on you Git host of choice ([Github](github.com), [BitBucket](bitbucket.org), or Napier's Gitlab: [gitgud.napier.ac.uk](gitgud.napier.ac.uk) )

For the rest of this process I'm assuming you are using GitHub.
If you have not already created a GitHub account, create one and sign in.

( As a student you get some cool swag from GitHub: [education.github.com/pack](https://education.github.com/pack) )

![newRepoprocess](assets/images/github_newrepo_1.png)
![NewRepoOptions](assets/images/github_newrepo_2.png)

1. Give your repo a simple name and descriptive description

1. Check - Initialize with a readme

1. Choose an open source license, so people can't legally steal your work
without crediting you. The inbuilt guide from GitHub covers this neatly,
when in doubt: choose MIT license.

After this stage Github will create the repo for you and you should see
something like the following image, now it's time to **clone** the
repo down so you can start to work within it. Click the green clone
button and copy the link within the box

![Clone from Github](assets/images/github_clone.png)

### Post-pandemic special: Getting GitHub Desktop
So, usually we'd have everything you needed installed on the D2 machines. We still do, but you might not be there as much, or you might prefer your laptop. As such, you'll need to install GitHub Desktop onto your own machine. Here it is: [https://desktop.github.com/](https://desktop.github.com/). You can use Git directly if you like, but if you're new, I'd get the GUI. Once you've got that installed, make sure you also install Git, as described below.

### Clone it

If you haven't installed Git on your pc yet, [go here](https://git-scm.com/downloads)

Open a cmd (or git-bash) window somewhere (desktop is best). Now clone
your repo

```bash
git clone git@github.com:you/yourRepo.git
```

This will create a folder named set09121\_labs (or whatever you called you repo). 
Let's move into that folder in the terminal

```bash
cd set09121_labs
```

Now if you run

```bash
git status
```

You should see something similar to

```bash
On branch master
Your branch is up-to-date with 'origin/master'.
nothing to commit, working tree clean
```

Now we can Start to get to work properly.

### Setup project structure
Create the following empty folders.
* **res** - (where resources go, like images and fonts)
* **lib** - (libraries that we need)
* **practical_1** - (source code for practical 1)

#### .gitignore
Next, create a **.gitignore** file, open with a text editor.
Navigate to [gitignore.io](https://gitignore.io) and create an ignore file for "C++" and the IDE you intend to use, i.e "Visual Studio". Copy the generated file into your gitignore file and save it. Git will not look at or track any files that match the rules in the gitignore file, keeping junk you don't need out of your repo.


### Get SFML
We will be building SFML from source, which means we need to get the code.
We will be doing this via Git Submodules, which makes it look  like the SFML code is now copied into your repo, but is actually saved a virtual link to the separate SFML repo.
```bash
git submodule add https://github.com/SFML/SFML.git lib/sfml
git submodule init
git submodule update
cd lib\sfml
git checkout 2.6.1
cd ..\..
```
**Note, you need to be in the root directory of your repository for this to work!**

---
Next step: [build setup](build_setup)