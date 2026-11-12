module sign_extend (
    input logic [31:0] din,
    input logic immsrc,
    output logic [31:0] immOp
);
always_comb begin
    if (immsrc == 1'b0) begin
            immOp = {{20{din[31]}}, din[31:20]};
    end
    else begin
        immOp = {{19{din[31]}}, din[31], din[7], din[30:25], din[11:8], 1'b0};
    end
end
endmodule
