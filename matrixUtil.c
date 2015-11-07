/*
gcc -arch i386 -O3 -o matrixUtil MatrixMath.c matrixUtil.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#import "MatrixMath.h"
#define MAXLEN 256

void usage()
{
	printf( "matrixUtil <a> <b> <c> <d> <e> <f> <g> <h> <i>\n" );
	printf( "matrixUtil <rx> <ry> <rz>\n" );
	printf( "matrixUtil <ldraw_file>\n" );
	printf( "matrixUtil\n" );
	printf( "<rx> <ry> <rz>\n" );
	printf( "<a> <b> <c> <d> <e> <f> <g> <h> <i>\n" );
	printf( "\n" );
	exit(1);
}

int doubleEquals(double i, double j) {
    return (fabs(i - j) < 0.000001);
}

void printTruncatedDouble(double dd, int max_len) {
	char str[50];
	int match = 0;
	for ( int ii = 0; ii < max_len; ii++ ) {
		if (doubleEquals(dd * pow(10,ii), floor(dd * pow(10,ii)))) {
			sprintf (str,"%f", round(dd*pow(10,ii))/pow(10,ii));
			match = 1;
			break;
		}
	}
	if ( match != 1 )
		sprintf (str,"%f", round(dd*pow(10,max_len))/pow(10,max_len));
	char *pp;
	int count;
	pp = strchr (str,'.');
	if (pp != NULL) {
		count = max_len;
		while (count >= 0) {
			 count--;
			 if (*pp == '\0')
				 break;
			 pp++;
		}
		*pp-- = '\0';
		while (*pp == '0')
			*pp-- = '\0';
		if (*pp == '.')
			*pp = '\0';
	}
	printf ("%s", str);
}

void printFloatArray( float *ff, int count, int length ) {
    for ( int ii = 0; ii < count-1; ii++ ) {
    	printTruncatedDouble( ff[ii], length );
    	printf(" ");
    }
	printTruncatedDouble( ff[count-1], length );
}

void print_matrix_from_rotation( float array[] ) {
	TransformComponents components = {
							{1, 1, 1},	//scale;
							0,			//shear_XY;
							0,			//shear_XZ;
							0,			//shear_YZ;
							{radians(array[0]), radians(array[1]), radians(array[2])}, //rotate;		//in radians
							{0, 0, 0},	//translate;
							{0, 0, 0, 0} //perspective;
						};
	Matrix4 transformation = Matrix4CreateTransformation(&components);
	float matrix[9] = {
		transformation.element[0][0], // a transformationX.x
		transformation.element[1][0], // b transformationX.y
		transformation.element[2][0], // c transformationX.z
		transformation.element[0][1], // d transformationY.x
		transformation.element[1][1], // e transformationY.y
		transformation.element[2][1], // f transformationY.z
		transformation.element[0][2], // g transformationZ.x
		transformation.element[1][2], // h transformationZ.y
		transformation.element[2][2] // i transformationZ.z
	};
	printFloatArray( matrix, 9, 5 );
	printf ("\n");
}

void print_rotation_from_matrix( float matrix[], int printNewline ) {
	Matrix4 transformation = IdentityMatrix4;
	TransformComponents components = IdentityComponents;
	// Position
	transformation.element[3][0] = 0;
	transformation.element[3][1] = 0;
	transformation.element[3][2] = 0;
	// Transformation X1
	transformation.element[0][0] = matrix[0];
	transformation.element[1][0] = matrix[1];
	transformation.element[2][0] = matrix[2];
	// Transformation Y1
	transformation.element[0][1] = matrix[3];
	transformation.element[1][1] = matrix[4];
	transformation.element[2][1] = matrix[5];
	// Transformation Z1
	transformation.element[0][2] = matrix[6];
	transformation.element[1][2] = matrix[7];
	transformation.element[2][2] = matrix[8];
	// Corner
	transformation.element[3][3] = 1;
	Matrix4DecomposeTransformation( transformation, &components );
	float rotation[3] = {
		degrees(components.rotate.x), degrees(components.rotate.y), degrees(components.rotate.z)
	};
	printFloatArray( rotation, 3, 2 );
	if (printNewline==1)
		printf("\n");
}

int main(int argc, char **argv) {
	int cc;
	while ((cc = getopt(argc, argv, "h")) != -1) {
		switch (cc) {
			case 'h':
			case '?':
				usage();
				break;
			}
	}

	if ( argc - optind == 3 ) {
		float rotation[3] = {
			atof(argv[optind]), atof(argv[1+optind]), atof(argv[2+optind]),   // rx ry rz
		};
		print_matrix_from_rotation( rotation );
	} else if ( argc - optind == 9 ) {
		float matrix[9] = {
			atof(argv[optind]),   atof(argv[1+optind]), atof(argv[2+optind]), // a b c
			atof(argv[3+optind]), atof(argv[4+optind]), atof(argv[5+optind]), // d e f
			atof(argv[6+optind]), atof(argv[7+optind]), atof(argv[8+optind])  // g h i
		};
		print_rotation_from_matrix( matrix, 1 );
	} else if ( argc - optind == 15 ) {
		float matrix[9] = {
			atof(argv[5+optind]), atof(argv[6+optind]), atof(argv[7+optind]),   // a b c
			atof(argv[8+optind]), atof(argv[9+optind]), atof(argv[10+optind]),  // d e f
			atof(argv[11+optind]), atof(argv[12+optind]), atof(argv[13+optind]) // g h i
		};
		print_rotation_from_matrix( matrix, 1 );
	} else if ( argc - optind == 0 || argc - optind == 1 ) {
		FILE *ff = NULL;
		char line[ 2 * MAXLEN ];
		if ( argc - optind == 0 ) {
			ff = stdin;
		} else {
			if ( ( ff = fopen( argv[ optind ], "r" )) == NULL ) {
				printf( "Couldn't open file" );
				exit( 2 );
			}
		}
		while ( fgets( line, MAXLEN, ff ) != NULL ) {
			char *templine, *linePtr, line2[ 2 * MAXLEN ];
			int jj = 0;
			float input[16];
			strcpy(line2,line);
			linePtr = line2;
			linePtr = strsep(&linePtr, "\n");
			while ((templine = strsep(&linePtr, " ")) != NULL && jj < 16)
				input[jj++] = atof(templine);
			if ( argc - optind == 0 ) {
				if ( jj == 3 ) {
					print_matrix_from_rotation( input );
				} else if ( jj == 9 ) {
					print_rotation_from_matrix( input, 1 );
				}
			} else {
				int len;
				len = strlen(line);
				if( line[len-1] == '\n' )
					line[len-1] = 0;
				if( line[len-2] == '\r' )
					line[len-2] = 0;
				if ( jj == 15 ) {
					float matrix[9] = {
						input[5], input[6], input[7],   // a b c
						input[8], input[9], input[10],  // d e f
						input[11], input[12], input[13] // g h i
					};
					printf("%s rotation: ", line);
					print_rotation_from_matrix( matrix, 1 );
				} else {
					printf("%s\n",line);
				}
			}
		}
		fclose( ff );
	} else {
		usage();
	}
	exit(0);
}
