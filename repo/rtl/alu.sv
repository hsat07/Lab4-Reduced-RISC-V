typedef enum logic [2:0] { 
    ADD = 3'b000, 
    SUB = 3'b001, 
    AND = 3'b010, 
    OR  = 3'b011, 
    XOR = 3'b100, 
    SLL = 3'b101, 
    SRL = 3'b110, 
    SRA = 3'b111 
} alu_op_t;

module alu(
    input logic [31:0] ALUop1,
    input logic [31:0] ALUop2,
    input alu_op_t ALUctrl,

    output logic [31:0] SUM,
    output logic EQ
);

always_comb begin
    case(ALUctrl)
        ADD: SUM = ALUop1 + ALUop2; //add
        SUB: SUM = ALUop1 - ALUop2; //minus
        AND: SUM = ALUop1 & ALUop2;
        OR:  SUM = ALUop1 | ALUop2;
        XOR: SUM = ALUop1 ^ ALUop2;
        SLL: SUM = ALUop1 << ALUop2[4:0];
        SRL: SUM = ALUop1 >> ALUop2[4:0];
        SRA: SUM = $signed(ALUop1) >>> ALUop2[4:0];
        default: SUM = '0;
    endcase
end

assign EQ = (ALUop1 == ALUop2); 

endmodule
