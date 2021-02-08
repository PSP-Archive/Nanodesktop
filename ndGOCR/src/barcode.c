/*
This is a Optical-Character-Recognition program
Copyright (C) 2000-2007  Joerg Schulenburg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 see README for email address
 
 ToDo:
  - transform special xml bar code symbols (<>&) to xml symbols (&lt;&gt;&amp;)
 
*/
#include <nanodesktop.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pgm2asc.h"
#include "gocr.h"
#include "pnm.h"

#undef g_debug
#define g_debug(a)

/*
     detect barcode and add a XML-string to the box (obj-pointer, ToDo)
     ToDo: barcode-output stderr->stdout
*/

double sqr(double x) { return(x*x); }

/* ----------------------------- code128 ---------------------------- *
 *    "BSBSBS", B=Bar, S=Space, better using 2*6=12bit-integer?       */
#define Num128 107
const char *code128[Num128+1]={ /* can be generated by an algorithm? */
/* 00 */"212222","222122","222221","121223","121322","131222","122213","122312",
/* 08 */"132212","221213","221312","231212","112232","122132","122231","113222",
/* 16 */"123122","123221","223211","221132","221231","213212","223112","312131",
/* 24 */"311222","321122","321221","312212","322112","322211","212123","212321",
/* 32 */"232121","111323","131123","131321","112313","132113","132311","211313",
/* 40 */"231113","231311","112133","112331","132131","113123","113321","133121",
/* 48 */"313121","211331","231131","213113","213311","213131","311123","311321",
/* 56 */"331121","312113","312311","332111","314111","221411","431111","111224",
/* 64 */"111422","121124","121421","141122","141221","112214","112412","122114",
/* 72 */"122411","142112","142211","241211","221114","413111","241112","134111",
/* 80 */"111242","121142","121241","114212","124112","124211","411212","421112",
/* 88 */"421211","212141","214121","412121","111143","111341","131141","114113",
/* 96 */"114311","411113","411311","113141","114131","311141","411131","211412",
/*104 */"211214","211232","2331112","???"};

/*
      code128: see code128.tex by Petr Olsak (108 codes)
       quiet_zone: size=10 (before and after code128)
       num_bars=3*(start+chars[N]+crc+stop)+1
       B=bar S=space char=BSBSBS (size=11), stop=BSBSBSB (size=11+2)
       Width: Bar,Space=1,[2,3,4] char=11 code=11*(N+3)+2 sumB=even,sumS=odd
       startA="211412"=103 startB="211214"=104 startC="211232"=105(2dec_digits)
       mode/code 0..95        96   97   98   99  100 101 102 103 104 105 106
         1=A   x20-x5f,0-x20  F3   F2   uB   mC  mB  F4  F1  ^A  ^B  ^C   $
         2=B   x20-x7f        F3   F2   uA   mC  F4  mA  F1  ^A  ^B  ^C   $
         3=C   "00"-"95"     "96" "97" "98" "99" mB  mA  F1  ^A  ^B  ^C   $
        uA,uB: switch mode for next char, mA,mB: switch mode permanently
       crc=(start+1*char1+2*char2+3*char3+...+N*charN) mod 103
       $=stop="2331112"=106 (4bars, with=13)  => start/end==211
       return num of chars or string
       
       size  B+W    even-variants      odd-variants      num_codes 
        11 = 8+3 = (1+3+4,2+2+4,2+3+3)+(1+1+1)       => (6+3+3)*(1)   =  12
           = 6+5 = (1+1+4,1+2+3,2+2+2)+(1+1+3,1+2+2) => (3+6+1)*(3+3) =  60
           = 4+7 = (1+1+2)+(1+2+4,1+3+3,2+2+3)       => (3)*(6+3+3)   =  36
                                                                  sum = 108
 */
  /* example: barcode -E -e 128c -b 1434600120000884 >a.eps */
  /* example: barcode -E -e 128b -b 14Test41         >a.eps */
  /* example: barcode -E -e 128raw -b 105 17 14 30   >a.eps */

