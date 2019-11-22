#include "cpu/exec.h"
#include "all-instr.h"
static inline void set_width(int width) {
  if (width == 0) {
    width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  }
  decinfo.src.width = decinfo.dest.width = decinfo.src2.width = width;
}
static make_EHelper(2byte_esc);
make_EHelper(call);
make_EHelper(push);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(ret);
make_EHelper(nop);
make_EHelper(lea);
make_EHelper(and);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(setcc);
make_EHelper(movzx);
make_EHelper(test);
make_EHelper(jcc);
make_EHelper(leave);
make_EHelper(inc);
make_EHelper(pop);
make_EHelper(adc);
make_EHelper(or);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(dec);
make_EHelper(not);
make_EHelper(mul);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);
make_EHelper(cltd);
make_EHelper(idiv);
make_EHelper(div);
make_EHelper(jmp);
make_EHelper(movsx);
make_EHelper(call_rm);
make_EHelper(jmp_rm);
make_EHelper(shr);
make_EHelper(sbb);
make_EHelper(in);
make_EHelper(out);
make_EHelper(cwtl);
make_EHelper(neg);
make_EHelper(rol);
make_EHelper(ror);
make_EHelper(lidt);
make_EHelper(int);
make_EHelper(popa);
make_EHelper(pusha);
make_EHelper(iret);
#define make_group(name, item0, item1, item2, item3, item4, item5, item6, item7) \
  static OpcodeEntry concat(opcode_table_, name) [8] = { \
    /* 0x00 */	item0, item1, item2, item3, \
    /* 0x04 */	item4, item5, item6, item7  \
  }; \
static make_EHelper(name) { \
  idex(pc, &concat(opcode_table_, name)[decinfo.isa.ext_opcode]); \
}

/* 0x80, 0x81, 0x83 */
make_group(gp1,
    EX(add), EX(or), EX(adc), EX(sbb),
    EX(and), EX(sub), EX(xor), EX(cmp))

/* 0xc0, 0xc1, 0xd0, 0xd1, 0xd2, 0xd3 */
make_group(gp2,
    EX(rol), EX(ror), EMPTY, EMPTY,
    EX(shl), EX(shr), EMPTY, EX(sar))

/* 0xf6, 0xf7 */
make_group(gp3,
    IDEX(test_I,test), EMPTY, EX(not), EX(neg),
    EX(mul), EX(imul1), EX(div), EX(idiv))

/* 0xfe */
make_group(gp4,
    EMPTY, EX(dec), EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY)

/* 0xff */
make_group(gp5,
    EX(inc), EX(dec), EX(call_rm), EX(call),
    EX(jmp_rm), EX(jmp), EX(push), EMPTY)

/* 0x0f 0x01*/
make_group(gp7,
    EMPTY, EMPTY, EMPTY, EX(lidt),
    EMPTY, EMPTY, EMPTY, EMPTY)

/* TODO: Add more instructions!!! */

