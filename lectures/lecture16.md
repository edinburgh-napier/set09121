---
title: "Lecture16"
keywords: Lecture
tags: [Lecture]
permalink:  lecture16.html
summary: lecture16
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 16 - Decision Trees
### SET09121 - Games Engineering

<br><br>
Leni Le Goff
<br>


School of Computing. Edinburgh Napier University



---

# Recommended Reading

- Artificial Intelligence for Games. Second Edition. Millington and
    Funge (2009).


 ![image](assets/images/ai_book.jpg)


---

| Decision Tree                                                               |                                                              Behaviour Tree |
| --------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ![image](assets/images/decision_tree_ex.png) <!-- .element width="100%"  --> | ![image](assets/images/behaviour_tree_ex.png) <!-- .element width=50%"  -->|



---


## Decision Trees

![image](assets/images/decision_tree_ex.png)

---

# What are Decision Trees?

- Decision trees provide us with an approach to modelling a decision.
- The decision structure is formed into a tree.
    - We traverse different branches based on the decision we wish to make.
- The decision to go down a branch can be determined by:
    - The current state of the game. (E.g., testing some value)
    - Randomly, by drawing a value from a distribution.
- At the end of a branch, a decision is made, and therefore an action is undertaken.


---

# Decision Tree - Example 1

- The sophisticated guard.
- The guard has some basic actions.
    - The guard patrols between point A and point B.
    - The guard has a 20% chance of stopping while patrolling.
    - If the guard is shot at, the guard will stop patrolling, engage with the player, and fire back.
    - If the guard sees the player, they will engage the player.
    - If the guard loses sight of the player, the guard will return to patrolling between point A and point B.


---

# Decision Tree - Diagram 


![image](assets/images/guard_decisions.png) <!-- .element width="85%"  -->


---

# Diagram Explanation

- A decision tree is made up of a number of nodes.
    - Nodes &rarr; decision points, edges &rarr; results
- And a number of transitions.
	- A transition has a condition associated with it, e.g. 80%
- We traverse the tree, starting at the root node, making decisions based on information, before arriving at a leaf.

---

# Using Activity Diagrams

- We have previously seen state diagrams used for modelling state machines.
- We can undertake a similar approach with activity diagrams for decision trees.
- Activity diagrams provide us with guarded transitions (edges)
    - The "guard" is simply a decision.
- Activity diagrams also provide a choice or branch symbol (nodes)
- If you want, you can use the action states as the actual actions to take.

---

# Activity Diagram example


![image](assets/images/activity_diagram_example.png) <!-- .element width="65%"  -->


---

# Decision Trees in Our Game Engine

- Our aim is to implement basic, reusable decision tree behaviour within our engine.
    - We want reusable so that is simple for us to extend functionality if required.
- We will be using a tree-like data structure to implement the decision tree behaviour.
- Each decision point will be tested to determine which path to follow. The end decision will result in an action.


---

# `DecisionTreeNode` Interface

- Defines only one method.
    - `make_decision`
- `make_decision` is called by the `entity` which in turn calls `make_decision` on any child nodes.


 ![image](assets/images/decision_tree_node.png)


---

# Implementing Nodes

- `Decision` and `MultiDecision` implement the `DecisionTreeNode` interface.
- Their `make_decision` method will call the `make_decision` on one of the child nodes returned by `get_branch`.
- `get_branch` is defined by the programmer based on required parameters.


 ![image](assets/images/decision_node_types.png) <!-- .element width="80%"  -->

---

# Using the Class

- We can make a random decision class implementation just by extending the decision class.
- On the `get_branch` code we just generate a random number and use it to determine the action to.

```cpp
static std::random_device rd;
static std::default_random_engine e(rd());
static std::uniform_real_distribution<> dist(0, 1);
bool choice = dist(e) <= _probability;
if (choice)
    return _true_node;
else
    return _false_node;
```


---

# Diagram to Implementation

```cpp
decision_tree = std::make_shared<PlayerVisibleDecision>( 
    std::make_shared<EngageDecision>(), 
    std::make_shared<ChanceDecision>( 
        0.8f, std::make_shared<PatrolDecision>(),
         0.2f, 
         std::make_shared<WaitDecision>())
    );
```


![image](assets/images/guard_decisions.png)


---

# Comments on Decision Trees
- Decision trees are very useful when you want to map a complex decision.
    - Granted, these are just nested if statements, but those can get messy.
- Decision trees can be reused easily enough.
- Decision trees can get quite complex however.
    - The deeper the tree, the longer it will take to make a decision.
- We are also using a number of virtual function calls to implement the tree.
    - Remember, virtual function calls are more expensive than normal function calls.


---

# Summary

- Decision trees are a useful diagrammatic technique and algorithm to create AI that can make decisions.
    - We still need to determine the decisions to program though.
- We can work with activity diagrams to model our decision trees.
- We can also combine decision trees and state machines to create more complex data.

---

## Behaviour Tree

![image](assets/images/behaviour_tree_ex.png)

---

# Limitations of State Machines

- State machines are good at modelling a system having a few states.
- They can get messy when having a lot of states and transistions.
- Complex tasks involving many steps will require a lot of states.
- **Behaviour Trees** are an alternative that you might want to consider when you need to deal with a lot of states.

---

# Definition

- A Behaviour Tree is a model for plan execution.
- They are popular in games to model AI characters.
- They consist of: <!-- .element: class="fragment" -->
    - A **root** node.
    - Branch nodes: **control** nodes and **decorator** nodes.
    - Leaf nodes: **action** nodes and **condition** nodes.

