# Curve-Line-Drawing-System
A simple curve line drawing system that uses Bezier and B-spline.


0.655556 0.93         x, y position for the control points in world coordinates (0 to 1)
0.782222 0.925556
0.805556 0.718889
0.591111 0.764444
0.596667 0.86
0.733333 0.736667

My program reads this file and draw the curves from this file and allows users to modify
these curves as well as add a new curve or delete a selected curve. After the user makes all
changes to the curves, and press esc to exit and save the scene, my program will output to
the same file (test.txt) with the new curves information.

All controls points are drew as a square for better mouse clicking.
Control points for Bezier curve is labled blue color, curve is red color.
Control points for Bspline curve is labled pink color, curve is green color.

To perform add points for a curve, the user need to select the curve to add points first.
To select a curve, simply use your mouse to left-click one of the control points of the 
curve you want to add. Once you select the curve, the user can simply left-click anywhere
in the window to add a new control point for the curve (it always adds the point as the last
point of the control points). Also, when the user selects a Bspline curve, the k value for that 
curve will be showed on the bottom right corner of the window. Then the user can press K to
increase k by 1 or press L to decrease k by 1. (Notice: k must be in range in (1, n + 1), so
if the user trys to increase or decrease more than this range, it won't work and an error message
will be printed out on the cosole.)

To move a control points of any curve, just hold the left-click down of the mouse on the point and
drag to somewhere you want it to be. To delete a control point of any curve, just right-click the point
you want to delete. (Notice: the number of control points can not be lower than 1 for Bezier curve
and not be lower than k for Bspline curve).

To add a new curve, press 1 to add a new Bezier curve, press 2 to add new Bspline curve. Then the user
needs to left-click on the window to choose the control point for the curve. (After the user left-click
somewhere, the control point will show up.) After the user is finished with choosing the points for the
curve, just press f to finish the process and draw out the new curve. (Notice: for Bspline curve, the user
also needs to enter the k value for the curve after finishing choosing the points. While constructing a new
curve try not to press any other keys.)

To delete a curve, just select a curve by left-click one of the control points of the curve (same as before 
for selecting a curve), and press d to delete the curve.

To change the resolution for Bezier curve, press A to enter a new resolution for Bezier curve using console.
To change the resolution for Bspline curve, press S to enter a new resolution for Bspline curve using console.

Lines for implementing algorithms:

de Casteljau: 
	bezier_c.cpp: line 12 - 35

de Boor:
	bspline_c.cpp: line 11 - 37

add points:
	bezier_c.cpp: line 38 - 42
	bspline_c.cpp: line 68 - 72
	main.cpp: 242 - 258

move points:
	bezier_c.cpp: line 74 - 78
	bspline_c.cpp: line 120 - 124
	main.cpp: 373 - 404
delete points:
	bezier_c.cpp: line 45 - 54
	bspline_c.cpp: line 75 - 83
	main.cpp: 289 - 334
