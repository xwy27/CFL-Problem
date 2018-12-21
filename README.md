# CFL-Problem

Capacitated Facility Location Problem

## Description

Suppose there are n facilities and m customers. We wish to choose: 
1. which of the n facilities to open 
2. the assignment of customers to facilities

The objective is to minimize the sum of the opening cost and the assignment cost.

The total demand assigned to a facility must not exceed its capacity.

There are currently 71 data files. 

## Instance

$J,I$
$s_1,f_1$
$s_2,f_2$
$...$
$s_J,f_J$
$d_1,d_2,d_3,…,d_I$
$c_{11},c_{12},c_{13},…,c_{1I}$
$c_{21},c_{22},c_{23},…,c_{2I}$
$...$
$c_{J1},c_{J2},c_{J3},…,c_{JI}$

where:

$J$ is the number of potential facility locations;
$I$ is the number of customers;
$s_j (j=1,...,J)$ is the capacity of facility $j$;
$f_j (j=1,...,J)$ is the fixed cost of opening facility $j$;
$d_i (i=1,...,I)$ is the demand of customer $i$;
$c_{ji} (j=1,...,J), (i=1,...,I)$ is the cost of allocating all the demand of customer $i$ to facility $j$.
