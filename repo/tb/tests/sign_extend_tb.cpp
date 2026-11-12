#include "base_testbench.h"

unsigned int ticks = 0;

enum imm_src_e : uint8_t {
    IMM_I = 0,
    IMM_B = 1
};

class SignExtendTestbench : public BaseTestbench {
    protected:
        void initializeInputs() override {
            top->din = 0;
            top->immsrc = IMM_I;
        }

        int32_t extend(uint32_t instr, uint8_t src) {
            top->din = instr;
            top->immsrc = src;

            top->eval();
            return (int32_t) top->immOp;
        }
};

TEST_F(SignExtendTestbench, ITypePositive) {
    EXPECT_EQ(extend(0x0ff00313, IMM_I), 255);     // addi t1, zero, 0xff
    EXPECT_EQ(extend(0x00158593, IMM_I), 1);       // addi a1, a1, 1
}

TEST_F(SignExtendTestbench, ITypeZero) {
    EXPECT_EQ(extend(0x00058513, IMM_I), 0);       // addi a0, a1, 0
}

TEST_F(SignExtendTestbench, ITypeNegative) {
    EXPECT_EQ(extend(0xfff00513, IMM_I), -1);      // addi a0, zero, -1
}

TEST_F(SignExtendTestbench, ITypeLimits) {
    EXPECT_EQ(extend(0x7ff00513, IMM_I), 2047);    // addi a0, zero, 2047
    EXPECT_EQ(extend(0x80000513, IMM_I), -2048);   // addi a0, zero, -2048
}

TEST_F(SignExtendTestbench, BTypeBackward) {
    EXPECT_EQ(extend(0xfe659ce3, IMM_B), -8);      // bne a1, t1, -8
}

TEST_F(SignExtendTestbench, BTypeForward) {
    EXPECT_EQ(extend(0x00b51863, IMM_B), 16);      // bne a0, a1, +16
}

TEST_F(SignExtendTestbench, BTypeLimits) {
    EXPECT_EQ(extend(0x7eb51fe3, IMM_B), 4094);    // bne a0, a1, +4094
    EXPECT_EQ(extend(0x80b51063, IMM_B), -4096);   // bne a0, a1, -4096
}

TEST_F(SignExtendTestbench, BTypeAlwaysEven) {
    for (uint32_t instr : {0xfe659ce3u, 0x00b51863u, 0x7eb51fe3u, 0x80b51063u}) {
        EXPECT_EQ(extend(instr, IMM_B) & 1, 0) << std::hex << instr;
    }
}

TEST_F(SignExtendTestbench, SameInstrDifferentFormat) {
    EXPECT_EQ(extend(0xfe659ce3, IMM_I), -26);
    EXPECT_EQ(extend(0xfe659ce3, IMM_B), -8);
}
