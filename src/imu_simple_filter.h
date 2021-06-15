#include <stdio.h>
#include<math.h>

class IMUFilterSimple {
    public:
        float wx, wy, wz;
        float nx, ny, nz;
        float theta;
        float s, c;
        float n11, n12, n13, n21, n22, n23, n31, n32, n33;
        float o11, o12, o13, o21, o22, o23, o31, o32, o33;
        float r11, r12, r13, r21, r22, r23, r31, r32, r33;
        float dt;

        IMUFilterSimple(void) {
            r11=1;
            r12=0;
            r13=0;
            r21=0;
            r22=1;
            r23=0;
            r31=0;
            r32=0;
            r33=1;
        };

        void update(float wx, float wy, float wz, float dt) {
            theta = sqrt(wx*wx + wy*wy + wz*wz)*dt;

            nx = wx*dt/theta;
            ny = wy*dt/theta;
            nz = wz*dt/theta;
 
            n11 = 0;
            n12 = -nz;
            n13 = ny;
            n21 = nz;
            n22 = 0;
            n23 = -nx;
            n31 = -ny;
            n32 = nx;
            n33 = 0;

            s = sin(theta);
            c = cos(theta);

            o11 = c*ny*ny + c*nz*nz - ny*ny - nz*nz + 1;
            o12 = -s*nz - c*nx*ny + nx*ny;
            o13 = s*ny - c*nx*nz + nx*nz;
            o21 = s*nz - c*nx*nz + nx*nz;
            o22 = c*nx*nx + c*nz*nz - nx*nx - nz*nz + 1; 
            o23 = -s*nx - c*ny*nz + ny*nz;
            o31 = -s*ny - c*nx*nz + nx*nz;
            o32 = s*nx - c*ny*nz + ny*nz;
            o33 = c*nx*nx + c*ny*ny - nx*nx - ny*ny + 1;

            r11 = r11*o11 + r12*o21 + r13*o31;
            r12 = r11*o12 + r12*o22 + r13*o32;
            r13 = r11*o13 + r12*o23 + r13*o33;
            r21 = r21*o11 + r22*o21 + r23*o31;
            r22 = r21*o12 + r22*o22 + r23*o32;
            r23 = r21*o13 + r22*o23 + r23*o33;
            r31 = r31*o11 + r32*o21 + r33*o31;
            r32 = r31*o12 + r32*o22 + r33*o32;
            r33 = r31*o13 + r32*o23 + r33*o33;
        };
};