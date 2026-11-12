#include "base_testbench.h"

unsigned int ticks = 0;

enum alu_op_e : uint8_t {
    OP_ADD = 0b000,
    OP_SUB = 0b001,
    OP_AND = 0b010,
    OP_OR  = 0b011,
    OP_XOR = 0b100,
    OP_SLL = 0b101,
    OP_SRL = 0b110,
    OP_SRA = 0b111
};

class AluTestbench : public BaseTestbench {
    protected:
        void initializeInputs() override {
            top->ALUop1 = 0;
            top->ALUop2 = 0;
            top->ALUctrl = OP_ADD;
        }

        uint32_t run(uint32_t a, uint32_t b, uint8_t op) {
            top->ALUop1 = a;
            top->ALUop2 = b;
            top->ALUctrl = op;

            top->eval();
            return top->SUM;
        }
};

TEST_F(AluTestbench, Add) {
    EXPECT_EQ(run(5, 3, OP_ADD), 8u);
}

TEST_F(AluTestbench, AddNegative) {
    EXPECT_EQ(run(5, (uint32_t) -3, OP_ADD), 2u);
}

TEST_F(AluTestbench, AddOverflow) {
    EXPECT_EQ(run(0xFFFFFFFF, 1, OP_ADD), 0u);
}

TEST_F(AluTestbench, Sub) {
    EXPECT_EQ(run(5, 3, OP_SUB), 2u);
}

TEST_F(AluTestbench, SubNegative) {
    EXPECT_EQ((int32_t) run(3, 5, OP_SUB), -2);
}

TEST_F(AluTestbench, SubUnderflow) {
    EXPECT_EQ(run(0, 1, OP_SUB), 0xFFFFFFFFu);
}

TEST_F(AluTestbench, And) {
    EXPECT_EQ(run(0b1100, 0b1010, OP_AND), 0b1000u);
}

TEST_F(AluTestbench, Or) {
    EXPECT_EQ(run(0b1100, 0b1010, OP_OR), 0b1110u);
}

TEST_F(AluTestbench, Xor) {
    EXPECT_EQ(run(0b1100, 0b1010, OP_XOR), 0b0110u);
}

TEST_F(AluTestbench, ShiftLeft) {
    EXPECT_EQ(run(1, 4, OP_SLL), 16u);
    EXPECT_EQ(run(1, 31, OP_SLL), 0x80000000u);
}

TEST_F(AluTestbench, ShiftRightLogical) {
    EXPECT_EQ(run(0x80000000, 4, OP_SRL), 0x08000000u);
}

TEST_F(AluTestbench, ShiftRightArithmetic) {
    EXPECT_EQ(run(0x80000000, 4, OP_SRA), 0xF8000000u);
    EXPECT_EQ(run(0x40000000, 4, OP_SRA), 0x04000000u);
}

TEST_F(AluTestbench, ShiftUsesOnlyLow5Bits) {
    EXPECT_EQ(run(1, 33, OP_SLL), 2u);
    EXPECT_EQ(run(0x80000000, 33, OP_SRL), 0x40000000u);
}

TEST_F(AluTestbench, EqWhenEqual) {
    run(7, 7, OP_SUB);
    EXPECT_EQ(top->EQ, 1);
}

TEST_F(AluTestbench, EqWhenNotEqual) {
    run(7, 6, OP_SUB);
    EXPECT_EQ(top->EQ, 0);

    run(0, 0x80000000, OP_SUB);
    EXPECT_EQ(top->EQ, 0);
}

TEST_F(AluTestbench, EqIndependentOfOp) {
    run(42, 42, OP_ADD);
    EXPECT_EQ(top->EQ, 1);
}
