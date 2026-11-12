module ctrl_unit (
    input  logic [6:0] op,
    input  logic [2:0] funct3,
    input  logic zero,       
    output logic pc_src,     
    output logic reg_write,
    output logic alu_src,     
    output logic imm_src,    
    output logic [2:0] alu_ctrl    
);

    localparam logic [6:0] OP_IMM = 7'b0010011;   // addi
    localparam logic [6:0] BRANCH = 7'b1100011;   // bne

    always_comb begin

        reg_write = 1'b0;
        alu_src   = 1'b0;
        imm_src   = 1'b0;
        alu_ctrl  = 3'b000;
        pc_src    = 1'b0;

        case (op)
            OP_IMM: begin
                if (funct3 == 3'b000) begin
                    reg_write = 1'b1;
                    alu_src   = 1'b1;
                    imm_src   = 1'b0;
                    alu_ctrl  = 3'b000;     
                end
            end
            BRANCH: begin
                if (funct3 == 3'b001) begin
                    alu_src   = 1'b0;
                    imm_src   = 1'b1;
                    alu_ctrl  = 3'b001;     
                    pc_src    = (funct3 == 3'b001) & ~zero;   
                end
            end
            default: ;
        endcase
    end

endmodule