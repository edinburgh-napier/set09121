---
title: "Lecture 2 - Workflow"
keywords: Lecture
tags: [Lecture]
permalink:  lecture2.html
summary: lecture2
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 2 - Workflow and Repo Management
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Version Control

Version Control is used everywhere, by everyone.

I expect to see best practices in use for this module!


---

# Version Control 

Why use Version control?

- Keep a log of changes to your code  <!-- .element: class="fragment" -->
- An unlimited "undo" button. <!-- .element: class="fragment" -->
- Experiment with new features without trashing your working code <!-- .element: class="fragment" -->
- Helps you work better in teams <!-- .element: class="fragment" --> 
- Mirror your codebase to multiple redundant locations <!-- .element: class="fragment" -->
- Integrate with continuous-integration, testing and deployment tools. <!-- .element: class="fragment" -->


---

<!-- .slide: class="leftalign" -->
# Git 

You can do a lot of things with git, and many of the rules of what
you *should* do are not so much technical limitations but are about
what works well when working together with other people.

VC / GIT is a very powerful set of tools, and that can not only be
overwhelming at first, it also means that you can often do the same (or
similar) things different ways, and they all "work." <!-- .element: class="fragment" -->

Generally, the best way to learn git is probably to first only do very
basic things and not even look at some of the things you can do until
you are familiar and confident about the basics. <!-- .element: class="fragment" -->


---

# Git Hosts

- Local - You don't *need* to push anywhere
- Run your own git server
- GitHub - The de facto for open-source code.
- Bitbucket - By Atlassian - more for private projects
- Gitlab - when you need multiple repos and project management


---

# Git clients

