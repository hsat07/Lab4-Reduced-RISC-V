module regfile(
    input [4:0] AD1,
    input [4:0] AD2,
    input [4:0] AD3,
    input clk,
    input WE3,
    input [31:0] WD3,

    output [31:0] RD1,
    output [31:0] RD2,
    output [31:0] a0
);

logic [31:0] regs [31:0];

initial begin
    for(int i=0; i<32; i++) regs[i] = 32'b0;
end

always_ff @(posedge clk) begin 
    if(WE3 && AD3 != 0) begin
        regs[AD3] <= WD3;
    end
end


assign RD1 = regs[AD1];
assign RD2 = regs[AD2];
assign a0 = regs[10];

endmodule