char *decode_code128(int *wb, int num_bars){
  int i, w, i1, i2, i3=0, i4, i5=0, crc, mode=1;
  double dww, dw, err, min_err;  char cc, *buf;
  char *result=NULL; /* malloc and store the result */
  
  for(w=i=0;i<2*num_bars-1;i++) w+=wb[i]; /* summ all bars and spaces */
  
  /* test code128 characteristics, ToDo: look for correct start/stop 211 seq. */
  if ((num_bars-1)%3!=0 || num_bars<10 || w<11*(num_bars-1)/3+2)
    return 0;
  g_debug(fprintf(stderr,"\n code128 b%d s%d b%d",wb[0],wb[1],wb[2]);)
  if (3*wb[0]<4* wb[1]
   || 3*wb[0]<4* wb[2]
   || 4*wb[0]<3*(wb[1]+wb[2])
   || 3*wb[0]>4*(wb[1]+wb[2])) return 0; /* 211 */
  dw=3.0*w/((num_bars-1)*11+6);
  /* get enough memory for all digits in longest mode C */
  buf   =(char *) malloc(    (num_bars-7)/3*2+1); if (!buf)    return result;
  result=(char *) malloc(256+(num_bars-7)/3*2+1);

  dww=crc=0;
  for(i4=i1=0;i1<(num_bars-1)/3;i1++) {
    for(min_err=1e8,i3=Num128,i5=0;i5<Num128;i5++){ /* get best fit */
      for(err=i2=0;i2<6;i2++) err+=sqr(code128[i5][i2]-'0'-wb[i1*6+i2]/dw);
      if (err<min_err) { min_err=err; i3=i5; }
    } dww+=min_err;
    g_debug(fprintf(stderr,"\n %7s %3d err=%.3f ",code128[i3],i3,min_err);)
    if(i3<Num128){  /* valid symbol */
      if(i1==0){ if (i3>102 && i3<106) mode=i3-103+1; crc=i3; } /* start */
      if(i1>0 && i1<(num_bars-1)/3-2){
        crc+=i3*(i1); cc=0;  /* first * 1 + second * 2 + third * 3 ... */
        i5=((mode>3)?mode>>2:mode&3); mode&=3;  /* mode can be modified now */
        switch (i5) {                           /* mode=1..3=modeA..modeC */
          case 1: if (i3>=64 && i3<96) cc=i3-64;   /* modeA: x20-x5f,0-x20 */
                                  else cc=i3+32; 
                  if (i3==101) mode=1;     /* switch to mode A */
                  if (i3== 99) mode=3;     /* switch to mode C */
                  if (i3== 98) mode|=2<<2; /* shift  to mode B */
                  break;
          case 2: cc=i3+32;                        /* modeB: x20-x7f */
                  if (i3==100) mode=2;     /* switch to mode B */
                  if (i3== 99) mode=3;     /* switch to mode C */
                  if (i3== 98) mode|=1<<2; /* shift  to mode A */
                  break;
          case 3:
                  if (i3==101) mode=1;     /* switch to mode A */
                  if (i3==100) mode=2;     /* switch to mode B */
        } 
        if (i5==3)  { buf[i4]='0'+i3/10; i4++;
                      buf[i4]='0'+i3%10; i4++; }  /* modeC:   two digits */
        else {
          if (cc>=0x20 && i3<=0x7f) { buf[i4]=cc;        i4++; }  /* modeA+B: one digit */
          if (cc>=0    && cc< 0x20) { buf[i4]='^';       i4++;
                                      buf[i4]=cc+'@';    i4++; } 
        }
      }
      if(i1==(num_bars-1)/3-2){ crc=(crc+103-i3)%103; }
      if(i1==(num_bars-1)/3-1){ if(i3!=106) i3=-1; } /* stop code */
      mode &= 3;  /* remove shift */
    }
    else fprintf(stderr," %s=%02d? ",buf,i5);
  }
  buf[i4]=0; /* end of string */
  if (result)
   sprintf(result,"<barcode type=\"128\" chars=\"%d\" code=\"%s\" "
                  "crc=\"%d\" error=\"%.3f\" />",
          i4,buf,crc,dww/((num_bars-1)));
  free(buf);
  return result;
}