static OpcodeEntry opcode_table [512] = {
  /* 0x00 */	IDEXW(G2E,add,1), IDEX(G2E,add), IDEXW(E2G,add,1), IDEX(E2G,add),
  /* 0x04 */	IDEXW(I2a,add,1), IDEX(I2a,add), EMPTY, EMPTY,
  /* 0x08 */	IDEXW(G2E,or,1), IDEX(G2E,or), IDEXW(E2G,or,1), IDEX(E2G,or),
  /* 0x0c */	IDEXW(I2a,or,1), IDEX(I2a,or), EMPTY, EX(2byte_esc),
  /* 0x10 */	IDEXW(G2E,adc,1), IDEX(G2E,adc), IDEX(E2G,adc), IDEX(E2G,adc),
  /* 0x14 */	IDEXW(I2a,adc,1), IDEX(I2a,adc), EMPTY, EMPTY,
  /* 0x18 */	IDEXW(G2E,sbb,1), IDEX(G2E,sbb), IDEXW(E2G,sbb,1), IDEX(E2G,sbb),
  /* 0x1c */	IDEXW(I2a,sbb,1), IDEX(I2a,sbb), EMPTY, EMPTY,
  /* 0x20 */	IDEXW(G2E,and,1), IDEX(G2E,and), IDEXW(E2G,and,1), IDEX(E2G,and),
  /* 0x24 */	IDEXW(I2a,and,1), IDEX(I2a,and), EMPTY, EMPTY,
  /* 0x28 */	IDEXW(G2E,sub,1), IDEX(G2E,sub), IDEXW(E2G,sub,1), IDEX(E2G,sub),
  /* 0x2c */	IDEXW(I2r,sub,1), IDEX(I2a,sub), EMPTY, EMPTY,
  /* 0x30 */	IDEXW(G2E,xor,1), IDEX(G2E,xor), IDEXW(E2G,xor,1), IDEX(E2G,xor),
  /* 0x34 */	IDEXW(I2a,xor,1), IDEX(I2a,xor), EMPTY, EMPTY,
  /* 0x38 */	IDEXW(G2E,cmp,1), IDEX(G2E,cmp), IDEXW(E2G,cmp,1), IDEX(E2G,cmp),
  /* 0x3c */	IDEXW(I2a,cmp,1), IDEX(I2a,cmp), EMPTY, EMPTY,
  /* 0x40 */	IDEX(r,inc), IDEX(r,inc), IDEX(r,inc), IDEX(r,inc),
  /* 0x44 */	IDEX(r,inc), IDEX(r,inc), IDEX(r,inc), IDEX(r,inc),
  /* 0x48 */	IDEX(r,dec), IDEX(r,dec), IDEX(r,dec), IDEX(r,dec),
  /* 0x4c */	IDEX(r,dec), IDEX(r,dec), IDEX(r,dec), IDEX(r,dec),
  /* 0x50 */	IDEX(r,push), IDEX(r,push), IDEX(r,push), IDEX(r,push),
  /* 0x54 */	IDEX(r,push), IDEX(r,push), IDEX(r,push), IDEX(r,push),
  /* 0x58 */	IDEX(r,pop), IDEX(r,pop), IDEX(r,pop), IDEX(r,pop),
  /* 0x5c */	IDEX(r,pop), IDEX(r,pop), IDEX(r,pop), IDEX(r,pop),
  /* 0x60 */	EX(pusha), EX(popa), EMPTY, EMPTY,
  /* 0x64 */	EMPTY, EMPTY, EX(operand_size), EMPTY,
  /* 0x68 */	IDEX(I,push), IDEX(I_E2G,imul3), IDEXW(I,push,1), EMPTY,
  /* 0x6c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x70 */	IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1),
  /* 0x74 */	IDEXW(J,jcc,1),IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1),
  /* 0x78 */	IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1),
  /* 0x7c */	IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1), IDEXW(J,jcc,1),
  /* 0x80 */	IDEXW(I2E, gp1, 1), IDEX(I2E, gp1), EMPTY, IDEX(SI2E, gp1),
  /* 0x84 */	IDEXW(G2E,test,1), IDEX(G2E,test), EMPTY, EMPTY,
  /* 0x88 */	IDEXW(mov_G2E, mov, 1), IDEX(mov_G2E, mov), IDEXW(mov_E2G, mov, 1), IDEX(mov_E2G, mov),
  /* 0x8c */	EMPTY, IDEX(lea_M2G,lea), EMPTY, EMPTY,
  /* 0x90 */	EX(nop), EMPTY, EMPTY, EMPTY,
  /* 0x94 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x98 */	EX(cwtl), EX(cltd), EMPTY, EMPTY,
  /* 0x9c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa0 */	IDEXW(O2a, mov, 1), IDEX(O2a, mov), IDEXW(a2O, mov, 1), IDEX(a2O, mov),
  /* 0xa4 */	EX(movsx), EX(movsx), EMPTY, EMPTY,
  /* 0xa8 */	IDEXW(I2a,test,1), IDEX(I2a,test), EMPTY, EMPTY,
  /* 0xac */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xb0 */	IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1),
  /* 0xb4 */	IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1), IDEXW(mov_I2r, mov, 1),
  /* 0xb8 */	IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov),
  /* 0xbc */	IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov), IDEX(mov_I2r, mov),
  /* 0xc0 */	IDEXW(gp2_Ib2E, gp2, 1), IDEX(gp2_Ib2E, gp2), EMPTY, EX(ret),
  /* 0xc4 */	EMPTY, EMPTY, IDEXW(mov_I2E, mov, 1), IDEX(mov_I2E, mov),
  /* 0xc8 */	EMPTY, EX(leave), EMPTY, EMPTY,
  /* 0xcc */	EMPTY, IDEXW(I,int,1), EMPTY, EX(iret),
  /* 0xd0 */	IDEXW(gp2_1_E, gp2, 1), IDEX(gp2_1_E, gp2), IDEXW(gp2_cl2E, gp2, 1), IDEX(gp2_cl2E, gp2),
  /* 0xd4 */	EMPTY, EMPTY, EX(nemu_trap), EMPTY,
  /* 0xd8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xdc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe4 */	IDEXW(in_I2a,in,1), IDEX(in_I2a,in), IDEXW(out_a2I,out,1), IDEX(out_a2I,out),
  /* 0xe8 */	IDEX(J,call), IDEX(J,jmp), EMPTY, IDEXW(J,jmp,1),
  /* 0xec */	IDEXW(in_dx2a,in,1), IDEX(in_dx2a,in), IDEXW(out_a2dx,out,1), IDEX(out_a2dx,out),
  /* 0xf0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf4 */	EMPTY, EMPTY, IDEXW(E, gp3, 1), IDEX(E, gp3),
  /* 0xf8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xfc */	EMPTY, EMPTY, IDEXW(E, gp4, 1), IDEX(E, gp5),

