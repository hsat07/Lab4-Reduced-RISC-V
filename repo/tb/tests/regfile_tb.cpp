#include "testbench.h"

unsigned int ticks = 0;

class RegFileTestbench : public Testbench {
    protected:
        void initializeInputs() override {
            top->clk = 0;
            top->WE3 = 0; 
            top->AD1 = 0; 
            top->AD2 = 0; 
            top->AD3 = 0; 
            top->WD3 = 0;
        }
};

TEST_F(RegFileTestbench, WriteThenRead) {
    top->WE3 = 1; 
    top->AD3 = 5; 
    top->WD3 = 0xDEADBEEF;

    runSimulation(1); // clock edge

    top->WE3 = 0; 
    top->AD1 = 5;

    top->eval();

    EXPECT_EQ(top->RD1, 0xDEADBEEFu);
}

TEST_F(RegFileTestbench, X0IsAlwaysZero) {
    top->WE3 = 1; 
    top->AD3 = 0; 
    top->WD3 = 123;

    runSimulation(1);

    top->AD1 = 0;

    top->eval();

    EXPECT_EQ(top->RD1, 0u);
}