- Command Line (Git bash on windows)
- GitHub Desktop - [https://desktop.github.com](https://desktop.github.com)
- Sourcetree - [https://sourcetreeapp.com](https://sourcetreeapp.com)
- GitKraken - [https://gitkraken.com](https://gitkraken.com)

![image](assets/images/sourcetree.png)  <!-- .element height="70%" width="70%" -->

---

# Git Hosts

Honestly I'd just recommend using GitHub Desktop these days!
It's the nicest one to use for basic stuff...

---

# Version Control - Best Practices


---

# Commit Often

- Committing often keeps your commits small and, again, helps you commit only related changes. <!-- .element: class="fragment" -->
- Allows you to share your code more frequently with others. <!-- .element: class="fragment" -->
- Easier for everyone to integrate changes regularly and avoid merge conflicts. <!-- .element: class="fragment" -->
- Having few large commits and sharing them rarely, in contrast, makes it hard both to solve conflicts and to comprehend what happened. <!-- .element: class="fragment" -->
- Don't commit only at the end of the day. <!-- .element: class="fragment" -->


---

# Commit Related Changes 

- A commit should be a wrapper for related changes.
- For example: 
    - Fixing two different bugs should produce two separate commits. <!-- .element: class="fragment" -->
- Small commits make it easier for other team members to understand the changes and roll them back if something went wrong. <!-- .element: class="fragment" -->
- Use tools like the staging area and stage only parts of a file, to create very granular commits. <!-- .element: class="fragment" -->


---

# Don't Commit Half-Done Work 

- You should only commit code when it's completed.
- This doesn't mean you have to complete a whole, large feature before committing. <!-- .element: class="fragment" -->
- Split the feature's implementation into logical chunks and remember to commit early and often. <!-- .element: class="fragment" -->
- Consider using Git's "Stash" feature. <!-- .element: class="fragment" -->
- If you absolutely need to commit, commit to a separate branch. <!-- .element: class="fragment" -->


---

# Test Before You Commit 

Resist temptations to commit something that you *think* works. 

- Test it thoroughly 
- make sure it really is completed ...
- ..and has no side effects (as far as one can tell).

<br>

### Don't Break The Build <!-- .element: class="fragment" -->


---

<!-- .slide: class="leftalign" -->
# Write Good Commit Messages

Begin your message with a short summary of your changes.

(Up to 50 characters as a guideline).

Separate it from the following body by including a blank line.

The body of your message should provide detailed answers to the following questions:
<!-- .element: class="fragment" -->
 - What was the motivation for the change?
 - How does it differ from the previous implementation?

<!-- .element: class="fragment" -->


---

# Use Branches 
Branching is Git's most powerful feature.

The tool to help you avoid mixing up different lines of development.

![image](assets/images/gitflowworkflow.png) <!-- .element height="70%" width="70%" -->


---

# Agree on a Workflow 

<div style="text-align: left;">

<p>However you choose to work, just make sure to agree on a common workflow that everyone follows.  </p>

<p>Following and enforcing other practices such as a code standard helps with merging. </p>

</div>

---

# Version Control is not a Backup System

Commit selectively - don't just cram in files.

<br>

### Git is not for Binary Files <!-- .element: class="fragment" -->

## Keep The Repo Clean <!-- .element: class="fragment" -->


---

# What typically goes into a code repository

- Source code files. (.cpp, .h, .java)
- README.md
- LICENSE
- .gitignore
- Buildscripts / makefiles (CMakeLists.txt)
- git submodules


---

# What does typically not go into a code repository

- Binary files (.wav, .mp3, .jpg, .pdf)  [ok for small things] <!-- .element: class="fragment" -->
- Compiled Code (.exe, .dll, .lib, .so, .obj) <!-- .element: class="fragment" -->
- IDE project files (.vcxproj, .sln) <!-- .element: class="fragment" -->
- Junk and temp files (.log) <!-- .element: class="fragment" -->
- Old code in a separate 'old-code' folder. <!-- .element: class="fragment" -->

<br>

__There are marks at stake here, do this right.__ <!-- .element: class="fragment" -->



---

# Why clean repos? 

Evidence of good software engineering - only include what you need.

Therefore it follows that you *know* should what you need.


<br>

You should understand everything that is in your repo, and be able to justify it's place there. <!-- .element: class="fragment" -->

If you can't; then it shows that you don't understand your own work. <!-- .element: class="fragment" -->

---

# Version Control Best Practices

## Summary
- Commit Often
- Commit Related Changes
- **Don't Break the build**
- Use Branches
- Agree on a Workflow
- Write Good Commit Messages
- **Keep The Repo Clean**


---

# Version Control - Working in teams

---

# Merging 

- Make sure you have the newest stuff
- **Always pull before starting working**

![image](assets/images/gitexample.png) <!-- .element height="110%" width="110%" -->

Advanced trick: You can do "Squash and Rebase" technique to make this
look neater.

---

# Merging 2 

If there were changes on the branch you want to merge into:

- Merge them into your branch, before merging onto it.
- Ensures your work is compatible
![image](assets/images/gitexample2.png) <!-- .element height="100%" width="100%" -->


---

# Branches 

For individual projects, 2 types of branches work fine.

- Master
- Feature Branches

When you scale up onto bigger projects you add more to suit the
development workflow. <!-- .element: class="fragment" -->

There is one rule to rule them all: <!-- .element: class="fragment" -->
** Never break Main ** <!-- .element: class="fragment" -->

- **Development / Master / Main**
    - Merge your feature branches into here
- **Testing**
    - Dev merges into here, get tested before merge to Release
- **Release**
    - Tagged and available for download to the public.
    - Also commonly called "Production" or "Live" branch

<!-- .element: class="fragment" -->

---

# Naming Note
GitHub, like many companies, is in the process of changing the name of the 'Master' branch. It seems to be in progress at time of writing, so you might see it called 'Main', 'Default', or 'Origin' in some tools!

(Don't worry, just remember to **Never break the main branch**)

---

<!-- .slide: class="leftalign" -->
# Pull Requests 

**Not actually a native git feature, Github introduced this concept**


Pull requests are a mechanism for a developer to notify team members that they have completed a feature.

Once their feature branch is ready, the developer files a pull request via their Bitbucket/GitHub account. 
This lets everybody involved know that they need to review the code and merge it into the master branch.

But, the pull request is more than just a notification.
It's a dedicated forum for discussing the proposed feature. 
If there are any problems with the changes, teammates can post feedback in the pull request and even tweak the feature by pushing follow-up commits.
All of this activity is tracked directly inside of the pull request.



---

# Merge Conflicts 

When git-dif doesn't know what you want to keep.

You can resolve them manually with a text editor


``` {language="c++"}
for(int i; i <3; ++i){
    <<<<<<< HEAD
    cout << "hello" << i << end;
    =======
    cout << i << end;
    >>>>>>> branch-a
}
```

Simply remove the bit you don't want, save the file, and commit.

Git GUIs can do most of this for you.


---

# Writing code for deployment 


---

# Writing code for deployment 
<!-- .slide: class="leftalign" -->
You will be building programs that run as standalone executables.


The big difference here is we are not running from an IDE. 


You must know how to **build, test, and deploy** your code. 

<br>
To do this, you must have knowledge of the following things:

- Know what your program does
- What your program needs to build
- Where it builds
- How it builds
- What it need to run

<!-- .element: class="fragment" -->
<br>
Submitting a game that doesn't run, due to something like missing .DLLs will lose you serious marks. <!-- .element: class="fragment" -->

---

# Writing code for deployment 
- People lose marks for this **Every. Single. Year.** 
- Don't make me go looking for the right version of a .DLL <!-- .element: class="fragment" -->
- **TEST YOUR CODE ON ANOTHER MACHINE BEFORE SUBMITTING** <!-- .element: class="fragment" -->

---

# C++ Software Engineering methods


---

<!-- .slide: class="leftalign" -->
# C++ ecosystem 

C++ is a low-level language.


It runs natively on the system, therefore must be compiled specifically for the system it is running on.


Unlike Java, there is no "C++ Virtual Machine" or universal cross-platform runtime. <!-- .element: class="fragment"data-fragment-index="1" -->


This makes writing robust and portable applications difficult. <!-- .element: class="fragment" data-fragment-index="1"-->


---

<!-- .slide: class="leftalign" -->
# Cross platform code 

Standard C++ code that just does logic is basically cross-platform.

Where we get into trouble is when hardware and system calls.
- Windowing
- Input
- Graphics
- Sound
- Threading (native since C++11)
- Time (native since C++11)
- Maths

<!-- .element: class="fragment" -->

Each operating system has it's own way of doing this. (See POSIX)
<!-- .element: class="fragment" -->

So, we use libraries that do all this for us.
e.g SFML, GLFW, SDL.
<!-- .element: class="fragment" -->

---

<!-- .slide: class="leftalign" -->
# C++ Package (Mis)Management

No native way of package management. So, how to include external libraries?
We can build from source (with submodules), or we can include external libraries in the repository.

Differences:
- Source allows access to newest code, external libraries are built with fixed settings/version
- Source allows us to debug through code, external libraries need to explicitly provide extra files
- Source results in better compatibility, as code is built with same settings as your other projects
- Setting up building from source is more complicated and brittle

Header-only libraries are popular, because it's the best of both worlds, but typically slower to compile

<!-- .element: class="fragment" -->

---

# Our build system

![image](assets/images/cmake.png)  <!-- .element height="100%" width="100%" -->


---

<!-- .slide: class="leftalign" -->
# CMake 
An open-source, cross-platform tools for configuring, building, testing and packaging software.

Write **one**  CMakeLists.txt config file, dictating what your application needs to compile.

CMake processes this and generates solution files relevent to the platform you are working on.

Not all C++ projects use CMake or have a working config. <!-- .element: class="fragment" -->

CMake is covered more in depth in the labs. <!-- .element: class="fragment" -->

---

# CMake 

- You will absolutely come to hate CMake...
- BUT, it is used in many big software houses, such as EA. <!-- .element: class="fragment" -->
- So... get used to it now, and make sure you understand the labs! <!-- .element: class="fragment" -->

</textarea>
</section>