/* -------------------------------------------------------------------- UPC
      EAN 13 (UPC,(1+6+1+6+1)*2bars,size=3+6*7+5+6*7+3=95) 
      EAN  8 (UPC,(1+4+1+4+1)*2bars,size=3+4*7+5+4*7+3=67)
      UPC: (10 codes)
       BSB SBSB^n SBSBS BSBS^n BSB
       bsb ...... sbsbs ...... bsb
       111 ...... 11111 ...... 111
       num_bars=2*(2*6+3) middle=SBSBS=11111 right/left=BSB=111="101"
       char: left=SBSB right=BSBS (size=7) only_dec_digits
           SS+BB = (S+S) + (B+B)     => BB:SS = 5:2 or 3:4
      size ev+odd  even  + odd       => variants     
       7 = 2 + 5 = (1+1) + (1+4,2+3) => (1)*(2+2) = 4 codes
         = 4 + 3 = (1+3,2+2) + (1+3) => (2+1)*(2) = 6 codes += 10 codes
       ToDo: make it more robust
             - return error as mean deviation
 * -------------------------------------------------------------------- */
  /* example: barcode -E -e upc -b 12345678901   >a.eps # ok */
  /* example: barcode -E -e ean -b 123456789012  >a.eps # ok */
#define NumUPC 20
const char *codeUPC[NumUPC+1]={ /* 0..9, first n = SBSB, last n = BSBS */
  "3211","2221","2122","1411","1132", /* 0,1,2,3,4 normal   (+0bit) */
  "1231","1114","1312","1213","3112", /* 5,6,7,8,9 */
  "1123","1222","2212","1141","2311", /* 0,1,2,3,4 mirrored (+1bit) */
  "1321","4111","2131","3121","2113", /* 5,6,7,8,9 */
  "????"}; /* not found */

char *decode_UPC(int *wb, int num_bars){ /* ToDo: char *dest, int len */
  int i, w, i1, i2, i3, i4, i5, crc, mirrored, ean;
  double err, min_err, dw, dww=0.0;  char digit;
  char *result=NULL, *buf=NULL; /* malloc and store the result */
  for(w=i=0;i<2*num_bars-1;i++) w+=wb[i];

  dw=2.0*w/((num_bars-6)*7+2*11); /* or min(wb[]) */
  crc=0;
  if ((num_bars)%2!=0 || num_bars<10 || w<7*(num_bars-6)/2+11
   || ((num_bars-6)/2)%2!=0) return 0;  /* should be balanced */
  /* check front BSB, middle SBSBS and end BSB */
  dww=0;
  for (i=0;i<3;i++) { dww=sqr(wb[i             ]/dw-1); if (dww>0.4) return 0; }
  for (i=0;i<5;i++) { dww=sqr(wb[i+  num_bars-3]/dw-1); if (dww>0.4) return 0; }
  for (i=0;i<3;i++) { dww=sqr(wb[i+2*num_bars-4]/dw-1); if (dww>0.4) return 0; }
  buf   =(char *)malloc(    (num_bars-6)/2+1); if (!buf)    return result;
  result=(char *)malloc(256+(num_bars-6)/2+1);

  for(ean=i5=0,i1=3;i1<2*num_bars-4;i1+=4) { /* each digit (2bars+2spaces) */
    if (i1==num_bars-3) { i1++; continue; } /* skip middle sync SBSBS */
    for (i4=NumUPC,mirrored=0,digit='?',min_err=16e8,i2=0;i2<NumUPC;i2++) {
      for (err=0,i3=0;i3<4;i3++) err+=sqr(codeUPC[i2][i3]-'0'-wb[i1+i3]/dw);
      if (err<min_err) { min_err=err; i4=i2; digit='0'+i2%10; mirrored=i2/10; }
    } dww+=min_err; crc+=(digit-'0')*((i5&1)?1:3); /* even*3+odd, last char is even */
    buf[i5++]=digit; if (i5<7) ean=(ean<<1)|mirrored; 
    /* ToDo: error as deviation wb from ideal */
    g_debug(fprintf(stderr,"\n UPC digit=%c mirrored=%d err=%.3f err_m=%.3f ",
                               digit,mirrored,min_err/4,dww/(i5*4));)
  }
  /* EAN has a 13th leading digit build by 3 of 6 mirorred digits */
  if (ean & 0x20) ean^=0x3f; /* UPC-E mirrored 1xxxxx => 0yyyyy */
  switch (ean) { 
    case 11: ean=1; break;
    case 13: ean=2; break;
    case 14: ean=3; break;
    case 19: ean=4; break;
    case 25: ean=5; break;
    case 28: ean=6; break;
    case 21: ean=7; break;
    case 22: ean=8; break;
    case 26: ean=9; break;
    default: ean=0; /* no or invalid EAN digit or UPC-extension */
  } crc+=ean*1;
  /* ToDo: fix possible buffer OVL, complement crc */
  buf[i5]=0;
  if (result)
   sprintf(result,"<barcode type=\"UPC\" chars=\"%d\" code=\"%d%s\" "
                  "crc=\"%d\" error=\"%.3f\" />",
          i5+1,ean,buf,(10-crc%10)%10,dww/((num_bars-6)*2));
  free(buf);
  return result;
}

    /* EAN/UPC add-on is either 2 or 5 digits.  It always starts with a
     * guard bar BSB, followed by ([digit + SB] * (N-1)) + digit. Digit is
     * SBSB.  Two digit add-on's have 7 bars, and 5 digit add ons have 16.
     */
