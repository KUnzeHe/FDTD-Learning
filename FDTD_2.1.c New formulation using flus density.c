# include <math.h>
# include <stdlib.h>
# include <stdio.h>

# define KE 200

int main()
{
    float dx[KE], ex[KE], hy[KE], ix[KE];
    float ga[KE], gb[KE];
    int n, k, kc, ke, kstart, nsteps;
    float ddx, dt, T, epsz, epsilon, sigma;
    float t0, spread, pi, freq_in, arg, pulse;
    FILE *fp;
    float ex_low_m1, ex_low_m2, ex_high_m2, ex_high_m1;

    pi = 3.14159;    
    kc = KE/2;    /* Center of the problem space */
    ddx = .01;    /* Cell size */
    dt = ddx/6e8;    /* time steps */
    epsz = 8.8e-12;

    ex_low_m2 = ex_low_m1 = ex_high_m1 = ex_high_m2 = 0;

    for ( k=0; k < KE; k++){
        ga[k] = 1.;
        gb[k] = 0.;
        ex[k] = 0.;
        dx[k] = 0.;
        hy[k] = 0.;
        ix[k] = 0.;
    }
        printf( "Dielectric starts at --> ");
        scanf("%d", &kstart );
        printf( "Epsilon --> ");
        scanf("%f", &epsilon);
        printf( "Conductivity --> ");
        scanf("%f", &sigma);
        printf("%d %6.2f %6.2f \n", kstart, epsilon, sigma);
    
    for ( k=kstart; k < KE; k++ ){
        ga[k] = 1./(epsilon + sigma*dt/epsz);
        gb[k] = sigma*dt/epsz;
    }

        for ( k=1; k <= KE; k++)
        { printf( "%2d %4.2f %4.2f\n", k, ga[k], gb[k]);}

    /* These parameters specify the input pulse */

    t0 = 50.0;
    spread = 20.0;
    T = 0;
    nsteps = 1;

    /* Main part of the program */

    while( nsteps > 0){
        printf( "nsteps --> ");
        scanf("%d", &nsteps);
        printf("%d \n", nsteps);

        for( n=1; n <= nsteps; n++)
        {
            T = T + 1;

            /* Calculate the Dx field */
            for( k=1; k < KE; k++)
            {
                dx[k] = dx[k] + 0.5*( hy[k-1] - hy[k] );
            }

            /* Put a Gaussian pulse at the low end */

            freq_in = 3e8;
            pulse = exp(-.5*(pow((t0-T)/spread, 2.0)));
            dx[kc] = dx[kc] + pulse;
            printf( "%5.1f %6.2f %6.2f\n", T, pulse, dx[kc]);

            /* Calculate Ex from Dx */
            for( k=0; k < KE-1; k++ ){
                ex[k] = ga[k]*(dx[k] - ix[k]);
                ix[k] = ix[k] + gb[k]*ex[k];
            }

            /* Boundary conditions */
            ex[0] = ex_low_m2;
            ex_low_m2 = ex_low_m1;
            ex_low_m1 = ex[1];

            ex[KE-1] = ex_high_m2;
            ex_high_m2 = ex_high_m1;
            ex_high_m1 = ex[KE-2];

            /* Calculate the Hy field */
            for ( k=0; k < KE-1; k++ )
            {
                hy[k] = hy[k] + .5*( ex[k] - ex[k+1] );
            }
        }
            for( k=0; k < KE; k++ )
            {
                printf( "%2d %6.2f %6.2f %6.2f \n", k, dx[k], ex[k], hy[k]);
            }

            /* Write the E field out to a file "EX" */
            fp = fopen( "D:/HKZ_Coding/FDTD_Learning/Ouput/Ex(2.1).txt", "w");
            for( k=0; k < KE; k++ )
            {
                fprintf( fp,"%2d %6.3f \n", k+1, ex[k]);
            }
            fclose(fp);
            printf( "%5.1f \n", T);
    }
    
}
