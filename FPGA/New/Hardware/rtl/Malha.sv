module Malha #(parameter int TamanhoMalha = 20*20, parameter int tamanhoDados = 8)(

    input clock,
    input reset,

    input [tamanhoDados - 1:0] value,
    input read, write,
    input [tamanhoDados - 1:0] address,

    output reg [tamanhoDados - 1:0] outData
    
);

localparam READ  = 1'b1,
           WRITE = 1'b0;

reg [tamanhoDados - 1:0] data[0:TamanhoMalha-1];

always_ff @(posedge clock, posedge reset) begin
    if (reset) begin

    		for(int i = 0; i < TamanhoMalha; i++) begin
    		
    			data[address] <= 0;
    		
    		end

    end else begin

        case (read)

            READ: begin

				outData <= data[address];

            end

            WRITE: begin
               
                data[address] <= value;
                
            end

        endcase
    end
end


/*
    wire [31:0] data00 = data[0];
    wire [31:0] data01 = data[1];
    wire [31:0] data02 = data[2];
    wire [31:0] data03 = data[3];
    wire [31:0] data04 = data[4];
    wire [31:0] data05 = data[5];
    wire [31:0] data06 = data[6];
    wire [31:0] data07 = data[7];
    wire [31:0] data08 = data[8];
    wire [31:0] data09 = data[9];
    wire [31:0] data10 = data[10];
    wire [31:0] data11 = data[11];
    wire [31:0] data12 = data[12];
    wire [31:0] data13 = data[13];
    wire [31:0] data14 = data[14];
    wire [31:0] data15 = data[15];
    wire [31:0] data16 = data[16];
    wire [31:0] data17 = data[17];
    wire [31:0] data18 = data[18];
    wire [31:0] data19 = data[19];
    wire [31:0] data20 = data[20];
    wire [31:0] data21 = data[21];
    wire [31:0] data22 = data[22];
    wire [31:0] data23 = data[23];
    wire [31:0] data24 = data[24];
    wire [31:0] data25 = data[25];
    wire [31:0] data26 = data[26];
    wire [31:0] data27 = data[27];
    wire [31:0] data28 = data[28];
    wire [31:0] data29 = data[29];
    wire [31:0] data30 = data[30];
    wire [31:0] data31 = data[31];
*/

endmodule