char *decode_UPC_addon(int *wb, int num_bars){ /* ToDo: char *dest, int len */
  int i, w, i1, i2, i3, i4, i5, digits=num_bars/3;
  double err, min_err, dw, dww=0.0;  char digit;
  char *result=NULL, *buf=NULL; /* malloc and store the result */
  if (num_bars!=7 && num_bars!=16)
    return 0;
  for(w=i=0;i<2*num_bars-1;i++) w+=wb[i];

  dw=1.0*w/(digits*7+4 + (digits-1)*2);
  /* check front BSB, and delineators SB */
  dww=0;
  for (i=0;i<2;i++) { dww=sqr(wb[i]/dw-1); if (dww>0.4) return 0; }
  dww=sqr(wb[i]*0.5/dw-1);  if (dww>0.4) return 0;
  for (i=1;i<digits; i++) {
    for (i1=0; i1<2; i1++) {
      dww = sqr(wb[i*6 + 1 + i1]/dw-1);
      if (dww > 0.4) return 0;
    }
  }
  buf   =(char *)malloc(digits+1); if (!buf)    return result;
  result=(char *)malloc(256+digits+1);

  for(i5=0,i1=3;i1<2*num_bars-1;i1+=6) { /* each digit (2bars+2spaces) */
    for (i4=NumUPC,digit='?',min_err=16e8,i2=0;i2<NumUPC;i2++) {
      for (err=0,i3=0;i3<4;i3++) err+=sqr(codeUPC[i2][i3]-'0'-wb[i1+i3]/dw);
      if (err<min_err) { min_err=err; i4=i2; digit='0'+i2%10; }
    }
    dww+=min_err;
    buf[i5++]=digit;
    /* ToDo: error as deviation wb from ideal */
    g_debug(fprintf(stderr,"\n UPC digit=%c mirrored=%d err=%.3f err_m=%.3f ",
                               digit,min_err/4,dww/(i5*4));)
  }
  buf[i5]=0;
  if (result)
   sprintf(result,"<barcode type=\"UPC_addon\" chars=\"%d\" code=\"%s\" "
                  "error=\"%.3f\" />",
          i5,buf,dww/((num_bars-6)*2));
  free(buf);
  return result;
}


