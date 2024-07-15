# Parallel Global VLSI Router

This repository contains the implementation of a Parallel Global Router for Very Large Scale Integration (VLSI) design. The focus is on optimizing the routing of connections between pins on a chip during the physical design stage. This involves converting an abstract netlist and logic level design into a chip layout that can be fabricated.

## Background

In VLSI design, routing metal interconnects between component pins is crucial. These interconnects are made of straight metal wires and vertical connections called vias, arranged in multiple layers. Wires travel in one direction: horizontally on odd-numbered layers and vertically on even-numbered layers. Given the NP-Hard nature of this routing problem, we utilize heuristics and OpenMP for parallelization to route nets


## Repository Structure

The repository includes C, C++, and Python files necessary for creating nets, routing, evaluating, and visualizing routes.

## tst_5

A small test case has been uploaded, for understanding the data formats, and verifying the working of the code. The input is a .net and .cap file and the output is a .PR_output file, along with an image for visualizing the connections
