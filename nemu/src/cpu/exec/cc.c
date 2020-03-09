#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
   enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  }; 

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
   switch (subcode & 0xe) {
    case CC_O:
    case CC_B:
	   rtl_get_CF(&t1);
	   *dest=t1;
	   break;
    case CC_E:
	    rtl_get_ZF(&t3);
//	    printf("ZF:%x\n",t3);
	    *dest=(t3 ==1 ? 1:0);
	//    printf("CC_E dest:%x\n",*dest);
	    break;
    case CC_BE:
	    rtl_get_CF(&t1);
	    rtl_get_ZF(&t2);
//	   printf("CF:%x ZF:%x\n",t1,t2);
	    rtl_or(dest,&t1,&t2);
            break;
    case CC_S:
	    rtl_get_SF(&t0);
	    *dest=t0;
	    break;
    case CC_L:
	    rtl_get_ZF(&t0);
            rtl_get_SF(&t1);
	    rtl_get_OF(&t2);
	    t1=(t1!=t2?1:0);
      	    if(invert){t0=(t0!=1?1:0);rtl_and(dest,&t0,&t1);}
	    else {*dest=t1;}
	    break;
    case CC_LE:
     // TODO();
            rtl_get_ZF(&t0);
	    rtl_get_SF(&t1);
	    rtl_get_OF(&t2);
	    t1=(t1!=t2?1:0);
	    //t0=(t0==0?1:0);
	    rtl_or(dest,&t0,&t1);
//	    printf("the dest now:%x\n",*dest);
	    break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
   }

    if (invert) {
    rtl_xori(dest, dest, 0x1);
  }  
  // printf("dest after invert:%x\n",*dest);
}
