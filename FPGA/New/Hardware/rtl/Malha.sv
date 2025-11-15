module Malha #(parameter int TamanhoMalha = 20*20, parameter int tamanhoAddr = 8, parameter int tamanhoDados = 8)(

    input clock,
    input reset,

    input [tamanhoDados - 1:0] value,
    input read, write,
    input [tamanhoAddr - 1:0] address,

    output reg [tamanhoDados - 1:0] outData
    
);

localparam READ  = 1'b1,
           WRITE = 1'b0;

reg [tamanhoDados - 1:0] data[0:TamanhoMalha-1];

always_ff @(posedge clock, posedge reset) begin
    if (reset) begin

    		for(int i = 0; i < TamanhoMalha*TamanhoMalha; i++) begin
    		
    			data[address] <= 0;
    		
    		end

    end else begin

        if(read) begin

			outData <= data[address];

        end

        if(write) begin
               
            data[address] <= value;
   
        end
    end
end

endmodule
