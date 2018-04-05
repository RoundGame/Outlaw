#pragma once
/*
(0, 0)		0
(0, 1)		1
(0, -1)		2
(-1, 0)		3
(1, 0)		4
(0.7, 0.7)	5
(0.7, -0.7)	6
(-0.7, 0.7)	7
(-0.7, -0.7)	8

x y z w
L R U D

a0 + a1x + a2y + a3z + a4w + a5xy + a6xz + a7xw + a8yz + a9yw + b0zw + b1xyz + b2xyw + b3xzw + b4yzw + b5xyzw= t

direction = 2L + R + 4U + 3D -3LR + LU + 3LD + 2RD -7UD -4LRU -6LRD - 3LUD + RUD + 6LRUD



direction = 2 * key[LEFT].isPressed + key[RIGHT].isPressed + 4 * key[UP].isPressed + 3 * key[DOWN].isPressed - 3 * key[LEFT].isPressed * key[RIGHT].isPressed + key[LEFT].isPressed * key[UP].isPressed + 3 * key[LEFT].isPressed * key[DOWN].isPressed + 2 * key[RIGHT].isPressed * key[DOWN].isPressed - 7 * key[UP].isPressed * key[DOWN].isPressed - 4 * key[LEFT].isPressed * key[RIGHT].isPressed * key[UP].isPressed - 6 * key[LEFT].isPressed * key[RIGHT].isPressed * key[DOWN].isPressed - 3 * key[LEFT].isPressed * key[UP].isPressed * key[DOWN].isPressed + key[RIGHT].isPressed * key[UP].isPressed * key[DOWN].isPressed + 6 * key[LEFT].isPressed * key[RIGHT].isPressed * key[UP].isPressed * key[DOWN].isPressed;


a0 + a1x + a2y + a3xy = t



*/