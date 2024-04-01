#include <stdio.h>

#define levmax 3
#define amax 2047

typedef enum
{
    LIT,
    OPR,
    LOD,
    STO,
    CAL,
    INT,
    JMP,
    JPC
}fct;

typedef struct tinstruction
{
    fct f;
    int l; // l: 0..levmax; // Level buffer
    int a; // a: 0..amax;   // Address buffer
} instruction;

instruction code[2048];

// procedure interpret;
//  const stacksize = 500;

#define stacksize 5012

// p, b, t: integer; {program-, base-, topstack-registers}
int p, // program-register
    b, // base-register
    t; // topstack-register

instruction i;   //: instruction; {instruction register}
int s[stacksize]; //: array [1..stacksize] of integer; {datastore}

int base(int l)
{           // l: integer)
    int b1; //

    b1 = b; // {find base l levels down}
    while (l > 0)
    {
        b1 = s[b1];
        l = l - 1;
    }
    return b1;
} // end-int-base

void include(fct f, int l, int a,int n){
    code[n].f = f;
    code[n].l = l;
    code[n].a = a;
}

// Determina se seu argumento é impar
int odd(int x) { return (((x % 2) == 1)); }

void pcodevhw()
{ // begin
    printf("\n start pl/0");
    printf("\n p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
    printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
    t = -1; // topstack-register
    b = 0;  // base-register
    p = 0;  // program-register

    // OBS: s[0] must not be used
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;

    // function value
    //  9 function value
    //  8 parameter
    //  7
    //  6
    //  5
    // main
    //  4 tmp
    //  3 n = 7
    //  2
    //  1
    //  0
    include(INT,0,5,0);
    include(LIT,0,4,1);
    include(LIT,0,1,2);
    include(LIT,0,1,3);
    include(STO,0,10,4);
    include(STO,0,11,5);
    include(STO,0,12,6);
            //inicio do loop
    include(LOD,0,12,7);
    include(LOD,0,11,8);
    include(LOD,0,10,9);
    include(OPR,0,4,10);
    include(STO,0,11,11);
    include(LOD,0,10,12);
    include(OPR,0,9,13);
    include(LOD,0,10,14);
    include(LIT,0,1,15);
    include(OPR,0,2,16);
    include(STO,0,10,17);
    include(JPC,0,7,18);
    include(LOD,0,11,19);
    include(OPR,0,0,21);

    do
    {
        i = code[p];
        // printf("\n %3d %3d %3d - %3d %3d %3d", p, b, t, i.f, i.l, i.a);
        printf("\n %3d %3d %3d   ", p, b, t);
        p = p + 1;
        // switch case i.f-BEGIN
        switch (i.f)
        { // with i do case f of
        case LIT:
        {
            t = t + 1;
            s[t] = i.a;
            printf("LIT %3d %3d", i.l, i.a);
        }
        break;
        case OPR:
            printf("OPR %3d %3d", i.l, i.a);
            //             switch case i.a-BEGIN
            switch (i.a)
            { // case a of {operator}
            case 0:
            { // return
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
            }
            break;

            case 1:
            { //
                s[t] = -s[t];
            }
            break;

            case 2:
            { //  ADD     : PUSH( POP()+POP() )
                t = t - 1;
                s[t] = s[t] + s[t + 1];
            }
            break;

            case 3:
            { // SUBSTRACT: PUSH( POP()-POP() )
                t = t - 1;
                s[t] = s[t] - s[t + 1];
            }
            break;

            case 4:
            { // MULTIPLY: PUSH( POP()*POP() )
                t = t - 1;
                s[t] = s[t] * s[t + 1];
            }
            break;

            case 5:
            { // DIVIDE  : PUSH( POP()/POP() )
                t = t - 1;
                s[t] = s[t] / s[t + 1];
            }
            break;

            case 6:
            {                     //
                s[t] = odd(s[t]); // s[t] = ord(odd(s[t]));
            }
            break;

            case 8:
            { // EQUAL
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
            }
            break;

            case 9:
            { // NOT.EQUAL
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
            }
            break;

            case 10:
            { // LESS THAN
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
            }
            break;

            case 11:
            { // GREATER EQUAL THAN
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
            }
            break;

            case 12:
            { // GREATER THAN
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
            }
            break;

            case 13:
            { // LESS EQUAL THAN
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
            }
            break;
            } // end;
              //         switch case i.a-END
            break;

        case LOD:
        { // LOAD
            printf("LOD %3d %3d", i.l, i.a);
            t = t + 1;
            s[t] = s[base(i.l) + i.a];
        }
        break;

        case STO:
        { // STORE
            printf("STO %3d %3d", i.l, i.a);
            s[base(i.l) + i.a] = s[t];
            t = t - 1;
        }
        break;

        case CAL:
        { // {generate new block mark}
            printf("CAL %3d %3d", i.l, i.a);
            s[t + 1] = base(i.l);
            s[t + 2] = b;
            s[t + 3] = p;
            b = t + 1;
            p = i.a;
        }
        break;

        case INT:
            t = t + i.a;
            printf("INT %3d %3d", i.l, i.a);
            break;
        case JMP:
            p = i.a;
            printf("JMP %3d %3d", i.l, i.a);
            break;
        case JPC:
            if (s[t] != 0)
            {
                p = i.a;
            }
            t = t - 1;
            printf("JPC %3d %3d", i.l, i.a);
            break;
        } // end {with, case}
        // switch case i.f-END

        // print stack
        printf("      s[] : ");
        for (int h = 0; h <= t; h++)
        {
            printf(" %3d", s[h]);
        }
    } while (p != 0);

    printf("\n === === ===   === === ===             === === === === === === === === === === === === === === ===");
    printf("\n p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14");
    printf("\n end pl/0");
} // end-void-pcmachine() {interpret};

int main()
{

    // Aqui vc preenche as instrucoes no vetor code
    code[0].f = OPR;
    code[0].l = 0;
    code[0].a = 0;

    // Aqui vc chama a P-code machine para interpretar essas instrucoes
    pcodevhw();
    return 0;
}
/*

 start pl/0
 p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
 === === ===   === === ===             === === === === === === === === === === === === === === ===
   0   0  -1   INT   0   5      s[] :    0   0   0   0   0
   1   0   4   LIT   0   4      s[] :    0   0   0   0   0   4
   2   0   5   STO   0   3      s[] :    0   0   0   4   0
   3   0   4   LOD   0   3      s[] :    0   0   0   4   0   4
   4   0   5   STO   0   8      s[] :    0   0   0   4   0
   5   0   4   CAL   0   9      s[] :    0   0   0   4   0
   9   5   4   INT   0   5      s[] :    0   0   0   4   0   0   0   6   4   0
  10   5   9   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   4
  11   5  10   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   4   1
  12   5  11   OPR   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   0
  13   5  10   JPC   0  24      s[] :    0   0   0   4   0   0   0   6   4   0
  14   5   9   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   4
  15   5  10   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   4   1
  16   5  11   OPR   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   3
  17   5  10   STO   0   8      s[] :    0   0   0   4   0   0   0   6   4   0
  18   5   9   CAL   0   9      s[] :    0   0   0   4   0   0   0   6   4   0
   9  10   9   INT   0   5      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0
  10  10  14   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   3
  11  10  15   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   3   1
  12  10  16   OPR   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0
  13  10  15   JPC   0  24      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0
  14  10  14   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   3
  15  10  15   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   3   1
  16  10  16   OPR   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   2
  17  10  15   STO   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0
  18  10  14   CAL   0   9      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0
   9  15  14   INT   0   5      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0
  10  15  19   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   2
  11  15  20   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   2   1
  12  15  21   OPR   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0
  13  15  20   JPC   0  24      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0
  14  15  19   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   2
  15  15  20   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   2   1
  16  15  21   OPR   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   1
  17  15  20   STO   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0
  18  15  19   CAL   0   9      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0
   9  20  19   INT   0   5      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0
  10  20  24   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0   1
  11  20  25   LIT   0   1      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0   1   1
  12  20  26   OPR   0   8      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0   1
  13  20  25   JPC   0  24      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0
  24  20  24   LOD   0   3      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   0   1
  25  20  25   STO   0   4      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0  15  15  19   1   1
  26  20  24   OPR   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0
  19  15  19   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0
  20  15  20   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0   0
  21  15  21   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0   0   0
  22  15  22   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0   0   0   0
  23  15  23   JMP   0  26      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0  10  10  19   2   0   0   0   0   0
  26  15  23   OPR   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0
  19  10  14   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0
  20  10  15   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0   0
  21  10  16   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0   0   0
  22  10  17   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0   0   0   0
  23  10  18   JMP   0  26      s[] :    0   0   0   4   0   0   0   6   4   0   5   5  19   3   0   0   0   0   0
  26  10  18   OPR   0   0      s[] :    0   0   0   4   0   0   0   6   4   0
  19   5   9   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   0
  20   5  10   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   0   0
  21   5  11   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   0   0   0
  22   5  12   LIT   0   0      s[] :    0   0   0   4   0   0   0   6   4   0   0   0   0   0
  23   5  13   JMP   0  26      s[] :    0   0   0   4   0   0   0   6   4   0   0   0   0   0
  26   5  13   OPR   0   0      s[] :    0   0   0   4   0
   6   0   4   LOD   0   9      s[] :    0   0   0   4   0   0
   7   0   5   STO   0   4      s[] :    0   0   0   4   0
   8   0   4   OPR   0   0      s[] :
 === === ===   === === ===             === === === === === === === === === === === === === === ===
 p   b   t     f   l   a                 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
 end pl/0
Normal program termination. Exit status: 0
*/