/* --------------------------------------------------------- *
 * code 3 of 9, 3 thick of 9 bars
 *    BSBSBSBSB<S> size=7+3*aw aw=2(3), sumS/sumB=2/1? 
 *    two widths: size=1 or size=2or3, digit_width=13(16)
 *    5 bars and 4(+1) spaces per digit, last space is not used
 *    with 2 (or 0) thick bars, 1 (or 3) thick spaces per digit
 *     => 3 of 9 => max=9*8*7=504
 *     evenBB=(0of5)+(2of5) oddSS=(1of4)+(3of4) max=44
 *  ToDo: better code -...-.-.. as 046 or 083 (even,even,odd)
 */
#define Num39 (40+4) /* (3of9)=(2of5)(1of4)+(0of5)(3of4), (2of5)(.-..)=0..9 */
const char *code39= /* rearranged to BBBBBSSSS<S> (bars,spaces) */
                                                "0..--..-.."
"1-...-.-..""2.-..-.-..""3--....-..""4..-.-.-..""5-.-...-.."
"6.--...-..""7...--.-..""8-..-..-..""9.-.-..-.."
"A-...-..-.""B.-..-..-.""C--.....-.""D..-.-..-.""E-.-....-."
"F.--....-.""G...--..-.""H-..-...-.""I.-.-...-.""J..--...-."
"K-...-...-""L.-..-...-""M--......-""N..-.-...-""O-.-.....-"
"P.--.....-""Q...--...-""R-..-....-""S.-.-....-""T..--....-"
"U-...--...""V.-..--...""W--...-...""X..-.--...""Y-.-..-..."
"Z.--..-...""-...---..."".-..-.-..."" .-.-.-...""*..--.-..."
/* (0of5)(3of4)=(.....)(3of4) store only 3of4? */
"$.....---.""/.....--.-""+.....-.--""%......---"
"?xxxxxxxxx";

/* example: barcode -E -e 39 -b 123abc | gs -sDEVICE=pnggray -r100 */

/* return index[] according to sorted values[], big first */
void sort(int *value, int *idx, int len){
  int i,j;
  for (j=0;j<len;j++) idx[j]=j; /* initialize */
  for(i=1;i;) {  /* bubble sort, len...len^2 steps */
    for(i=j=0;j<len-1;j++) if(value[idx[j]]<value[idx[j+1]]) {
      i=idx[j]; idx[j]=idx[j+1]; idx[j+1]=i; i=1;
    }
  } 
}

char *decode_39(int *wb, int num_bars){ /* ToDo: char *dest, int len */
  int i, w, i1, i3, i5, crc, idx[10];
  double dw,dww,err;  char *buf;
  char *result=NULL; /* malloc and store the result */

  /* check for multiple of 5 bars and minimum start+1char+stop=15 bars */
  if ((num_bars)%5!=0 || num_bars<15) return 0;
  for(w=i=0;    i<2*num_bars-1;i++  ) w+=wb[i];   /* summ width to w */
  dw=w*1.0/(16*(num_bars/5)); /* threshold = 1.5..2 */
  /* whats best and most rigorosely for dw=threshold_width?
   *  - (1.5..2)*mean_width of every 5th space
   *  - (1.5..2)*summ(5bars+5spaces)/(13..16)
   *  - 3/4*summ(three thickest)/3
   */
  dww=crc=0; /* error and checksum (not supported yet) */
#if 0  /* should we exclude any non-standard code39? */
  /* check for correct start and end symbol * or NwNnWnWnN Narrow+Wide */
  i=2*num_bars-2;
  if (wb[  0]>dw*2 || wb[  1]<=dw*2 || wb[  2]> dw*2) return 0;
  if (wb[i-0]>dw*2 || wb[i-1]> dw*2 || wb[i-2]<=dw*2) return 0;
#endif
  g_debug(fprintf(stderr," code39  base=%.3f chars=%2d\n ",dw,(num_bars)/5);)
  buf   =(char *)malloc(  1+(num_bars)/5); if (!buf)    return result;
  result=(char *)malloc(256+(num_bars)/5);

  for(i5=i1=0;i1<2*num_bars-3;i1+=10) {
    /* ToDo: looking for three widest bars/gaps */
    sort(wb+i1,idx,9);
    for(err=0,i3=3;i3<9;i3++) err+=sqr(wb[i1+idx[i3]]/dw-1.0); /* narrow=1, wide=2..3 */
    dww+=err;
    for(i3=0;i3<Num39;i3++)
      if (code39[10*i3+1+(idx[0]%2)*5+idx[0]/2]=='-'
       && code39[10*i3+1+(idx[1]%2)*5+idx[1]/2]=='-'
       && code39[10*i3+1+(idx[2]%2)*5+idx[2]/2]=='-') break;
    if (i5>0 && i5<num_bars/5-2) crc+=i3;
    buf[i5++]=code39[10*i3];

    /* ToDo: check if wee have even number of black bars within 3 biggest */
    g_debug(for(i3=0;i3<9;i3++)fprintf(stderr,"%02d ",wb[i1+i3]);
       fprintf(stderr," posWide=%d,%d,%d  %c  err=%.3f\n ",
         idx[0],idx[1],idx[2],buf[i5-1],err/6);)
  }
  buf[i5]=0;
  if (result)
   sprintf(result,"<barcode type=\"39\" chars=\"%d\" code=\"%s\" "
                  "crc=\"%c\" error=\"%.3f\" />",
          i5,buf,code39[(crc%44)*10],dww/((num_bars/5)*6));
  free(buf);
  return result;
}