---

# Evaluating a Behaviour Tree

- Each tick/update we evaluate the tree starting from the root.
- The root will ask its child node about its current status.
- The status of a node might depend on its children.
- This results in a depth-first traversal.
- A node's status can be: <!-- .element: class="fragment" -->
    - Success: The node has been successfully resolved.
    - Failure: The node has been unsuccessfully resolved.
    - Running: The node has not yet been resolved.

---

# Evaluating a Behaviour Tree

![image](assets/images/behavior_tree_traversal.gif) <!-- .element width="100%" -->

---

# Action Nodes

- Affects the state of the game.
    - Move the NPC.
    - Change the state of an object.
    - Play animation.
    - Play audio.
    - Trigger dialog.

---

# Example: Behaviour Tree with 1 Action

- Only one **action**: Enter House
- The action is directly connected to the root.
- The action node will return:
    - Success: If the inside of the house has been reached.
    - Failure: If it is not possible to reach the inside of the house.
    - Running: As long as the agent is on its way.

---

# Example: Behaviour Tree with 1 Action

![image](assets/images/behavior_tree_1action.png) <!-- .element width="80%" -->

---

# Example: Behaviour Tree with 1 Action

- At each tick/update, the tree is evaluated.
- The root will ask the node about its status.
- The node will keep returning **Running** while the agent is still on its way.

---

# Control Nodes

- The moment we want to have more than one action (leaf nodes), we need control nodes (branch nodes).
- The two most common control nodes are:
    - **Selector**: Evaluates each children until one is found that does **not** fail and returns the status of this child node.
    - **Sequence**: Evaluates each children until one is found that does **not** succeed and returns the status of this child node.
- Note that the child nodes have a pre-determined **order**.
- We will look at a few example to see how these control nodes are used.

---

# Selector

- Evaluates each children until one is found that does **not** fail and returns the status of this child node.
- As soon as a node is found that returns **running** or **success**, no further child nodes are evaluated.
- Consquently, only one node will change its status to **running** or **success**.
- The Selector node selects the first non-failing node.
- The order of the child nodes describes their **priority**.

---

# Example: Selector

- Can you describe in your own words what kind of behaviour this tree represents?

![image](assets/images/behavior_tree_selector.png) <!-- .element width="80%" -->

---

# Sequence

- Evaluates each children until one is found that does **not** succeed and returns the status of this child node.
- As soon as a node is found that returns **running** or **failure**, no further child nodes are evaluated.
- Consquently, all nodes will eventually change its status to **running** or **success**, unless one of the nodes in the squence fails.
- The Sequence node describes a sequence of nodes that need to be evaluated, but stops the sequence if a failing node is encountered.
- The order of the child nodes describes their order within the sequence.

---

# Example: Sequence

- Can you describe in your own words what kind of behaviour this tree represents?

![image](assets/images/behavior_tree_sequence.png) <!-- .element width="80%" -->

---

# Condition

- Like the action node, the condition node is a leaf node.
- A condition node does not change the state of the game world, unlike the action node.
- Condition nodes test for something in the game world: <!-- .element: class="fragment" -->
    - Is the enemy in sight?
    - Is the player on low health?
- A condition returns: <!-- .element: class="fragment" -->
    - **Success** if the test was successful.
    - **Failure** if the test failed.
    - **Running** if the test needs more time to determine the outcome.

---

# Using Conditions

- Conditions can be used to avoid evaluating part of the tree.
- Inside a selector:
    - All children after the condition are only evaluated when the condition **fails**.
- Inside a sequence:
    - All children after the condition are only evaluated when the condition **succeeds**.

---

# Example: Selector

- Can you describe in your own words what kind of behaviour this tree represents?
- What would happen if the condition were missing?

![image](assets/images/behavior_tree_condition_selector.png) <!-- .element width="70%" -->

---

# Example: Sequence

- Can you describe in your own words what kind of behaviour this tree represents?
- What would happen if the condition were missing?

![image](assets/images/behavior_tree_condition_sequence.png) <!-- .element width="70%" -->

---

# Decorator

- Decorators have exactly one child.
- They are somewhat similar to the Decorator Pattern in OOP (hence the name). <!-- .element: class="fragment" -->
- Their return value depends on the state of the child and on the specific type of decorator. <!-- .element: class="fragment" -->
- Decorators are used to change the behaviour of a node. <!-- .element: class="fragment" -->
- There are many different types of decorators with very different use-cases. <!-- .element: class="fragment" -->

---

# Example: Decorator

![image](assets/images/DecoratorEnterRoom.svg)

---

# Extending Behaviour Trees

- Some implementations add Sequence and Selection nodes with **memory**, avoiding evaluating a node that has been successful previously.
- Some implementations add a **Blackboard**, which allows passing information between nodes.
    - This can be used to coordinate different actions.
    - For example: *walk to* and *talk to* should have the same target.
- Some implemntations add additional states like *Error*.

---

# A More Complex Example

- Can you describe in your own words what kind of behaviour this tree represents?

![image](assets/images/behavior_tree_complex.png) <!-- .element width="80%" -->

---

# Summary

- Behaviour Trees can be used to model AI behaviour.
- At each tick/update, the tree gets evaluated and returns a status.
- A tree consist of nodes. 
    - A node's status can be: Success, Failure, Running.
    - Branch nodes: control nodes and decorator nodes.
    - Leaf nodes: action nodes and condition nodes.
- Implementation and resource: [BehaviorTree.CPP](https://www.behaviortree.dev/docs/intro) 
