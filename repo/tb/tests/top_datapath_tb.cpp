#include "testbench.h"

unsigned int ticks = 0;

enum alu_op_e : uint8_t {
    OP_ADD = 0b000,
    OP_SUB = 0b001
};

enum reg_e : uint8_t {
    ZERO = 0,
    T0 = 5,
    T1 = 6,
    A0 = 10,
    A1 = 11
};

class TopDatapathTestbench : public Testbench {
    protected:
        void initializeInputs() override {
            top->clk = 0;
            top->rs1 = 0;
            top->rs2 = 0;
            top->rd = 0;
            top->RegWrite = 0;
            top->ALUsrc = 0;
            top->ALUctrl = OP_ADD;
            top->ImmOp = 0;
        }

        // rd = rs1 + imm, then clock
        void addi(uint8_t rd, uint8_t rs1, uint32_t imm) {
            top->rs1 = rs1;
            top->rd = rd;
            top->ImmOp = imm;
            top->ALUsrc = 1;
            top->ALUctrl = OP_ADD;
            top->RegWrite = 1;

            runSimulation(1);
            top->RegWrite = 0;
        }

        // set up a bne comparison (no write), evaluate EQ
        uint8_t compare(uint8_t rs1, uint8_t rs2) {
            top->rs1 = rs1;
            top->rs2 = rs2;
            top->ALUsrc = 0;
            top->ALUctrl = OP_SUB;
            top->RegWrite = 0;

            top->eval();
            return top->EQ;
        }
};

TEST_F(TopDatapathTestbench, AddiToA0) {
    addi(A0, ZERO, 5);
    EXPECT_EQ(top->a0, 5u);
}

TEST_F(TopDatapathTestbench, AddiAccumulates) {
    addi(A0, ZERO, 5);
    addi(A0, A0, 3);
    EXPECT_EQ(top->a0, 8u);
}

TEST_F(TopDatapathTestbench, AddiNegativeImm) {
    addi(A0, ZERO, 10);
    addi(A0, A0, (uint32_t) -3);
    EXPECT_EQ(top->a0, 7u);
}

TEST_F(TopDatapathTestbench, MoveBetweenRegisters) {
    addi(A1, ZERO, 42);
    addi(A0, A1, 0);
    EXPECT_EQ(top->a0, 42u);
}

TEST_F(TopDatapathTestbench, WriteToX0Ignored) {
    addi(ZERO, ZERO, 99);
    addi(A0, ZERO, 0);
    EXPECT_EQ(top->a0, 0u);
}

TEST_F(TopDatapathTestbench, NoWriteWhenRegWriteLow) {
    addi(A0, ZERO, 5);

    top->rs1 = ZERO;
    top->rd = A0;
    top->ImmOp = 77;
    top->ALUsrc = 1;
    top->RegWrite = 0;

    runSimulation(1);
    EXPECT_EQ(top->a0, 5u);
}

TEST_F(TopDatapathTestbench, EqWhenRegistersEqual) {
    addi(T0, ZERO, 7);
    addi(T1, ZERO, 7);
    EXPECT_EQ(compare(T0, T1), 1);
}

TEST_F(TopDatapathTestbench, NotEqWhenRegistersDiffer) {
    addi(T0, ZERO, 7);
    addi(T1, ZERO, 8);
    EXPECT_EQ(compare(T0, T1), 0);
}

TEST_F(TopDatapathTestbench, ComparisonDoesNotWrite) {
    addi(A0, ZERO, 5);
    addi(T1, ZERO, 9);

    top->rd = A0;
    compare(A0, T1);

    runSimulation(1);
    EXPECT_EQ(top->a0, 5u);
}

// mirrors the inner loop of tb/asm/program.S
TEST_F(TopDatapathTestbench, CounterLoop) {
    addi(T1, ZERO, 0xff);
    addi(A1, ZERO, 0);

    int iterations = 0;
    do {
        addi(A0, A1, 0);
        addi(A1, A1, 1);
        iterations++;
        ASSERT_LT(iterations, 1000);
    } while (!compare(A1, T1));

    EXPECT_EQ(top->a0, 254u);
    EXPECT_EQ(iterations, 255);
}
