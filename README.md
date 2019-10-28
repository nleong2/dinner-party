# Dinner Party

## The Goal
Find a "good" table score for a given set of people based on party selection and preferences to one another.

### Scoring
- 1 point for every adjacent pair (seated next to each other) of people with one a host and the other a guest.
- 2 points for every opposite pair (seated across from each other) of people with one a host and the other a guest.
- h(p1, p2) + h(p2, p1) points for every adjacent or opposite pair of people p1, p2.

### Algorithm
This algorithm uses local search by swaping seats next to each other and comparing the scores. 

Randomizing the algorithm helps discover new directions and lesson the impact of local minima.

### Execute Program
`make`

`./dinnerparty file1 file2 ...`

### Driver
`main.cpp` will loop through each file and search the given input 4 time for ~30 seconds. Each file will be searched using 0%, 25%, 50% and 100% randomizations.

*This program was written for the 2019 Fall AI course at Portland State University.*
