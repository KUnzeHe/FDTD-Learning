# include <math.h>
# include <stdlib.h>
# include <stdio.h>

#define IE 60
#define JE 60

int main()
{
    float ga[IE][JE], dz[IE][JE], ez[IE][JE], hx[IE][JE], hy[IE][JE];
    int l, n, i, j, ic, jc, nsteps;
    float ddx, dt, T, epsz, pi, epsilon, sigma, eaf;
    float t0, spread, pulse;
    FILE *fp;

    ic = IE/2;
    jc = JE/2;
    ddx = .01;    /* Cell size */
    dt = ddx/6e8;    /* Time steps */
    epsz = 8.8e-12;
    pi = 3.14159;

    for ( j=0; j < JE; j++ ){
        printf( "%2d ", j);
        for ( i=0; i < IE; i++ ){
            dz[i][j] = 0.;
            ez[i][j] = 0.;
            hx[i][j] = 0.;
            hy[i][j] = 0.;
            ga[i][j] = 1.0;
            printf( "%5.2f ", ga[i][j]);
        }
        printf( " \n ");
    }

    t0 = 20.0;
    spread = 6.0;
    T = 0;
    nsteps = 1;

    while ( nsteps > 0 ){
        printf( "nsteps --> ");
        scanf("%d", &nsteps);
        printf("%d \n", nsteps);

        for ( n=1; n <= nsteps; n++ ){
            T = T + 1;

            /* Calculate the Dz field */
            for ( j=1; j < JE; j++ ){
                for ( i=1; i < IE; i++ ){
                    dz[i][j] = dz[i][j] + .5*( hy[i][j] - hy[i-1][j] - hx[i][j] + hx[i][j-1]);
                }
            }

            /* Put a Gaussian pulse in the middle */

            pulse = exp(-.5*(pow((t0-T)/spread, 2.0)));
            dz[ic][jc] = dz[ic][jc] + pulse;

            /* Calculate the Ez Field */
            for ( j=1; j < JE; j++){
                for ( i=1; i < IE; i++){
                    ez[i][j] = ga[i][j]*dz[i][j];
                }
            }
            /* Calculate the Hx field */
            for ( j=0; j < JE-1; j++){
                for ( i=0; i < IE-1; i++){
                    hx[i][j] = hx[i][j] + .5*( ez[i][j] - ez[i][j+1] );
                }
            }
            /* Calculate the Hy field */
            for ( j=0; j < IE-1; j++ ){
                for ( i=0; i <= IE-1; i++ ){
                    hy[i][j] = hy[i][j] + .5*( ez[i+1][j] - ez[i][j]);
                }
            }
        }
        /* --- End of the main FDTD loop --- */

        for ( j=1; j < jc; j++ ){
            printf( "%2d", j );
            for ( i=1; i < ic; i++ ){
                printf( "%5.2f", ez[2*i][2*j]);
            }
            printf( "\n" );
        }
        printf( "T = %5.0f \n", T);

        /* Write the E field out to a file 'Ez' */
        fp = fopen("D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_3.1/Ez.txt", "w");
        if (fp == NULL) {
            perror("Error opening file");
            exit(1);
        }
    

        for (j = 0; j < JE; j++) {
            for (i = 0; i < IE; i++) {
                fprintf(fp, "%d %d %6.3f\n", j, i, ez[i][j]);
            }
        }
        fclose(fp);
    }
}
    