/* code interleaved 2 of 5
   B B B B B  <= digit 1,3,5,...
    S S S S S <= digit 2,4,6,...
    1 2 4 7 0 <= weight   digit=sum(bit*weight)%11
 */
const char *code25= /* is the code sorted randomly? */
"1-...-2.-..-3--...4..-.-5-.-..6.--..7...--8-..-.9.-.-.0..--.";

/* example: barcode -E -e i25 -b 123456 >a.eps */

/* ToDo: decode_i25 (low priority) */

/*
    decode barcode
    - check main characteristics (num bars, min+max width, etc.)
    - detect code type
    - the current algorithm measures the width of bars and spaces
    called by detect_barcode()
 */
char *decode_barcode(struct box *bb){ /* ToDo: char *dest, int len */
  int i, num_bars, yy, w, ww, dx, xx, cs=JOB->cfg.cs, *wb;
  char *result=NULL; /* store the result */
  yy=(bb->y0+bb->y1)/2;
  w=ww=bb->x1-bb->x0+1;
  num_bars = num_cross(bb->x0,bb->x1,yy,yy,bb->p,JOB->cfg.cs);
  if(JOB->cfg.verbose)  
    fprintf(stderr,"\n# ... detect bars=%3d w=%4d",num_bars,ww);
  
  /* store width of bars and spaces to buffer wb */
  wb=(int *)malloc(2*num_bars*sizeof(int)); if(!wb) return NULL;
  xx=bb->x0;
  xx-=loop(bb->p,xx,yy, 8,cs,1,LE);
  xx+=loop(bb->p,xx,yy,ww,cs,0,RI); /* start with a bar! */
  for (i=0;i<2*num_bars;i++) {
    dx=loop(bb->p,xx,yy,w,cs,1^(i&1),RI);
    xx+=dx;
    w-=dx;
    wb[i]=dx;
  } wb[2*num_bars-1]=0;
  
  /* ToDo: what about probability? if not unique
   *      - add argument char *result which can be modified or not,
   *      - or add box2? (would reuse of this code more difficult)
   */
  /* test code128 characteristics, ToDo: look for correct start/stop 211 seq. */
  if ((num_bars-1)%3==0 && num_bars>=10 && ww>=11*(num_bars-1)/3+2){
    if (!result) result=decode_code128(wb,num_bars);
  }
  /* test UPC/EAN characteristics */
  if ((num_bars)%2==0 && num_bars>=8 && ww>=7*(num_bars-6)/2+11
  && ((num_bars-6)/2)%2==0){  /* should be balanced */
    if (!result) result=decode_UPC(wb,num_bars);
  }
  /* test UPC_addon by Michael van Rooyen, often on books */
  if (num_bars==7 || num_bars==16)
    if (!result) result=decode_UPC_addon(wb,num_bars);

  /* test code39 characteristics */
  if ((num_bars)%5==0 && num_bars>14){
    if (!result) result=decode_39(wb,num_bars);
  }
  free(wb);
  return result;
}

