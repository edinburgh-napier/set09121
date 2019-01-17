---
title: "Lecture18"
keywords: Lecture
tags: [Lecture]
permalink:  lecture18.html
summary: lecture18
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 18 - QA and TRC
### SET09121 - Games Engineering

<br><br>
Kevin Chalmers and Sam Serrels

School of Computing. Edinburgh Napier University


---

# QA
==

Bug-Catching ![image](portal2_bugs)

QA \"\[QA and publishing\] is like being test on how well you can
crash-land a plane\" - Tim Schafer

<https://www.youtube.com/embed/ssUFuZyu5bw?start=974&end=1353>

Bug-Tracking ![image](bug_tracker)

Bug-Catching ![image](civ)

Bug-Catching

[**Gamasutra Article - My Hardest Bug
Ever**](https://www.gamasutra.com/blogs/DaveBaggett/20131031/203788/My_Hardest_Bug_Ever.php)\
The symptom was that you'd go to save your progress and it would access
the memory card, and almost all the time, it worked normally\... But
every once in a while the write or read would time out\... for no
obvious reason. A short write would often corrupt the memory card. The
player would go to save, and not only would we not save, we'd wipe their
memory card.

TRC
===

TRC Examples When save data is loaded, the state of the application at
the save timing (including game progress, status of characters and their
owned items, play history, etc.) is restored correctly.

TRC Examples If the time required for loading exceeds 30 seconds, the
application displays an animation. Progress information such as a
progress bar or the remaining time is displayed if the time required for
loading exceeds 60 seconds.

TRC Examples Requirement: Users are not requested or instructed to
vigorously shake the wireless controllers with one hand.

Test all on-line activity with a 2000 friend account

Your TRC

-   2D graphics engine using SFML

-   Main menu (Ability to quit to menu, and restart game)

-   Some form of AI

-   Interactive Sound (i.e not just background music)

-   1080p/60fps on a reasonable systems specification

-   Usability options:\
    Remappable controls\
    Controller support

-   Graphics options (Resolution & window mode)

-   Windows: 10 x64

-   Single file .exe game installer/uninstaller

-   User preference/savegame saving/loading from disk

-   Web presence with game promo material and downloads

Your TRC pt2 Software Design & Code Quality

-   Tidy, documented, and organised code.

-   Use of appropriate software patterns.

-   Evidence of performance analysis and optimisation.

Software Engineering Methods & Testing

-   Evidence of proper version control best practises

-   Evidence of proper project management

-   Working continuous integration

-   Working build testing

-   Evidence and reports from playtests

Accessibility <http://gameaccessibilityguidelines.com>

-   Allow the game to be started without the need to navigate through
    multiple levels of menus

-   Ensure no essential information is conveyed by a colour alone

-   Ensure no essential information is conveyed by sounds alone

-   Offer a wide choice of difficulty levels

-   Give a clear indication that interactive elements are interactive

-   Allow interfaces to be resized

-   Allow all narrative and instructions to be replayed

Localisation Watch out for\...

-   EFIGS!

-   Vertical text!

-   Text in Art Assets!

-   Gendered Languages

-   Player chat & Unicode

![image](MissingNo){width="40%"}

Publishing
==========

Role of publishers Then:

-   Bankroll Development

-   Handle All Marketing

-   Negotiate Physical Sales and distribution

-   Localisation and QA

-   Legal Protection

-   Occasionally hotdrop producers into dev team

Now:

-   *might* pay you something before game is finished

-   Handle Some Marketing, Devs do \"Community management\"

-   Negotiate -some- sales deals

-   **Localisation and QA**

-   **Legal Protection**

Legals

I'm Not a Lawyer, and Neither are you.

-   Get A lawyer

-   Form A Company

-   Protect your IP

-   Don't Get Sued

[LINK: IGDA white papers on IP](http://www.igda.org/?page=resources)\
If you game can be cloned. It will be cloned.

Legal costs

-   **Form a company** - Protects *you* if you are sued \[Actually
    pretty easy in the UK\]  100-1000

-   **Contracts** - Formal agreement on ownership of work. Get one even
    if someone is doing work for free.  600-1000

-   **Trademark your game and company** - People can still clone your
    game, but they can't pretend to be you  500 -£2000

-   **Terms of Service and Privacy Policy** Stop you being sued in the
    first place  600-1000

-   **Package deals** - Common for Indies  3000

Disclaimer: costs found after 20 mins of googling + I'm not a lawyer.

Review

-   Keep enough time for QA

-   Consider Localisation & Accessibility

-   TRC == MARKS

-   Game publishing is a legal battleground

-   If you plan on Selling your game.. Good luck
