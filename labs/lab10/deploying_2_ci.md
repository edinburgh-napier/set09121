---
title: "Lab 10_2 - Continuous Integration"
keywords: ab10_2
tags: [ab10_2]
permalink: lab10_2.html
summary: lab10_2
sidebar: home_sidebar
---
## Continuous Integration

### Building in the Cloud

There are a few major free CI services I would recommend:
1. [AppVeyor](https://www.appveyor.com/) - Windows only
2. [Travis-CI](https://travis-ci.org) - Linux/Mac only
3. [GitHub Actions](https://github.com/features/actions) - Newish GitHub CI
3. Or host your own service on a server somewhere with Jenkins [Jenkins](https://jenkins.io)


### AppVeyor

As we are mainly working with Windows, I will only walk through the AppVeyor steps. If you look at the Repo you will see a travis.yml file to see how the configuration differs for travis.

1. Log into AppVeyor via Github 
2. Select "New Project" and select your repo. You could work through this using your coursework repo. I'm going to use the main lab repo.
3. Head over over to the Build settings.

#### AppVeyor Settings

There are two methods for configuring CI. You can change settings using the Website UI, or use a configuration .yml file saved in your repo. If AV sees a configuration file it will use these settings to overwrite the Web UI settings. Have a look in the Astral Insanity repo and you will see an 'appveyor.yml'. This contains all the settings I have set.

If you prefer to use the Web UI, Here is the main settings you need to change to get your project building:

- Environment > Build worker image
  - "Visual Studio 2017"
- Build > Configuration
  - Debug, Release
- Build > Visual Studio solution or project file
  - For me this is set to "build/PACKAGE.vcxproj" of you it may be "build/game_name.vcxproj"
- Build > Before build script
  - ```bash
     git submodule update --init --recursive
     mkdir build & cd build & cmake -G "Visual Studio 15 2017 Win64" ../
     ```
- Build > After build script
  - This is optional, it does not change the build. It will zip up all the output files, this enables us to use it as an artifact
  - ```bash
    cd %APPVEYOR_BUILD_FOLDER% 
    set LOC=%APPVEYOR_BUILD_FOLDER%/build/bin/%CONFIGURATION%/
    set ZIPNAME=set09121_%CONFIGURATION%_%APPVEYOR_REPO_COMMIT%.zip
    7z a %ZIPNAME% %LOC%/*.exe %LOC%/*.dll %LOC%/res/* %LOC%/res/**/*
  ```
- Artifacts > After build script
  - Path: \*.zip, Deployment Name : zip

#### Triggering the build

AppVeyor will be able to build your repo manually, by triggering a build from the Web UI. If you want this to happen automaitcally everytime a commit is pushed, you must set up a webhook via Github. The AV docs walk you though this.

#### Badges

The most important part of CI, is having that amazing "Build Passing" badge on your Repo. The Badge page under the AV settings supplies you with the markdown code. Simply put this in your Repo's README and you will see badge rendered on your github page.

#### Artifacts

We have set these up in the above config, but what are they? Artifacts are just things that the build process generated that you want to keep a hold of. In our case it is the entire contents of the build output folder. If you are building the installer with CI, the installer would also be an artifact. This is useful as it allows you to go back in time to any commit (so long as it built successfully) and run the output.

### CI Summary

As you can see from our configuration steps, all we are really doing is automated the steps that we usually do ourselves. You will probably have to tweak some settings somewhere, there is no one size fits all for CI configuration. You will have to play about with settings to get things to work.

Once you have tweaked everything to work properly, export the web settings to a .yml file so you can quickly reference and re-use your hard work.

Previous step: [Deploying](lab10_1)

Next step: [Networking](lab11)