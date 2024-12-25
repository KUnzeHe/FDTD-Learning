# include <math.h>
# include <stdlib.h>
# include <stdio.h>

# define KE 200    /*KE is the number of cells to be used*/ 
# define PI 3.14159265358979323846

int main()
{
    float ex[KE], hy[KE];
    int n, k, kc, ke, kstart, NSTEPS;
    float T;
    float t0, spread, pulse;
    float ex_low_m2, ex_low_m1, ex_high_m2, ex_high_m1;
    float cb[KE], ca[KE], epsz;
    float ddx, dt, epsilon, sigma, eaf, freq_in;
    FILE *fp;

    /*Initialize*/

    for ( k=0; k < KE; k++ )
    {
        ex[k] = 0;
        hy[k] = 0;
    }

    kc = KE/2;    /*Center of the problem space*/
    t0 = 40.0;    /*Center of the incident pulse*/
    spread = 12;    /*Width of the incident pulse*/
    ddx = .01;    /* Set the cell size to 1cm */
    dt = ddx/(2*3e8);    /* Calculate the time step */
    epsz = 8.85419e-12;
    T = 0;
    NSTEPS = 1;

    /* These parameters specify the input pulse */
    printf( "Input freq(MHz) --> " );
    scanf("%f", &freq_in);
    freq_in = freq_in*1e6;
    printf(" %8.0f \n", freq_in);

    for ( k=1; k <= KE; k++ ){
        ca[k] = 1.;
        cb[k] = .5;    /*Initialize to free space*/
    }
        printf( "Dielectric starts at --> ");
        scanf("%d", &kstart);
        printf( "Epsilon --> ");
        scanf("%f", &epsilon);
        printf( "Conductivity --> ");
        scanf("%f", &sigma);
        printf("%d %6.2f %6.2f \n", kstart, epsilon, sigma);

        eaf = dt*sigma/(2*epsz*epsilon);
        printf(" %6.4f \n", eaf );

    for ( k=kstart; k <= KE; k++){
        ca[k] = (1. - eaf)/(1 + eaf);
        cb[k] = .5/(epsilon*(1 + eaf) );
    }
        for ( k=1; k <= KE; k++ )
        { printf( "%2d %4.2f %4.2f \n", k, ca[k], cb[k]);}
    
while ( NSTEPS > 0 ){
    printf( "NSTEPS --> " );    /*NSTEPS is the number of times the main loop has exected*/
    scanf("%d", &NSTEPS);
    printf("%d \n", NSTEPS);
    n = 0;

    for ( n=1; n <=NSTEPS ; n++)
    {
        T = T + 1;    /*T keeps track of the total number of times the main loop is executed*/
    
    /*Main FDTD Loop*/
        
        /*Calculated the Ex field*/
        for ( k=1; k < KE; k++ )
        { ex[k] = ca[k]*ex[k] + cb[k]*( hy[k-1] - hy[k] ); }

        /*Put a Gaussian pulse in the middle */

        pulse = sin(2*PI*freq_in*dt*T);
        ex[5] = ex[5] + pulse;
        printf( "%5.1f %6.2f\n", t0-T, ex[5]);

            /* Absorbing Boundary Conditions*/

            
                ex[0] = ex_low_m2;
                ex_low_m2 = ex_low_m1;
                ex_low_m1 = ex[1];

                ex[KE-1] = ex_high_m2;
                ex_high_m2 = ex_high_m1;
                ex_high_m1 = ex[KE-2];
            

        /*Calculate the Hy field */
        for ( k=0; k < KE-1; k++ )
        { hy[k] = hy[k] + .5*( ex[k] - ex[k+1] ); }

    }
    /* End of the Main FDTD Loop */
        /* At the end of calculation, print out the Ex and Hy fields */
        for ( k=1; k <= KE; k++)
        { printf( "%3d %6.2f %6.2f\n", k, ex[k], hy[k]); }


        /*Write the E field out to ta file 'Ex' */
        fp = fopen( "Ex", "w" );
        for ( k=1; k <= KE; k++ )
        { fprintf( fp," %6.2f \n", hy[k]); }
        fclose(fp);

        printf( "T = %5.0f\n", T );
}

}