#include "base_testbench.h"

unsigned int ticks = 0;

struct ctrl_case_t {
    const char *name;
    uint32_t instr;
    uint8_t zero;
    uint8_t pc_src, reg_write, alu_src, imm_src, alu_ctrl;
};

static const ctrl_case_t cases[] = {
    // name                          instr       zero  pc_src reg_write alu_src imm_src alu_ctrl
    {"addi t1, zero, 0xff",          0x0ff00313, 0,    0,     1,        1,      0,      0b000},
    {"addi a0, a1, 0",               0x00058513, 0,    0,     1,        1,      0,      0b000},
    {"addi a1, a1, 1",               0x00158593, 0,    0,     1,        1,      0,      0b000},
    {"addi with zero=1",             0x00158593, 1,    0,     1,        1,      0,      0b000},
    {"bne a1, t1, -8 (taken)",       0xfe659ce3, 0,    1,     0,        0,      1,      0b001},
    {"bne a1, t1, -8 (not taken)",   0xfe659ce3, 1,    0,     0,        0,      1,      0b001},
    {"beq (unsupported)",            0x00b50463, 0,    0,     0,        0,      0,      0b000},
    {"add (unsupported)",            0x00c58533, 0,    0,     0,        0,      0,      0b000},
    {"slti (unsupported)",           0x0035a513, 0,    0,     0,        0,      0,      0b000},
    {"all zeros",                    0x00000000, 0,    0,     0,        0,      0,      0b000},
};

class CtrlUnitTestbench : public BaseTestbench {
    protected:
        void initializeInputs() override {
            top->op = 0;
            top->funct3 = 0;
            top->zero = 0;
        }

        void decode(uint32_t instr, uint8_t zero) {
            top->op = instr & 0x7F;
            top->funct3 = (instr >> 12) & 0x7;
            top->zero = zero;

            top->eval();
        }
};

TEST_F(CtrlUnitTestbench, DecodesAllCases) {
    for (const auto &c : cases) {
        SCOPED_TRACE(c.name);
        decode(c.instr, c.zero);
        EXPECT_EQ(top->pc_src,    c.pc_src);
        EXPECT_EQ(top->reg_write, c.reg_write);
        EXPECT_EQ(top->alu_src,   c.alu_src);
        EXPECT_EQ(top->imm_src,   c.imm_src);
        EXPECT_EQ(top->alu_ctrl,  c.alu_ctrl);
    }
}

TEST_F(CtrlUnitTestbench, BranchNeverWritesRegisters) {
    for (uint8_t zero = 0; zero <= 1; zero++) {
        decode(0xfe659ce3, zero);
        EXPECT_EQ(top->reg_write, 0);
    }
}

TEST_F(CtrlUnitTestbench, OnlyBranchesChangePc) {
    for (uint8_t op = 0; op < 128; op++) {
        if (op == 0b1100011) {
            continue;
        }

        for (uint8_t f3 = 0; f3 < 8; f3++) {
            top->op = op;
            top->funct3 = f3;
            top->zero = 0;

            top->eval();
            ASSERT_EQ(top->pc_src, 0) << "op=" << (int) op << " funct3=" << (int) f3;
        }
    }
}