/*2 byte_opcode_table */

  /* 0x00 */	EMPTY, IDEX(gp7_E, gp7), EMPTY, EMPTY,
  /* 0x04 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x08 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x0c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x10 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x14 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x18 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x1c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x20 */	IDEX(mov_G2E,mov), EMPTY, IDEX(mov_E2G,mov),EMPTY,
  /* 0x24 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x28 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x2c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x30 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x34 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x38 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x3c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x40 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x44 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x48 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x4c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x50 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x54 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x58 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x5c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x60 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x64 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x68 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x6c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x70 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x74 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x78 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x7c */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0x80 */	IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc),
  /* 0x84 */	IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc),
  /* 0x88 */	IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc),
  /* 0x8c */	IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc), IDEX(J,jcc),
  /* 0x90 */	IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1),
  /* 0x94 */	IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1),
  /* 0x98 */	IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1),
  /* 0x9c */	IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1), IDEXW(setcc_E,setcc,1),
  /* 0xa0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xa8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xac */	EMPTY, EMPTY, EMPTY, IDEX(E2G,imul2),
  /* 0xb0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xb4 */	EMPTY, EMPTY, IDEXW(mov_E2G,movzx,1), IDEXW(mov_E2G,movzx,2),
  /* 0xb8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xbc */	EMPTY, EMPTY, IDEXW(mov_E2G,movsx,1), IDEXW(mov_E2G,movsx,2),
  /* 0xc0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xc4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xc8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xcc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xd8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xdc */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xe8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xec */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf0 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf4 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xf8 */	EMPTY, EMPTY, EMPTY, EMPTY,
  /* 0xfc */	EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(2byte_esc) {
  uint32_t opcode = instr_fetch(pc, 1) | 0x100;
  decinfo.opcode = opcode;
  set_width(opcode_table[opcode].width);
  idex(pc, &opcode_table[opcode]);
}

void isa_exec(vaddr_t *pc) {
  uint32_t opcode = instr_fetch(pc, 1);
  decinfo.opcode = opcode;
  set_width(opcode_table[opcode].width);
  idex(pc, &opcode_table[opcode]);
}