/*
 * taking the list of boxes and search for groups of bars (1D-barcodes)
 */
int detect_barcode(job_t *job)
{
  int j=0, j2=0, bx0, by0, bx1, by1, bdx, bdy, bbx, rm,
     x0, y0, dx, dy, cs, y, yl0, yl1, yr0, yr1;
  struct box *box2, *box3;

  if(JOB->cfg.verbose)  
    fprintf(stderr,"# barcode.c detect_barcode ");
  x0=y0=0; rm=0; dx=job->src.p.x;  dy=job->src.p.y; cs=JOB->cfg.cs;
  for_each_data(&(JOB->res.boxlist)) {
    box2 = (struct box *)list_get_current(&(JOB->res.boxlist));
    /* detect width (bdx) and height (bdy) of possible bar */
    /* ToDo: better check for a line */
    bdx=box2->x1-box2->x0+1  /* substract correction for skewed bars */
       -loop(box2->p,box2->x1,(box2->y0+box2->y1)/2,box2->x1-box2->x0,cs,0,LE)
       -loop(box2->p,box2->x0,(box2->y0+box2->y1)/2,box2->x1-box2->x0,cs,0,RI);
    bdy=box2->y1-box2->y0+1;
    if (box2->c == PICTURE || box2->c == UNKNOWN)
    if (box2->y0 >= y0 && box2->y1 <= y0 + dy  /* within frame? */
     && box2->x0 >= x0 && box2->x1 <= x0 + dx
     && box2->y1 - box2->y0 > 19               /* min. length */
     && box2->y1 - box2->y0 >  8 * bdx
    ) { /* a bar? */
       j=1; /* number of bars */
       bx0=box2->x0; bx1=box2->x1;  /* initial values for barcode frame */
       by0=box2->y0; by1=box2->y1;
       bbx=bx1-bx0+2; /* width of bar */
       /* this is for scans which are not exactly horizontal */
       yl0=yr0=by0; /* left and right upper bound */
       yl1=yr1=by1; /* left and right lower bound */
       /* --- iteratively take into account next nearest bar ---
        * this is important, because bar-boxes are not in right order */
       for (j2=1;j2;) {
         j2=0;
         /* expand a frame around the potential barcode (bx0,by0,bx1,by1) */
         for_each_data(&(JOB->res.boxlist)) {
           box3 = (struct box *)list_get_current(&(JOB->res.boxlist));
           /* bdy=box3->y1-box3->y0+1; */
           if (box2!=box3)
           if (box3->c == PICTURE || box3->c == UNKNOWN)
           if (box3->y0 >= y0 && box3->y1 <= y0 + dy  /* within image */
            && box3->x0 >= x0 && box3->x1 <= x0 + dx
            && box3->y1 - box3->y0 > 19               /* min. length */
            && box3->y1 - box3->y0 >  4 * (bdx) /* height > 4*width2 */
            && box3->x1 - box3->x0 <  4 * (bdy) /* width  < height/4 = bar */
            && ((  abs(box3->y0-by0)<bdy/16+4   /* within bar-box ? */
                && abs(box3->y1-by1)<bdy/2   )  /* lower ends of UPC could be longer */
               ||( abs(box3->y0-yl0)<bdy/16+4   /* left side of frame */
                && abs(box3->y1-yl1)<bdy/2
                && box3->x0 <= bx0           )
               ||( abs(box3->y0-yr0)<bdy/16+4   /* right side of frame */
                && abs(box3->y1-yr1)<bdy/2
                && box3->x0 >= bx1           ) )
            && box3->x0 > bx0 - 12*bbx /* startspace=5...10 */
            && box3->x1 < bx1 + 12*bbx
            && box3->x0 > bx0 - bdy/2  /* dont glue two barcodes together */
            && box3->x1 < bx1 + bdy/2                   /* ex: ean13a.jpg */
            /* dont check bars which already within the frame twice ? */
            && (  box3->x1 > bx1 || box3->x0 < bx0 )
           ) { /* a bar? -> extend barcode frame only in x direction */
               /* take minimum of y to have valid barcode for all y */
              if (box3->x0<bx0) { bx0=box3->x0; yl0=box3->y0; yl1=box3->y1; }
              if (box3->x1>bx1) { bx1=box3->x1; yr0=box3->y0; yr1=box3->y1; }
              if (4*(box3->y1-box3->y0)>3*(by1-by0)) { /* carefull reduce */
                if (box3->y0>by0) by0=box3->y0; /* ToDo: fix for non-horizontal */
                if (box3->y1<by1) by1=box3->y1;
              }
              j++;  /* found a near bar and count to num bars */
              j2=1; /* continue searching (endless loop?) */
           }
         } end_for_each(&(JOB->res.boxlist));
       }
       /* j is the num of bars found above, some inner bars are not counted */
       /* ToDo: better iterative add next nearest bars from sorted list near bars? */
       if (j>5) {
         char *code=0;
         box2->c=PICTURE; /* BARCODE */
         box2->x0=bx0;       box2->y0=by0;
         box2->x1=bx1;       box2->y1=by1;
         /* ToDo: add pointer to decoded text */

         y=(box2->y0+box2->y1)/2;
         if (JOB->cfg.verbose){
           int nbars;
           nbars=num_cross(box2->x0,box2->x1,y,y,box2->p,JOB->cfg.cs);
           fprintf(stderr,"\n# barcode at %3d %3d size %3d %3d nbars %d (%d)",
             bx0,by0,bx1-bx0+1,by1-by0+1,nbars,j);
           if (j!=nbars)
             fprintf(stderr,"\n# ... trouble: num_found_bars != num_cross");
           /* this is because some far bars are detected before near bars */
         }
         /* transport the info to the gocr-output (development) */
         /* ToDo: decode and print/store barcode bars=j */
         code=decode_barcode(box2); /* ToDo: char *dest, int len */
         if (!code) { /* failed */
           code=(char *)malloc(128);
           /* ToDo: analyze and output num_bars, width of bars etc. */
           if(code) strncpy(code,"<barcode type=\"unknown\" />",128);
         }
         if (JOB->cfg.verbose)  
           fprintf(stderr,"\n# ... decoded as: %s", code);
         setas(box2,code,99); /* ToDo: set a better weight */
         free(code);

         /* remove inner boxes, only if sure!? (ToDo: use cfg.certainty) */
         for_each_data(&(JOB->res.boxlist)) {
           box3 = (struct box *)list_get_current(&(JOB->res.boxlist));
           /* bdy=box3->y1-box3->y0+1; */
           if (box2!=box3)
           if (box3->c == PICTURE || box3->c == UNKNOWN)
           if (    abs(box3->y0-by0)<bdy/16+4   /* within bar-box ? */
                && abs(box3->y1-by1)<bdy/2      /* lower ends of UPC could be longer */
                && box3->x1 <= bx1
                && box3->x0 >= bx0
           ) {
              rm++; /* count removed boxes */
              list_del(&(JOB->res.boxlist),box3);
              free_box(box3);
           }
         } end_for_each(&(JOB->res.boxlist));
         if (JOB->cfg.verbose)  
           fprintf(stderr,"\n# ... removed boxes: %d", rm);
         rm=0;
       }
   }
  } end_for_each(&(JOB->res.boxlist));

  /* recalculate averages without bars */
  JOB->res.numC=JOB->res.sumX=JOB->res.sumY=j2=0;
  for_each_data(&(JOB->res.boxlist)) {
    j2++;
    box3 = (struct box *)list_get_current(&(JOB->res.boxlist));
    if (box3->c==PICTURE) continue;
    JOB->res.numC++; /* count remaining boxes */
    JOB->res.sumX+=box3->x1-box3->x0+1;
    JOB->res.sumY+=box3->y1-box3->y0+1;
  } end_for_each(&(JOB->res.boxlist));
  if(JOB->cfg.verbose)  
    fprintf(stderr,"\n# ... boxes %d nC %d\n",
            j2, JOB->res.numC);

  return 0;
}

