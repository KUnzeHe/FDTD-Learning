/* FD1D_2.4.c. Plasma.*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define KE 2000

int main ()
{
    float dx[KE], ex[KE], hy[KE], ix[KE], sx[KE], sxm1[KE], sxm2[KE];
    float ga[KE], gb[KE], gc[KE];
    int n, m, k, kc, ke, kstart, nsteps, kend;
    float ddx, dt, T, epsz, epsilon, sigma, omega, vc, freq_p, freq_in;
    float t0, spread, pi, pulse;
    FILE *fp;
    float ex_low_m1, ex_low_m2, ex_high_m1, ex_high_m2;
    float tau, chil, del_exp;
    float real_pt[5][KE], imag_pt[5][KE];
    float freq[5], arg[5], ampn[5][KE], phasen[5][KE];
    float real_in[5], imag_in[5], amp_in[5], phase_in[5];
    float mag[KE];

    kc = KE/2;                /* Center of the space */
    pi = 3.14159;
    epsz = 8.8e-12;
    ddx = 1.e-9;                /* Cells size */
    dt = ddx/6e8;             /* Time steps */
    printf(" %6.4f %10.5e \n", ddx, dt);

    for (k=0; k < KE-1; k++) {  /* Initialize to free space */
        ga[k] = 1.;
        gb[k] = 0.;
        gc[k] = 0.;
        dx[k] = 0.;
        ex[k] = 0.;
        hy[k] = 0.;
        ix[k] = 0.;
        /*mag[k] = 0.;*/
        sx[k] = 0.;
        sxm1[k] = 0.;
        sxm2[k] = 0.;

        for (m=0; m <= 2; m++) {
            real_pt[m][k] = 0.; /* Real and imaginary parts */
            imag_pt[m][k] = 0.; /* of the Fourier Transform */
            ampn[m][k] = 0.;    /* Amplitude and phase of the */
            phasen[m][k] = 0.;  /* Fourier Transforms */
        }
    }

    for (m=0; m <= 2; m++) {
        real_in[m] = 0.;        /* Fourier Trans. of input pulse */
        imag_in[m] = 0.;
    }

    ex_low_m1 = 0.;
    ex_low_m2 = 0.;

    ex_high_m1 = 0.;
    ex_high_m2 = 0.;

    /* Parameters for the Fourier Transform */
    freq[0] = 50.e6;
    freq[1] = 200.e6;
    freq[2] = 500.e6;

    for (n=0; n<=2; n++) {
        arg[n] = 2*pi*freq[n]*dt;
        printf(" %2d %6.2f %7.5f \n", n, freq[n]*1e-6, arg[n]);
    }

    printf("Plasma starts at --> ");
    scanf("%d", &kstart);
    printf("Plasma ends at --> ");
    scanf("%d", &kend);
    
    /* printf("Epsilon --> ");
    scanf("%f", &epsilon);
    
    printf("Conductivity --> ");
    scanf("%f", &sigma);*/

    printf("Plasma frequency(THz) --> ");
    scanf("%f", &freq_p);
        freq_p = freq_p*1e12;
        omega = freq_p*2*pi;

    printf("Electron collision freq(THz) --> ");
    scanf("%f", &vc);
    vc = vc*1e12;

    printf( "Input freq(THz) --> " );
    scanf("%f", &freq_in);
    freq_in = freq_in*1e12;

    /*printf(" chil --> ");
    scanf("%f", &chil);
    tau = 1000.;
    if (chil > 0.0001){
        printf( "tau ( in microseconds) --> ");
        scanf("%f", &tau );
            del_exp = exp(-dt/tau);
    }*/

    printf("%d %d %6.2f %6.2f %6.2f\n", kstart, kend, freq_p, omega, vc);

    /*tau = 1.e-6*tau;
    { printf( "del_exp = %8.5f \n", del_exp); }*/

    /*for (k=kstart; k <= KE; k++) {
        ga[k] = 1./(epsilon + sigma*dt/epsz + chil*dt/tau);
        gb[k] = sigma*dt/epsz;
        gc[k] = chil*dt/tau;
    }

    
    for (k=1; k <= KE; k++) {
        printf(" %2d %6.2f %6.4f %6.4f \n", k, ga[k], gb[k], gc[k]);
    }
    */


    /* These parameters specify the input pulse */
    t0 = 50.0;
    spread = 10.0;

    T = 0;
    nsteps = 1;

    /* Main part of the program */
    while (nsteps > 0) {
        printf("nsteps --> ");
        scanf("%d", &nsteps);
        printf("%d \n", nsteps);

        for (n=1; n<=nsteps; n++) {
            T = T + 1;

            /* Calculate the Dx field */
            for (k=0; k < KE; k++) {
                dx[k] = dx[k] + 0.5*(hy[k-1] - hy[k]);
                
            }

            /* Initialize with a pulse */
            /*pulse = exp(-.5*(pow((t0-T)/spread,2.0)));*/
            pulse = sin(2*pi*freq_in*dt*T);
            dx[5] = dx[5] + pulse;
            printf(" %5.1f %6.2f %6.2f \n", T, pulse, dx[5]);

            /* Calculate Ex from Dx */
            for (k=0; k < KE-1; k++){

                if (kstart <= k && k <= kend ){
                    ex[k] = dx[k] - sx[k];
                    sx[k] = (1 + exp(-vc*dt))*sxm1[k] - exp(-vc*dt)*sxm2[k] + (pow(omega, 2.)*dt/vc)*(1 - exp(-vc*dt))*ex[k];
                    sxm2[k] = sxm1[k];
                    sxm1[k] = sx[k];
                    }

                else{
                    /*printf("%d Dx = %f\n", k, dx[k]);*/
                    ex[k] = dx[k];
                    /*printf("Ex = %f\n", ex[k]);*/
                }
            }

            /* Calculate the Fourier transform of Ex */
            for (k=0; k < KE; k++) {
                for (m=0; m <= 2; m++) {
                    real_pt[m][k] = real_pt[m][k] + cos(arg[m]*T)*ex[k];
                    imag_pt[m][k] = imag_pt[m][k] - sin(arg[m]*T)*ex[k];
                }
            }

            /* Fourier Transform of the input pulse */
            if (T < 100) {
                for (m=0; m <= 2; m++) {
                    real_in[m] = real_in[m] + cos(arg[m]*T)*ex[10];
                    imag_in[m] = imag_in[m] - sin(arg[m]*T)*ex[10];
                }
            }

            /* Boundary conditions */
            ex[0] = ex_low_m2;
            ex_low_m2 = ex_low_m1;
            ex_low_m1 = ex[1];

            ex[KE-1] = ex_high_m2;
            ex_high_m2 = ex_high_m1;
            ex_high_m1 = ex[KE-2];

            /* Calculate the Hy field */
            for (k=0; k < KE-1; k++) {
                hy[k] = hy[k] + .5*(ex[k] - ex[k+1]);
            }
        }

        /* End of the main loop */
            /* for ( k = 0; k < KE; k++ )
            {
                printf( %2d %6.2f %6.2 \n", k, dx[k], ex[k]");
            }*/

        /* Write the E field out to a file "Ex" */
        fp = fopen("D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.4/Ex","w");
        for (k=0; k < KE; k++) {
            fprintf(fp, " %2d %6.3f \n", k, ex[k]);
        }
        fclose(fp);

        /* Calculate the amplitude and phase of each frequency */

        /* Amplitude and phase of the input pulse */
        /*for (m=0; m <= 2; m++) {
            amp_in[m] = sqrt( pow(imag_in[m],2.) + pow(real_in[m],2.) );
            phase_in[m] = atan2( imag_in[m], real_in[m] );
            printf(" %3d Input Pulse : %8.4f %8.4f %8.4f %7.2f\n", m, real_in[m], imag_in[m], amp_in[m], (180.0/pi)*phase_in[m]);*/
        

            for (k=1; k < KE; k++) {
                ampn[m][k] = (1./amp_in[m])*sqrt( pow(real_pt[m][k],2.) + pow(imag_pt[m][k],2.) );
                phasen[m][k] = atan2( imag_pt[m][k], real_pt[m][k]) - phase_in[m];
                }
        

            /* Write the amplitude field out to a files "Amp" */
            fp = fopen("D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.4/Amp0","w");
            for (k=0; k < KE; k++) {
                fprintf(fp, " %2d %8.5f \n", k, ampn[0][k]);
            }
            fclose(fp);

            fp = fopen("D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.4/Amp1","w");
            for (k=0; k < KE; k++) {
                fprintf(fp, " %2d %8.5f \n", k, ampn[1][k]);
            }
            fclose(fp);

            fp = fopen("D:/HKZ_Coding/FDTD_Learning/Ouput/FDTD_2.4/Amp2","w");
            for (k=0; k < KE; k++) {
                fprintf(fp, " %2d %8.5f \n", k, ampn[2][k]);
            }
            fclose(fp);

            printf("%5.1f \n", T);
    }
}

