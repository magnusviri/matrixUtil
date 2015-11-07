# matrixUtil

version 1.0

Unix command line tool that converts between matrixes and &lt;x y z&gt; rotations.

matrixUtil.c by James Reynolds http://www.magnusviri.com

MatrixMath.c and MatrixMath.h were obtained from the [Bricksmith project](http://bricksmith.sourceforge.net) (and have been slightly modified).  These files are probably overkill for what I need, but I'm sick and tired of math.

Usage:

### Convert from matrix to rotation

	matrixUtil <a> <b> <c> <d> <e> <f> <g> <h> <i>

Prints <rx> <ry> <rz>

### Convert from rotation to matrix

	matrixUtil <rx> <ry> <rz>

Prints <a> <b> <c> <d> <e> <f> <g> <h> <i>

### Convert LDraw file

	matrixUtil <ldraw_file>

Prints every line.  LDraw part lines are followed by the rotation x y z>

LDraw part line:

	1 <color> <tx> <ty> <tz> <a> <b> <c> <d> <e> <f> <g> <h> <i> <part>

### Interactive mode

	matrixUtil
	<rx> <ry> <rz>
	<a> <b> <c> <d> <e> <f> <g> <h> <i>
	...
	<EOF>

Prints matrix or rotation until EOF (cntl-d) is reached.

	<a> <b> <c> <d> <e> <f> <g> <h> <i>
	<rx> <ry> <rz>
	...

