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

# Lecture 16 - Behavior Trees
### SET09121 - Games Engineering

<br><br>
Babis Koniaris, Tobias Grubenmann
<br>


School of Computing. Edinburgh Napier University



---

# Recommended Reading

- Artificial Intelligence for Games. Second Edition. Millington and
    Funge (2009).


![image](assets/images/ai_book.jpg)

---

# Limitations of State Machines

- State machines are good at modelling a system having a few states.
- They can get messy when having a lot of states and transistions.
- Complex tasks involving many steps will require a lot of states.
- **Behavior Trees** are an alternative that you might want to consider when you need to deal with a lot of states.

---

# Definition

- A Behavior Tree is a model for plan execution.
- They are popular in games to model AI characters.
- They consist of: <!-- .element: class="fragment" -->
    - A **root** node.
    - Branch nodes: **control** nodes and **decorator** nodes.
    - Leaf nodes: **action** nodes and **condition** nodes.

---

# Evaluating a Behavior Tree

- Each tick/update we evaluate the tree starting from the root.
- The root will ask its child node about its current status.
- The status of a node might depend on its children.
- This results in a depth-first traversal.
- A node's status can be: <!-- .element: class="fragment" -->
    - Success: The node has been successfully resolved.
    - Failure: The node has been unsuccessfully resolved.
    - Running: The node has not yet been resolved.

---

# Evaluating a Behavior Tree

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

# Example: Behavior Tree with 1 Action

- Only one **action**: Enter House
- The action is directly connected to the root.
- The action node will return:
    - Success: If the inside of the house has been reached.
    - Failure: If it is not possible to reach the inside of the house.
    - Running: As long as the agent is on its way.

---

# Example: Behavior Tree with 1 Action

![image](assets/images/behavior_tree_1action.png) <!-- .element width="80%" -->

---

# Example: Behavior Tree with 1 Action

- At each tick/update, the tree is evaluated.
- The root will ask the node about its status.
- The node will keep returning **Running** while the agent is still on its way.

---

# Control Nodes

- The moment we want to have more than one action (leaf nodes), we need control nodes (branch nodes).
- The two most common control nodes are:
    - **Selector**: Evaluates each children until one is found that does **not** fail and returns the status of this child node.
    - **Squence**: Evaluates each children until one is found that does **not** succeed and returns the status of this child node.
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

- Can you describe in your own words what kind of behavior this tree represents?

![image](assets/images/behavior_tree_selector.png) <!-- .element width="80%" -->

---

# Sequence

- Evaluates each children until one is found that does **not** succeed and returns the status of this child node.
- As soon as a node is found that returns **running** or **failure**, no further child nodes are evaluated.
- Consquently, all nodes will eventually change its status to **running** or **success**, unless one of the nodes in the squence fails.
- The Squence node describes a sequence of nodes that need to be evaluated, but stops the sequence if a failing node is encountered.
- The order of the child nodes describes their order within the sequence.

---

# Example: Sequence

- Can you describe in your own words what kind of behavior this tree represents?

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

- Can you describe in your own words what kind of behavior this tree represents?
- What would happen if the condition were missing?

![image](assets/images/behavior_tree_condition_selector.png) <!-- .element width="70%" -->

---

# Example: Sequence

- Can you describe in your own words what kind of behavior this tree represents?
- What would happen if the condition were missing?

![image](assets/images/behavior_tree_condition_sequence.png) <!-- .element width="70%" -->

---

# Decorator

- Decorators have exactly one child.
- They are somewhat similar to the Decorator Pattern in OOP (hence the name). <!-- .element: class="fragment" -->
- Their return value depends on the state of the child and on the specific type of decorator. <!-- .element: class="fragment" -->
- Decorators are used to change the behavior of a node. <!-- .element: class="fragment" -->
- There are many different types of decorators with very different use-cases. <!-- .element: class="fragment" -->

---

# Extending Behaviour Trees

- Some implementations add Sequence and Selection nodes with **memory**, avoiding evaluating a node that has been successful previously.
- Some implementations add a **Blackboard**, which allows passing information between nodes.
    - This can be used to coordinate different actions.
    - For example: *walk to* and *talk to* should have the same target.
- Some implemntations add additional states like *Error*.

---

# A More Complex Example

- Can you describe in your own words what kind of behavior this tree represents?

![image](assets/images/behavior_tree_complex.png) <!-- .element width="80%" -->

---

# Summary

- Behavior Trees can be used to model AI behavior.
- At each tick/update, the tree gets evaluated and returns a status. <!-- .element: class="fragment" -->
- A tree consist of nodes. <!-- .element: class="fragment" -->
    - A node's status can be: Success, Failure, Running.
    - Branch nodes: control nodes and decorator nodes.
    - Leaf nodes: action nodes and condition nodes.
