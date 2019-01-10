---
title: "Lecture16"
keywords: Lecture
tags: [Lecture]
permalink:  lecture16.html
summary: lecture16
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
Recommended Reading

.5

-   Artificial Intelligence for Games. Second Edition. Millington and
    Funge (2009).

.5 ![image](ai_book){width="\textwidth"}

Review
======

Review -- Activity Diagrams

.5

-   Activity diagrams allow us to model the flow through the application
    from a high-level.

    -   Menu screens.

    -   Internal system interactions.

-   The goal is to be able to create a skeleton of the functionality
    before implementing it.

.5 ![image](activity_diagram){width="\textwidth"}

Review -- AI Techniques

-   There are numerous usable AI techniques applicable to games
    development.

    -   Classical, deterministic techniques -- popular.

    -   Academic, non-deterministic techniques -- useful in some areas.

-   Different techniques accomplish different aspects of game behaviour.

    -   Movement.

    -   Decision making.

    -   Strategy.

    -   Learning.

-   Today we will look at the encoding decisions using decision tree
    techniques.

Review -- State Machines

-   State machines (or specifically in our case Finite State Machines --
    FSM) are one of the most fundamental concepts and cornerstones of
    computer science.

-   A state machine is a technique of describing and modelling the state
    (e.g. behaviour, control, etc.) of a system in a mathematical
    manner.

-   The system is modelled with a number of states and the transitions
    between these states.

    -   The idea of a graph of states can come into play here --
        remember our description of a graph last week.

Review -- State Machines

.6

-   Let us return to the guard concept we presented last week.

-   We will take a simple view so we can just focus on state.

-   The guard has some basic actions:

    -   The guard patrols between point A and point B.

    -   If the guard is shot at, the guard will stop patrolling, engage
        the player, and fire back.

    -   If the guard loses sight of the player, the guard will return to
        patrolling between point A and point B.

    -   If the guard is hit, the guard will fall onto the ground and
        die.

.5 ![image](simple_state_guard){width="\textwidth"}

What are Decision Trees?
========================

What are Decision Trees?

-   Decision trees provide us with an approach to modelling a decision.

-   The decision structure is formed into a tree.

    -   We traverse different branches based on the decision we wish to
        make.

-   The decision to go down a branch can be determined by a number of
    factors:

    -   Variable checking.

    -   Probability.

    -   Chance.

-   At the end of a branch, a decision is made, and therefore an action
    is undertaken.

Decision Tree -- Example 1

-   The sophisticated guard.

-   The guard has some basic actions.

    -   The guard patrols between point A and point B.

    -   The guard has a 20% chance of stopping while patrolling.

    -   If the guard is shot at, the guard will stop patrolling, engage
        with the player, and fire back.

    -   If the guard sees the player, they will engage the player.

    -   If the guard loses sight of the player, the guard will return to
        patrolling between point A and point B.

Decision Tree -- Diagram ![image](guard_decisions){width="\textwidth"}

Diagram Explanation

-   A decision tree is made up of a number of nodes.

    -   Decision points.

-   And a number of transitions.

-   A transition has a condition associated with it.

    -   For example, 88%.

-   We travel down the tree, starting at the root node, making decisions
    based on information, before hitting a final point.

-   Decision trees are used in computers to diagnose problems.

    -   And by call centre operators in a similar manner -- not that I
        am saying a call centre operator are as simple as a computer
        when making decisions!

Using Activity Diagrams

-   We have used state diagrams to help us model state machines for our
    previous look into AI.

-   We can undertake a similar approach with activity diagrams for
    decision trees.

-   Activity diagrams provide us with guarded transitions.

    -   The guard is simply a decision.

-   Activity diagrams also provide a choice or branch symbol.

-   If you want, you can use the action states as the actual actions to
    take.

Decision Tree -- Example 2 ![image](decision_tree){width="\textwidth"}

Decision Trees in Our Game Engine
=================================

Decision Trees in Our Game Engine

-   Our aim is to implement basic, reusable decision tree behaviour
    within our engine.

    -   We want reusable so that is is simple for us to extend
        functionality if required.

-   We will be using a tree like data structure to implement the
    decision tree behaviour.

    -   Those of you doing algorithms and data structures will see how
        we do this.

-   Each decision point will be tested to determine which path to
    follow. The end decision will result in an action.

`DecisionTreeNode` Interface

.5

-   Defines only one method.

    -   `makeDecision`

-   `makeDecision` is called by the `entity` which in turn calls
    `makeDecision` on any child nodes.

.5 ![image](decision_tree_node){width="\textwidth"}

Implementing Nodes

.5

-   `Decision` and `MultiDecision` implement the `DecisionTreeNode`
    interface.

-   Their `makeDecision` method will call the `makeDecision` on one of
    the child nodes returned by `getBranch`.

-   `getBranch` is defined by the programmer based on required
    parameters.

.5 ![image](decision_node_types){width="\textwidth"}

Using the Class

-   We can make a random decision class implementation just by extending
    the decision class.

-   On the `getBranch` code we just generate a random number and use it
    to determine the action to.

<!-- -->

    bool nextChoice = rand() == 0 ? true : false;
    if (nextChoice)
        return trueNode;
    else
        return falseNode;

Diagram to Implementation

.6

``` {basicstyle="\tiny\ttfamily"}
decisionTree =
    make_shared<PlayerVisibleDecision>(
        make_shared<EngageDecision>(),
        make_shared<ChanceDecision>(
            0.8f,
            make_shared<PatrolDecision>(),
            0.2f,
            make_shared<WaitDecision>()));
```

.4 ![image](guard_decisions){width="\textwidth"}

Combining Decision Trees and State Machines
===========================================

Combining State Machines and Decision Trees

-   We can combine state machines and decision trees to create a more
    complex AI behaviour.

-   Decision trees are used to make decisions.

-   State machines are used to perform the actions made by decisions.

-   This is a powerful technique, and it is what we will do in the
    practical.

    -   Steering behaviours can then be merged into the states to allow
        stateful movement based on decisions.

State Machine and Decision Tree
![image](simple_state_guard){width=".8\textwidth"}\
![image](guard_decisions){width=".6\textwidth"}

Combined Diagram ![image](guard_decision_state){width="\textwidth"}

Comments on Decision Trees

-   Decision trees are very useful when you want to map a complex
    decision.

    -   Granted, these are just nested if statements, but those can get
        messy.

-   Decision trees can be reused easily enough.

-   Decision trees can get quite complex however.

    -   The deeper the tree, the longer it will take to make a decision.

-   We are also using a number of virtual function calls to implement
    the tree.

    -   Remember, virtual function calls are more expensive than normal
        function calls.

Summary
=======

Summary

-   Decision trees are a useful diagrammatic and implementation
    technique to create AI that can make decisions.

    -   We still need to determine the decisions to program though.

-   We can work with activity diagrams to model our decision trees.

-   We can also combine decision trees and state machines to create more
    complex data.
